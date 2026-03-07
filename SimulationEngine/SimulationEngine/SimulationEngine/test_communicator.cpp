#include "catch_amalgamated.hpp"
#include "test.hpp"


/// <summary>
/// A text with the default value of the skin parameter
/// </summary>

TEST_CASE("Testing the communication class") {
    std::cout << "Testing the communication class" << std::endl;
    std::cout << std::string(80, '=') << std::endl;


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
    -178.64,   254.19,  -133.58,    // particle 1
     -45.22,   199.73,  -287.90,    // particle 3
    -299.45,   120.38,   -44.91,    // particle 5
     -88.17,   281.56,   -12.49,    // particle 8
    -215.90,    33.14,   119.62,    // particle 10
     -31.50,   146.22,  -250.84,    // particle 12
    -194.05,    77.80,  -299.12,    // particle 17
    -270.33,   295.88,   -21.76     // particle 19
    };
    std::vector<double> expectedVs3 = {
    -88.6,  14.3, -56.7,   // particle 1
    -97.1,  28.9, -74.4,   // particle 3
     54.7, -66.3,  77.1,   // particle 5
     59.7, -24.6,  10.9,   // particle 8
     21.7, -38.4,  68.2,   // particle 10
     46.1, -61.5,  33.8,   // particle 12
     81.9, -35.0,  24.1,   // particle 17
    -16.7,  73.3, -58.9    // particle 19
    };
    std::vector<double> expectedFs3 = {
    -18.6,   7.3, -41.5,   // particle 1
    -22.1,  16.0, -49.7,   // particle 3
     44.5, -36.7,  21.4,   // particle 5
     47.3, -19.5,   6.2,   // particle 8
     14.8,  -7.3,  39.6,   // particle 10
     41.2, -29.8,   9.7,   // particle 12
     35.0, -18.1,  13.7,   // particle 17
     -8.2,  46.7, -24.5    // particle 19
    };
    std::vector<double> expectedMs3 = {
     7.8,  // particle 1
     5.6,  // particle 3
     2.7,  // particle 5
     8.4,  // particle 8
     0.5,  // particle 10
     3.0,  // particle 12
     8.2,  // particle 17
     4.6   // particle 19
    };

    std::vector<double> expectedRs3 = {
     89.3,  // particle 1
     3.9,   // particle 3
     24.6,  // particle 5
     76.2,  // particle 8
     8.7,   // particle 10
     19.8,  // particle 12
     83.1,  // particle 17
     26.4   // particle 19
    };

    // core 4
    std::vector<double> expectedXs4 = {
      39.96,   208.13,  -174.29,    // particle 15
    };
    std::vector<double> expectedVs4 = {
     64.0, -22.8,  15.2,   // particle 15
    };
    std::vector<double> expectedFs4 = {
     26.4, -11.3,   4.9,   // particle 15
    };
    std::vector<double> expectedMs4 = {
     2.1,  // particle 15
    };

    std::vector<double> expectedRs4 = {
     71.0,  // particle 15
    };

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


    auto checking_communicator = [&](const int& myId, std::array<int, 3>& nranks) {
        // creating the communicator object
        // each communicator gets its id from the MPI_Comm_rank before creation
        // in the real scenarios
        std::unique_ptr<Communicator> communicator = std::make_unique<Communicator>(myId, nranks);
        // box dimensions
        array<double, 3> min = { -300.0,-300.0,-300.0 };
        array<double, 3> max = { 300.0, 300.0, 300.0 };
        // box
        std::unique_ptr<Box> box = std::make_unique<Box>(min, max);
        // particles
        int nmax = 20;
        int nlocal = 20;
        // copies since we used the std::move in the mockedParticles class
        std::vector<double> xCopy = x;
        std::vector<double> vCopy = v;
        std::vector<double> fCopy = f;
        std::vector<double> rCopy = r;
        std::vector<double> mCopy = m;
        // creating the mockedParticles object
        std::unique_ptr<Particles> mockedParticles =
            std::make_unique<MockedParticles>(nmax, xCopy, vCopy, fCopy, rCopy, mCopy);
        // putting everything into the engine
        std::vector<std::unique_ptr<Ref>> inputs;
        inputs.push_back(std::move(mockedParticles));
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
        particles->getNmaxNlocal(nmax, nlocal);
        // 
        auto& expXs = expectedXsVec[myId];
        auto& expVs = expectedVsVec[myId];
        auto& expFs = expectedFsVec[myId];
        auto& expRs = expectedRsVec[myId];
        auto& expMs = expectedMsVec[myId];
        // checking the number of particles
        REQUIRE(3 * nlocal == expXs.size());
        REQUIRE(3 * nlocal == expVs.size());
        REQUIRE(3 * nlocal == expFs.size());
        REQUIRE(nlocal == expRs.size());
        REQUIRE(nlocal == expMs.size());
        for (int i = 0; i < 3 * nlocal; i++) {
            REQUIRE_THAT(expXs[i], Catch::Matchers::WithinAbs(myXs[i], 1e-6));
            REQUIRE_THAT(expVs[i], Catch::Matchers::WithinAbs(myVs[i], 1e-6));
            REQUIRE_THAT(expFs[i], Catch::Matchers::WithinAbs(myFs[i], 1e-6));
        }
        for (int i = 0; i < nlocal; i++) {
            REQUIRE_THAT(expRs[i], Catch::Matchers::WithinAbs(myRs[i], 1e-6));
            REQUIRE_THAT(expMs[i], Catch::Matchers::WithinAbs(myMs[i], 1e-6));

        }
    };

    // the communicator configuration
    std::array<int, 3> nranks = { 2,2,1 };
    // first rank
    int id = 0;
    checking_communicator(id, nranks);
    // the second rank
    id = 1;
    checking_communicator(id, nranks);
    // the third rank
    id = 2;
    checking_communicator(id, nranks);
    // the fourth rank
    id = 3;
    checking_communicator(id, nranks);
}

/// <summary>
/// A text with the skin value of 50
/// </summary>

