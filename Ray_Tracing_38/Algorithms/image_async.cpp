#include "image_async.h"
#include "simpleComm.h"


image_async::image_async(settings* img_setting_,
	communicator* para_,
	std::array<int, 2> domain_size_):
	image{para_},
	domain_size{domain_size_}
{
	// checking the setting type
	image_settings* sett = dynamic_cast<image_settings*>(img_setting_);
	if (!sett)
		throw std::invalid_argument("Wrong settings object");

	// changing the settings according to that settings.
	image_width = sett->get_image_width();
	image_height = sett->get_image_height();


	// image queue
	for (int i = 0; i < domain_size[0]; i++)
	{
		for (int j = 0; j < domain_size[1]; j++)
		{
			std::array<int, 2> domain_config{ i,j };
			std::unique_ptr<communicator> comm =
				std::make_unique<simpleComm>(domain_config, domain_size);
			std::unique_ptr<image> img = std::make_unique<image>(img_setting_, comm.get());
			this->push(std::move(img));
		}
	}
}

void image_async::push(std::unique_ptr<image> img_)
{
	image_queue.push(std::move(img_));
}

bool image_async::try_pop(std::unique_ptr<image>& img_)
{
	return image_queue.try_pop(img_);
}