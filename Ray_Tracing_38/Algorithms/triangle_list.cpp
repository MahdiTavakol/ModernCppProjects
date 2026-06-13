#include "triangle_list.h"
#include "../Geometry/triangle_mesh.h"
#include <numeric>

triangle_list::triangle_list() : 
	hittable_list{ std::make_unique<hittable_list>() },
	non_triangles{std::make_unique<hittable_list>()}
{}

triangle_list::triangle_list(std::unique_ptr<hittable> object_):
	hittable_list{},
	non_triangles{ std::make_unique<hittable_list>() }
{
	add(std::move(object_));
}

triangle_list::triangle_list(std::unique_ptr<hittable>& object_) :
	hittable_list{},
	non_triangles{ std::make_unique<hittable_list>() }
{
	add(std::move(object_));
}

triangle_list::triangle_list(std::unique_ptr<hittable_list> list_) :
	triangles{}, bboxes{},
	non_triangles{std::make_unique<hittable_list>()}
{
	std::unique_ptr<hittable_list> temp_list = std::move(list_);
	for (auto& object : temp_list->objects)
	{
		this->add(std::move(object));
	}
}

size_t triangle_list::size() const
{
	if (triangles.size() != bboxes.size())
		throw std::runtime_error("Internal error!");
	return triangles.size();
}

void triangle_list::resize(const int size_)
{
	triangles.resize(size_);
	bboxes.resize(size_);
}

void triangle_list::sort()
{
	// it has not been implemented yet!
}

void triangle_list::sort_range(size_t& start_, size_t& end_)
{
	// sorting them
	std::vector<size_t> indexes(end_ - start_);
	std::iota(indexes.begin(), indexes.end(), start_);
	int axis = bbox.longest_axis();
	auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;
	std::sort(
		indexes.begin(),
		indexes.end(),
		[&](const size_t& a, const size_t& b) ->bool
		{
			return comparator(bboxes[a], bboxes[b]);
		});

	std::vector<aabb> sorted_boxes(end_ - start_);
	std::vector<triangle_struct> sorted_triangles(end_ - start_);

	for (int i = 0; i < end_ - start_; i++)
	{
		sorted_boxes[i] = bboxes[indexes[i]];
		sorted_triangles[i] = triangles[indexes[i]];
	}

	// putting the sorted arrays back
	auto sorted_boxes_begin = sorted_boxes.begin();
	auto sorted_triangles_begin = sorted_triangles.begin();
	std::copy(sorted_boxes.begin(), sorted_boxes.end(), &bboxes[start_]);
	std::copy(sorted_triangles.begin(), sorted_triangles.end(), &triangles[start_]);
}

void triangle_list::add(std::unique_ptr<hittable> object_)
{
	triangle_mesh* conv = dynamic_cast<triangle_mesh*>(object_.get());
	if (conv == nullptr)
	{
		// So it is not a triangle object so it will be added to 
		// the objects method of the hittable_list
		non_triangles->add(std::move(object_));
		return;
	}

	// getting its labels
	std::unordered_set<std::string> labels = object_->return_labels();


	std::array<point3, 3> vs, vns;
	std::array<point2, 3> vts, vts_1;
	conv->return_params(vs, vts, vts_1, vns);
	int mat_indx = conv->return_mat_indx();

	add(vs, vts, vts_1, vns,mat_indx,labels);
}

void triangle_list::add(
	const std::array<point3, 3>& vs_,
	const std::array<point2, 3>& vts_,
	const std::array<point2, 3>& vts_1_,
	const std::array<point3, 3>& vns_,
	const int& mat_indx_,
	const std::unordered_set<std::string> labels_)
{
	aabb newBox = triangle_bbox(vs_);
	bbox = aabb(bbox, newBox);
	bboxes.push_back(newBox);

	// calculating some parameters
	vec3 n1 = cross(vs_[1] - vs_[0], vs_[2] - vs_[0]);
	vec3 unit_n1 = unit_vector(n1);
	vec3 w1 = n1 / dot(n1, n1);
	point3 Q1 = vs_[0];
	double D1 = dot(unit_n1, Q1);
	


	triangle_struct new_triangle{ vs_,vts_,vts_1_,vns_,unit_n1,w1,Q1,D1, mat_indx_,labels_};
	triangles.push_back(new_triangle);
}

void triangle_list::add(
	const std::array<point3, 3>& vs_,
	const std::array<point2, 3>& vts_,
	const std::array<point3, 3>& vns_,
	const int& mat_indx_,
	const std::unordered_set<std::string> labels_)
{
	std::array<point2, 3> vts_1;
	add(vs_, vts_, vts_1, vns_,mat_indx_,labels_);
}

