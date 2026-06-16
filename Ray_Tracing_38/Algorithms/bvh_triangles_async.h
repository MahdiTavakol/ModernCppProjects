#pragma once
#include <mutex>
#include <thread>

#include "../Output/profiler.h"
#include "bvh_triangles.h"

struct bvh_running_state
{
	const int max_threads;
	std::atomic<int> running_threads;

	bvh_running_state(int max_threads_):
		max_threads{max_threads_}, running_threads{0}
	{}
};

class bvh_triangles_async : public bvh_triangles
{
public:
	bvh_triangles_async(
		profiler* timer_,
		std::unique_ptr<triangle_list> list_,
		BVH_Split_Method split_method_ = BVH_Split_Method::MEDIAN,
		int max_threads_ = 1);
	virtual ~bvh_triangles_async() override;


	virtual void set_internal_left_right(
		size_t start_,
		size_t mid_,
		size_t end_,
		const int left_indx_,
		const int right_indx_,
		const int level_ = 0,
		BVH_Split_Method split_method_ = BVH_Split_Method::MEDIAN) override;


protected:
	// default empty c'tor for empty leaves
	bvh_triangles_async(profiler* timer_, int max_threads_ = 1);
	// c'tor for internal nodes
	bvh_triangles_async(
		profiler* timer_,
		size_t start, size_t end,
		const int indx_,
		const int level_,
		std::shared_ptr<bvh_running_state>& bvh_state_,
		BVH_Split_Method split_method_ = BVH_Split_Method::MEDIAN);
	// c'tor for leaves
	bvh_triangles_async(
		profiler* timer_,
		const size_t& box_indx_,
		const size_t& triangle_indx_,
		const aabb& bbox_,
		Node_Type type = Node_Type::LEAF);



private:
	std::shared_ptr<bvh_running_state> bvh_state;

	bool try_acquire_thread();

	static std::unique_ptr<bvh_triangles_async> build_node_async(
		profiler* timer_,
		size_t start, size_t end,
		const int indx_,
		const int level_,
		std::shared_ptr<bvh_running_state> bvh_state,
		BVH_Split_Method split_method_ = BVH_Split_Method::MEDIAN);
};