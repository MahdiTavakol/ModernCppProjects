#include "obj_model_reader.h"
#include <filesystem>
#include <fstream>
#include "../Geometry/sphere.h"


obj_model_reader::obj_model_reader(
	std::string obj_file_name_,
    communicator* para_) :
	para{ para_ },
	obj_file_name(obj_file_name_),
	v_num(0), vt_num(0), vn_num(0),
	world{ std::make_unique<hittable_list>() },
	mtl_list{std::make_unique<material_list>()}

{
	set_silent_status();
	std::istringstream iss(obj_file_name);
	std::string file_name;
	std::getline(iss, file_name, '.');
	mtl_file_name = file_name + ".mtl";
	obj_file_ptr = open_file(obj_file_name);
	mtl_file_ptr = open_file(mtl_file_name);
	path = extract_path(obj_file_name);
}

obj_model_reader::obj_model_reader(std::string _obj_file_name,
	std::string _mtl_file_name,
	communicator* _para):
	para{ _para},
	obj_file_name{_obj_file_name},
	mtl_file_name{_mtl_file_name},
	v_num{ 0 }, vt_num{ 0 }, vn_num{ 0 },
	world{ std::make_unique<hittable_list>() },
	mtl_list{ std::make_unique<material_list>() }
{
	set_silent_status();

	obj_file_ptr = open_file(obj_file_name);
	mtl_file_ptr = open_file(mtl_file_name);
	path = extract_path(obj_file_name);
}

obj_model_reader::obj_model_reader(
	std::unique_ptr<std::iostream> _obj_file_ptr,
	std::unique_ptr<std::iostream> _mtl_file_ptr,
	communicator* _para):
	para{ _para},
	obj_file_name{},
	mtl_file_name{},
	v_num{ 0 }, vt_num{ 0 }, vn_num{ 0 },
	world{ std::make_unique<hittable_list>() },
	mtl_list{ std::make_unique<material_list>() },
	obj_file_ptr{ std::move(_obj_file_ptr) },
	mtl_file_ptr{ std::move(_mtl_file_ptr) }
{
	silent = true;
}

obj_model_reader::obj_model_reader(
	std::unique_ptr<std::iostream> _obj_file_ptr,
	std::unique_ptr<std::iostream> _mtl_file_ptr,
	std::unique_ptr<communicator>& _para) :
	obj_model_reader{ std::move(_obj_file_ptr), std::move(_mtl_file_ptr), _para.get()}
{}


void obj_model_reader::read()
{
	if (!silent) {
		std::cout << "Started reading the mtl file " << std::endl;
	}
	read_mtl_file();
	if (!silent) {
		std::cout << "Finished reading the mtl file " << std::endl;
		std::cout << "Started reading the obj file " << std::endl;
	}
	read_obj_file();
	if (!silent) {
		std::cout << "Finished reading the obj file " << std::endl;
		std::cout << "Started adding the items " << std::endl;
	}
	int low, hi;
	set_range(low, hi);
	add_item(low,hi);
	if (!silent)
		std::cout << "Finished adding items to the hittable_list" << std::endl;

}

