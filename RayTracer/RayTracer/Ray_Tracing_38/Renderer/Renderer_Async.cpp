#include "Renderer_Async.h"
#include "../Algorithms/simpleComm.h"
#include "../Output/output_async.h"


renderer_async::renderer_async(
	communicator* para_,
	std::unique_ptr<path>&& pth_,
	int max_threads_,
	std::string info_,
	bool verbose_) :
	renderer{ para_,std::move(pth_),info_,verbose_ },
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


	auto img = cam_->return_image();

	std::vector<std::thread> thread_pool;
	thread_pool.reserve(max_threads);

	image_async* img_async = dynamic_cast<image_async*>(img.get());
	output_async* out_async = dynamic_cast<output_async*>(writer_);

	if (!img_async || !out_async)
		throw std::invalid_argument("The output and image must be of async type for the async_renderer!");


	for (int i = 0; i < max_threads; i++)
	{
		thread_pool.emplace_back(
			&renderer_async::render_thread,
			this,
			cam_,
			world_,
			list_,
			img_async,
			out_async,
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
	camera* cam_,
	hittable_list* world_,
	material_list* list_,
	image_async* img_async_,
	output_async* out_async_,
	const std::streampos npos_)
{
	std::atomic<int> number = 0;
	while (true)
	{
		std::unique_ptr<image> img_thread;
		std::unique_ptr<output> wrt_thread;

		if (!img_async_->try_pop(img_thread))
			break;
		if (!out_async_->try_pop(wrt_thread))
			break;

		if (!img_thread || !wrt_thread)
			break;


		cam_->render(img_thread.get(), *world_, *list_);
		wrt_thread->reset_image(std::move(img_thread));
		wrt_thread->open_file();
		wrt_thread->write_file( npos_);
		number++;

	}
}




