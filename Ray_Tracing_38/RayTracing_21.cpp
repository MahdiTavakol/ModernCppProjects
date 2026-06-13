#include <iostream>
#include <fstream>

#include <mpi.h>

#include "Shared/rtweekend.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer_facade.h"
#include "Output/Logger.h"






int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	std::unique_ptr<Logger> error;

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
		error = std::move(rendFacade->return_error());
	}
	catch (const std::invalid_argument& e)
	{
		std::string message = "Invalid_argument exception " + std::string(e.what());
		error->print_message(message, 0);
	}
	catch (const std::runtime_error& e)
	{
		std::string message = "Runtime error exception " + std::string(e.what());
		error->print_message(message, 0);
	}
	catch (const std::out_of_range& e)
	{
		std::string message = "Out of range exception " + std::string(e.what());
		error->print_message(message, 0);
	}
	catch (const std::exception& e)
	{
		std::string message = "Exception " + std::string(e.what());
		error->print_message(message, 0);
	}
	catch (...)
	{
		std::string message = "Unknown exception ";
		error->print_message(message, 0);
	}


	MPI_Finalize();

	return 0;
}
