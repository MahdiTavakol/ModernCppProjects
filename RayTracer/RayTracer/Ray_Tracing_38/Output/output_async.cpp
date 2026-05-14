#include "output_async.h"

#include <fstream>
#include <shared_mutex>
#include "../Data/color_array.h"
#include "../Algorithms/simpleComm.h"

output_async::output_async(
	settings* out_settings_,
	communicator* para_) :
	output{ out_settings_,para_ },
	out_settings{out_settings_}
{
}


output_async::output_async(
	settings* out_settings_,
	std::unique_ptr<image>&& img_,
	communicator* para_) :
	output{ out_settings_,std::move(img_),para_ },
	out_settings{ out_settings_ }
{
}



output_async::~output_async()
{
}


void output_async::setup()
{
	output::setup();
	// closing the file to assure the header has been written
	std::fstream* filestream = dynamic_cast<std::fstream*>(stream.get());
	if (filestream)
		filestream->close();

}




bool output_async::try_pop(std::unique_ptr<output>& out_)
{
	std::unique_ptr<output> out
		= std::make_unique<output>(out_settings, para);

	out_ = std::move(out);
	return true;
}