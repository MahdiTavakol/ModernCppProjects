#include "output_async.h"

#include <fstream>
#include <shared_mutex>
#include "../Data/color_array.h"

output_async::output_async(
	settings* out_settings,
	communicator* para_) :
	output{ out_settings,para_ }
{
}


output_async::output_async(
	settings* out_settings,
	std::unique_ptr<image>&& img_,
	communicator* para_) :
	output{ out_settings,std::move(img_),para_ }
{
}



output_async::~output_async()
{
}

/*
void output_async::setup()
{
	/*
	 * First, the thread id 0 checks if the file exists
	 * it removes that file.
	 * Every rank needs to wait for the check to be finished before
	 * proceeding, otherwise the file opened by another rank
	 * will be deleted by the rank 0!
	 * Then, each rank opens the file in the non-trunk mode
	 * as they want to write together and it is possible
	 * that another rank has already created the file.
	 * Which rank first creates the file does not matter.
	 */
	 /*
	 * Since out ios mode is in and out
	 * the file needs to exist when it is opened.
	 * That is the reason why here the file is created.
	 */

	 /*

		 if (outMode == outputMode::P3)
			 throw std::invalid_argument("The text format is not supported for async writing");

		 auto setfile = [](std::string file_name_)
			 {
				 remove_file(file_name_);
				 std::ofstream temp{ file_name_ };
			 };

		 std::call_once(*file_set_ptr, setfile, file_name);
		 open_new_file(file_name);

		 if (!img)
			 return;

		 int image_width, image_height;
		 img->returnSize(image_width, image_height);
		 //std::call_once(*header_set_ptr, &output_async::write_header, this, image_width, image_height);
		 write_header(image_width, image_height);
	 }
	 */