void obj_model_reader::read_obj_file()
{
	std::string line;

	int file_vs_num = 0, file_vts_num = 0, file_vns_num = 0;
	int file_polygon_num = 0, file_triangle_num = 0;

	face_indx faces;
	int num_polygons = 0;
	int num_triangles = 0;
	int current_mat_indx = -1;
	std::string current_obj_name = "";
	std::string current_group = "";

	while (std::getline(*obj_file_ptr, line))
	{
		int dummy_int;
		std::string dummy_str;
		vec2 pnt2;
		vec3 pnt3;
		std::stringstream iss(line);
		if (iss >> dummy_str)
		{
			// reading the comments section
			if (!dummy_str.compare("#"))
			{
				if (iss >> dummy_int >> dummy_str)
				{
					if (dummy_str == "vertices") {
						file_vs_num = dummy_int;
						check_data(file_vs_num, v_num, "vertices",silent);
						v_num = 0;
					}
					if (dummy_str == "vertex") {
						file_vns_num = dummy_int;
						check_data(file_vns_num, vn_num, "vertex", silent);
						vn_num = 0;
					}
					if (dummy_str == "texture") {
						file_vts_num = dummy_int;
						check_data(file_vts_num, vt_num, "texture", silent);
						vt_num = 0;
					}
						
					if (dummy_str == "polygons")
					{
						file_polygon_num = dummy_int;
						check_data(file_polygon_num, num_polygons, "polygons", silent);
						num_polygons = 0;
						if (iss >> dummy_str >> dummy_int >> dummy_str)
						{
							if (dummy_str == "triangles")
								file_triangle_num = dummy_int;
							{
								file_triangle_num = dummy_int;
								check_data(file_triangle_num, num_triangles, "triangles", silent);
								num_triangles= 0;
							}
						}
					}
				}
			}
			// material library file
			else if (!dummy_str.compare("mtllib"))
			{
				iss >> dummy_str;
				mtl_file_name = std::string(dummy_str);
			}
			// object name
			else if (!dummy_str.compare("o"))
			{
				iss >> dummy_str;
				current_obj_name = dummy_str;
			}
			else if (!dummy_str.compare("g"))
			{
				iss >> dummy_str;
				current_group = dummy_str;
			}
			if (!dummy_str.compare("usemtl"))
			{
				iss >> dummy_str;
				std::string current_mat_name = dummy_str;
				current_mat_indx = mtl_list->find(current_mat_name);
			}
			if (!dummy_str.compare("s"))
			{
				// I do not know what to do with this!
			}
			else if (!dummy_str.compare("f"))
			{
				face_indx face;
				face.num_edges = 0;
				while (iss >> dummy_str)
				{
					int v_indx = -1, vt_indx = -1, vn_indx = -1;
					std::string v_indx_str, vt_indx_str, vn_indx_str;
					char delimiter = '/';
					std::istringstream issr(dummy_str);
					std::getline(issr, v_indx_str,  delimiter);
					std::getline(issr, vt_indx_str, delimiter);
					std::getline(issr, vn_indx_str, delimiter);

					if (!v_indx_str.empty())
						v_indx = std::stoi(v_indx_str);
					if (!vt_indx_str.empty())
						vt_indx = std::stoi(vt_indx_str);
					if (!vn_indx_str.empty())
						vn_indx = std::stoi(vn_indx_str);



					face.v_indx.push_back(v_indx);
					face.vt_indx.push_back(vt_indx);
					face.vn_indx.push_back(vn_indx);

					face.num_edges++;
				}



				std::vector<face_indx> triangulated;
				switch (face.num_edges)
				{
				case 0:
					[[fallthrough]];
				case 1:
					[[fallthrough]];
				case 2:
					if (!silent)
						std::cout << "Ignoring the line " << line << std::endl;
					break;
				case 3:
					num_triangles++;
					face.mat_indx = current_mat_indx;
					face.object = current_obj_name;
					face.group = current_group;
					face_indexes.push_back(face);
					break;
				default:
					face_triangulate(face, triangulated);
					// it is a polygon considered as a series of triangles
					num_polygons++;
					for (auto& triangle : triangulated)
					{
						triangle.object = current_obj_name;
						triangle.group = current_group;
						triangle.mat_indx = current_mat_indx;
						face_indexes.push_back(triangle);
					}
				}
			}
			else if (!dummy_str.compare("v"))
			{
				iss >> pnt3;
				vs.push_back(pnt3);
				v_num++;
			}
			else if (!dummy_str.compare("vt"))
			{
				iss >> pnt2;
				pnt3 = vec3(pnt2);
				vts.push_back(pnt3);
				vt_num++;
			}
			else if (!dummy_str.compare("vn"))
			{
				iss >> pnt3;
				vns.push_back(pnt3);
				vn_num++;
			}
		}
	}
}

