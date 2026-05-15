#ifndef RENDERER_H
#define RENDERER_H
#include <memory>
#include <string>

#include "../Algorithms/communicator.h"
#include "../Algorithms/path.h"
#include "../Algorithms/camera.h"
#include "../Output/output.h"
#include "../Algorithms/hittable_list.h"
#include "../Materials/material_list.h"


class renderer
{
public:
	renderer(communicator* para_,std::unique_ptr<path>&& pth_, std::string info_ = "", bool verbose_ = false);
	virtual ~renderer();
	void setup(hittable_list* world_, material_list* mtls_);
	virtual void render(image* img_, camera* cam_, output* writer_);
	virtual void write_file(output* writer_, image *img_);
	
	

protected:
	std::string info = "";
	bool verbose = false;
	communicator* para;

	std::unique_ptr<path> pth;

	void message(std::string _text);
	std::string filename;


	// since the world and the materials
	// rarely change during rendering 
	// we put these two as properties of
	// the renderer class
	hittable_list* world;
	material_list* mtls;
};

#endif
