#pragma once
#include "test.hpp"


// a specific modification of the enginefixture
// which creates the engine first and add out of rank
// particles next ...
std::unique_ptr<Engine> build_engine_set_particles(const int& myId_,
    std::array<int, 3> ranks_,
    std::vector<double>& x_,
    std::vector<double>& v_,
    std::vector<double>& f_,
    std::vector<double>& r_,
    std::vector<double>& m_,
    int nGhosts = 0);


// building the engine .. it first creates particles and then builds the engine
// so each particle is in the correct rank in the returned engine
std::unique_ptr<Engine>  set_particles_build_engine(
    const int& myId_,
    std::array<int, 3> ranks_,
    std::vector<double>& x_,
    std::vector<double>& v_,
    std::vector<double>& f_,
    std::vector<double>& r_,
    std::vector<double>& m_,
    double skin_);


// checking the particle coordinates inside the communicator
void build_engine_check_communicator(
    const int& myId_,
    std::array<int, 3> ranks_,
    std::vector<double>& x_,
    std::vector<double>& v_,
    std::vector<double>& f_,
    std::vector<double>& r_,
    std::vector<double>& m_,
    std::vector<std::vector<double>> expectedXsVec_,
    std::vector<std::vector<double>> expectedVsVec_,
    std::vector<std::vector<double>> expectedFsVec_,
    std::vector<std::vector<double>> expectedRsVec_,
    std::vector<std::vector<double>> expectedMsVec_,
    const double& skin_ = 0.0
    );



void checking_communicator(
    const int& myId_,
    Engine* engine_,
    std::vector<std::vector<double>> expectedXsVec_,
    std::vector<std::vector<double>> expectedVsVec_,
    std::vector<std::vector<double>> expectedFsVec_,
    std::vector<std::vector<double>> expectedRsVec_,
    std::vector<std::vector<double>> expectedMsVec_);