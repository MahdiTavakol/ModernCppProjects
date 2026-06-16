#include "bvh_triangles_async.h"
#include <future>

bvh_triangles_async::bvh_triangles_async(
	profiler* timer_,
	std::unique_ptr<triangle_list> list_,
	BVH_Split_Method split_method_,
	int max_threads_) :
	bvh_triangles{timer_}
{
	timer_->start_event(" bvh tri_async creation");
	// number of levels
	size_t num_levels = log2(list_->size()) + 1;
	// the size of boxes array
	size_t array_size = std::pow(2, num_levels + 1) - 1;


	bboxes.resize(array_size);

	// putting the input data
	list = std::move(list_);


	size_t start = 0;
	size_t end = list->size();

	// creating the bvh_state
	bvh_state = std::make_shared<bvh_running_state>(max_threads_);


	set_left_right(
		start, end,
		0,
		0,
		split_method_);

	// getting the triangles object
	triangles = list->return_triangles();

	// taking care of non_triangles objects
	std::unique_ptr<hittable_list> non_triangles = list->return_non_triangles();
	non_triangles_bvh = std::make_unique<bvh_node>(timer,std::move(non_triangles));


	timer_->stop_event(" bvh tri_async creation");
}

bvh_triangles_async::bvh_triangles_async(
	profiler* timer_, 
	int max_threads_):
	bvh_triangles{timer_},bvh_state{nullptr}
{}

// c'tor for internal nodes
bvh_triangles_async::bvh_triangles_async(
	profiler* timer_,
	size_t start_, size_t end_,
	const int indx_,
	const int level_,
	std::shared_ptr<bvh_running_state>& bvh_state_,
	BVH_Split_Method split_method_) :
	bvh_triangles{timer_},
	bvh_state{bvh_state_}
{
	set_left_right(
		start_, end_,
		indx_,
		level_,
		split_method_);
}

// c'tor for leaves
bvh_triangles_async::bvh_triangles_async(
	profiler* timer_,
	const size_t& box_indx_,
	const size_t& triangle_indx_,
	const aabb& bbox_,
	Node_Type type):
	bvh_triangles{timer_,box_indx_,triangle_indx_,bbox_,type},
	bvh_state{nullptr}
{}

bvh_triangles_async::~bvh_triangles_async()
{
}

void bvh_triangles_async::set_internal_left_right(
	size_t start_,
	size_t mid_,
	size_t end_,
	const int left_indx_,
	const int right_indx_,
	const int level_,
	BVH_Split_Method split_method_)
{
	std::future<std::unique_ptr<bvh_triangles_async>> right_ftr;
	bool parallel = try_acquire_thread();

	if (parallel)
		right_ftr =
			std::async(
				std::launch::async,
				build_node_async,
				timer,
				mid_, end_,
				right_indx_,
				level_ + 1,
				bvh_state,
				split_method_);


	left = std::unique_ptr<bvh_triangles>(
		new bvh_triangles_async(
			timer,
			start_, mid_,
			left_indx_,
			level_ + 1,
			bvh_state,
			split_method_));

	if (parallel == true)
	{
		// it is possible that after reading the running_threads
		// another thread decreases its values but it does not matter
		// since it still be lower than num_threads
		right = std::move(right_ftr.get());
		bvh_state->running_threads--;
	}
	else
	{
		right = std::unique_ptr<bvh_triangles>(
			new bvh_triangles_async(
				timer,
				mid_, end_,
				right_indx_,
				level_ + 1,
				bvh_state,
				split_method_));
	}
}


std::unique_ptr<bvh_triangles_async> bvh_triangles_async::build_node_async(
	profiler* timer_,
	size_t start_, size_t end_,
	const int indx_,
	const int level_,
	std::shared_ptr<bvh_running_state> bvh_state,
	BVH_Split_Method split_method_)
{
	std::string myName = timer_->start_thread_event("  bvh tri_async thread");
	return std::unique_ptr<bvh_triangles_async>(
		new bvh_triangles_async(
			timer_,
			start_, end_,
			indx_,
			level_,
			bvh_state,
			split_method_));
	timer_->stop_thread_event(myName, "  bvh tri_async thread");
}

bool bvh_triangles_async::try_acquire_thread()
{
	std::atomic<int>& running_threads = bvh_state->running_threads;
	int current_threads = running_threads.load();
	int max_threads = bvh_state->max_threads;

	while (current_threads < max_threads)
	{
		if (running_threads.compare_exchange_strong(current_threads, current_threads + 1))
			return true;
	}
	return false;
}