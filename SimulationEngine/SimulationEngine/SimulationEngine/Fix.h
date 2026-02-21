#pragma once

#include "Ref.h"
#include "Engine.h"

enum FixMask {
	INIT_INTEGRATE = 1 << 1,
	PRE_FORCE = 1 << 2,
	POST_FORCE = 1 << 3,
	FINAL_INTEGRATE = 1 << 4
};

class Fix : public Ref
{
public:
	Fix(Engine& engine_,
		std::string id_,
		int nevery_ = 1) :
		Ref{ engine_, id_},
		nevery{nevery_}
	{}
	virtual ~Fix() {}
	
	virtual void init() {};
	virtual void setup() {};
	virtual void initial_integrate() = 0;
	virtual void pre_force() = 0;
	virtual void post_force() = 0;
	virtual void final_integrate() = 0;

	int mask = 0;

protected:
	int nevery;
};

