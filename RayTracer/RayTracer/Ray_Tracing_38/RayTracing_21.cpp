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


	try {
		std::unique_ptr<renderer_facade> rendFacade =
			std::make_unique<renderer_facade>(argc, argv, mode, MPI_COMM_WORLD);


		rendFacade->setup();
		rendFacade->render();
		rendFacade->write();
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "Invalid_argument exception " << e.what() << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "Runtime error exception " << e.what() << std::endl;
	}


	MPI_Finalize();

	return 0;
}
