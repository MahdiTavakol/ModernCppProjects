#ifndef PATH_H
#define PATH_H

#include <vector>
#include <string>

#include "../Shared/rtweekend.h"



#include "../Types/vec3.h"
#include "../Types/point3_animated.h"

class path
{
public:
	path(std::vector<point3_animated>&& init_locs_, int& _num_seconds, int& _fps);
	path(const path& pth) = default; // the copy constructor
	path(point3& center, double& radius,  double& theta, int& _num_seconds, int& _fps);
	path(point3 _point); // For backward compatibility with the static case.
	~path() {}

	point3 operator[](int i) const {
		point3 output = locs[i].return_loc();
		return output;
	}

	int return_num_frames();


private:
	int fps,num_frames, num_seconds;

	int num_init_locs;
	std::vector<point3_animated> init_locs; // Locations read from the input file and then they are interpolated based on the frames and put into locs
	std::vector<point3_animated> locs;
	void interpolate_points();
};

#endif
