#include "ColliIntegrator.h"
#include "Particles.h"
#include "Neighbor.h"
#include <iostream>

ColliIntegrator::ColliIntegrator(Engine& engine_, std::vector<std::string> args_) :
	Integrator{ engine_, args_ }
{
}

ColliIntegrator::ColliIntegrator(Engine& engine) :
	Integrator{ engine }
{
}

void ColliIntegrator::post_force()
{
	Integrator::post_force();
	velocityUpdate();
	deltaVUpdate();
	velocityUpdate_2();
	positionUpdate();
}

void ColliIntegrator::deltaVUpdate() {

	// check the distance criteria based on the neighbors
	auto& neighbor = engine().getNeighbor();
	// getting particles
	auto& particles = engine().getParticlesForUpdate();
	auto& P = *particles;
	// getting the neighbor list
	int nNeigh; 
	int* neighList, * firstNeigh, * numNeigh;
	neighbor->getNeighborList(nNeigh, neighList, firstNeigh, numNeigh);

	// checking the number of particles
	int new_nmax, new_nlocal;
	particles->getNmaxNlocal(new_nmax, new_nlocal);
	// checking the vector capacity
	if (new_nmax > nmax) {
		nmax = new_nmax;
		dv.reserve(3*nmax);
	}
	// zeroing the vector
	if (new_nlocal > nlocal) {
		nlocal = new_nlocal;
		dv.resize(3*nlocal);
	}

	for (int ii = 0; ii < nNeigh; ii++) {
		int i = ii;
		int numJs = numNeigh[ii];
		int firstNeighI = firstNeigh[ii];
		for (int jj = 0; jj < numJs; jj++)
		{
			int j = neighList[firstNeighI + jj];

			double dX = P.X(i, 0) - P.X(j, 0);
			double dY = P.X(i, 1) - P.X(j, 1);
			double dZ = P.X(i, 2) - P.X(j, 2);
			double dist2 = dX * dX + dY * dY + dZ * dZ;

			double dVX = P.V(i, 0) - P.V(j, 0);
			double dVY = P.V(i, 1) - P.V(j, 1);
			double dVZ = P.V(i, 2) - P.V(j, 2);

			double r2 = (P.R(i) + P.R(j)) * (P.R(i) + P.R(j));
			double vrel = (dVX * dX + dVY * dY + dVZ * dZ) / std::sqrt(dist2);

			if (dist2 < r2 && vrel < 0)
			{
				dV(i, 0) = -(2 * P.M(j) / (P.M(i) + P.M(j))) * (dVX * dX + dVY * dY + dVZ * dZ) * dX / dist2;
				dV(i, 1) = -(2 * P.M(j) / (P.M(i) + P.M(j))) * (dVX * dX + dVY * dY + dVZ * dZ) * dY / dist2;
				dV(i, 2) = -(2 * P.M(j) / (P.M(i) + P.M(j))) * (dVX * dX + dVY * dY + dVZ * dZ) * dZ / dist2;
			}

		}
	}

}

void ColliIntegrator::velocityUpdate_2()
{
	// particles
	auto& particles = engine().getParticlesForUpdate();
	// velocities
	double* v = particles->getVData();
	// updating the velocities
	for (int i = 0; i < nlocal; i++) {
		for (int j = 0; j < 3; j++)
			v[3 * i+j] += dv[3 * i+j];
	}

}