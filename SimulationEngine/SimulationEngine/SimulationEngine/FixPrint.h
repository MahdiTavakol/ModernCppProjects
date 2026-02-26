#pragma once

#include "Fix.h"
#include <array>
#include <vector>


class FixPrint : public Fix {
public:
	FixPrint(Engine& engine_, std::vector<std::string> args_);

	FixPrint(Engine& engine_,
		std::string id_,
		int nevery_ = 1,
		int mask_ = POST_FORCE,
		std::string variable_ = "x",
		int particleId_ = 0);

	void injectDependencies(Engine& engine_) override;
	void init() override;
	void initial_integrate() override;
	void pre_force() override;
	void post_force() override;
	void final_integrate() override;
	const std::vector<std::array<double,3>>& getOutputVector() const;

private:
	// Pointer to Particles
	Particles* particles;
	double* inData = nullptr;
	std::string variable;
	int particleId;

	void fillData();
	std::vector<std::array<double,3>> data;
};