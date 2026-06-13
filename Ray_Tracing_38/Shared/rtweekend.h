#ifndef RT_WEEKEND
#define RT_WEEKEND


#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

#define FLERR __FILE__, __LINE__

enum class outputType {
	SERIAL,
	PARALLEL,
	ASYNC
};

enum class outputMode {
	P3,
	P6
};




enum {
	RANDOM_SPHERES, CHECKER_BOARDS, EARTH_SPHERE,
	PERLIN_SPHERE, QUADS, SIMPLE_LIGHT, 
	TWO_LIGHTS,CORNELL_BOX, TWO_BOXES,
	TWO_BOXES_ROTATED,CORNELL_SMOKE, RANDOM_SPHERES_ANIMATED,
	FINAL_SCENE,
	SIMPLE_2D_PARALEL_TEST, FINAL_SCENE,


	OBJ_MODEL, OBJ_MODEL_PARALLEL, GLTF_MODEL
};


enum class renderMode {
	STATIC,
	ANIMATION,
	ASYNC
};




using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degree)
{
	return degree * pi / 180.0;
}

inline double random_double()
{
	return std::rand() / (RAND_MAX + 1.0);
}

inline float random_float()
{
	return static_cast<float>(std::rand() / (RAND_MAX + 1.0));
}

inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}

inline float random_float(float min, float max)
{
	return min + (max - min) * random_float();
}

inline int random_int(int min, int max)
{
	return int(random_double(min, max + 1));
}







#endif