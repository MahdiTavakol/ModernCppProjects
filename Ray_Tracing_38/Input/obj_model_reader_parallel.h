#ifndef OBJ_MODEL_READER_PARALLEL_H
#define OBJ_MODEL_READER_PARALLEL_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "../Types/color.h"
#include "../Materials/material.h"
#include "../Algorithms/communicator.h"
#include "../Algorithms/hittable_list.h"
#include "../Algorithms/hittable_list_parallel.h"
#include "../Types/vec3.h"
#include "../Geometry/triangle_mesh.h"
#include "obj_model_reader.h"
#include "../Geometry/mesh.h"


class obj_model_reader_parallel: public obj_model_reader 
{
public:
	obj_model_reader_parallel(
		std::string _obj_file_name,
		std::unique_ptr<communicator>& _para);
	obj_model_reader_parallel(
		std::string _obj_file_name,
		communicator* _para);

protected:
	void set_range(int& low_, int& hi_) override;
};


#endif
