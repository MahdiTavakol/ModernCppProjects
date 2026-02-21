#include "Particles.h"
#include "Engine.h"
#include "Error.h"

Particles::Particles(Engine& engine_) :
	Ref{ engine_,"Particles-1" },
	nmax{ 0 }
{
	x.reserve(0);
	v.reserve(0);
	f.reserve(0);
	m.reserve(0);
}

Particles::Particles(Engine& engine_, int nmax_) :
	Ref{ engine_,"Particles-1" },
	nmax{ nmax_ }
{
	x.reserve(3 * nmax);
	v.reserve(3 * nmax);
	f.reserve(3 * nmax);
	m.reserve(nmax);
}
Particles::Particles(Engine& engine_, std::vector<std::string> args_):
	Ref{ engine_, "Particles-1" }
{
	auto nargs = args_.size();
	if (nargs < 2) {
		engine().getError()->one("Not enough arguments for particles command! Expected at least 2, got " + std::to_string(nargs));
		nmax = 0;
		return;
	}
	nmax = std::stoi(args_[1]);
}

void Particles::addParticle(std::array<double, 3> newX_,
	std::array<double, 3> newV_ ,
	std::array<double, 3> newF_ ,
	double newM_ )
{
	//check the boundary conditions
	// new positions
	x.push_back(newX_[0]);
	x.push_back(newX_[1]);
	x.push_back(newX_[2]);
	// new velocities
	v.push_back(newV_[0]);
	v.push_back(newV_[1]);
	v.push_back(newV_[2]);
	// new forces
	f.push_back(newF_[0]);
	f.push_back(newF_[1]);
	f.push_back(newF_[2]);
	// new masses
	m.push_back(newM_);
}
void Particles::getNmaxNlocal(int& nmax_, int& nlocal_) const {
	nmax_ = nmax; nlocal_ = static_cast<int>(x.size()) / 3;
}
// get atom i position
void Particles::getAtomVec(const int& i,
	std::array<double, 3>& xi,
	std::array<double, 3>& vi,
	std::array<double, 3>& fi,
	double& mi)
{
	xi = std::array<double, 3>
	{ x[3 * i], x[3 * i + 1], x[3 * i + 2] };
	vi = std::array<double, 3>
	{ v[3 * i], v[3 * i + 1], v[3 * i + 2] };
	fi = std::array<double, 3>
	{ f[3 * i], f[3 * i + 1], f[3 * i + 2] };
	mi = m[i];
}