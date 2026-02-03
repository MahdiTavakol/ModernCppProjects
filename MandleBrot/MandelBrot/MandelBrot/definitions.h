#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define FLERR __FILE__, __LINE__
#define ERR __LINE__


class thread_config {
public:
	int threads_x; 
	int threads_y;
	
	thread_config(int _x = 1, int _y = 1) : threads_x(_x), threads_y(_y) {}
	};

enum class Mesh_type { SERIAL, XMESH_INNER_LOOP, XMESH_OUTER_LOOP, YMESH_INNER_LOOP, YMESH_OUTER_LOOP };

#endif