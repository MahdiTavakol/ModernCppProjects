#include "image_parallel.h"


image_parallel::image_parallel(
	std::unique_ptr<camera_settings>& _cam_setting,
	std::unique_ptr<parallel>& _para
): image{ _cam_setting}, para{_para.get()}
{
    int rank = _para->return_rank();
    int size = _para->return_size();
    auto rank_config = _para->return_rank_config();
    auto size_config = _para->return_size_config();

    if (size_config[0] == 0 || size_config[1] == 0)
        throw std::invalid_argument("Size configuration for parallel rendering cannot be zero!");


    // setting up the camera
    auto set_range_per_node = [&](const int dimension, const int myRank, const int mySize,
        int& min, int& max, int& perNode) {
            if (mySize == 1) {
                min = 0;
                max = dimension;
                perNode = dimension;
                return;
            }
            perNode = static_cast<int>(dimension / mySize + 1);
            min = myRank * perNode;
            max = min + perNode;
            min = min >= dimension ? dimension : min;
            max = max >= dimension ? dimension : max;
        };


    set_range_per_node(image_width, rank_config[0], size_config[0],
        widthMin, widthMax, width_per_rank);
    set_range_per_node(image_height, rank_config[1], size_config[1],
        heightMin, heightMax, height_per_rank);

    // double check this please
    int myWidth = widthMax - widthMin;
    int myHeight = heightMax - heightMin;
    // creating the color_array
    c_array = std::make_unique<color_array>(myWidth, myHeight);
}


void image_parallel::gather(std::unique_ptr<color_array>& c_array_) const
{
    auto size_config = para->return_size_config();
    int extended_width = width_per_rank * size_config[0];
    int extended_height = height_per_rank * size_config[1];

    color_data** colors = c_array_->return_array();
    color_data** colors_all = (color_data**)malloc(extended_width * sizeof(color_data*));
    color_data* temp = (color_data*)malloc(extended_width * extended_height * sizeof(color_data));
    for (int i = 0; i < extended_width; i++)
        colors_all[i] = &temp[i * extended_height];


    para->gather(colors, colors_all, width_per_rank, height_per_rank);


    // a copy of data is involved here I should work on removing this!
    std::unique_ptr<color_array> c_array_all = 
        std::make_unique<color_array>(image_width, image_height, colors_all);

    free(colors_all[0]);
    free(colors_all);
    colors_all = nullptr;
    // swapping 
    c_array_.swap(c_array_all);
}