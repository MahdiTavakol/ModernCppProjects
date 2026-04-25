#include <iostream>
#include <fstream>

#include <mpi.h>

#include "Shared/rtweekend.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer_facade.h"


int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int mode;

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
	//mode = OBJ_MODEL;
	//mode = OBJ_MODEL_PARALLEL;
	//mode = RANDOM_SPHERES_ANIMATED;
	mode = SIMPLE_2D_PARALEL_TEST;

	
	std::unique_ptr<renderer_facade> rendFacade = 
		std::make_unique<renderer_facade>(argc, argv, mode, MPI_COMM_WORLD);


	rendFacade->setup();
	rendFacade->render();
	rendFacade->write();


	MPI_Finalize();

	return 0;
}
