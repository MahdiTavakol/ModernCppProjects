#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "../Shared/rtweekend.h"

#include "../Geometry/aabb.h"
#include "../Geometry/hittable.h"
#include "../Geometry/interval.h"


#include <vector>



class hittable_list : public hittable {
public:
	std::vector<std::unique_ptr<hittable>> objects;



	hittable_list() : hittable{ "hittable_list" } {}
	hittable_list(std::unique_ptr<hittable> object);
	hittable_list(std::unique_ptr<hittable>& object);

	void clear() { objects.clear(); }

	size_t size() { return objects.size(); }

	hittable* operator[](const int& i) {
		return objects[i].get();
	}

	void resize(const int size);
	void sort();

	virtual void add(std::unique_ptr<hittable> object);

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override;

	aabb bounding_box() const override {return bbox; }

	virtual material* get_material() override {
		return nullptr;
	}



protected:
};

#endif