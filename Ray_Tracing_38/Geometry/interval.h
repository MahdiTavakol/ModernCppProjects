#ifndef INTERVAL_H
#define INTERVAL_H

#include "../Shared/rtweekend.h"

class interval {
public:
	double min, max;

	interval();

	interval(double _min, double _max);

	interval(const interval& a, const interval& b);

	double size() const;

	double mid() const;

	bool contains(double x) const;

	bool surrounds(double x) const;

	double clamp(double x) const;

	interval expand(double delta) const;

	interval operator+(double _displacement);

	static const interval empty, universe;
};




#endif