#include "Particles.h"
#include "Engine.h"
#include "Error.h"

Particles::Particles() :
	Ref{"Particles-1" },
	nlocal{ 0 },
	nmax{ 0 }
{
	/*
	x.reserve(0);
	v.reserve(0);
	f.reserve(0);
	m.reserve(0);
	r.reserve(0);
	*/
}

Particles::Particles(int nmax_) :
	Ref{ "Particles-1" },
	nlocal{0},
	nmax{ nmax_ }
{
	x.reserve(3 * nmax);
	v.reserve(3 * nmax);
	f.reserve(3 * nmax);
	m.reserve(nmax);
	r.reserve(nmax);
}

Particles::Particles(std::vector<std::string> args_):
	Ref{"Particles-1" }
{
	auto nargs = args_.size();
	if (nargs < 2) {
		throw std::invalid_argument("Not enough arguments for particles command! Expected at least 2, got " + std::to_string(nargs));
		nmax = 0;
		return;
	}
	nmax = std::stoi(args_[1]);
	nlocal = 0;
}

Particles::Particles(
	int nmax,
	std::vector<double>& xs_,
	std::vector<double>& vs_,
	std::vector<double>& fs_,
	std::vector<double>& ms_,
	std::vector<double>& rs_) :
	Ref{ "Particles-1" },
	x{ std::move(xs_) },
	v{ std::move(vs_) },
	f{ std::move(fs_) },
	m{ std::move(ms_) },
	r{ std::move(rs_) }

{
	nlocal  = static_cast<int>(x.size()) / 3;
	int nlocalV = static_cast<int>(v.size()) / 3;
	int nlocalF = static_cast<int>(f.size()) / 3;
	int nlocalM = static_cast<int>(m.size());
	int nlocalR = static_cast<int>(r.size());

	if (nlocal != nlocalV ||
		nlocal != nlocalF ||
		nlocal != nlocalM ||
		nlocal != nlocalR)
		throw std::invalid_argument("Wrong data size");
}


Particles& Particles::operator=(const Particles& rhs_) {
	if (*this == rhs_)
		return *this;

	nlocal = rhs_.nlocal;
	nghosts = rhs_.nghosts;
	nmax = rhs_.nmax;
	x = rhs_.x;
	v = rhs_.v;
	f = rhs_.f;
	m = rhs_.m;
	r = rhs_.r;

	return *this;
}

Particles& Particles::operator=(Particles&& rhs_) {
	if (*this == rhs_)
		return *this;

	nlocal = rhs_.nlocal;
	nghosts = rhs_.nghosts;
	nmax = rhs_.nmax;
	x = std::move(rhs_.x);
	v = std::move(rhs_.v);
	f = std::move(rhs_.f);
	m = std::move(rhs_.m);
	r = std::move(rhs_.r);

	return *this;
}


void Particles::addParticle(std::array<double, 3> newX_,
	std::array<double, 3> newV_ ,
	std::array<double, 3> newF_ ,
	double newM_ ,
	double newR_)
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
	// new radiuses
	r.push_back(newR_);
	// increasing the nlocal
	nlocal++;
}

void Particles::removeParticle(const int& id_) {
	int lastGhost = nlocal + nghosts - 1;
	int lastLocal = nlocal - 1;
	// if it is a local atom
	if (id_ < nlocal) {
		// copying the last particle to the id_ location
		copyParticle(lastLocal, id_);
		// copying the last ghost into the last particle location
		copyParticle(lastGhost, lastLocal);
		// decreasing the nlocal
		nlocal--;
	}
	else if (id_ < nlocal + nghosts) {
		// copying the last ghost into the id_ location
		copyParticle(lastGhost, id_);
		// reducing the nghosts
		nghosts--;
	}
	// removing the last particle
	pop_back();
}

void Particles::getParticle(const int& id_,
	std::array<double, 3>& newX_,
	std::array<double, 3>& newV_,
	std::array<double, 3>& newF_,
	double& newM_,
	double& newR_)
{
	newX_[0] = X(id_, 0);
	newX_[1] = X(id_, 1);
	newX_[2] = X(id_, 2);
	newV_[0] = V(id_, 0);
	newV_[1] = V(id_, 1);
	newV_[2] = V(id_, 2);
	newF_[0] = F(id_, 0);
	newF_[1] = F(id_, 1);
	newF_[2] = F(id_, 2);
	newM_ = M(id_);
	newR_ = R(id_);
}

void Particles::copyParticle(Particles* other_,
	                        const int& id_)
{
	std::array<double, 3> newX, newV, newF;
	double newM, newR;
	other_->getParticle(id_, newX, newV, newF, newM, newR);
	this->addParticle(newX, newV, newF, newM, newR);

	if (nmax == 0)
		nmax = 1;
	while (nlocal > nmax) {
		nmax *= 2;
		x.reserve(3 * nmax);
		v.reserve(3 * nmax);
		f.reserve(3 * nmax);
		m.reserve(nmax);
		r.reserve(nmax);
	}
}

void Particles::copyParticle(const int& src_, const int& tgt_)
{
	X(tgt_, 0) = X(src_, 0);
	X(tgt_, 1) = X(src_, 1);
	X(tgt_, 2) = X(src_, 2);
	V(tgt_, 0) = V(src_, 0);
	V(tgt_, 1) = V(src_, 1);
	V(tgt_, 2) = V(src_, 2);
	F(tgt_, 0) = F(src_, 0);
	F(tgt_, 1) = F(src_, 1);
	F(tgt_, 2) = F(src_, 2);
	R(tgt_) = R(src_);
	M(tgt_) = M(src_);
}

void Particles::pop_back()
{
	for (int i = 0; i < 3; i++)
	{
		x.pop_back();
		v.pop_back();
		f.pop_back();
	}
	r.pop_back();
	m.pop_back();
}

void Particles::setNmax(const int& nmax_) {
	this->nmax = nmax_;
}

void Particles::setNGhosts(const int& nghosts_) {
	nghosts = nghosts_;
}

void Particles::setNmaxNlocal(const int& nmax_, const int& nlocal_)
{
	nlocal = nlocal_;
	nmax = nmax_;
}

void Particles::getNmaxNlocal(int& nmax_, int& nlocal_) const {
	nlocal_ = nlocal;
	nmax_ = nmax;
}

void Particles::getNGhosts(int& nghosts_) const
{
	nghosts_ = nghosts;
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