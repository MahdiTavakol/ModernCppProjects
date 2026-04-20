#include "image.h"

image::image(std::unique_ptr<camera_settings>& cam_setting_,
	std::unique_ptr<parallel>& _para) :
	para{_para.get()},
	image_width{ cam_setting_->get_image_width()},
	image_height{cam_setting_->get_image_height()}
{
    int rank = _para->return_rank();
    int size = _para->return_size();
    auto rank_config = _para->return_rank_config();
    auto size_config = _para->return_size_config();

    if (size_config[0] == 0 || size_config[1] == 0)
        throw std::invalid_argument("Size configuration for parallel rendering cannot be zero!");

    set_range_per_node(rank_config[0], size_config[0],
        image_width,
		widthMin, widthMax, width_per_rank);
	set_range_per_node(rank_config[1], size_config[1], 
        image_height, 
		heightMin, heightMax, height_per_rank);


    // double check this please
    int myWidth = widthMax - widthMin;
    int myHeight = heightMax - heightMin;
    // creating the color_array
    c_array = std::make_unique<color_array>(myWidth, myHeight);
}

image::image(const int& image_width_, const int& image_height_, parallel* _para) :
    image_width{ image_width_ }, image_height{ image_height_ }, para{ _para }
{
	c_array = std::make_unique<color_array>(image_width, image_height);
	width_per_rank = image_width;
	height_per_rank = image_height;
	widthMin = 0;
	heightMin = 0;
	widthMax = image_width;
	heightMax = image_height;
}

std::unique_ptr<image> image::all_copy() const {
	auto copy = std::make_unique<image>(image_width, image_height, para);
	return std::move(copy);
}

void image::returnRange(std::array<int, 2>& widthRange_, std::array<int, 2>& heightRange_) const
{
	widthRange_[0] = widthMin;
	widthRange_[1] = widthMax;
	heightRange_[0] = heightMin;
	heightRange_[1] = heightMax;
}

void image::returnSize(int& width_, int& height_) const
{
	c_array->return_size(width_, height_);
}

std::array<int, 2> image::returnSize() const
{
    std::array<int,2> size{
        image_width,
        image_height
	};
    return size;
}

std::array<int, 2> image::returnSizePerRank() const
{
    std::array<int, 2> size_per_rank{
        width_per_rank,
        height_per_rank
	};
	return size_per_rank;
}

void image::set_range_per_node(
    const int rank_, const int size_,
    const int dimension_,
    int& min_, int& max_, int& dimPerNode_)
{
    if (size_ == 1) {
        min_ = 0;
        max_ = dimension_;
        dimPerNode_ = dimension_;
        return;
    }
    dimPerNode_ = static_cast<int>(dimension_ / size_ + 1);
    min_ = rank_ * dimPerNode_;
    max_ = min_ + dimPerNode_;
    min_ = min_ >= dimension_ ? dimension_ : min_;
    max_ = max_ >= dimension_ ? dimension_ : max_;
}