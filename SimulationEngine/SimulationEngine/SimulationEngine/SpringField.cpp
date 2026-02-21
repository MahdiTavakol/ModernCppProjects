#include "SpringField.h"
#include "Error.h"

SpringField::SpringField(Engine& engine_, std::vector<std::string> args_) :
	ForceField{ engine_, args_ }
{
	auto nargs = args_.size();
	auto& error = engine().getError();
	if (nargs < 3) {
		error->one("Not enough arguments for springfield command! Expected at least 3, got " + std::to_string(nargs));
		coeff = 0.0;
		return;
	}
	coeff = std::stod(args_[2]);
}

SpringField::SpringField(Engine& engine_, double coeff_) :
	ForceField{ engine_ },
	coeff{ coeff_ }
{
}

void SpringField::calculate_pair(std::array<double,3>& dist_,
	double* fforce_, double& energy_)
{
	for (int i = 0; i < 3; i++) {
		fforce_[i] = -coeff * dist_[i];
		energy_ += 0.5 * coeff * dist_[i] * dist_[i];
	}
}