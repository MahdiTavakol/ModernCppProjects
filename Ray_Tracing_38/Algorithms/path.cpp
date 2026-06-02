#include <fstream>
#include <sstream>


#include "path.h"


path::path(std::vector<point3_animated>&& init_locs_, int& _num_seconds, int& _fps) : 
	init_locs{std::move(init_locs_)}, num_seconds(_num_seconds), fps(_fps)
{
	num_frames = num_seconds * fps;
	interpolate_points();
}

path::path(point3& _center, double& _radius, double& theta, int& _num_seconds, int& _fps): num_seconds(_num_seconds), fps(_fps)
{
	double theta_radians = pi * theta / 180.0;
	num_frames = num_seconds * fps;
	double speed = theta_radians / num_frames;

	for (int i = 0; i < num_frames; i++)
	{
		double second = i * fps;
		double _x = _center.x() - _radius * sin(static_cast<double>(i) * speed);
		double _y = _center.y();
		double _z = _center.z() + _radius * cos(static_cast<double>(i) * speed);
		point3_animated _loc(_x, _y, _z,second);
		locs.push_back(_loc);
	}
}

path::path(point3 _point):
	fps{ 0 }, num_seconds{ 0 }, num_frames{ 1 }
{
	point3_animated _loc(_point, 0);
	locs.push_back(_loc);
	init_locs.push_back(_loc);
}


void path::interpolate_points()
{
	int i = 0;  // counter for frames
	int j = 0;  // counter for init_seconds

	for (; i < num_frames; i++)
	{
		vec3 point;

		double time = static_cast<double>(i) / static_cast<double>(fps);
		double init_time = init_locs[j].return_time();

		// Finding the first frame in the input file that is bigger than the frame i to be used in the interpolation 
		while (init_time <= time)
		{
			if (std::abs(init_time-time) < 0.001)
			{
				point3_animated _loc(init_locs[j]);
				locs.push_back(_loc);
				continue;
			}
			j++;
			if (j < num_init_locs)
				init_time = init_locs[j].return_time();
			else
				break;
		}
		// the cases where extra polation is required
		if (j == num_init_locs) j -= 2;
		else if (j == 0) j++;
		// interpolation points
		point3_animated init_loc_a = init_locs[j - 1];
		point3_animated init_loc_b = init_locs[j];
		point3_animated _loc = interpolate(init_loc_a, init_loc_b, time);
		locs.push_back(_loc);
	}
}

int path::return_num_frames()
{
	return num_frames;
}