void obj_model_reader::read_mtl_file()
{
	std::string line;
	std::stringstream iss;

	double Ns, d, Tr;
	color Ka, Kd, Ks, Tf;
	double Ni = 1.5;
	std::string texture_path = "";
	std::string dummy_str;
	int material_counter = 0;
	bool read_Tr = false;
	std::string material_name;



	std::unique_ptr<material> material_i;
	std::unique_ptr<image_texture> texture;
	std::unique_ptr<std::istream> file_stream;

	while (std::getline(*mtl_file_ptr, line))
	{
		if (line.length() < 3)
			continue;
		std::stringstream iss(line);

		
		if (iss >> dummy_str)
		{
			if (dummy_str == "newmtl")
			{
				if (material_counter)
				{
					if (!read_Tr)
						Tr = 1.0 - d;
					read_Tr = false;
					if (!silent)
						std::cout << "Creating material " << material_name << std::endl;
					
					//if (!texture_path.empty())
						file_stream = open_file(texture_path, path,false);
					
					if (file_stream)
					{
						texture = std::make_unique<image_texture>(texture_path.c_str());
						material_i = std::make_unique<lambertian>(std::move(texture));
					}
					else
					{
						//std::cout << "The file was not found reverting to simple rendering" << std::endl;
						material_i = std::make_unique<general>(Kd, Ns, Tr, Tf, Ks,Ni);
					}
					if (!silent)
						std::cout << "Adding material " << material_name << std::endl;


					// adding the material
					mtl_list->push_back(material_name, std::move(material_i));
				}
				// resetting the texture_path
				texture_path.clear();

				iss >> dummy_str;
				if (!silent)
					std::cout << "Reading the material " << dummy_str << std::endl;
				material_name = dummy_str;
				material_counter++;
			}
			else if (dummy_str == "Ns")
			{
				iss >> Ns;
			}
			else if (dummy_str == "d")
			{
				iss >> d;
			}
			else if (dummy_str == "Tr")
			{
				read_Tr = true;
				iss >> Tr;
				iss.clear();
				iss.str("");
			}
			else if (dummy_str == "Tf")
			{
				iss >> Tf;
			}
			else if (dummy_str == "Ka")
			{
				iss >> Ka;
			}
			else if (dummy_str == "Kd")
			{
				iss >> Kd;
			}
			else if (dummy_str == "Ks")
			{
				iss >> Ks;
			}
			else if (dummy_str == "Ni")
			{
				iss >> Ni;
			}
			else if (dummy_str == "map_Kd")
			{
				iss >> texture_path;
			}
		}
	}

	// Since we add to the materials vector whenever we read a newmtl line the last material would not be added otherwise.
	if (!silent)
		std::cout << "Creating material " << material_name << std::endl;

	if (!read_Tr)
		Tr = 1.0 - d;


	file_stream = open_file(texture_path, path, false);

	if (file_stream)
	{
		texture = std::make_unique<image_texture>(texture_path);
		material_i = std::make_unique<lambertian>(std::move(texture));
	}
	else
	{
		//std::cout << "The file was not found reverting to simple rendering" << std::endl;
		material_i = std::make_unique<general>(Kd, Ns, Tr, Tf, Ks, Ni);
	}
	if (!silent)
		std::cout << "Adding material " << material_name << std::endl;

	mtl_list->push_back(material_name, std::move(material_i));
}

void obj_model_reader::set_range(int& _low, int& _hi) {
	_low = 0;
	_hi = face_indexes.size()-1;
}

