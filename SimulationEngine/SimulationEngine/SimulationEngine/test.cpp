#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>

#include "test.hpp"



TEST_CASE("Testing the communication class") {
	std::cout << "Testing the communication class" << std::endl;
	std::cout << std::string(80, '=') << std::endl;


	// box dimensions
	array<double, 3> min = { 0.0,0.0,0.0 };
	array<double, 3> max = { 10.0,10.0,10.0 };
	// box 
	auto box = make_unique<Box>(min, max);
	// Particles
	std::vector<double> x = {
     145.32,  -210.77,   89.45,     // particle 0
    -178.64,   254.19,  -133.58,    // particle 1
      12.87,   -98.44,   276.31,    // particle 2
     -45.22,   199.73,  -287.90,    // particle 3
     233.11,    -5.66,   147.82,    // particle 4
    -299.45,   120.38,   -44.91,    // particle 5
      67.59,  -243.80,    18.26,    // particle 6
     190.74,   -75.33,  -166.48,    // particle 7
     -88.17,   281.56,   -12.49,    // particle 8
      54.03,  -132.67,   204.95,    // particle 9
    -215.90,    33.14,   119.62,    // particle 10
     174.28,  -299.99,   -63.77,    // particle 11
     -31.50,   146.22,  -250.84,    // particle 12
     287.44,   -18.39,    92.71,    // particle 13
    -122.63,  -187.55,   241.08,    // particle 14
      39.96,   208.13,  -174.29,    // particle 15
     256.77,  -264.44,    11.38,    // particle 16
    -194.05,    77.80,  -299.12,    // particle 17
      83.61,   -54.27,   165.49,    // particle 18
    -270.33,   295.88,   -21.76     // particle 19
    };
    std::vector<double> v = {
     72.4, -33.1,  95.0,   // particle 0
    -88.6,  14.3, -56.7,   // particle 1
     41.2, -19.8,  63.5,   // particle 2
    -97.1,  28.9, -74.4,   // particle 3
      3.6,  89.2, -12.5,   // particle 4
     54.7, -66.3,  77.1,   // particle 5
    -45.9,  18.0,  92.6,   // particle 6
    -11.4,  36.8, -83.2,   // particle 7
     59.7, -24.6,  10.9,   // particle 8
    -72.5,  44.3, -95.8,   // particle 9
     21.7, -38.4,  68.2,   // particle 10
    -14.9,  87.6, -29.3,   // particle 11
     46.1, -61.5,  33.8,   // particle 12
    -79.2,  12.4,  99.1,   // particle 13
    -53.7,  27.5, -41.6,   // particle 14
     64.0, -22.8,  15.2,   // particle 15
    -90.3,  48.6,  -7.4,   // particle 16
     81.9, -35.0,  24.1,   // particle 17
     57.8, -69.2,  39.4,   // particle 18
    -16.7,  73.3, -58.9    // particle 19
    };
    std::vector<double> f = {
     12.4, -33.9,  45.2,   // particle 0
    -18.6,   7.3, -41.5,   // particle 1
     29.7,  -5.4,  38.9,   // particle 2
    -22.1,  16.0, -49.7,   // particle 3
      3.1,  27.6, -14.8,   // particle 4
     44.5, -36.7,  21.4,   // particle 5
     -9.9,   5.6,  31.2,   // particle 6
    -12.7,  18.9, -28.4,   // particle 7
     47.3, -19.5,   6.2,   // particle 8
    -34.8,  25.1, -45.6,   // particle 9
     14.8,  -7.3,  39.6,   // particle 10
    -21.9,  30.4, -16.5,   // particle 11
     41.2, -29.8,   9.7,   // particle 12
    -48.1,  12.6,  33.9,   // particle 13
    -25.7,  17.8, -38.6,   // particle 14
     26.4, -11.3,   4.9,   // particle 15
    -43.2,  22.5,  -6.8,   // particle 16
     35.0, -18.1,  13.7,   // particle 17
     28.6, -31.4,  19.3,   // particle 18
     -8.2,  46.7, -24.5    // particle 19
    };
    std::vector<double> m = {
     3.4,  // particle 0
     7.8,  // particle 1
     0.9,  // particle 2
     5.6,  // particle 3
     9.1,  // particle 4
     2.7,  // particle 5
     6.0,  // particle 6
     1.3,  // particle 7
     8.4,  // particle 8
     4.1,  // particle 9
     0.5,  // particle 10
     9.8,  // particle 11
     3.0,  // particle 12
     7.4,  // particle 13
     5.9,  // particle 14
     2.1,  // particle 15
     6.7,  // particle 16
     8.2,  // particle 17
     1.8,  // particle 18
     4.6   // particle 19
    };

    std::vector<double> r = {
     12.5,  // particle 0
     89.3,  // particle 1
     45.7,  // particle 2
     3.9,   // particle 3
     67.1,  // particle 4
     24.6,  // particle 5
     98.0,  // particle 6
     31.4,  // particle 7
     76.2,  // particle 8
     54.3,  // particle 9
     8.7,   // particle 10
     63.5,  // particle 11
     19.8,  // particle 12
     92.4,  // particle 13
     37.6,  // particle 14
     71.0,  // particle 15
     14.2,  // particle 16
     83.1,  // particle 17
     58.9,  // particle 18
     26.4   // particle 19
    };

    // expected values for each core
    // core 1
    std::vector<double> expectedXs1 = {
    -122.63,  -187.55,   241.08,    // particle 14
    };
    std::vector<double> expectedVs1 = {
    -53.7,  27.5, -41.6,   // particle 14
    };
    std::vector<double> expectedFs1 = {
    -25.7,  17.8, -38.6,   // particle 14
    };
    std::vector<double> expectedMs1 = {
     5.9,  // particle 14
    };

    std::vector<double> expectedRs1 = {
     37.6,  // particle 14
    };

    // core 2
    std::vector<double> expectedXs2 = {
     145.32,  -210.77,   89.45,     // particle 0
      12.87,   -98.44,   276.31,    // particle 2
     233.11,    -5.66,   147.82,    // particle 4
      67.59,  -243.80,    18.26,    // particle 6
     190.74,   -75.33,  -166.48,    // particle 7
      54.03,  -132.67,   204.95,    // particle 9
     174.28,  -299.99,   -63.77,    // particle 11
     287.44,   -18.39,    92.71,    // particle 13
     256.77,  -264.44,    11.38,    // particle 16
      83.61,   -54.27,   165.49,    // particle 18
    };
    std::vector<double> expectedVs2 = {
     72.4, -33.1,  95.0,   // particle 0
     41.2, -19.8,  63.5,   // particle 2
      3.6,  89.2, -12.5,   // particle 4
    -45.9,  18.0,  92.6,   // particle 6
    -11.4,  36.8, -83.2,   // particle 7
    -72.5,  44.3, -95.8,   // particle 9
    -14.9,  87.6, -29.3,   // particle 11
    -79.2,  12.4,  99.1,   // particle 13
    -90.3,  48.6,  -7.4,   // particle 16
     57.8, -69.2,  39.4,   // particle 18
    };
    std::vector<double> expectedFs2 = {
     12.4, -33.9,  45.2,   // particle 0
     29.7,  -5.4,  38.9,   // particle 2
      3.1,  27.6, -14.8,   // particle 4
     -9.9,   5.6,  31.2,   // particle 6
    -12.7,  18.9, -28.4,   // particle 7
    -34.8,  25.1, -45.6,   // particle 9
    -21.9,  30.4, -16.5,   // particle 11
    -48.1,  12.6,  33.9,   // particle 13
    -43.2,  22.5,  -6.8,   // particle 16
     28.6, -31.4,  19.3,   // particle 18
    };
    std::vector<double> expectedMs2 = {
     3.4,  // particle 0
     0.9,  // particle 2
     9.1,  // particle 4
     6.0,  // particle 6
     1.3,  // particle 7
     4.1,  // particle 9
     9.8,  // particle 11
     7.4,  // particle 13
     6.7,  // particle 16
     1.8,  // particle 18
    };

    std::vector<double> expectedRs2 = {
     12.5,  // particle 0
     45.7,  // particle 2
     67.1,  // particle 4
     98.0,  // particle 6
     31.4,  // particle 7
     54.3,  // particle 9
     63.5,  // particle 11
     92.4,  // particle 13
     14.2,  // particle 16
     58.9,  // particle 18
    };

    // core 3
    std::vector<double> expectedXs3 = {
      39.96,   208.13,  -174.29,    // particle 15
    };
    std::vector<double> expectedVs3 = {
     64.0, -22.8,  15.2,   // particle 15
    };
    std::vector<double> expectedFs3 = {
     26.4, -11.3,   4.9,   // particle 15
    };
    std::vector<double> expectedMs3 = {
     2.1,  // particle 15
    };

    std::vector<double> expectedRs3 = {
     71.0,  // particle 15
    };

    // core 4
    std::vector<double> expectedXs4 = {
    -178.64,   254.19,  -133.58,    // particle 1
     -45.22,   199.73,  -287.90,    // particle 3
    -299.45,   120.38,   -44.91,    // particle 5
     -88.17,   281.56,   -12.49,    // particle 8
    -215.90,    33.14,   119.62,    // particle 10
     -31.50,   146.22,  -250.84,    // particle 12
    -194.05,    77.80,  -299.12,    // particle 17
    -270.33,   295.88,   -21.76     // particle 19
    };
    std::vector<double> expectedVs4 = {
    -88.6,  14.3, -56.7,   // particle 1
    -97.1,  28.9, -74.4,   // particle 3
     54.7, -66.3,  77.1,   // particle 5
     59.7, -24.6,  10.9,   // particle 8
     21.7, -38.4,  68.2,   // particle 10
     46.1, -61.5,  33.8,   // particle 12
     81.9, -35.0,  24.1,   // particle 17
    -16.7,  73.3, -58.9    // particle 19
    };
    std::vector<double> expectedFs4 = {
    -18.6,   7.3, -41.5,   // particle 1
    -22.1,  16.0, -49.7,   // particle 3
     44.5, -36.7,  21.4,   // particle 5
     47.3, -19.5,   6.2,   // particle 8
     14.8,  -7.3,  39.6,   // particle 10
     41.2, -29.8,   9.7,   // particle 12
     35.0, -18.1,  13.7,   // particle 17
     -8.2,  46.7, -24.5    // particle 19
    };
    std::vector<double> expectedMs4 = {
     7.8,  // particle 1
     5.6,  // particle 3
     2.7,  // particle 5
     8.4,  // particle 8
     0.5,  // particle 10
     3.0,  // particle 12
     8.2,  // particle 17
     4.6   // particle 19
    };

    std::vector<double> expectedRs4 = {
     89.3,  // particle 1
     3.9,   // particle 3
     24.6,  // particle 5
     76.2,  // particle 8
     8.7,   // particle 10
     19.8,  // particle 12
     83.1,  // particle 17
     26.4   // particle 19
    };

    // box dimensions
    array<double, 3> min = { -100.0,-100.0,-100.0 };
    array<double, 3> max = { 100.0,100.0,100.0 };


    std::vector<std::vector<double>> expectedXsVec = {
        expectedXs1, expectedXs2, expectedXs3, expectedXs4
    };
    std::vector<std::vector<double>> expectedVsVec = {
        expectedVs1, expectedVs2, expectedVs3, expectedVs4
    };
    std::vector<std::vector<double>> expectedFsVec = {
        expectedFs1, expectedFs2, expectedFs3, expectedFs4
    };
    std::vector<std::vector<double>> expectedRsVec = {
        expectedRs1, expectedRs2, expectedRs3, expectedRs4
    };
    std::vector<std::vector<double>> expectedMsVec = {
        expectedMs1, expectedMs2, expectedMs3, expectedMs4
    };

    auto checking_communicator = [&](const int& myId) {
        // creating the communicator object
        // each communicator gets its id from the MPI_Comm_rank before creation
        // in the real scenarios
        std::unique_ptr<Comm> communicator = std::unique_ptr<Comm>(myId);
        // box
        auto box = std::make_unique<Box>(min, max);
        // putting everything into the engine
        std::vector<std::unique_ptr<Ref>> inputs;
        inputs.push_back(std::move(box));
        inputs.push_back(std::move(communicator));
        // creating the engine fixture
        EngineFixture engineFixture(inputs);
        // getting the engine_ptr
        auto engine_ptr = engineFixture.returnEngine();
        // checking the ids of atoms
        auto& particles = engine_ptr->getParticlesForUpdate();
        // getting xs, vs, fs, ms and rs
        double* myXs = particles->getXData();
        double* myVs = particles->getVData();
        double* myFs = particles->getFData();
        double* myMs = particles->getMData();
        double* myRs = particles->getRData();
        // the number of particles
        int nlocal, nmax;
        particles->getNmaxNlocal(nmax, nlocal);
        // 
        auto& expXs = expectedXsVec[myId];
        auto& expVs = expectedVsVec[myId];
        auto& expFs = expectedFsVec[myId];
        auto& expRs = expectedRsVec[myId];
        auto& expMs = expectedMsVec[myId];
        // checking the number of particles
        REQUIRE(nlocal == expXs.size());
        REQUIRE(nlocal == expVs.size());
        REQUIRE(nlocal == expFs.size());
        REQUIRE(nlocal == expRs.size());
        REQUIRE(nlocal == expMs.size());
        for (int i = 0; i < nlocal; i++) {
            REQUIRE_THAT(expXs[i], Catch::Matchers::WithinAbs(myXs[i], 1e-6));
            REQUIRE_THAT(expVs[i], Catch::Matchers::WithinAbs(myVs[i], 1e-6));
            REQUIRE_THAT(expFs[i], Catch::Matchers::WithinAbs(myFs[i], 1e-6));
            REQUIRE_THAT(expRs[i], Catch::Matchers::WithinAbs(myRs[i], 1e-6));
            REQUIRE_THAT(expMs[i], Catch::Matchers::WithinAbs(myMs[i], 1e-6));

        }
    };
}

