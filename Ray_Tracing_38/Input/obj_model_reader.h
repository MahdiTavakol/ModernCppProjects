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
#include "../Output/Logger.h"

#include "model_reader.h"

struct material_info
{
	std::string material_name;
	color Ka;
	color Kd;
	double Ns;
	double Tr;
	color Tf;
	color Ks;
	double Ni;
	double d;
	bool read_Tr = false;
	std::string texture_path;
};

class obj_model_reader: public model_reader {
public:
	obj_model_reader(std::string obj_file_name_,
		             Logger* error_,
		             communicator* para_);
	obj_model_reader(std::string _obj_file_name,
		             std::string _mtl_file_name,
		             Logger* error_,
		             communicator* _para);
	obj_model_reader(std::unique_ptr<std::iostream> _obj_file_ptr,
					 std::unique_ptr<std::iostream> _mtl_file_ptr,
		             Logger* error_,
		             communicator* _para);
	obj_model_reader(std::unique_ptr<std::iostream> _obj_file_ptr,
		             std::unique_ptr<std::iostream> _mtl_file_ptr,
		             Logger* error_,
		             std::unique_ptr<communicator>& _para);
	virtual void read() override;


protected:
	// my rank
	std::array<int, 2> my_rank_config;
	// file names
	std::string& obj_file_name, mtl_file_name;

	// number of shapes
	int polygon_num = 0, triangle_num = 0;
	// names
	// unordered_map is better in performance
	std::vector<std::string> obj_mat_names;



	// input streams
	std::unique_ptr<std::istream> obj_file_ptr;
	std::unique_ptr<std::istream> mtl_file_ptr;
	// helper functions
	void read_obj_file();
	void read_mtl_file();
	virtual void set_range(int& _low, int& _hi);
	virtual void add_item(const int& _low, const int& _hi) override;

	// setting the silent status
	void set_silent_status();
	// checking data
	static void check_data(
		const int& num_file_, 
		const int& num_read_,
		const std::string& title,
		Logger* error_,
		const bool silent_ = true);
	// triangulating a face
	static void face_triangulate(const face_indx& input_, std::vector<face_indx>& output_);
	// creating and adding materials to the material list
	void create_and_add_material(material_info& mat_info);


	static std::string extract_path(std::string obj_file_name);
	std::string path;
};


#endif
