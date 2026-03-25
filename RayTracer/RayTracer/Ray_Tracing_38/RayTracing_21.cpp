#include <iostream>
#include <fstream>

#include <mpi.h>

#include "Shared/rtweekend.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer_Animation.h"


int main(int argc, char** argv)
{
	int mode;
	std::string obj_file = "../models/56-chair/Chair.obj";
	obj_file = "../models/Simple/four_meshes.obj";
	std::string filename = "test.ppm";
	//mode = RANDOM_SPHERES;
	//mode = CHECKER_BOARDS;
	//mode = RANDOM_SPHERES;
	//mode = EARTH_SPHERE;
	//mode = PERLIN_SPHERE;
	//mode = QUADS;
	//mode = SIMPLE_LIGHT;
	//mode = TWO_LIGHTS;
	//mode = CORNELL_BOX;
	//mode = TWO_BOXES;
	//mode = TWO_BOXES_ROTATED;
	//mode = CORNELL_SMOKE;
	mode = OBJ_MODEL;
	//mode = OBJ_MODEL_PARALLEL;
	//mode = RANDOM_SPHERES_ANIMATED;


	std::unique_ptr<renderer> rendererObj = std::make_unique<renderer>(argc, argv, mode, filename, obj_file);
	//std::unique_ptr<renderer> render = std::make_unique<renderer_animation>(argc,argv,mode,filename);

	rendererObj->setup();
	rendererObj->render();
	rendererObj->write_file();

	return 0;
}
