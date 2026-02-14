#pragma once

#include "Engine.h"
#include "Type.h"

#include <vector>
#include <array>

using std::vector, std::array;

class Particles : public Type {
public:
	Particles(Engine& engine_, int nmax_);
	void addParticle(array<double, 3> newX_,
		array<double, 3> newV_ = { 0.0,0.0,0.0 },
		array<double, 3> newF_ = { 0.0,0.0,0.0 },
		double newM_ = 10.0);
	void getNmaxNlocal(int& nmax_, int& nlocal_) const;
	// get atom i position
	void getAtomVec(const int& i,
		array<double, 3>& xi,
		array<double, 3>& vi,
		array<double, 3>& fi,
		double& mi);
	// used int* const on purpose to avoid the 
	// memory managed by vector to be destructed or changed
	double* const getXData() { return x.data(); }
	double* const getVData() { return v.data(); }
	double* const getFData() { return f.data(); }
	double* const getMData() { return m.data(); }

private:
	int nmax;
	vector<double> x;
	vector<double> v;
	vector<double> f;
	vector<double> m;
};

