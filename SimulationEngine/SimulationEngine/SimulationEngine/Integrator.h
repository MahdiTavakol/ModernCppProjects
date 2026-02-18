#pragma once

#include "Type.h"
#include "Engine.h"

class Integrator : public Type {
public:
	Integrator(Engine& engine_) :
		Type{ engine_,"1" }
	{}
	Integrator(Engine& engine_, double dt_) :
		Type{ engine_,"1" },
		dt{ dt_ }
	{}
	void setDt(double dt_) { dt = dt_; }


	virtual void init();
	virtual void setup();
	virtual void initial_integrate();
	virtual void pre_force();
	virtual void force();
	virtual void post_force();
	virtual void final_integrate();

protected:
	double dt = 1.0;
	virtual void velocityUpdate();
	virtual void positionUpdate();
};

