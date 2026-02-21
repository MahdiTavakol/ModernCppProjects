#pragma once

#include "Ref.h"
#include "Engine.h"

class Integrator : public Ref {
public:
	Integrator(Engine& engine_,
		std::vector<std::string> args_);
	Integrator(Engine& engine_);
	Integrator(Engine& engine_, double dt_);
	void setDt(double dt_);


	virtual void init();
	virtual void setup();
	virtual void neighbor_build();
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