void obj_model_reader::add_item(const int& _low, const int& _hi)
{
	int counter = 0;

	int hi = _hi;
	if (hi >= face_indexes.size())
		hi = static_cast<int>(face_indexes.size());


	int num_triangle = 0;
	int num_mesh = 0;
	std::string current_group = "";
	std::string current_obj = "";
	
	for (int i = _low; i < hi; i++)
	{
		std::string group = face_indexes[0].group;
		std::string object = face_indexes[0].object;
		if (!silent && !object.empty() && object.compare(current_obj))
		{
			current_obj = object;
			std::cout << "\t adding object " << object << std::endl;
		}
		if (!silent && !group.empty() && group.compare(current_group))
		{
			current_group = group;
			std::cout << "\t\t adding group " + group << std::endl;
		}
		else if (!silent && group.empty() && counter % 100 == 0)
		{
			std::cout << "\t adding item " << i << " out of " << face_indexes.size() << std::endl;
		}
		std::array<point3, 3> vs_i, vts_i, vns_i;
		std::array<bool, 3> set_vn_i = { true,true,true };
		bool any_missing_vn = false;


		auto& face = face_indexes[i];

		object = face.object;
		group = face.group;

		counter++;
		//std::cout << "Adding the face " << counter << std::endl;
		std::unique_ptr<material> mat;
		int num_edges = face.num_edges;

		for (int j = 0; j < num_edges; j++)
		{
			int v_indx_j = face.v_indx[j];
			int vt_indx_j = face.vt_indx[j];
			int vn_indx_j = face.vn_indx[j];

			if (j >= face.v_indx.size() || j >= face.vt_indx.size() || j >= face.vn_indx.size()) {
				std::cerr << "Out of bounds access for edges" << std::endl;
				continue;
			}
			if (v_indx_j != -1) {
				if (v_indx_j - 1 < 0 || v_indx_j - 1 >= this->vs.size()) {
					std::cerr << "Out of bounds access for v " << v_indx_j << "," << vs.size() << std::endl;
					continue;
				}
			}
			if (vt_indx_j != -1) {
				if (vt_indx_j - 1 < 0 || vt_indx_j - 1 >= this->vts.size()) {
					std::string error_text =
						"Out of bounds access for vt " +
						std::to_string(vt_indx_j) +
						" out of " +
						std::to_string(vts.size());
					std::cerr << error_text << std::endl;
					continue;
				}
			}
			if (vn_indx_j != -1) {
				if (vn_indx_j - 1 < 0 || vn_indx_j - 1 >= this->vns.size()) {
					std::cout << "Out of bounds access for vn " << vn_indx_j << "," << vns.size() << std::endl;
					continue;
				}
			}
			else
			{
				// we estimate it later
				set_vn_i[j] = false;
				any_missing_vn = true;
			}
			
			// if we do not have the vs so there is nothing in this edge!
			vs_i[j] = this->vs[v_indx_j - 1];
			// if the vt has not been mentioned so we do not care about possibly!
			vts_i[j] = (vt_indx_j == -1) ? point3() : this->vts[vt_indx_j - 1];
			// we estimate the vn from the cross section
			vns_i[j] = (vn_indx_j == -1) ? point3() : this->vns[vn_indx_j - 1];
		}
		// mapping from obj material into the mtl file material
		// we ourselves set the mat_indx so it is zero based!
		int mat_indx = face.mat_indx;

		// estimating missing vt vectors
		if (any_missing_vn)
		{
			estimate_vns( vs_i, set_vn_i, vns_i);
		}

		auto trngle = std::make_unique<triangle_mesh>(vs_i, vts_i, vns_i, mat_indx);
		trngle->add_label(object);
		trngle->add_label(group);
		world->add(std::move(trngle));
		num_triangle++;
	}


	// setting all the read objects as main label
	world->add_label("main");


}

std::unique_ptr<hittable_list> obj_model_reader::return_world()
{
	return std::move(world);
}

std::unique_ptr<material_list> obj_model_reader::return_mtl_list()
{
	return std::move(mtl_list);
}

std::unique_ptr<std::istream> obj_model_reader::open_file(std::string& file_name_, std::string path_, bool silent_)
{
	char delimiter;
#ifdef _WIN32
	delimiter = '\\';
#else
	delimiter = '/';
#endif
	if (file_name_.empty())
		return nullptr;
	auto file_ptr =
		std::make_unique<std::ifstream>(file_name_);
	if (!file_ptr->is_open()) {
		file_name_ = path_ + file_name_;
		file_ptr = std::make_unique<std::ifstream>(file_name_);
		std::cout << "Trying opening " << file_name_ << std::endl;
		if (!file_ptr->is_open()) {
			if (!silent_)
				std::cout << "The file " << std::endl
				<< file_name_ << std::endl
				<< "does not exists in the path " << std::endl
				<< std::filesystem::current_path() << std::endl;
		return nullptr;
		}
	}
	return std::move(file_ptr);
}

