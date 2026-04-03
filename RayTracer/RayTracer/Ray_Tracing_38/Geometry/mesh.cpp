#include "mesh.h"
#include <mpi.h>

mesh::mesh(const std::array<point3, 4>& _vs,
	       const std::array<point3, 4>& _vts,
	       const std::array<point3, 4>& _vns,
	       std::unique_ptr<material> _mat,
		   std::string type_ )
	: hittable{type_,std::move(_mat)},
	  vs{ _vs }, vts{ _vts }, 
	  vns{ _vns },
	  num_edges{ 4 }
{
	initialize();
}

mesh::mesh(std::unique_ptr<material> _mat, std::string type_) :
	hittable{ type_ ,std::move(_mat)} { }

void mesh::initialize()
{
	//breaking the mesh to two triangles across the 0-2 direction
	auto n1a = cross(vs[1] - vs[0], vs[2] - vs[0]);
	auto n2a = cross(vs[2] - vs[0], vs[3] - vs[0]);
	vec3 unit_n1a = unit_vector(n1a);
	vec3 unit_n2a = unit_vector(n2a);
	auto dot_a = dot(unit_n1a, unit_n2a);
	//breaking the mesh to two triangles across the 0-3 direction
	auto n1b = cross(vs[2] - vs[0], vs[3] - vs[0]);
	auto n2b = cross(vs[3] - vs[0], vs[1] - vs[0]);
	vec3 unit_n1b = unit_vector(n1b);
	vec3 unit_n2b = unit_vector(n2b);
	auto dot_b = dot(unit_n1b, unit_n2b);
	// comparing them
	n1 = dot_a > dot_b ? n1a : n1b;
	n2 = dot_a > dot_b ? n2a : n2b;
	unit_n1 = dot_a > dot_b ? unit_n1a : unit_n1b;
	unit_n2 = dot_a > dot_b ? unit_n2a : unit_n2b;
	Q1 = vs[0];
	Q2 = vs[0];
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

	if (std::fabs(denom1) < 1e-8 || std::fabs(denom2) < 1e-8)
		return false;

	auto t1 = (D1 - dot(unit_n1, _r.origin())) / denom1;
	auto t2 = (D2 - dot(unit_n2, _r.origin())) / denom2;
	if (!_ray_t.contains(t1) && !_ray_t.contains(t2))
		return false;

	if (_ray_t.contains(t1))
	{
		auto intersection = _r.at(t1);
		vec3 planar_hitpt_vector = intersection - Q1;
		auto alpha = dot(w1, cross(planar_hitpt_vector, vs[2]-vs[0]));
		auto beta = dot(w1, cross(vs[1]-vs[0], planar_hitpt_vector));

		if (!is_interior(alpha, beta, _rec))
			return false;


		std::vector<vec3> triangle, normals, textures;
		triangle.push_back(vs[0]);
		triangle.push_back(vs[1]);
		triangle.push_back(vs[2]);
		normals.push_back(vns[0]);
		normals.push_back(vns[1]);
		normals.push_back(vns[2]);
		textures.push_back(vts[0]);
		textures.push_back(vts[1]);
		textures.push_back(vts[2]);
		auto normal = interpolate(triangle, normals, intersection);
		auto texture = interpolate(triangle, textures, intersection);

		_rec.t = t1;
		_rec.p = intersection;
		_rec.u = texture[0];
		_rec.v = texture[1];
		_rec.w = texture[2];
		_rec.mat = mat.get();
		_rec.set_face_normal(_r,normal);
	}
	if (_ray_t.contains(t2))
	{
		auto intersection = _r.at(t2);
		vec3 planar_hitpt_vector = intersection - Q2;
		auto alpha = dot(w1, cross(planar_hitpt_vector, vs[1] - vs[3]));
		auto beta = dot(w1, cross(vs[2] - vs[3], planar_hitpt_vector));

		if (!is_interior(alpha, beta, _rec))
			return false;

		std::vector<vec3> triangle, normals, textures;
		triangle.push_back(vs[1]);
		triangle.push_back(vs[2]);
		triangle.push_back(vs[3]);
		normals.push_back(vns[1]);
		normals.push_back(vns[2]);
		normals.push_back(vns[3]);
		textures.push_back(vts[1]);
		textures.push_back(vts[2]);
		textures.push_back(vts[3]);
		auto normal = interpolate(triangle, normals, intersection);
		auto texture = interpolate(triangle, textures, intersection);

		_rec.t = t2;
		_rec.p = intersection;
		_rec.u = texture[0];
		_rec.v = texture[1];
		_rec.w = texture[2];
		_rec.mat = mat.get();
		_rec.set_face_normal(_r, normal);
	}


	return true;
}

bool mesh::is_interior(double _a, double _b, hit_record& _rec) const {
	
	if (_a > 0 && _b > 0 && _a + _b < 1)
		return true;

	return false;
}

vec3 mesh::interpolate(const std::vector<point3>& _triangle, const std::vector<point3>& _normals, const vec3& _point)
{
	vec3 PA = _point - _triangle[0];
	vec3 PB = _point - _triangle[1];
	vec3 PC = _point - _triangle[2];
	vec3 AB = _triangle[1] - _triangle[0];
	vec3 AC = _triangle[2] - _triangle[0];

	auto Apbc = 0.5 * dot(PB, PC);
	auto Apac = 0.5 * dot(PA, PC);
	auto Apab = 0.5 * dot(PA, PB);
	auto Aabc = 0.5 * dot(AB, AC);

	return ((Apbc / Aabc) * _normals[0] + (Apac / Aabc) * _normals[1] + (Apab / Aabc) * _normals[2]);
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