bool triangle_list::hit(const ray& r_, interval ray_t_, hit_record& rec_) const
{
	bool hit_anything = false;
	auto closest_so_far = ray_t_.max;

	for (const auto& bbox : bboxes)
	{
		interval temp_ray_t = interval(ray_t_.min, closest_so_far);
		if (!bbox.hit(r_, temp_ray_t))
			continue;

		auto indx = &bbox - bboxes.data();
		const triangle_struct& triangle_i = triangles[indx];
		const auto& unit_n1 = triangle_i.unit_n1;
		const auto& D1 = triangle_i.D1;
		const auto& Q1 = triangle_i.Q1;
		const auto& w1 = triangle_i.w1;

		const auto& vs = triangle_i.vs;
		const auto& vts = triangle_i.vts;
		const auto& vts_1 = triangle_i.vts_1;
		const auto& vns = triangle_i.vns;
		const int& mat_indx = triangle_i.mat_indx;


		auto denom1 = dot(unit_n1, r_.direction());
		if (std::fabs(denom1) < 1e-8)
			continue;

		
		auto t1 = (D1 - dot(unit_n1, r_.origin())) / denom1;
		if (!temp_ray_t.contains(t1))
			continue;


		auto intersection = r_.at(t1);
		vec3 planar_hitpt_vector = intersection - Q1;
		auto alpha = dot(w1, cross(planar_hitpt_vector, vs[2] - vs[0]));
		auto beta = dot(w1, cross(vs[1] - vs[0], planar_hitpt_vector));

		if (!is_interior(alpha, beta))
			continue;

		hit_anything = true;

		auto normal = interpolate(alpha, beta, vns);
		auto texture = interpolate(alpha, beta, vts);
		auto texture_1 = interpolate(alpha, beta, vts_1);

		normal = unit_vector(normal);

		rec_.t = t1;
		rec_.p = intersection;
		rec_.u = texture[0];
		rec_.v = texture[1];
		rec_.w = 0.0;
		rec_.u1 = texture_1[0];
		rec_.v1 = texture_1[1];
		rec_.mat_indx = mat_indx;
		rec_.set_face_normal(r_, normal);

		closest_so_far = t1;
	}

	// non triangles
	hit_anything = hit_anything && hittable_list::hit(r_, ray_t_, rec_);

	return hit_anything;
}

bool triangle_list::hit_indx(const ray& r_, interval ray_t_, hit_record& rec_, size_t indx_) const
{
	const triangle_struct& triangle_i = triangles[indx_];
	const auto& unit_n1 = triangle_i.unit_n1;
	const auto& D1 = triangle_i.D1;
	const auto& Q1 = triangle_i.Q1;
	const auto& w1 = triangle_i.w1;

	const auto& vs = triangle_i.vs;
	const auto& vts = triangle_i.vts;
	const auto& vts_1 = triangle_i.vts_1;
	const auto& vns = triangle_i.vns;
	const int& mat_indx = triangle_i.mat_indx;


	auto denom1 = dot(unit_n1, r_.direction());
	if (std::fabs(denom1) < 1e-8)
		return false;


	auto t1 = (D1 - dot(unit_n1, r_.origin())) / denom1;
	if (!ray_t_.contains(t1))
		return false;


	auto intersection = r_.at(t1);
	vec3 planar_hitpt_vector = intersection - Q1;
	auto alpha = dot(w1, cross(planar_hitpt_vector, vs[2] - vs[0]));
	auto beta = dot(w1, cross(vs[1] - vs[0], planar_hitpt_vector));

	if (!is_interior(alpha, beta))
		return false;


	auto normal = interpolate(alpha, beta, vns);
	auto texture = interpolate(alpha, beta, vts);
	auto texture_1 = interpolate(alpha, beta, vts_1);

	normal = unit_vector(normal);

	rec_.t = t1;
	rec_.p = intersection;
	rec_.u = texture[0];
	rec_.v = texture[1];
	rec_.w = 0.0;
	rec_.u1 = texture_1[0];
	rec_.v1 = texture_1[1];
	rec_.mat_indx = mat_indx;
	rec_.set_face_normal(r_, normal);

	return true;
}

bool triangle_list::hit_triangle(const ray& r_, interval ray_t_, hit_record& rec_, const triangle_struct& tri_)
{
	const auto& unit_n1 = tri_.unit_n1;
	const auto& D1 = tri_.D1;
	const auto& Q1 = tri_.Q1;
	const auto& w1 = tri_.w1;

	const auto& vs = tri_.vs;
	const auto& vts = tri_.vts;
	const auto& vts_1 = tri_.vts_1;
	const auto& vns = tri_.vns;
	const int& mat_indx = tri_.mat_indx;




	auto denom1 = dot(unit_n1, r_.direction());
	if (std::fabs(denom1) < 1e-8)
		return false;


	auto t1 = (D1 - dot(unit_n1, r_.origin())) / denom1;
	if (!ray_t_.contains(t1))
		return false;


	auto intersection = r_.at(t1);
	vec3 planar_hitpt_vector = intersection - Q1;
	auto alpha = dot(w1, cross(planar_hitpt_vector, vs[2] - vs[0]));
	auto beta = dot(w1, cross(vs[1] - vs[0], planar_hitpt_vector));

	if (!is_interior_static(alpha, beta))
		return false;


	auto normal = interpolate(alpha, beta, vns);
	auto texture = interpolate(alpha, beta, vts);
	auto texture_1 = interpolate(alpha, beta, vts_1);

	normal = unit_vector(normal);

	rec_.t = t1;
	rec_.p = intersection;
	rec_.u = texture[0];
	rec_.v = texture[1];
	rec_.w = 0.0;
	rec_.u1 = texture_1[0];
	rec_.v1 = texture_1[1];
	rec_.mat_indx = mat_indx;
	rec_.set_face_normal(r_, normal);

	return true;
}

