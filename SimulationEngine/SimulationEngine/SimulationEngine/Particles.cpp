#include "Particles.h"
#include "Engine.h"
#include "Error.h"

// the header for the iota function
#include <numeric>

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
	id.reserve(nmax);
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
	std::vector<double>& rs_,
	int nghosts_) :
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

	nghosts = nghosts_;
	nlocal = nlocal - nghosts;
	resetIds();
}

Particles::Particles(
	int nmax,
	std::vector<int>& id_,
	std::vector<double>& xs_,
	std::vector<double>& vs_,
	std::vector<double>& fs_,
	std::vector<double>& ms_,
	std::vector<double>& rs_,
	int nghosts_) :
	Ref{ "Particles-1" },
	id{ std::move(id_) },
	x{ std::move(xs_) },
	v{ std::move(vs_) },
	f{ std::move(fs_) },
	m{ std::move(ms_) },
	r{ std::move(rs_) }
{
	nlocal = id.size();
	int nlocalX = static_cast<int>(x.size()) / 3;
	int nlocalV = static_cast<int>(v.size()) / 3;
	int nlocalF = static_cast<int>(f.size()) / 3;
	int nlocalM = static_cast<int>(m.size());
	int nlocalR = static_cast<int>(r.size());

	if (
		nlocal != nlocalX ||
		nlocal != nlocalV ||
		nlocal != nlocalF ||
		nlocal != nlocalM ||
		nlocal != nlocalR)
		throw std::invalid_argument("Wrong data size");

	nghosts = nghosts_;
	nlocal = nlocal - nghosts;
}


Particles& Particles::operator=(const Particles& rhs_) {
	if (*this == rhs_)
		return *this;

	nlocal = rhs_.nlocal;
	nghosts = rhs_.nghosts;
	nmax = rhs_.nmax;
	id = rhs_.id;
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
	id = std::move(rhs_.id);
	x = std::move(rhs_.x);
	v = std::move(rhs_.v);
	f = std::move(rhs_.f);
	m = std::move(rhs_.m);
	r = std::move(rhs_.r);

	return *this;
}

void Particles::resetIds()
{
	id.resize(nlocal+nghosts);
	std::iota(id.begin(), id.end(), 0);
}

void Particles::addParticle(
	std::array<double, 3> newX_,
	std::array<double, 3> newV_,
	std::array<double, 3> newF_,
	double newM_,
	double newR_)
{ 
	addParticle(nlocal, newX_, newV_, newF_, newM_, newR_);
}

void Particles::addParticle(
	const int& newId_,
	std::array<double, 3> newX_,
	std::array<double, 3> newV_ ,
	std::array<double, 3> newF_ ,
	double newM_ ,
	double newR_)
{
	// first we add a dummy particle at the end
	addZeroParticle(0);
	//
	// then we copy the first ghost particle
	// to this last position
	int lastPosition = nlocal + nghosts;
	int firstGhost = nlocal;
	copyParticle(firstGhost, lastPosition);
	// the new particle is added to the firstGhost position now
	id[firstGhost] = newId_;
	X(firstGhost, 0) = newX_[0];
	X(firstGhost, 1) = newX_[1];
	X(firstGhost, 2) = newX_[2];
	V(firstGhost, 0) = newV_[0];
	V(firstGhost, 1) = newV_[1];
	V(firstGhost, 2) = newV_[2];
	F(firstGhost, 0) = newF_[0];
	F(firstGhost, 1) = newF_[1];
	F(firstGhost, 2) = newF_[2];
	M(firstGhost) = newM_;
	R(firstGhost) = newR_;
	// the particle has been added 
	// increasing the nlocal
	nlocal++;
}

void Particles::removeParticle(const int& gid_) {
	int ilocal = global2local(gid_);
	// searching for a global id
	// among various particles objects
	// should be the communicator job.
	// The communicator should decide
	// if any of the particles objects
	// have that id_ in it or not
	if (gid_ == -1)
		return;
	
	removeParticleLocalId(ilocal);
}

