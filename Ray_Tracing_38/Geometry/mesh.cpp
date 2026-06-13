#include "mesh.h"
#include <mpi.h>



mesh::mesh(
	const std::array<point3, 4>& vs_,
	const std::array<point3, 4>& vts_,
	const std::array<point3, 4>& vns_,
	const int mat_indx_,
	std::string type_ ):
	hittable{type_,mat_indx_},
	vs{ vs_ }, vts{ vts_ },
	vns{ vns_ },
	num_edges{ 4 }
{
	initialize();
}


mesh::mesh(const int mat_indx, std::string type_):
	hittable{type_, mat_indx}
{}

void mesh::initialize()
{
	//breaking the mesh to two triangles across the 0-2 direction
	auto n1a = cross(vs[1] - vs[0], vs[2] - vs[0]);
	auto n2a = cross(vs[2] - vs[0], vs[3] - vs[0]);
	vec3 unit_n1a = unit_vector(n1a);
	vec3 unit_n2a = unit_vector(n2a);
	auto dot_a = dot(unit_n1a, unit_n2a);
	//breaking the mesh to two triangles across the 1-3 direction
	auto n1b = cross(vs[0] - vs[1], vs[3] - vs[1]);
	auto n2b = cross(vs[3] - vs[1], vs[2] - vs[1]);
	vec3 unit_n1b = unit_vector(n1b);
	vec3 unit_n2b = unit_vector(n2b);
	auto dot_b = dot(unit_n1b, unit_n2b);
	// comparing them
	n1 = dot_a > dot_b ? n1a : n1b;
	n2 = dot_a > dot_b ? n2a : n2b;
	unit_n1 = dot_a > dot_b ? unit_n1a : unit_n1b;
	unit_n2 = dot_a > dot_b ? unit_n2a : unit_n2b;
	Q1 = Q2 = dot_a > dot_b ? vs[0] : vs[1];
	edge11 = dot_a > dot_b ? vs[1] - vs[0] : vs[0] - vs[1];
	edge21 = dot_a > dot_b ? vs[2] - vs[0] : vs[3] - vs[1];
	edge12 = dot_a > dot_b ? vs[2] - vs[0] : vs[2] - vs[1];
	edge22 = dot_a > dot_b ? vs[3] - vs[0] : vs[3] - vs[1];
	// ind?[0] --> gamma, ind?[1] -->alpha, ind?[2] -->beta
	ind1 = dot_a > dot_b ? std::array<int, 3>{0, 1, 2} : std::array<int, 3>{ 1,0,3 };
	ind2 = dot_a > dot_b ? std::array<int, 3>{0, 2, 3} : std::array<int, 3>{ 1,2,3 };
	D1 = dot(unit_n1, Q1);
	D2 = dot(unit_n2, Q2);
	w1 = n1 / dot(n1, n1);
	w2 = n2 / dot(n2, n2);



	set_bounding_box();
}

void mesh::set_bounding_box()
{
	auto bbox_1 = aabb(vs[0], vs[1]);
	auto bbox_2 = aabb(vs[2], vs[3]);
	bbox = aabb(bbox_1, bbox_2);
}

bool mesh::hit(const ray& _r, interval _ray_t, hit_record& _rec) const
{
	auto denom1 = dot(unit_n1, _r.direction());
	auto denom2 = dot(unit_n2, _r.direction());

	double ts[2] =
	{
		0.0,0.0
	};

	// which one did it hit
	int hitme[2] = { 0,0 };
	bool canhitme[2] = { false,false };

	if (std::fabs(denom1) >= 1e-8)
		canhitme[0] = true;
	if (std::fabs(denom2) >= 1e-8)
		canhitme[1] = true;

	if (!(canhitme[0] || canhitme[1]))
		return false;

	if (canhitme[0]) {
		ts[0] = (D1 - dot(unit_n1, _r.origin())) / denom1;
	}
	else
	{
		ts[0] = std::numeric_limits<double>::infinity();
	}
	if (canhitme[1]) {
		ts[1] = (D2 - dot(unit_n2, _r.origin())) / denom2;
	}
	else
	{
		ts[1] = std::numeric_limits<double>::infinity();
	}
	
	if (!_ray_t.contains(ts[0]) && !_ray_t.contains(ts[1]))
		return false;


	point3 intersections[2];
	double alphas[2], betas[2];

	// checking the first triangle
	if (canhitme[0])
	{
		intersections[0] = _r.at(ts[0]);
		vec3 planar_hitpt_vector = intersections[0] - Q1;
		alphas[0] = dot(w1, cross(planar_hitpt_vector, edge21));
		betas[0] = dot(w1, cross(edge11, planar_hitpt_vector));
	}
	// checking the second triangle
	if (canhitme[1])
	{
		intersections[1] = _r.at(ts[1]);
		vec3 planar_hitpt_vector = intersections[1] - Q2;
		alphas[1] = dot(w2, cross(planar_hitpt_vector, edge22));
		betas[1] = dot(w2, cross(edge12, planar_hitpt_vector));
	}

	if (canhitme[0] && _ray_t.contains(ts[0]) && is_interior(alphas[0], betas[0], _rec))
		hitme[0] = 1;
	if (canhitme[1] && _ray_t.contains(ts[1]) && is_interior(alphas[1], betas[1], _rec))
		hitme[1] = 1;

	if (!(hitme[0] + hitme[1]))
		return false;

	point3* intersection;
	double* alpha, *beta;
	double* t;
	std::array<int, 3> indx;

	int ind = 0;

	if (hitme[0] + hitme[1] == 2)
	{
		if (ts[0] < ts[1])
		{
			ind = 0;
			indx = ind1;
		}
		else
		{
			ind = 1;
			indx = ind2;
		}
	}
	else if (hitme[0] == 1)
	{
		ind = 0;
		indx = ind1;
	}
	else if (hitme[1] == 1) {
		ind = 1;
		indx = ind2;
	}

	intersection = &intersections[ind];
	alpha = &alphas[ind];
	beta = &betas[ind];
	t = ts+ind;


	auto normal = unit_vector(interpolate(*alpha, *beta, vns, indx));
	auto texture = interpolate(*alpha, *beta, vts, indx);

	_rec.t = *t;
	_rec.p = *intersection;
	_rec.u = texture[0];
	_rec.v = texture[1];
	_rec.w = texture[2];
	_rec.mat_indx = mat_indx;
	_rec.set_face_normal(_r, normal);



	return true;
}