aabb triangle_list::triangle_bbox(const std::array<point3, 3>& vs_)
{
	auto bbox_diagonal1 = aabb(vs_[0], vs_[1]);
	auto bbox_diagonal2 = aabb(vs_[0], vs_[2]);
	auto bbox_diagonal3 = aabb(vs_[1], vs_[2]);

	auto bbox_diagonal = aabb(bbox_diagonal1, bbox_diagonal2);
	bbox_diagonal = aabb(bbox_diagonal, bbox_diagonal3);
	return bbox_diagonal;
}

bool triangle_list::is_interior(double _a, double _b) const {
	constexpr double eps = 1e-8;
	if (_a >= -eps && _b >= -eps && _a + _b <= 1 + eps)
		return true;

	return false;
}

bool triangle_list::is_interior_static(double a_, double b_)
{
	constexpr double eps = 1e-8;
	if (a_ >= -eps && b_ >= -eps && a_ + b_ <= 1 + eps)
		return true;

	return false;
}

vec3 triangle_list::interpolate(double alpha_, double beta_, const std::array<point3, 3>& arr_)
{
	double gamma = 1.0 - alpha_ - beta_;
	return gamma * arr_[0] + alpha_ * arr_[1] + beta_ * arr_[2];
}

vec2 triangle_list::interpolate(double alpha_, double beta_, const std::array<point2, 3>& arr_)
{
	std::array<point3, 3> arr3;
	arr3[0] = point3(arr_[0]);
	arr3[1] = point3(arr_[1]);
	arr3[2] = point3(arr_[2]);
	vec3 output = interpolate(alpha_, beta_, arr3);
	return vec2{ output.x(), output.y() };
}


vec3 triangle_list::com() const
{
	vec3 cm = vec3{ 0.0,0.0,0.0 };
	int count = 0;
	for (const auto& triangle: triangles)
	{
		const std::array<point3, 3>& vs = triangle.vs;
		cm += vs[0];
		cm += vs[1];
		cm += vs[2];
		count += 3;
	}
	cm /= static_cast<int>(count);
	return cm;
}

void triangle_list::scale(const vec3& center_, const double& factor_) 
{
	for (auto& triangle : triangles)
	{
		std::array<point3, 3>& vs = triangle.vs;
		vs[0] += factor_*(vs[0] - center_);
		vs[1] += factor_*(vs[1] - center_);
		vs[2] += factor_*(vs[2] - center_);
	}
}

void triangle_list::add_label(std::string label_)
{
	for (auto& triangle : triangles)
	{
		triangle.labels.insert(label_);
	}
}

aabb triangle_list::bounding_box(std::string label_, bool& set_)
{
	aabb boxLabel = aabb::empty;
	for (auto& triangle : triangles)
	{
		if (triangle.labels.find(label_) != triangle.labels.end())
		{
			set_ = set_ || true;
			size_t indx = &triangle - &triangles[0];
			boxLabel = aabb(boxLabel, bboxes[indx]);
		}
	}
	return boxLabel;
}

void triangle_list::clear()
{
	triangles.clear();
	bboxes.clear();
	objects.clear(); 
}

hittable* triangle_list::operator[](const int& i) {
	return objects[i].get();
}


void triangle_list::return_params(
	std::vector<triangle_struct>& triangles_,
	std::vector<aabb>& bboxes_)
{
	triangles_ = triangles;
	bboxes_ = bboxes;
}

aabb triangle_list::return_bbox(size_t i)
{
	if (i >= bboxes.size())
		throw std::invalid_argument("Out of range argument!");
	return bboxes[i];
}

std::vector<triangle_struct> triangle_list::return_triangles()
{
	return triangles;
}

std::vector<aabb> triangle_list::return_bboxes()
{
	return bboxes;
}

std::vector<triangle_struct>& triangle_list::return_triangles_ref()
{
	return triangles;
}

std::vector<aabb>& triangle_list::return_bboxes_ref()
{
	return bboxes;
}

std::unique_ptr<hittable_list> triangle_list::return_non_triangles()
{
	return std::move(non_triangles);
}