void obj_model_reader::set_silent_status()
{
	std::array<int, 2> rank_config = para->return_rank_config();
	if (rank_config[0] == 0 && rank_config[1] == 0)
		silent = false;
	else
		silent = true;
}

void obj_model_reader::check_data(const int& num_file_, const int& num_read_, const std::string& title, const bool silent_)
{
	if (!silent_ && num_file_ != num_read_)
		std::cout << "Inconsistency in the number of " << title 
		<< " read vs with those in the obj file  "
		<< num_read_ << "," << num_file_ << std::endl;

}

void obj_model_reader::face_triangulate(
	const face_indx& input_,
	std::vector<face_indx>& output_)
{
	constexpr int triangle_num_edges = 3;
	std::vector<int> v_indx = input_.v_indx;
	std::vector<int> vt_indx = input_.vt_indx;
	std::vector<int> vn_indx = input_.vn_indx;
	int num_edges = input_.num_edges;
	int mat_indx = input_.mat_indx;

	// some safety check!
	if (v_indx.size() != num_edges ||
		vt_indx.size() != num_edges ||
		vn_indx.size() != num_edges)
	{
		std::string error_text =
			"Inconsisten input: v_size==" +
			std::to_string(v_indx.size()) + " Vs\n" +
			"vt_size==" +
			std::to_string(vt_indx.size()) + " Vs\n" +
			"vn_size==" +
			std::to_string(vn_indx.size()) + " Vs\n" +
			"num_edges==" + 
			std::to_string(num_edges) + "\n";
		throw std::invalid_argument(error_text.c_str());
	}

	output_.clear();
	output_.reserve(num_edges - 2);
	std::vector<int> indx_i(triangle_num_edges);
	std::vector<int> v_indx_i(triangle_num_edges);
	std::vector<int> vt_indx_i(triangle_num_edges);
	std::vector<int> vn_indx_i(triangle_num_edges);
	face_indx face_i;


	for (int i = 1; i < num_edges - 1; i++)
	{
		indx_i = { 0,i,i + 1 };
		v_indx_i = { v_indx[indx_i[0]],v_indx[indx_i[1]],v_indx[indx_i[2]] };
		vt_indx_i = { vt_indx[indx_i[0]],vt_indx[indx_i[1]],vt_indx[indx_i[2]] };
		vn_indx_i = { vn_indx[indx_i[0]],vn_indx[indx_i[1]],vn_indx[indx_i[2]] };
		face_i.v_indx = v_indx_i;
		face_i.vt_indx = vt_indx_i;
		face_i.vn_indx = vn_indx_i;
		face_i.num_edges = triangle_num_edges;
		face_i.mat_indx = mat_indx;
		output_.push_back(face_i);
	}
}

void obj_model_reader::estimate_vns(
	const std::array<point3, 3>& vs_,
	const std::array<bool, 3>& set_vn_,
	std::array<point3, 3>& vns_)
{
	constexpr int num_edges = 3;
	for (int i = 0; i < num_edges; i++)
	{
		if (set_vn_[i] == true)
			continue;
		int prev = i ? i - 1 : num_edges - 1;
		int next = (i != num_edges - 1) ? i + 1 : 0;
		vec3 a = vs_[next] - vs_[i];
		vec3 b = vs_[prev] - vs_[i];
		vec3 normal = cross(a, b);
		vec3 unit_normal = unit_vector(normal);
		vns_[i] = unit_normal;
	}
}

std::string obj_model_reader::extract_path(std::string file_name_)
{
	std::stringstream iss(file_name_);
	std::string path;

	char delimiter;

#ifdef _WIN32
	delimiter = '\\';
#else
	delimiter = '/';
#endif
	
	std::vector<std::string> folders;
	std::string folder;

	while (std::getline(iss, folder, delimiter))
	{
		folders.push_back(folder);
	}

	folders.pop_back();

	for (auto& folder : folders)
		path += folder + delimiter;


	return path;
}



