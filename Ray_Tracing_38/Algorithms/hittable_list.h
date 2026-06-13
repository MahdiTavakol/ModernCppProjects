#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "../Shared/rtweekend.h"

#include "../Geometry/aabb.h"
#include "../Geometry/hittable.h"
#include "../Geometry/interval.h"


#include <vector>
#include <unordered_set>



class hittable_list : public hittable {
public:
	std::vector<std::unique_ptr<hittable>> objects;



	hittable_list() : hittable{ "hittable_list" } {}
	hittable_list(std::unique_ptr<hittable> object);
	hittable_list(std::unique_ptr<hittable>& object);

	virtual void clear();

	size_t size() const override;

	virtual hittable* operator[](const int& i);

	virtual void resize(const int size);
	virtual void sort();

	virtual void add(std::unique_ptr<hittable> object);

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

	aabb bounding_box() const override {return bbox; }



	virtual vec3 com() const;
	virtual vec3 com(std::string label_, bool& set_);

	virtual aabb bounding_box(std::string label_, bool& set_) override;

	virtual void add_label(std::string label_) override;

	virtual void scale(const vec3& center_, const double& factor_) override;


protected:

};

#endif