#pragma once

#include "Fix.h"
#include <array>
#include <vector>

using std::array, std::vector;

class FixPrint : public Fix {
public:
	FixPrint(Engine& engine_,
		std::string id_,
		int nevery_ = 1,
		int mask_ = POST_FORCE,
		string variable_ = "x",
		int particleId_ = 0);

	void init() override;
	void initial_integrate() override;
	void pre_force() override;
	void post_force() override;
	void final_integrate() override;
	const vector<array<double,3>>& getOutputVector() const;

private:
	double* inData = nullptr;
	string variable;
	int particleId;

	void fillData();
	vector<array<double,3>> data;
};