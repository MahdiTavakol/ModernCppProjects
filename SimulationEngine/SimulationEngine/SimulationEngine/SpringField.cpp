#include "SpringField.h"

void SpringField::calculate_pair(std::array<double,3>& dist_,
	double* fforce_, double& energy_)
{
	for (int i = 0; i < 3; i++) {
		fforce_[i] = coeff * dist_[i];
		energy_ += 0.5 * coeff * dist_[i] * dist_[i];
	}
}