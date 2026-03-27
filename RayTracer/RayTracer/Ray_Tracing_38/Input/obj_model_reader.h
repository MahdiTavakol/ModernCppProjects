#ifndef OBJ_MODEL_READER_H
#define OBJ_MODEL_READER_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "../Types/color.h"
#include "../Materials/material.h"
#include "../Algorithms/hittable_list.h"
#include "../Algorithms/parallel.h"
#include "../Types/vec3.h"
#include "../Geometry/triangle_mesh.h"
#include "../Geometry/mesh.h"

struct face_indx
{
	std::vector<int> v_indx;
	std::vector<int> vt_indx;
	std::vector<int> vn_indx;
	int num_edges;
	int mat_indx;
};

class obj_model_reader {
public:
	obj_model_reader(){}
	obj_model_reader(std::string _obj_file_name, parallel* _para);
	obj_model_reader(std::string _obj_file_name, std::string _mtl_file_name, parallel* _para);
	obj_model_reader(std::unique_ptr<std::istream> _obj_file_ptr,
		             std::unique_ptr<std::istream> _mtl_file_ptr,
		             parallel* para);
	virtual void read();
	std::unique_ptr<hittable_list> return_world();

protected:
	// the parallel object
	parallel* para;
	// the object container
	std::unique_ptr<hittable_list> world;
	// file names
	std::string obj_file_name, mtl_file_name;
	// data
	std::vector<vec3> vs, vts, vns;
	// number of data
	int v_num = 0, vt_num = 0, vn_num = 0;
	int polygon_num = 0, triangle_num = 0;
	// names
	// unordered_map is better in performance
	std::vector<std::string> obj_mat_names;
	// face indexes
	std::vector<face_indx> face_indexes;
	// materials
	std::unordered_map<std::string, std::unique_ptr<material>> materials_map;

	// 
	bool silent = false;

	// input streams
	std::unique_ptr<std::istream> obj_file_ptr;
	std::unique_ptr<std::istream> mtl_file_ptr;
	// helper functions
	void read_obj_file();
	void read_mtl_file();
	virtual void set_range(int& _low, int& _hi);
	virtual void add_item(const int& _low, const int& _hi);
	static std::unique_ptr<std::istream> open_file(std::string file_name_);

	// setting the silent status
	void set_silent_status();
	// checking data
	static void check_data(const int& num_file_, const int& num_read_, const std::string& title);
};


#endif
