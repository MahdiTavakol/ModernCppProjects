#pragma once
#include <vector>
#include <string>
#include "../Algorithms/hittable_list.h"
#include "../Materials/material_list.h"
#include "../Algorithms/communicator.h"

// in the case where all
// the indexes are the same for a face
// such as the gltf format, the 
// vt_indx and vn_indx vectors will be empty.
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


class model_reader
{
public:
	model_reader(std::string file_path_, communicator* para_);
	virtual void read() = 0;
	std::unique_ptr<hittable_list> return_world();
	std::unique_ptr<material_list> return_mtl_list();

protected:
	std::string file_path;
	communicator* para = nullptr;
	// should I be silent
	bool silent = false;
	// how much verbose I want to be in the messaging interface
	int max_message_level = 1000;
	// the object container
	std::unique_ptr<hittable_list> world;
	// the material list
	std::unique_ptr<material_list> mtl_list;

	virtual void add_item(const int& _low, const int& _hi) = 0;

	// data
	std::vector<face_indx> faces;
	std::vector<vec3> vs, vns;
	std::vector<vec2> vts;


	// function useful for the parallel implementation of the reader classes
	virtual void set_range(int& _low, int& _hi);


	static std::unique_ptr<std::istream> open_file(
		std::string& file_name_,
		std::string path_ = "",
		bool silent = false);

	// calculating the vn values from the edges
	static void estimate_vns(
		const std::array<point3, 3>& vs_,
		std::array<point3, 3>& vns_);


	// The messaging interface.. 
// should have its own class at the end
	void print_message(const std::string message_, int level_ = 0, char delimiter = ' ')
	{
		if (level_ > max_message_level)
			return;
		std::cout << std::string(level_, delimiter) << message_ << "\n";
	}

};