TEST_CASE("Testing updating the ghost atoms in the particles class") {
    std::cout << "Testing the communication class" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    // Particles
    std::vector<double> x = {
        // Global: x,y,z in [-300,300], skin=50
        // Owned regions (quadrants):
        // Q00: x<0, y<0     Q10: x>0, y<0
        // Q01: x<0, y>0     Q11: x>0, y>0
        //
        // Ghost meaning (example):
        // For Q00 (x<0,y<0): needs ghosts from neighbor sides:
        //   +x slab: 0<=x<50 with y<0
        //   +y slab: x<0 with 0<=y<50
        //   corner:  0<=x<50 and 0<=y<50

        // ---------- OWNED (interior, far from boundaries) ----------
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior)
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior)

         180.62, -210.44,   60.91,   // particle 2  owned by Q10 (+- interior)
         260.11, -140.88, -200.30,   // particle 3  owned by Q10 (+- interior)

        -200.90,  190.36,  -40.58,   // particle 4  owned by Q01 (-+ interior)
        -270.22,  120.74,  210.46,   // particle 5  owned by Q01 (-+ interior)

         210.83,  220.41, -150.88,   // particle 6  owned by Q11 (++ interior)
         140.62,  160.33,  198.21,   // particle 7  owned by Q11 (++ interior)


        // For Q00 ghosts: need atoms in (0<=x<50,y<0) and/or (x<0,0<=y<50)
          25.60, -160.15,  -55.27,   // particle 8  in ghost of Q00 from +x slab (0<=x<50,y<0); owned by Q10
        -120.33,   20.18, -199.05,   // particle 9  in ghost of Q00 from +y slab (x<0,0<=y<50); owned by Q01
          30.25,   22.40,   10.66,   // particle 10 corner ghost for Q00 (0<=x<50,0<=y<50); owned by Q11

        // For Q10 ghosts: need atoms in (-50<x<=0,y<0) and/or (x>0,0<=y<50)
         -35.10, -140.48,   12.66,   // particle 11 in ghost of Q10 from -x slab (-50<x<=0,y<0); owned by Q00
         160.81,   35.42,  289.73,   // particle 12 in ghost of Q10 from +y slab (x>0,0<=y<50); owned by Q11
         -22.80,   18.10, -140.25,   // particle 13 corner ghost for Q10 (-50<x<=0,0<=y<50); owned by Q01

        // For Q01 ghosts: need atoms in (0<=x<50,y>0) and/or (x<0,-50<y<=0)
          40.11,  170.25,  204.67,   // particle 14 in ghost of Q01 from +x slab (0<=x<50,y>0); owned by Q11
        -180.66,  -30.77,   14.95,   // particle 15 in ghost of Q01 from -y slab (x<0,-50<y<=0); owned by Q00
          18.40,  -22.10,   95.50,   // particle 16 corner ghost for Q01 (0<=x<50,-50<y<=0); owned by Q10

        // For Q11 ghosts: need atoms in (-50<x<=0,y>0) and/or (x>0,-50<y<=0)
         -28.20,  180.66,   66.03,   // particle 17 in ghost of Q11 from -x slab (-50<x<=0,y>0); owned by Q01
         190.27,  -40.22, -236.70,   // particle 18 in ghost of Q11 from -y slab (x>0,-50<y<=0); owned by Q10
         -12.60,  -18.90,  130.10    // particle 19 corner ghost for Q11 (-50<x<=0,-50<y<=0); owned by Q00
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


    // the skin values
    double skin = 50.0;
    // expected values for each core
    // core 1
    int expectedNlocal1 = 5;
    int expectedNghost1 = 4;
    std::vector<double> expectedXs1 = {
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) rank1
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) rank1
         -35.10, -140.48,   12.66,   // particle 11 owned by Q00 (-- interior) rank1
        -180.66,  -30.77,   14.95,   // particle 15 owned by Q00 (-- interior) rank1
         -12.60,  -18.90,  130.10,   // particle 19 owned by Q00 (-- interior) rank1

          25.60, -160.15,  -55.27,   // particle 8  owned by Q10 (+- interior) rank2
        -120.33,   20.18, -199.05,   // particle 9  owned by Q01 (-+ interior) rank3
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
          18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
    };
    std::vector<double> expectedVs1 = {
     72.4, -33.1,  95.0,   // particle 0
    -88.6,  14.3, -56.7,   // particle 1
    -14.9,  87.6, -29.3,   // particle 11
     64.0, -22.8,  15.2,   // particle 15
    -16.7,  73.3, -58.9,   // particle 19

     59.7, -24.6,  10.9,   // particle 8
    -72.5,  44.3, -95.8,   // particle 9
    -79.2,  12.4,  99.1,   // particle 13
    -90.3,  48.6,  -7.4,   // particle 16
    };
    std::vector<double> expectedFs1 = {
     12.4, -33.9,  45.2,   // particle 0
    -18.6,   7.3, -41.5,   // particle 1
    -21.9,  30.4, -16.5,   // particle 11
     26.4, -11.3,   4.9,   // particle 15
     -8.2,  46.7, -24.5,   // particle 19

     47.3, -19.5,   6.2,   // particle 8
    -34.8,  25.1, -45.6,   // particle 9
    -48.1,  12.6,  33.9,   // particle 13
    -43.2,  22.5,  -6.8,   // particle 16
    };
    std::vector<double> expectedMs1 = {
     3.4,  // particle 0
     7.8,  // particle 1
     9.8,  // particle 11
     2.1,  // particle 15
     4.6,  // particle 19

     8.4,  // particle 8
     4.1,  // particle 9
     7.4,  // particle 13
     6.7,  // particle 16
    };

    std::vector<double> expectedRs1 = {
     12.5,  // particle 0
     89.3,  // particle 1
     63.5,  // particle 11
     71.0,  // particle 15
     26.4,   // particle 19

     76.2,  // particle 8
     54.3,  // particle 9
     92.4,  // particle 13
     14.2,  // particle 16
    };

    // core 2
    int expectedNlocal2 = 5;
    int expectedNghost2 = 4;
    std::vector<double> expectedXs2 = {
         180.62, -210.44,   60.91,   // particle 2  owned by Q10 (+- interior) rank2
         260.11, -140.88, -200.30,   // particle 3  owned by Q10 (+- interior) rank2
          25.60, -160.15,  -55.27,   // particle 8  owned by Q10 (+- interior) rank2
          18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
         190.27,  -40.22, -236.70,   // particle 18 owned by Q10 (+- interior) rank2

          30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
         -35.10, -140.48,   12.66,   // particle 11 owned by Q00 (-- interior) rank1
         160.81,   35.42,  289.73,   // particle 12 owned by Q11 (++ interior) rank4
         -12.60,  -18.90,  130.10    // particle 19 owned by Q00 (-- interior) rank1
    };

    std::vector<double> expectedVs2 = {
     41.2, -19.8,  63.5,   // particle 2
    -97.1,  28.9, -74.4,   // particle 3
     59.7, -24.6,  10.9,   // particle 8
    -90.3,  48.6,  -7.4,   // particle 16
     57.8, -69.2,  39.4,   // particle 18

     21.7, -38.4,  68.2,   // particle 10
    -14.9,  87.6, -29.3,   // particle 11
     46.1, -61.5,  33.8,   // particle 12
    -16.7,  73.3, -58.9    // particle 19
    };
    std::vector<double> expectedFs2 = {
     29.7,  -5.4,  38.9,   // particle 2
    -22.1,  16.0, -49.7,   // particle 3
     47.3, -19.5,   6.2,   // particle 8
    -43.2,  22.5,  -6.8,   // particle 16
     28.6, -31.4,  19.3,   // particle 18

     14.8,  -7.3,  39.6,   // particle 10
    -21.9,  30.4, -16.5,   // particle 11
     41.2, -29.8,   9.7,   // particle 12
     -8.2,  46.7, -24.5    // particle 19
    };
    std::vector<double> expectedMs2 = {
     0.9,  // particle 2
     5.6,  // particle 3
     8.4,  // particle 8
     6.7,  // particle 16
     1.8,  // particle 18

     0.5,  // particle 10
     9.8,  // particle 11
     3.0,  // particle 12
     4.6   // particle 19
    };

    std::vector<double> expectedRs2 = {
     45.7,  // particle 2
     3.9,   // particle 3
     76.2,  // particle 8
     14.2,  // particle 16
     58.9,  // particle 18

     8.7,   // particle 10
     63.5,  // particle 11
     19.8,  // particle 12
     26.4   // particle 19
    };

    // core 3
    int expectedNlocal3 = 5;
    int expectedNghost3 = 4;
    std::vector<double> expectedXs3 = {
        -200.90,  190.36,  -40.58,   // particle 4  owned by Q01 (-+ interior) rank3
        -270.22,  120.74,  210.46,   // particle 5  owned by Q01 (-+ interior) rank3
        -120.33,   20.18, -199.05,   // particle 9  owned by Q01 (-+ interior) rank3
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
         -28.20,  180.66,   66.03,   // particle 17 owned by Q01 (-+ interior) rank3

          30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
          40.11,  170.25,  204.67,   // particle 14 owned by Q11 (++ interior) rank4
        -180.66,  -30.77,   14.95,   // particle 15 owned by Q00 (-- interior) rank1
         -12.60,  -18.90,  130.10    // particle 19 owned by Q00 (-- interior) rank1
    };
    std::vector<double> expectedVs3 = {
      3.6,  89.2, -12.5,   // particle 4
     54.7, -66.3,  77.1,   // particle 5
    -72.5,  44.3, -95.8,   // particle 9
    -79.2,  12.4,  99.1,   // particle 13
     81.9, -35.0,  24.1,   // particle 17


     21.7, -38.4,  68.2,   // particle 10
    -53.7,  27.5, -41.6,   // particle 14
     64.0, -22.8,  15.2,   // particle 15
    -16.7,  73.3, -58.9    // particle 19
    };
    std::vector<double> expectedFs3 = {
      3.1,  27.6, -14.8,   // particle 4
     44.5, -36.7,  21.4,   // particle 5
    -34.8,  25.1, -45.6,   // particle 9
    -48.1,  12.6,  33.9,   // particle 13
     35.0, -18.1,  13.7,   // particle 17

     14.8,  -7.3,  39.6,   // particle 10
    -25.7,  17.8, -38.6,   // particle 14
     26.4, -11.3,   4.9,   // particle 15
     -8.2,  46.7, -24.5    // particle 19
    };
    std::vector<double> expectedMs3 = {
     9.1,  // particle 4
     2.7,  // particle 5
     4.1,  // particle 9
     7.4,  // particle 13
     8.2,  // particle 17

     0.5,  // particle 10
     5.9,  // particle 14
     2.1,  // particle 15
     4.6   // particle 19
    };

    std::vector<double> expectedRs3 = {
     67.1,  // particle 4
     24.6,  // particle 5
     54.3,  // particle 9
     92.4,  // particle 13
     83.1,  // particle 17

     8.7,   // particle 10
     37.6,  // particle 14
     71.0,  // particle 15
     26.4   // particle 19
    };

    // core 4
    int expectedNlocal4 = 5;
    int expectedNghost4 = 4;
    std::vector<double> expectedXs4 = {
         210.83,  220.41, -150.88,   // particle 6  owned by Q11 (++ interior) rank4
         140.62,  160.33,  198.21,   // particle 7  owned by Q11 (++ interior) rank4
          30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
         160.81,   35.42,  289.73,   // particle 12 owned by Q11 (++ interior) rank4
          40.11,  170.25,  204.67,   // particle 14 owned by Q11 (++ interior) rank4

         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
          18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
         -28.20,  180.66,   66.03,   // particle 17 owned by Q01 (-+ interior) rank3
         190.27,  -40.22, -236.70    // particle 18 owned by Q10 (+- interior) rank2
    };
    std::vector<double> expectedVs4 = {
    -45.9,  18.0,  92.6,   // particle 6
    -11.4,  36.8, -83.2,   // particle 7
     21.7, -38.4,  68.2,   // particle 10
     46.1, -61.5,  33.8,   // particle 12
    -53.7,  27.5, -41.6,   // particle 14

    -79.2,  12.4,  99.1,   // particle 13
    -90.3,  48.6,  -7.4,   // particle 16
     81.9, -35.0,  24.1,   // particle 17
     57.8, -69.2,  39.4,   // particle 18
    };
    std::vector<double> expectedFs4 = {
     -9.9,   5.6,  31.2,   // particle 6
    -12.7,  18.9, -28.4,   // particle 7
     14.8,  -7.3,  39.6,   // particle 10
     41.2, -29.8,   9.7,   // particle 12
    -25.7,  17.8, -38.6,   // particle 14

    -48.1,  12.6,  33.9,   // particle 13
    -43.2,  22.5,  -6.8,   // particle 16
     35.0, -18.1,  13.7,   // particle 17
     28.6, -31.4,  19.3,   // particle 18
    };
    std::vector<double> expectedMs4 = {
     6.0,  // particle 6
     1.3,  // particle 7
     0.5,  // particle 10
     3.0,  // particle 12
     5.9,  // particle 14

     7.4,  // particle 13
     6.7,  // particle 16
     8.2,  // particle 17
     1.8,  // particle 18
    };

    std::vector<double> expectedRs4 = {
     98.0,  // particle 6
     31.4,  // particle 7
     8.7,   // particle 10
     19.8,  // particle 12
     37.6,  // particle 14


     92.4,  // particle 13
     14.2,  // particle 16
     83.1,  // particle 17
     58.9,  // particle 18
    };

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
    std::vector<int> expectedNLocalVec = {
        expectedNlocal1, expectedNlocal2, expectedNlocal3, expectedNlocal4
    };
    std::vector<int> expectedNGhostVec = {
        expectedNghost1, expectedNghost2, expectedNghost3, expectedNghost4
    };


    int coreId = 0;
    auto checking_communicator = [&](const int& myId, std::array<int, 3>& nranks) {
        // creating the communicator object
        // each communicator gets its id from the MPI_Comm_rank before creation
        // in the real scenarios
        std::unique_ptr<Communicator> communicator = std::make_unique<Communicator>(myId, nranks, skin);
        // box dimensions
        array<double, 3> min = { -300.0,-300.0,-300.0 };
        array<double, 3> max = { 300.0, 300.0, 300.0 };
        // box
        std::unique_ptr<Box> box = std::make_unique<Box>(min, max);
        // particles
        int nmax = 20;
        int nlocal = 20;
        int nghosts = 0;
        // copies since we used the std::move in the mockedParticles class
        std::vector<double> xCopy = x;
        std::vector<double> vCopy = v;
        std::vector<double> fCopy = f;
        std::vector<double> rCopy = r;
        std::vector<double> mCopy = m;
        // creating the mockedParticles object
        std::unique_ptr<Particles> mockedParticles =
            std::make_unique<MockedParticles>(nmax, xCopy, vCopy, fCopy, rCopy, mCopy);
        // putting everything into the engine
        std::vector<std::unique_ptr<Ref>> inputs;
        inputs.push_back(std::move(mockedParticles));
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
        particles->getNmaxNlocal(nmax, nlocal);
        particles->getNGhosts(nghosts);
        // checking the number of ghost atoms
        REQUIRE(expectedNGhostVec[myId] == nghosts);
        // 
        auto& expXs = expectedXsVec[myId];
        auto& expVs = expectedVsVec[myId];
        auto& expFs = expectedFsVec[myId];
        auto& expRs = expectedRsVec[myId];
        auto& expMs = expectedMsVec[myId];
        // checking the number of particles
        REQUIRE(expectedNLocalVec[myId] == nlocal);
        const int natoms = nlocal + nghosts;
        for (int i = 0; i < 3 * natoms; i++) {
            REQUIRE_THAT(expXs[i], Catch::Matchers::WithinAbs(myXs[i], 1e-6));
            REQUIRE_THAT(expVs[i], Catch::Matchers::WithinAbs(myVs[i], 1e-6));
            REQUIRE_THAT(expFs[i], Catch::Matchers::WithinAbs(myFs[i], 1e-6));
        }
        for (int i = 0; i < nlocal + nghosts; i++) {
            REQUIRE_THAT(expRs[i], Catch::Matchers::WithinAbs(myRs[i], 1e-6));
            REQUIRE_THAT(expMs[i], Catch::Matchers::WithinAbs(myMs[i], 1e-6));
        }

        };

    // the communicator configuration
    std::array<int, 3> nranks = { 2,2,1 };
    // first rank
    int id = 0;
    checking_communicator(id, nranks);
    // the second rank
    id = 1;
    checking_communicator(id, nranks);
    // the third rank
    id = 2;
    checking_communicator(id, nranks);
    // the fourth rank
    id = 3;
    checking_communicator(id, nranks);
}



