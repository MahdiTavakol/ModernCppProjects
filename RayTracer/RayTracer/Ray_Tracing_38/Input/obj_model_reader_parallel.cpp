#include "obj_model_reader_parallel.h"



obj_model_reader_parallel::obj_model_reader_parallel(
	std::string _obj_file_name,
	parallel* para_) :
	para{para_}
{
	rank = para->return_rank();
	if (rank == 0)
		silent = false;
	else
		silent = true;
}


void obj_model_reader_parallel::add_item()
{
	int counter = 0;


	int size_per_rank = static_cast<int>(face_indexes.size()) / static_cast<int>(para->return_size()) + 1;
	int ilo = para->return_rank() * size_per_rank;
	int ihi = ilo + size_per_rank;
	if (ihi >= face_indexes.size()) ihi = static_cast<int>(face_indexes.size()) - 1;


	//for (auto face : face_indexes)
	for (int i = ilo; i < ihi; i++)
	{
		auto face = face_indexes[i];
		counter++;
		//std::cout << "Adding the face " << counter << std::endl;
		std::unique_ptr<material> mat;
		int num_edges = face.num_edges;

		std::vector<point3> vs_i;
		std::vector<point3> vts_i;
		std::vector<point3> vns_i;

		for (int j = 0; j < num_edges; j++)
		{
			if (j >= face.vt_indx.size()) {
				std::cerr << "1-Out of bonds access from the rank " << para->return_rank() << std::endl;
				continue;
			}
			if (face.vt_indx[j] - 1 < 0 || face.vt_indx[j] - 1 >= this->vs.size()) {
				std::cerr << "2-Out of bonds access from the rank " << para->return_rank() << std::endl;
				continue;
			}
			if (face.vt_indx[j] - 1 >= this->vts.size()) {
				std::cerr << "3-Out of bonds access from the rank " << para->return_rank() << std::endl;
				continue;
			}
			if (face.vt_indx[j] - 1 >= this->vns.size()) { 
				std::cout << "4-Out of bonds access from the rank " << para->return_rank() << std::endl;
				continue;
			}
			point3 v_j = this->vs[face.vt_indx[j] - 1]; // 1 based indexing in the obj file
			point3 vt_j = this->vts[face.vt_indx[j] - 1];
			point3 vn_j = this->vns[face.vt_indx[j] - 1];
			vs_i.push_back(v_j);
			vts_i.push_back(vt_j);
			vns_i.push_back(vn_j);
		}

		if (face.mat_indx > materials.size())
			std::cout << "Accessing out of the range for " <<
			face.mat_indx << "," << materials.size() << std::endl;
		general* material_conv = dynamic_cast<general*>(materials[face.mat_indx].get());
		if (material_conv == nullptr)
			std::cout << "Wrong material type" << std::endl;
		else if(material_conv != nullptr)
			mat = std::make_unique<general>(*material_conv);


		switch (num_edges)
		{
		case 3:
			world->add(std::make_unique<triangle_mesh>(vs_i, vts_i, vns_i, std::move(mat)));
			break;
		case 4:
			world->add(std::make_unique<mesh>(vs_i, vts_i, vns_i,std::move(mat)));
			break;
		}
	}

	if (para->return_rank() == 0)
		std::cout << "End of the object_model_reader_parallel" << std::endl;

	para->barrier();
	if (para->return_rank() == 0)
		std::cout << "Every threads has passed the barrier" << std::endl;
}



