#pragma once

#include <memory>


#include "Renderer.h"
#include "../Algorithms/camera.h"
#include "../Algorithms/communicator.h"
#include "../Output/output.h"
#include "../Algorithms/hittable_list.h"


#include "../Input/factory.hpp"


class renderer_facade
{
public:
	renderer_facade(int argc, char** argv, int _mode, MPI_Comm comm_ = MPI_COMM_WORLD);
	virtual ~renderer_facade();
	virtual void setup();
	void add(std::unique_ptr<hittable>& object);
	virtual void render();
	void write_file();



protected:
	// the factory object
	std::unique_ptr<factory> builder;

	// resources
	std::unique_ptr<renderer> rendererObj;
	std::unique_ptr<camera> cam;
	std::unique_ptr<communicator> para;
	std::unique_ptr<output> writer;
	std::unique_ptr<hittable_list> world;
	
};