bool mesh::is_interior(double _a, double _b, hit_record& _rec) const {
	constexpr double eps = 1e-8;
	if (_a >= -eps && _b >= -eps && _a + _b <= 1+eps)
		return true;

	return false;
}

vec3 mesh::interpolate(double alpha_, double beta_,const std::array<point3, 4>& arr_, std::array<int, 3> indx_)
{
	double gamma = 1.0 - alpha_ - beta_;
	int indx1 = indx_[0];
	int indx2 = indx_[1];
	int indx3 = indx_[2];

	return gamma * arr_[indx1] + alpha_ * arr_[indx2] + beta_ * arr_[indx3];
}

bool mesh::compare(hittable* rhs_, const double& tol) const
{
	// checking the rhs type
	mesh* rhsConv = dynamic_cast<mesh*>(rhs_);
	// it is not of triangle mesh type
	if (!rhsConv)
	{
		return true;
	}


	struct edge
	{
		point3 v;
		point3 vt;
		point3 vn;
	} edgeInfo[4] = {
		{vs[0],vts[0],vns[0]},
		{vs[1],vts[1],vns[1]},
		{vs[2],vts[2],vns[2]},
		{vs[3],vts[3],vns[3]}
	};
	
	edge rhsEdgeInfo[4] =
	{
		{rhsConv->vs[0],rhsConv->vts[0],rhsConv->vns[0]},
		{rhsConv->vs[1],rhsConv->vts[1],rhsConv->vns[1]},
		{rhsConv->vs[2],rhsConv->vts[2],rhsConv->vns[2]},
		{rhsConv->vs[3],rhsConv->vts[3],rhsConv->vns[3]}
	};

	auto sortLambda = [&](const edge& a_, const edge& b_)
	{
		double length_a = a_.v.length();
		double length_b = b_.v.length();
		if (length_a < length_b)
			return true;
		return false;
	};

	std::sort(edgeInfo, edgeInfo + 4, sortLambda);
	std::sort(rhsEdgeInfo, rhsEdgeInfo + 4, sortLambda);

	


	// comparing the geometry
	for (int i = 0; i < 4; i++)
	{
		point3 datavsi  = edgeInfo[i].v - rhsEdgeInfo[i].v;
		point3 datavtsi = edgeInfo[i].vt - rhsEdgeInfo[i].vt;
		point3 datavnsi = edgeInfo[i].vn - rhsEdgeInfo[i].vn;

		if (datavsi.length() >= tol)
		{

			std::cout << "v " << i << std::endl;
			std::cout << edgeInfo[i].v << std::endl;
			std::cout << rhsEdgeInfo[i].v << std::endl;
			std::cout << std::endl;
			return true;
		}
		if (datavtsi.length() >= tol) {
			std::cout << "vt" << std::endl;
			std::cout << edgeInfo[i].vt << std::endl;
			std::cout << rhsEdgeInfo[i].vt << std::endl;
			std::cout << std::endl;
			return true;
		}
		if (datavnsi.length() >= tol) {
			std::cout << "vn" << std::endl;
			std::cout << edgeInfo[i].vn << std::endl;
			std::cout << rhsEdgeInfo[i].vn << std::endl;
			std::cout << std::endl;
			return true;
		}
	}
	return false;
}

bool mesh::comparator(const std::unique_ptr<hittable>& rhs_) const
{
	auto rhs_cast = dynamic_cast<mesh*>(rhs_.get());
	if (!rhs_cast)
		throw std::invalid_argument("Different types!");

	auto area = this->get_area();
	auto rhs_area = rhs_cast->get_area();

	if (area < rhs_area)
		return true;
	return false;
}

double mesh::get_area() const
{
	double area = (n1.length() + n2.length()) / 2.0;
	return area;
}