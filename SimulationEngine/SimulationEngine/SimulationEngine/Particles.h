#pragma once

#include "Engine.h"
#include "Ref.h"

#include <vector>
#include <array>
#include <iostream>


class Particles : public Ref {
public:
	Particles();
	Particles(int nmax_);
	Particles(std::vector<std::string> args_);
	Particles(
		int nmax,
		std::vector<double>& xs_,
		std::vector<double>& vs_,
		std::vector<double>& fs_,
		std::vector<double>& ms_,
		std::vector<double>& rs_,
		int nghosts_ = 0);
	Particles(
		int nmax,
		std::vector<int>& ids_,
		std::vector<double>& xs_,
		std::vector<double>& vs_,
		std::vector<double>& fs_,
		std::vector<double>& ms_,
		std::vector<double>& rs_,
		int nghosts_ = 0);
	Particles& operator=(const Particles& rhs_);
	Particles& operator=(Particles&& rhs_);
	// resetting the global ids of particles
	void resetIds();
	void addParticle(
		std::array<double, 3> newX_,
		std::array<double, 3> newV_ = { 0.0,0.0,0.0 },
		std::array<double, 3> newF_ = { 0.0,0.0,0.0 },
		double newM_ = 10.0, 
		double newR_ = 0.0);
	void addParticle(
		const int& id,
		std::array<double,3> newX_,
		std::array<double, 3> newV_ = { 0.0,0.0,0.0 },
		std::array<double, 3> newF_ = { 0.0,0.0,0.0 },
		double newM_ = 10.0,
		double newR_ = 0.0);
	void removeParticle(const int& gid_);
	void removeParticleLocalId(const int& id_);
	void getParticle(const int& id_,
		std::array<double, 3>& newX_,
		std::array<double, 3>& newV_,
		std::array<double, 3>& newF_,
		double& newM_,
		double& newR_);

	void copyParticle(Particles* other_, const int& id_);
	// src_ and tgt_ are local ids
	void copyParticle(const int& src_, const int& tgt_);
	void pop_back();
	void setNmax(const int& nmax_);
	void setNGhosts(const int& nghosts_);
	void setNmaxNlocal(const int& nmax_, const int& nlocal_);
	void getNmaxNlocal(int& nmax_, int& nlocal_) const;
	void getNGhosts(int& nghosts_) const;
	// adding a zero line at the end of each container
	void addZeroParticle(const int id_);
	// updating a particle
	void updateGhostParticle(const int gid,
						std::array<double,3> newX_,
						std::array<double,3> newV_,
						std::array<double,3> newF_,
						double M_,
						double R_);
	// get atom i position
	void getAtomVec(const int& i,
		std::array<double, 3>& xi,
		std::array<double, 3>& vi,
		std::array<double, 3>& fi,
		double& mi);
	// global to local id conversion
	// the global id is saved in the id vector
	// while the local id is the order of the particle
	// in each vector
	int global2local(const int& gid_);
	// swapping two particles
	void swapParticles(const int& id1, const int& id2);
	// packing the data for the ghost transfer
	std::vector<double> packParticleData(const int& id);
	void unpackParticleData(std::vector<double>& data);

	// used int* const on purpose to avoid the 
	// memory managed by vector to be destructed or changed
	int*    const getIdData() { return id.data(); }
	double* const getXData()  { return x.data(); }
	double* const getVData()  { return v.data(); }
	double* const getFData()  { return f.data(); }
	double* const getMData()  { return m.data(); }
	double* const getRData()  { return r.data(); }
	inline int& Id(const int& i) {
		return id[i];
	}
	inline double& X(const int& i, const int& j) {
		return x[3 * i + j];
	}
	inline double& V(const int& i, const int& j) {
		return v[3 * i + j];
	}
	inline double& F(const int& i, const int& j) {
		return f[3 * i + j];
	}
	inline double& M(const int& i) {
		return m[i];
	}
	inline double& R(const int& i) {
		return r[i];
	}
	std::vector<double>& xRef() {
		return x;
	}

	static const int dataPerParticle = 12;

protected:
	int nlocal;
	int nghosts;
	int nmax;
	std::vector<int> id;
	std::vector<double> x;
	std::vector<double> v;
	std::vector<double> f;
	std::vector<double> m;
	std::vector<double> r;

};

