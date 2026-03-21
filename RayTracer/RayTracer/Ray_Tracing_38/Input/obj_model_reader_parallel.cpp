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

void obj_model_reader_parallel::set_range(int& low_, int& hi_) {
	int size_per_rank = static_cast<int>(face_indexes.size()) / static_cast<int>(para->return_size()) + 1;
	int ilo = para->return_rank() * size_per_rank;
	int ihi = ilo + size_per_rank;
	if (ihi >= face_indexes.size()) ihi = static_cast<int>(face_indexes.size()) - 1;
}



