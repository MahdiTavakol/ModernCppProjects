#pragma once

#include "Ref.h"
#include "Engine.h"
#include "Fix.h"

class FixList : public Fix
{
public:
	FixList(Engine& engine_);
	FixList(Engine& engine_, std::vector<std::string>& args_);
	virtual ~FixList();

	void injectDependencies(Engine& engine_) override;
	void addFix(std::unique_ptr<Fix>&& fix_);
	std::unique_ptr<Fix>& returnFixById(std::string id_);

	virtual void init() override;
	virtual void setup() override;
	virtual void initial_integrate() override;
	virtual void pre_force() override;
	virtual void post_force() override;
	virtual void final_integrate() override;

	// all the bits are 1.
	int mask = static_cast<FixMask>(~0);

protected:
	std::vector<std::unique_ptr<Fix>> fixVec;
	int nevery = -1;
};
