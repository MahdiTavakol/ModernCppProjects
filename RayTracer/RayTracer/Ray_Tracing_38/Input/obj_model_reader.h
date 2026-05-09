#ifndef OBJ_MODEL_READER_H
#define OBJ_MODEL_READER_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "../Types/color.h"
#include "../Materials/material.h"
#include "../Materials/material_list.h"
#include "../Algorithms/hittable_list.h"
#include "../Algorithms/communicator.h"
#include "../Types/vec3.h"
#include "../Geometry/triangle_mesh.h"
#include "../Geometry/mesh.h"

struct face_indx
{
	std::vector<int> v_indx;
	std::vector<int> vt_indx;
	std::vector<int> vn_indx;
	std::string object;
	std::string group;
	int num_edges;
	int mat_indx;
};

class obj_model_reader {
public:
	obj_model_reader(){}
	obj_model_reader(std::string obj_file_name_,
		             communicator* para_);
	obj_model_reader(std::string _obj_file_name,
		             std::string _mtl_file_name,
		             communicator* _para);
	obj_model_reader(std::unique_ptr<std::iostream> _obj_file_ptr,
					 std::unique_ptr<std::iostream> _mtl_file_ptr,
		             communicator* _para);
	obj_model_reader(std::unique_ptr<std::iostream> _obj_file_ptr,
		             std::unique_ptr<std::iostream> _mtl_file_ptr,
		             std::unique_ptr<communicator>& _para);
	virtual void read();
	std::unique_ptr<hittable_list> return_world();
	std::unique_ptr<material_list> return_mtl_list();

protected:
	// my rank
	std::array<int, 2> my_rank_config;
	// the parallel object
	communicator* para = nullptr;
	// the object container
	std::unique_ptr<hittable_list> world;
	// the material list
	std::unique_ptr<material_list> mtl_list;
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
	static std::unique_ptr<std::istream> open_file(std::string file_name_, bool silent = false);

	// setting the silent status
	void set_silent_status();
	// checking data
	static void check_data(const int& num_file_, const int& num_read_, const std::string& title, const bool silent_ = true);
	// triangulating a face
	static void face_triangulate(const face_indx& input_, std::vector<face_indx>& output_);
	// calculating the vn values from the edges
	static void estimate_vns(
		const std::array<point3,3>& vs_,
		const std::array<bool, 3>& set_vn_,
		std::array<point3,3>& vns_);
};


#endif
