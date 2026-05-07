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
	int num_edges = 0;
	int num_polygons = 0;
	int num_triangles = 0;
	int current_mat_indx = -1;

	while (std::getline(*obj_file_ptr, line))
	{
		int dummy_int;
		std::string dummy_str;
		vec3 point3;
		if (line.length() >= 3)
		{
			std::stringstream iss(line);
			iss >> dummy_str;
			if (!dummy_str.compare("#"))
			{
				if (iss >> dummy_int >> dummy_str)
				{
					if (dummy_str == "vertices") {
						file_vs_num = dummy_int;
						check_data(file_vs_num, v_num, "vertices");
						v_num = 0;
					}
					if (dummy_str == "vertex") {
						file_vns_num = dummy_int;
						check_data(file_vns_num, vn_num, "vertex");
						vn_num = 0;
					}
					if (dummy_str == "texture") {
						file_vts_num = dummy_int;
						check_data(file_vts_num, vt_num, "texture");
						vt_num = 0;
					}
						
					if (dummy_str == "polygons")
					{
						file_polygon_num = dummy_int;
						check_data(file_polygon_num, num_polygons, "polygons");
						num_polygons = 0;
						if (iss >> dummy_str >> dummy_int >> dummy_str)
						{
							if (dummy_str == "triangles")
								file_triangle_num = dummy_int;
							{
								file_triangle_num = dummy_int;
								check_data(file_triangle_num, num_triangles, "polygons");
								num_triangles= 0;
							}
						}
					}
				}
			}
			if (!dummy_str.compare("mtllib"))
			{
				iss >> dummy_str;
				mtl_file_name = std::string(dummy_str);
			}
			if (!dummy_str.compare("o"))
			{
				iss >> dummy_str;
			}
			if (!dummy_str.compare("g"))
			{
				iss >> dummy_str;
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
				int num_edges = 0;
				while (iss >> dummy_str)
				{
					int v_indx, vt_indx, vn_indx;
					char delimiter = '/';
					std::istringstream issr(dummy_str);
					std::getline(issr, dummy_str, delimiter);
					v_indx = std::stoi(dummy_str);
					std::getline(issr, dummy_str, delimiter);
					vt_indx = std::stoi(dummy_str);
					std::getline(issr, dummy_str, delimiter);
					vn_indx = std::stoi(dummy_str);



					face.v_indx.push_back(v_indx);
					face.vt_indx.push_back(vt_indx);
					face.vn_indx.push_back(vn_indx);

					num_edges++;
				}

				bool isTriangle = false;
				bool isPolygon = false;

				switch (num_edges)
				{
				case 3:
					num_triangles++;
					isTriangle = true;
					break;
				case 4:
					num_polygons++;
					isPolygon = true;
					break;
					/*default:
						std::cerr << "Error in reading the object file " << line << " , " <<  num_edges << std::endl;*/
				}
				if (isTriangle || isPolygon) {
					face.num_edges = num_edges;
					face.mat_indx = current_mat_indx;
					face_indexes.push_back(face);
				}
			}
			if (iss >> point3)
			{
				if (!dummy_str.compare("vt"))
				{
					vts.push_back(point3);
					vt_num++;
				}
				else if (!dummy_str.compare("vn"))
				{
					vns.push_back(point3);
					vn_num++;
				}
				else if (!dummy_str.compare("v"))
				{
					vs.push_back(point3);
					v_num++;
				}
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
	std::string dummy_str;
	int material_counter = 0;
	std::string material_name;
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
					std::array<int, 2> rank_config;
					rank_config = para->return_rank_config();
					if (rank_config == std::array<int, 2>{ 0,0 })
						std::cout << "Creating material " << material_name << std::endl;
					std::unique_ptr<material>  material_i = std::make_unique<general>(Kd, Ns, d, Tr, Tf, Ks);


					if (rank_config == std::array<int, 2>{ 0, 0 })
					std::cout << "Adding material " << material_name << std::endl;
					if (mtl_list == nullptr)
						std::cout << "Null" << std::endl;

					std::unique_ptr<material> mat = std::make_unique<metal>();
					//mtl_list->push_back(material_name, std::move(material_i));
					mtl_list->push_back(material_name, std::move(mat));
				}
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

		}
	}

	// Since we add to the materials vector whenever we read a newmtl line the last material would not be added otherwise.
	std::cout << "Creating material " << material_name << std::endl;
	std::unique_ptr<material> material_i = std::make_unique<general>(Kd, Ns, d, Tr, Tf, Ks);
	std::cout << "Adding material " << material_name << std::endl;

	std::unique_ptr<material> mat = std::make_unique<metal>();
	//mtl_list->push_back(material_name, std::move(material_i));
	mtl_list->push_back(material_name, std::move(mat));
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
	
	for (int i = _low; i < hi; i++)
	{
		if (counter % 100 == 0 && !silent)
			std::cout << "item " << counter << " out of " << face_indexes.size() << std::endl;
		std::array<point3, 4> vs_i, vts_i, vns_i;
		std::array<point3, 3> vs_d, vts_d, vns_d;
		vs_d = std::array<point3, 3>{ point3{0.0,0.0,0.0} };
		vts_d = vs_d;
		vns_d = vs_d;

		auto& face = face_indexes[i];

		counter++;
		//std::cout << "Adding the face " << counter << std::endl;
		std::unique_ptr<material> mat;
		int num_edges = face.num_edges;

		for (int j = 0; j < num_edges; j++)
		{
			if (j >= face.v_indx.size() || j >= face.vt_indx.size() || j >= face.vn_indx.size()) {
				std::cerr << "Out of bounds access for edges" << std::endl;
				continue;
			}
			if (face.v_indx[j] - 1 < 0 || face.v_indx[j] - 1 >= this->vs.size()) {
				std::cerr << "Out of bounds access for v " << face.vt_indx[j] << "," << vs.size() << std::endl;
				continue;
			}
			if (face.vt_indx[j] - 1 < 0 || face.vt_indx[j] - 1 >= this->vts.size()) {
				std::cerr << "Out of bounds access for vt " << face.vt_indx[j] << "," << vts.size() << std::endl;
				continue;
			}
			if (face.vn_indx[j] - 1 < 0 || face.vn_indx[j] - 1 >= this->vns.size()) {
				std::cout << "Out of bounds access for vn " << face.vn_indx[j] << "," << vns.size() << std::endl;
				std::cout << face.vt_indx[j] << ">=" << this->vns.size() << std::endl;
				continue;
			}
			point3 v_j = this->vs[face.v_indx[j] - 1]; // 1 based indexing in the obj file
			point3 vt_j = this->vts[face.vt_indx[j] - 1];
			point3 vn_j = this->vns[face.vn_indx[j] - 1];
			vs_i[j] = v_j;
			vts_i[j] = vt_j;
			vns_i[j] = vn_j;
		}
		// mapping from obj material into the mtl file material
		// we ourselves set the mat_indx so it is zero based!
		int mat_indx = face.mat_indx;


		switch (num_edges)
		{
		case 3:
			std::copy_n(vs_i.data(), 3, vs_d.data());
			std::copy_n(vts_i.data(), 3, vts_d.data());
			std::copy_n(vns_i.data(), 3, vns_d.data());
			world->add(std::make_unique<triangle_mesh>(vs_d, vts_d, vns_d,mat_indx));
			num_triangle++;
			break;
		case 4:
			world->add(std::make_unique<mesh>(vs_i, vts_i, vns_i, mat_indx));
			num_mesh++;
			break;
		}
	}





	//world->add(std::make_unique<sphere>(point3(0, 38, 0), 20,1));
}

std::unique_ptr<hittable_list> obj_model_reader::return_world()
{
	return std::move(world);
}

std::unique_ptr<material_list> obj_model_reader::return_mtl_list()
{
	return std::move(mtl_list);
}

std::unique_ptr<std::istream> obj_model_reader::open_file(std::string file_name_)
{
	auto file_ptr =
		std::make_unique<std::ifstream>(file_name_);
	if (!file_ptr->is_open())
		std::cout << "The file " << std::endl
		<< file_name_ << std::endl
		<< "does not exists in the path " << std::endl
		<< std::filesystem::current_path() << std::endl;
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

void obj_model_reader::check_data(const int& num_file_, const int& num_read_, const std::string& title)
{
	if (num_file_ != num_read_)
		std::cout << "Inconsistency in the number of " << title 
		<< " read vs with those in the obj file  "
		<< num_file_ << "," << num_read_ << std::endl;

}



