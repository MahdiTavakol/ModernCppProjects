#pragma once
#include "camera_settings.h"
#include "../Data/color_array.h"
#include "communicator.h"
#include <memory>
#include <array>

class image
{
public:
	image(std::unique_ptr<camera_settings>& cam_setting_,
		std::unique_ptr<communicator>& _para);
	image(const int& image_width_, const int& image_height_, communicator* _para);
	image(const std::array<int, 2>& size_,
		 const std::array<int, 2> widthRange_, const std::array<int, 2> heightRange_,
		 std::unique_ptr<color_array>&& c_array_,
		 communicator* _para);
	image(const int& image_width_, const int& image_height_,
		std::unique_ptr<color_array>&& c_array_,
		communicator* para_);
	image(const int& image_width_, const int& image_height_,
		 std::unique_ptr<color_array>& c_array_,
		 std::unique_ptr<communicator>& para_);
	// a nondefault copy constructor 
	// required by the parallel to create the gathered image object
	std::unique_ptr<image> all_copy() const;
	virtual void returnRange(std::array<int, 2>& widthRange_, std::array<int, 2>& heightRange_) const;
	void returnSize(int& width_, int& height_) const;
	std::array<int, 2> returnSize() const;
	std::array<int, 2> returnSizePerRank() const;
	communicator* returnPara() { return para; }
	color_array* array() { return c_array.get(); }
	std::unique_ptr<color_array>& array_unique_ptr() { return c_array; }
	color_data** returnColorData() { return c_array->return_array(); }

	static void gather(
		std::unique_ptr<image>& one_,
		std::unique_ptr<image>& one_all_,
		communicator* para_
	);

protected:
	communicator* para;
	// the whole image dimensions. 
	const int image_width, image_height;
	// the color_array object
	std::unique_ptr<color_array> c_array;


	// size per rank
	int width_per_rank{ 0 }, height_per_rank{ 0 };

	// ranges for this rank
	int widthMin{ 0 }, widthMax{ 0 };
	int heightMin{ 0 }, heightMax{ 0 };

	// helper function to set the range for each node
	static void set_range_per_node(
		const int rank_, const int size_,
		const int dimension_,
		int& min_, int& max_, int& dimPerNode_);
};
