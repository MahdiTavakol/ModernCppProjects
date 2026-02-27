#pragma once

#include "Ref.h"
#include "Engine.h"

class Integrator : public Ref {
public:
	Integrator(std::vector<std::string> args_);
	Integrator();
	Integrator(double dt_);
	virtual void injectDependencies(Engine& engine_) override;
	void setDt(double dt_);
	virtual void post_force();

protected:
	// pointer to particles
	Particles* particles = nullptr;
	double dt = 1.0;
	virtual void velocityUpdate();
	virtual void positionUpdate();
};