void Particles::removeParticleLocalId(const int& id_)
{
	int lastGhost = nlocal + nghosts - 1;
	int lastLocal = nlocal - 1;
	// if it is a local atom
	if (id_ < nlocal) {
		// copying the last particle to the particle with id value of id_
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
	else
		throw std::invalid_argument("The particle was not found");
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
	Id(tgt_) = Id(src_);
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
	id.pop_back();
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

void Particles::addZeroParticle(const int id_)
{
	id.push_back(id_);
	x.push_back(0.0); x.push_back(0.0); x.push_back(0.0);
	v.push_back(0.0); v.push_back(0.0); v.push_back(0.0);
	f.push_back(0.0); f.push_back(0.0); f.push_back(0.0);
	r.push_back(0.0); m.push_back(0.0);
}

void Particles::updateGhostParticle(
	const int gid,
	std::array<double, 3> newX_,
	std::array<double, 3> newV_,
	std::array<double, 3> newF_,
	double newM_,
	double newR_)
{
	int i = this->global2local(gid);

	// checking three different situations
	// first if it is not found -->> add a new empty line and update i to the last local id
	// second if it is found --->>> update its values
	// third if it is local atoms -->> do nothing!
	if (i == -1) {
		this->addZeroParticle(gid);
		i = nlocal + nghosts;
		nghosts++;
	}
	else if (i < nlocal) {
		// do nothing
		return;
	}


	X(i, 0) = newX_[0];
	X(i, 1) = newX_[1];
	X(i, 2) = newX_[2];
	V(i, 0) = newV_[0];
	V(i, 1) = newV_[1];
	V(i, 2) = newV_[2];
	F(i, 0) = newF_[0];
	F(i, 1) = newF_[1];
	F(i, 2) = newF_[2];
	M(i) = newM_;
	R(i) = newR_;
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

int Particles::global2local(const int& gid_)
{
	// finding the gid_ in the id vector
	auto iter = std::find(id.begin(), id.end(), gid_);
	// checking if it presents
	if (iter == id.end())
		return -1;
	int dist = std::distance(id.begin(), iter);
	return dist;
}

void Particles::swapParticles(const int& id1, const int& id2)
{
	std::swap(id[id1], id[id2]);
	for (int j = 0; j < 3; j++) {
		std::swap(x[3 * id1 + j], x[3 * id2 + j]);
		std::swap(v[3 * id1 + j], v[3 * id2 + j]);
		std::swap(f[3 * id1 + j], f[3 * id2 + j]);
	}
	std::swap(r[id1], r[id2]);
	std::swap(m[id1], m[id2]);
}

std::vector<double> Particles::packParticleData(const int& id)
{
	std::vector<double> out;
	double gid = static_cast<double>(Id(id));
	double x0 = X(id, 0);
	double x1 = X(id, 1);
	double x2 = X(id, 2);
	double v0 = V(id, 0);
	double v1 = V(id, 1);
	double v2 = V(id, 2);
	double f0 = F(id, 0);
	double f1 = F(id, 1);
	double f2 = F(id, 2);
	double m = M(id);
	double r = R(id);

	out.push_back(gid);
	out.push_back(x0);
	out.push_back(x1);
	out.push_back(x2);
	out.push_back(v0);
	out.push_back(v1);
	out.push_back(v2);
	out.push_back(f0);
	out.push_back(f1);
	out.push_back(f2);
	out.push_back(m);
	out.push_back(r);

	return out;
}

void Particles::unpackParticleData( std::vector<double>& data)
{
	std::array<double, 3> newX, newV, newF;
	double newR, newM;

	int loc = 0;

	int gid = static_cast<int>(data[loc++]);
	newX[0] = data[loc++];
	newX[1] = data[loc++];
	newX[2] = data[loc++];
	newV[0] = data[loc++];
	newV[1] = data[loc++];
	newV[2] = data[loc++];
	newF[0] = data[loc++];
	newF[1] = data[loc++];
	newF[2] = data[loc++];
	newM = data[loc++];
	newR = data[loc++];

	updateGhostParticle(gid, newX, newV, newF, newM, newR);
}