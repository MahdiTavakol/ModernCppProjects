#ifndef CAMERA_DERIVED_H
#define CAMERA_DERIVED_H

/* This class was defined since if we have the _in->setup_camera(*camera) 
   inside the camera class we would have circular dependency */

#include "camera.h"
#include "../Input/input.h"
#include "parallel.h"

class camera_derived : public camera
{
	friend class input;
	friend class parallel;

public:

    camera_derived(std::unique_ptr<input>& _in):
		camera{}
	{
		_in->setup_camera(this);
		initialize_storage();
	}

	void render(const hittable& world) override;

	void move_camera(point3 _lookfrom) override {
		this->lookfrom = _lookfrom;
	}

protected:
	void initialize_storage() override;

};

#endif