TEST_CASE("Testing the forward communication") {
    std::cout << "Testing the forward communication" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    // data 
    // number of ranks 
    constexpr int nranks = 4;
    // Particles
    std::vector<double> x = {
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) rank1
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) rank1
         180.62, -210.44,   60.91,   // particle 2  owned by Q10 (+- interior) rank2
         260.11, -140.88, -200.30,   // particle 3  owned by Q10 (+- interior) rank2
        -200.90,  190.36,  -40.58,   // particle 4  owned by Q01 (-+ interior) rank3
        -270.22,  120.74,  210.46,   // particle 5  owned by Q01 (-+ interior) rank3
         210.83,  220.41, -150.88,   // particle 6  owned by Q11 (++ interior) rank4
         140.62,  160.33,  198.21,   // particle 7  owned by Q11 (++ interior) rank4
          25.60, -160.15,  -55.27,   // particle 8  owned by Q10 (+- interior) rank2
        -120.33,   20.18, -199.05,   // particle 9  owned by Q01 (-+ interior) rank3
          30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
         -35.10, -140.48,   12.66,   // particle 11 owned by Q00 (-- interior) rank1
         160.81,   35.42,  289.73,   // particle 12 owned by Q11 (++ interior) rank4
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
          40.11,  170.25,  204.67,   // particle 14 owned by Q11 (++ interior) rank4
        -180.66,  -30.77,   14.95,   // particle 15 owned by Q00 (-- interior) rank1
          18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
         -28.20,  180.66,   66.03,   // particle 17 owned by Q01 (-+ interior) rank3
         190.27,  -40.22, -236.70,   // particle 18 owned by Q10 (+- interior) rank2
         -12.60,  -18.90,  130.10    // particle 19 owned by Q00 (-- interior) rank1
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

    // the skin values
    double skin = 50.0;

    // expected values for each core
    // core 1
    std::vector<double> xs1 = {
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) rank1
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) rank1
         -35.10, -140.48,   12.66,   // particle 11 owned by Q00 (-- interior) rank1
        -180.66,  -30.77,   14.95,   // particle 15 owned by Q00 (-- interior) rank1
         -12.60,  -18.90,  130.10,   // particle 19 owned by Q00 (-- interior) rank1

          25.60, -160.15,  -55.27,   // particle 8  owned by Q10 (+- interior) rank2
        -120.33,   20.18, -199.05,   // particle 9  owned by Q01 (-+ interior) rank3
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
          18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
    };
    std::vector<double> vs1 = {
     72.4, -33.1,  95.0,   // particle 0
    -88.6,  14.3, -56.7,   // particle 1
    -14.9,  87.6, -29.3,   // particle 11
     64.0, -22.8,  15.2,   // particle 15
    -16.7,  73.3, -58.9,   // particle 19

    34.8,  -67.2,   12.5,  // particle 8
   -81.4,   53.7,  -42.9,  // particle 9
   -15.6,   27.9,   88.3,  // particle 13
   -92.1,  -11.4,   46.7,  // particle 16
    };
    std::vector<double> fs1 = {
     12.4, -33.9,  45.2,   // particle 0
    -18.6,   7.3, -41.5,   // particle 1
    -21.9,  30.4, -16.5,   // particle 11
     26.4, -11.3,   4.9,   // particle 15
     -8.2,  46.7, -24.5,   // particle 19

     21.7, -34.9,  12.4,   // particle 8
    -18.3,  27.6, -41.5,   // particle 9
    -45.2,   9.8,  36.1,   // particle 13
    -32.7,  14.3,  -7.9,   // particle 16
    };
    std::vector<double> ms1 = {
     3.4,  // particle 0
     7.8,  // particle 1
     9.8,  // particle 11
     2.1,  // particle 15
     4.6,  // particle 19

     8.4,  // particle 8
     4.1,  // particle 9
     7.4,  // particle 13
     6.7,  // particle 16
    };

    std::vector<double> rs1 = {
     12.5,  // particle 0
     89.3,  // particle 1
     63.5,  // particle 11
     71.0,  // particle 15
     26.4,   // particle 19

     76.2,  // particle 8
     54.3,  // particle 9
     92.4,  // particle 13
     14.2,  // particle 16
    };
    // expected values
    int expectedNlocal1 = 5;
    int expectedNghost1 = 4;
    std::vector<double> expectedXs1 = {
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) rank1
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) rank1
         -35.10, -140.48,   12.66,   // particle 11 owned by Q00 (-- interior) rank1
        -180.66,  -30.77,   14.95,   // particle 15 owned by Q00 (-- interior) rank1
         -12.60,  -18.90,  130.10,   // particle 19 owned by Q00 (-- interior) rank1

          25.60, -160.15,  -55.27,   // particle 8  owned by Q10 (+- interior) rank2
        -120.33,   20.18, -199.05,   // particle 9  owned by Q01 (-+ interior) rank3
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
          18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
    };
    std::vector<double> expectedVs1 = {
     72.4, -33.1,  95.0,   // particle 0
    -88.6,  14.3, -56.7,   // particle 1
    -14.9,  87.6, -29.3,   // particle 11
     64.0, -22.8,  15.2,   // particle 15
    -16.7,  73.3, -58.9,   // particle 19

     59.7, -24.6,  10.9,   // particle 8
    -72.5,  44.3, -95.8,   // particle 9
    -79.2,  12.4,  99.1,   // particle 13
    -90.3,  48.6,  -7.4,   // particle 16
    };
    std::vector<double> expectedFs1 = {
     12.4, -33.9,  45.2,   // particle 0
    -18.6,   7.3, -41.5,   // particle 1
    -21.9,  30.4, -16.5,   // particle 11
     26.4, -11.3,   4.9,   // particle 15
     -8.2,  46.7, -24.5,   // particle 19

     47.3, -19.5,   6.2,   // particle 8
    -34.8,  25.1, -45.6,   // particle 9
    -48.1,  12.6,  33.9,   // particle 13
    -43.2,  22.5,  -6.8,   // particle 16
    };
    std::vector<double> expectedMs1 = {
     3.4,  // particle 0
     7.8,  // particle 1
     9.8,  // particle 11
     2.1,  // particle 15
     4.6,  // particle 19

     8.4,  // particle 8
     4.1,  // particle 9
     7.4,  // particle 13
     6.7,  // particle 16
    };

    std::vector<double> expectedRs1 = {
     12.5,  // particle 0
     89.3,  // particle 1
     63.5,  // particle 11
     71.0,  // particle 15
     26.4,   // particle 19

     76.2,  // particle 8
     54.3,  // particle 9
     92.4,  // particle 13
     14.2,  // particle 16
    };

    // core 2
    std::vector<double> xs2 = {
        180.62, -210.44,   60.91,   // particle 2  owned by Q10 (+- interior) rank2
        260.11, -140.88, -200.30,   // particle 3  owned by Q10 (+- interior) rank2
         25.60, -160.15,  -55.27,   // particle 8  owned by Q10 (+- interior) rank2
         18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
        190.27,  -40.22, -236.70,   // particle 18 owned by Q10 (+- interior) rank2

         30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
        -35.10, -140.48,   12.66,   // particle 11 owned by Q00 (-- interior) rank1
        160.81,   35.42,  289.73,   // particle 12 owned by Q11 (++ interior) rank4
        -12.60,  -18.90,  130.10    // particle 19 owned by Q00 (-- interior) rank1
    };
    std::vector<double> vs2 = {
     41.2, -19.8,  63.5,   // particle 2
    -97.1,  28.9, -74.4,   // particle 3
     59.7, -24.6,  10.9,   // particle 8
    -90.3,  48.6,  -7.4,   // particle 16
     57.8, -69.2,  39.4,   // particle 18

    73.4,  -26.1,   14.8,   // particle 10
   -58.7,   39.5,  -82.3,   // particle 11
   -67.9,   18.2,   91.6,   // particle 12
   -84.1,   47.2,   -9.3,   // particle 19
    };
    std::vector<double> fs2 = {
     29.7,  -5.4,  38.9,   // particle 2
    -22.1,  16.0, -49.7,   // particle 3
     47.3, -19.5,   6.2,   // particle 8
    -43.2,  22.5,  -6.8,   // particle 16
     28.6, -31.4,  19.3,   // particle 18

    -62.4,  18.7, -33.1,   // particle 10
     41.6, -72.3,  54.8,   // particle 11
     -9.5,  63.2, -47.6,   // particle 12
     78.1, -14.9,  21.7,   // particle 19
    };
    std::vector<double> ms2 = {
     0.9,  // particle 2
     5.6,  // particle 3
     8.4,  // particle 8
     6.7,  // particle 16
     1.8,  // particle 18

     2.6,  // particle 10
     9.3,  // particle 11
     5.8,  // particle 12
     1.7,  // particle 19
    };

    std::vector<double> rs2 = {
     45.7,  // particle 2
     3.9,   // particle 3
     76.2,  // particle 8
     14.2,  // particle 16
     58.9,  // particle 18

     38.7,  // particle 10
     81.5,  // particle 11
     67.2,  // particle 12
     23.9,  // particle 19
    };
    // expected values 
    int expectedNlocal2 = 5;
    int expectedNghost2 = 4;
    std::vector<double> expectedXs2 = {
         180.62, -210.44,   60.91,   // particle 2  owned by Q10 (+- interior) rank2
         260.11, -140.88, -200.30,   // particle 3  owned by Q10 (+- interior) rank2
          25.60, -160.15,  -55.27,   // particle 8  owned by Q10 (+- interior) rank2
          18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
         190.27,  -40.22, -236.70,   // particle 18 owned by Q10 (+- interior) rank2

          30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
         -35.10, -140.48,   12.66,   // particle 11 owned by Q00 (-- interior) rank1
         160.81,   35.42,  289.73,   // particle 12 owned by Q11 (++ interior) rank4
         -12.60,  -18.90,  130.10    // particle 19 owned by Q00 (-- interior) rank1
    };

    std::vector<double> expectedVs2 = {
     41.2, -19.8,  63.5,   // particle 2
    -97.1,  28.9, -74.4,   // particle 3
     59.7, -24.6,  10.9,   // particle 8
    -90.3,  48.6,  -7.4,   // particle 16
     57.8, -69.2,  39.4,   // particle 18

     21.7, -38.4,  68.2,   // particle 10
    -14.9,  87.6, -29.3,   // particle 11
     46.1, -61.5,  33.8,   // particle 12
    -16.7,  73.3, -58.9    // particle 19
    };
    std::vector<double> expectedFs2 = {
     29.7,  -5.4,  38.9,   // particle 2
    -22.1,  16.0, -49.7,   // particle 3
     47.3, -19.5,   6.2,   // particle 8
    -43.2,  22.5,  -6.8,   // particle 16
     28.6, -31.4,  19.3,   // particle 18

     14.8,  -7.3,  39.6,   // particle 10
    -21.9,  30.4, -16.5,   // particle 11
     41.2, -29.8,   9.7,   // particle 12
     -8.2,  46.7, -24.5    // particle 19
    };
    std::vector<double> expectedMs2 = {
     0.9,  // particle 2
     5.6,  // particle 3
     8.4,  // particle 8
     6.7,  // particle 16
     1.8,  // particle 18

     0.5,  // particle 10
     9.8,  // particle 11
     3.0,  // particle 12
     4.6   // particle 19
    };

    std::vector<double> expectedRs2 = {
     45.7,  // particle 2
     3.9,   // particle 3
     76.2,  // particle 8
     14.2,  // particle 16
     58.9,  // particle 18

     8.7,   // particle 10
     63.5,  // particle 11
     19.8,  // particle 12
     26.4   // particle 19
    };

    // core 3
    std::vector<double> xs3 = {
        -200.90,  190.36,  -40.58,   // particle 4  owned by Q01 (-+ interior) rank3
        -270.22,  120.74,  210.46,   // particle 5  owned by Q01 (-+ interior) rank3
        -120.33,   20.18, -199.05,   // particle 9  owned by Q01 (-+ interior) rank3
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
         -28.20,  180.66,   66.03,   // particle 17 owned by Q01 (-+ interior) rank3

          30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
          40.11,  170.25,  204.67,   // particle 14 owned by Q11 (++ interior) rank4
        -180.66,  -30.77,   14.95,   // particle 15 owned by Q00 (-- interior) rank1
         -12.60,  -18.90,  130.10    // particle 19 owned by Q00 (-- interior) rank1
    };
    std::vector<double> vs3 = {
        3.6,  89.2, -12.5,   // particle 4
       54.7, -66.3,  77.1,   // particle 5
      -72.5,  44.3, -95.8,   // particle 9
      -79.2,  12.4,  99.1,   // particle 13
       81.9, -35.0,  24.1,   // particle 17

       34.6, -57.1,  82.4,   // particle 10
      -61.3,  18.9, -36.7,   // particle 14
       72.5, -11.4,  23.8,   // particle 15
      -28.9,  64.7, -49.2    // particle 19
    };
    std::vector<double> fs3 = {
      3.1,  27.6, -14.8,   // particle 4
     44.5, -36.7,  21.4,   // particle 5
    -34.8,  25.1, -45.6,   // particle 9
    -48.1,  12.6,  33.9,   // particle 13
     35.0, -18.1,  13.7,   // particle 17

     63.5,  -21.4,  48.7,   // particle 10
    -47.9,   36.2, -72.1,   // particle 14
     18.6,  -54.3,  12.8,   // particle 15
    -32.4,   69.5, -15.7    // particle 19
    };
    std::vector<double> ms3 = {
     9.1,  // particle 4
     2.7,  // particle 5
     4.1,  // particle 9
     7.4,  // particle 13
     8.2,  // particle 17

     3.7,  // particle 10
     8.2,  // particle 14
     1.4,  // particle 15
     6.9   // particle 19
    };

    std::vector<double> rs3 = {
     67.1,  // particle 4
     24.6,  // particle 5
     54.3,  // particle 9
     92.4,  // particle 13
     83.1,  // particle 17

     42.3,  // particle 10
     19.8,  // particle 14
     86.5,  // particle 15
     63.1   // particle 19
    };
    // expected values
    int expectedNlocal3 = 5;
    int expectedNghost3 = 4;
    std::vector<double> expectedXs3 = {
        -200.90,  190.36,  -40.58,   // particle 4  owned by Q01 (-+ interior) rank3
        -270.22,  120.74,  210.46,   // particle 5  owned by Q01 (-+ interior) rank3
        -120.33,   20.18, -199.05,   // particle 9  owned by Q01 (-+ interior) rank3
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
         -28.20,  180.66,   66.03,   // particle 17 owned by Q01 (-+ interior) rank3

          30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
          40.11,  170.25,  204.67,   // particle 14 owned by Q11 (++ interior) rank4
        -180.66,  -30.77,   14.95,   // particle 15 owned by Q00 (-- interior) rank1
         -12.60,  -18.90,  130.10    // particle 19 owned by Q00 (-- interior) rank1
    };
    std::vector<double> expectedVs3 = {
      3.6,  89.2, -12.5,   // particle 4
     54.7, -66.3,  77.1,   // particle 5
    -72.5,  44.3, -95.8,   // particle 9
    -79.2,  12.4,  99.1,   // particle 13
     81.9, -35.0,  24.1,   // particle 17


     21.7, -38.4,  68.2,   // particle 10
    -53.7,  27.5, -41.6,   // particle 14
     64.0, -22.8,  15.2,   // particle 15
    -16.7,  73.3, -58.9    // particle 19
    };
    std::vector<double> expectedFs3 = {
      3.1,  27.6, -14.8,   // particle 4
     44.5, -36.7,  21.4,   // particle 5
    -34.8,  25.1, -45.6,   // particle 9
    -48.1,  12.6,  33.9,   // particle 13
     35.0, -18.1,  13.7,   // particle 17

     14.8,  -7.3,  39.6,   // particle 10
    -25.7,  17.8, -38.6,   // particle 14
     26.4, -11.3,   4.9,   // particle 15
     -8.2,  46.7, -24.5    // particle 19
    };
    std::vector<double> expectedMs3 = {
     9.1,  // particle 4
     2.7,  // particle 5
     4.1,  // particle 9
     7.4,  // particle 13
     8.2,  // particle 17

     0.5,  // particle 10
     5.9,  // particle 14
     2.1,  // particle 15
     4.6   // particle 19
    };

    std::vector<double> expectedRs3 = {
     67.1,  // particle 4
     24.6,  // particle 5
     54.3,  // particle 9
     92.4,  // particle 13
     83.1,  // particle 17

     8.7,   // particle 10
     37.6,  // particle 14
     71.0,  // particle 15
     26.4   // particle 19
    };

    // core 4
    std::vector<double> xs4 = {
     210.83,  220.41, -150.88,   // particle 6  owned by Q11 (++ interior) rank4
     140.62,  160.33,  198.21,   // particle 7  owned by Q11 (++ interior) rank4
      30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
     160.81,   35.42,  289.73,   // particle 12 owned by Q11 (++ interior) rank4
      40.11,  170.25,  204.67,   // particle 14 owned by Q11 (++ interior) rank4

     -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
      18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
     -28.20,  180.66,   66.03,   // particle 17 owned by Q01 (-+ interior) rank3
     190.27,  -40.22, -236.70    // particle 18 owned by Q10 (+- interior) rank2
    };
    std::vector<double> vs4 = {
    -45.9,  18.0,  92.6,   // particle 6
    -11.4,  36.8, -83.2,   // particle 7
     21.7, -38.4,  68.2,   // particle 10
     46.1, -61.5,  33.8,   // particle 12
    -53.7,  27.5, -41.6,   // particle 14

    -63.4,  28.7,   74.5,   // particle 13
    -88.1,  41.3,  -12.9,   // particle 16
     69.6, -22.4,   31.8,   // particle 17
     44.7, -57.9,   53.2,   // particle 18
    };
    std::vector<double> fs4 = {
     -9.9,   5.6,  31.2,   // particle 6
    -12.7,  18.9, -28.4,   // particle 7
     14.8,  -7.3,  39.6,   // particle 10
     41.2, -29.8,   9.7,   // particle 12
    -25.7,  17.8, -38.6,   // particle 14

    -21.6,  34.2,   17.5,   // particle 13
    -42.8,  19.7,  -11.3,   // particle 16
     28.4, -14.6,   22.1,   // particle 17
     13.9, -37.5,   41.6,   // particle 18
    };
    std::vector<double> ms4 = {
     6.0,  // particle 6
     1.3,  // particle 7
     0.5,  // particle 10
     3.0,  // particle 12
     5.9,  // particle 14

     2.9,  // particle 13
     9.1,  // particle 16
     4.6,  // particle 17
     7.3,  // particle 18
    };

    std::vector<double> rs4 = {
     98.0,  // particle 6
     31.4,  // particle 7
     8.7,   // particle 10
     19.8,  // particle 12
     37.6,  // particle 14


     64.8,  // particle 13
     27.5,  // particle 16
     83.2,  // particle 17
     41.9,  // particle 18
    };
    // expected values
    int expectedNlocal4 = 5;
    int expectedNghost4 = 4;
    std::vector<double> expectedXs4 = {
         210.83,  220.41, -150.88,   // particle 6  owned by Q11 (++ interior) rank4
         140.62,  160.33,  198.21,   // particle 7  owned by Q11 (++ interior) rank4
          30.25,   22.40,   10.66,   // particle 10 owned by Q11 (++ interior) rank4
         160.81,   35.42,  289.73,   // particle 12 owned by Q11 (++ interior) rank4
          40.11,  170.25,  204.67,   // particle 14 owned by Q11 (++ interior) rank4

         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior) rank3
          18.40,  -22.10,   95.50,   // particle 16 owned by Q10 (+- interior) rank2
         -28.20,  180.66,   66.03,   // particle 17 owned by Q01 (-+ interior) rank3
         190.27,  -40.22, -236.70    // particle 18 owned by Q10 (+- interior) rank2
    };
    std::vector<double> expectedVs4 = {
    -45.9,  18.0,  92.6,   // particle 6
    -11.4,  36.8, -83.2,   // particle 7
     21.7, -38.4,  68.2,   // particle 10
     46.1, -61.5,  33.8,   // particle 12
    -53.7,  27.5, -41.6,   // particle 14

    -79.2,  12.4,  99.1,   // particle 13
    -90.3,  48.6,  -7.4,   // particle 16
     81.9, -35.0,  24.1,   // particle 17
     57.8, -69.2,  39.4,   // particle 18
    };
    std::vector<double> expectedFs4 = {
     -9.9,   5.6,  31.2,   // particle 6
    -12.7,  18.9, -28.4,   // particle 7
     14.8,  -7.3,  39.6,   // particle 10
     41.2, -29.8,   9.7,   // particle 12
    -25.7,  17.8, -38.6,   // particle 14

    -48.1,  12.6,  33.9,   // particle 13
    -43.2,  22.5,  -6.8,   // particle 16
     35.0, -18.1,  13.7,   // particle 17
     28.6, -31.4,  19.3,   // particle 18
    };
    std::vector<double> expectedMs4 = {
     6.0,  // particle 6
     1.3,  // particle 7
     0.5,  // particle 10
     3.0,  // particle 12
     5.9,  // particle 14

     7.4,  // particle 13
     6.7,  // particle 16
     8.2,  // particle 17
     1.8,  // particle 18
    };

    std::vector<double> expectedRs4 = {
     98.0,  // particle 6
     31.4,  // particle 7
     8.7,   // particle 10
     19.8,  // particle 12
     37.6,  // particle 14


     92.4,  // particle 13
     14.2,  // particle 16
     83.1,  // particle 17
     58.9,  // particle 18
    };



    // building the engine
    auto build_engine = [&](const int& myId_, const int& nranks_) -> std::unique_ptr<Engine> {
        // communicators
        std::unique_ptr<Communicator> communicator = std::make_unique<Communicator>(myId_, nranks_, skin);
        // box dimensions
        array<double, 3> min = { -300.0,-300.0,-300.0 };
        array<double, 3> max = { 300.0, 300.0, 300.0 };
        // box
        std::unique_ptr<Box> box = std::make_unique<Box>(min, max);
        // copy since we used the std::move in the mockedParticles class
        std::vector<double> xCopy = x, vCopy = v, fCopy = f, rCopy = r, mCopy = m;
        // creating the mockedParticles object
        int nmax = 20;
        int nlocal = 20;
        int nghosts = 0;
        std::unique_ptr<Particles> mockedParticles =
            std::make_unique<MockedParticles>(nmax, xCopy, vCopy, fCopy, rCopy, mCopy);
        // putting everything into the engine
        std::vector<std::unique_ptr<Ref>> inputs;
        inputs.push_back(std::move(mockedParticles));
        inputs.push_back(std::move(box));
        inputs.push_back(std::move(communicator));
        // creating the engine fixture
        EngineFixture engineFixture1(inputs);
        // getting the engine_ptr
        auto engine_ptr = engineFixture1.returnEngine();
        // returning the engine
        return engine_ptr;

        };

    // forward communication
    // rank 1 
    int myId1 = 0, myId2 = 1, myId3 = 2, myId4 = 3;
    // transfered data
    void* tranDataRaw12 = nullptr;
    void* tranDataRaw13 = nullptr;
    void* tranDataRaw21 = nullptr;
    void* tranDataRaw24 = nullptr;
    void* tranDataRaw31 = nullptr;
    void* tranDataRaw34 = nullptr;
    void* tranDataRaw42 = nullptr;
    void* tranDataRaw43 = nullptr;


    auto engine_ptr1 = build_engine(myId1, nranks);
    auto engine_ptr2 = build_engine(myId2, nranks);
    auto engine_ptr3 = build_engine(myId3, nranks);
    auto engine_ptr4 = build_engine(myId4, nranks);
    // returning the communicators
    auto& communicator1Ref = engine_ptr1->getCommunicator();
    auto& communicator2Ref = engine_ptr2->getCommunicator();
    auto& communicator3Ref = engine_ptr3->getCommunicator();
    auto& communicator4Ref = engine_ptr4->getCommunicator();
    // sending data
    // sending data from rank1 to rank2
    communicator1Ref->forward_particle({ 1,0,0 }, tranDataRaw12);
    // sending data from rank1 to rank3
    communicator1Ref->forward_particle({ 0,1,0 }, tranDataRaw13);
    // sending data from rank2 to rank1
    communicator2Ref->forward_particle({ -1,0,0 }, tranDataRaw21);
    // sending data from rank2 to rank4
    communicator2Ref->forward_particle({ 0,1,0 }, tranDataRaw24);
    // sending data from rank3 to rank1
    communicator3Ref->forward_particle({ 0,-1,0 }, tranDataRaw31);
    // sending data from rank3 to rank4
    communicator3Ref->forward_particle({ 1,0,0 }, tranDataRaw34);
    // sending data from rank4 to rank3
    communicator4Ref->forward_particle({ -1,0,0 }, tranDataRaw43);
    // sending data from rank4 to rank2
    communicator4Ref->forward_particle({ 0,-1,0 }, tranDataRaw42);
    // updating ghosts
    communicator1Ref->updateGhosts({ 1,0,0 }, tranDataRaw21);
    communicator1Ref->updateGhosts({ 0,1,0 }, tranDataRaw31);
    communicator2Ref->updateGhosts({ -1,0,0 }, tranDataRaw12);
    communicator2Ref->updateGhosts({ 0,1,0 }, tranDataRaw42);
    communicator3Ref->updateGhosts({ 1,0,0 }, tranDataRaw43);
    communicator3Ref->updateGhosts({ 0,-1,0 }, tranDataRaw13);
    communicator4Ref->updateGhosts({ -1,0,0 }, tranDataRaw34);
    communicator4Ref->updateGhosts({ 0,-1,0 }, tranDataRaw24);

    // data checker struct
    struct {
        std::unique_ptr<Engine>& engine_ptr;
        int expectedNlocal, expectedNghost;
        std::vector<double> expectedXs, expectedVs, expectedFs, expectedRs, expectedMs;
    } dataCheckerArray[4] = {
        {engine_ptr1,expectedNlocal1,expectedNghost1,expectedXs1,expectedVs1,expectedFs1,expectedRs1,expectedMs1},
        {engine_ptr2,expectedNlocal2,expectedNghost2,expectedXs2,expectedVs2,expectedFs2,expectedRs2,expectedMs2},
        {engine_ptr3,expectedNlocal3,expectedNghost3,expectedXs3,expectedVs3,expectedFs3,expectedRs3,expectedMs3},
        {engine_ptr4,expectedNlocal4,expectedNghost4,expectedXs4,expectedVs4,expectedFs4,expectedRs4,expectedMs4}

    };

    // checking data
    for (auto& dataCheck : dataCheckerArray) {
        // particle reference
        auto& particlesRef = dataCheck.engine_ptr->getParticlesForUpdate();
        // getting the data data
        auto myXs = particlesRef->getXData();
        auto myVs = particlesRef->getVData();
        auto myFs = particlesRef->getFData();
        auto myRs = particlesRef->getRData();
        auto myMs = particlesRef->getMData();
        // natoms, nlocal, nghosts
        int natoms, nlocal, nghosts;
        int nmax;
        particlesRef->getNmaxNlocal(nmax, nlocal);
        particlesRef->getNGhosts(nghosts);
        // checking the total number of particles
        REQUIRE(nlocal == dataCheck.expectedNlocal);
        // checking the number of ghost atoms
        REQUIRE(nghosts == dataCheck.expectedNghost);
        // 
        natoms = nlocal + nghosts;
        // checking the data
        for (int i = 0; i < 3 * natoms; i++) {
            REQUIRE_THAT(dataCheck.expectedXs[i], Catch::Matchers::WithinAbs(myXs[i], 1e-6));
            REQUIRE_THAT(dataCheck.expectedVs[i], Catch::Matchers::WithinAbs(myVs[i], 1e-6));
            REQUIRE_THAT(dataCheck.expectedFs[i], Catch::Matchers::WithinAbs(myFs[i], 1e-6));
        }
        for (int i = 0; i < natoms; i++) {
            REQUIRE_THAT(dataCheck.expectedMs[i], Catch::Matchers::WithinAbs(myMs[i], 1e-6));
            REQUIRE_THAT(dataCheck.expectedRs[i], Catch::Matchers::WithinAbs(myRs[i], 1e-6));
        }
    }

}


