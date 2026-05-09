#include "Renderer_Async.h"
#include "../Algorithms/simpleComm.h"
#include "../Output/output_async.h"


renderer_async::renderer_async(
	communicator* para_,
	std::unique_ptr<path>&& pth_,
	int max_threads_,
	std::unique_ptr<image_queue_fg>&& queue_,
	std::string info_,
	bool verbose_) :
	renderer{ para_,std::move(pth_),info_,verbose_ },
	queue{ std::move(queue_) },
	max_threads{max_threads_}
{
	if (max_threads <= 0)
	{
		std::cout << "Resetting the maximum number of threads" << std::endl;
		max_threads = std::thread::hardware_concurrency();
	}
}

void renderer_async::render(camera* cam_, output* writer_, hittable_list* world_, material_list* list_)
{
	auto npos = writer_->return_binary_begin();

	path& pth_ref = *pth;
	cam_->move_camera(pth_ref[0]);

	std::vector<std::thread> thread_pool;
	thread_pool.reserve(max_threads);

	max_threads = 1;
	std::cout << "Motherfucker " << static_cast<std::streamoff>(npos) << std::endl;

	for (int i = 0; i < max_threads; i++)
	{
		thread_pool.emplace_back(
			&renderer_async::render_thread,
			this,
			std::ref(*cam_),
			std::cref(*world_),
			std::cref(*list_),
			npos
		);
	}

	for (auto& thread : thread_pool)
	{
		if (thread.joinable())
			thread.join();
	}
}


void renderer_async::write_file(output* writer_)
{
	// writing is done at the same time as the rendering
	// so there is nothing to do here!
}

void renderer_async::render_thread(
	camera& cam_,
	const hittable_list& world_,
	const material_list& list_,
	const std::streampos npos_)
{
	static int number = 0;
	std::cout << "Fuck " << (queue->empty()==true) << std::endl;
	std::cout << "Fuckeeer " << (queue->empty() == false) << std::endl;

	while (true)
	{
		std::unique_ptr<image> img_thread;
		std::unique_ptr<output> wrt_thread;

		if (!queue->try_pop(img_thread, wrt_thread))
			break;

		if (!img_thread || !wrt_thread)
			break;

		std::cout << "before render\n";
		cam_.render(img_thread.get(), world_, list_);

		std::cout << "after render\n";
		wrt_thread->write_file(img_thread.get(), npos_);

		std::cout << "after write\n";
		bool try_i = queue->try_pop(img_thread, wrt_thread);
		std::cout << "chose " << try_i << std::endl;
	}

	std::cout << "Fart\n";
}




