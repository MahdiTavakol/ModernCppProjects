#pragma once
#include "Renderer.h"

class renderer_animation final : public renderer
{
public:
	renderer_animation(int argc, char** argv, int _mode, std::string _file_name, MPI_Comm comm_);
	renderer_animation(int argc, char** argv, int _mode, std::string _filename,
		std::string obj_file_, MPI_Comm comm_);
	void setup() override;
	void render() override;


private:
	std::unique_ptr<path> pth;
	void open_file(const int frame_i);
	int return_num_frames() const;
	void move_camera(const int frame_i);
	void message(int _rank, std::string _text);
};