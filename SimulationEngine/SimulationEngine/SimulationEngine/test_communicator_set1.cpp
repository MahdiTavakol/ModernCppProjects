#include "catch_amalgamated.hpp"
#include "test_communicator_helpers.hpp"


/// <summary>
/// The default value of the skin parameter
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


    // the communicator configuration
    std::array<int, 3> nranks = { 2,2,1 };
    // first rank
    int id = 0;
    build_engine_check_communicator(
        id, nranks,
        x, v, f, r, m,
        expectedXsVec,
        expectedVsVec,
        expectedFsVec,
        expectedRsVec,
        expectedMsVec);
    // the second rank
    id = 1;
    build_engine_check_communicator(
        id, nranks,
        x, v, f, r, m,
        expectedXsVec,
        expectedVsVec,
        expectedFsVec,
        expectedRsVec,
        expectedMsVec);
    // the third rank
    id = 2;
    build_engine_check_communicator(
        id, nranks,
        x, v, f, r, m,
        expectedXsVec,
        expectedVsVec,
        expectedFsVec,
        expectedRsVec,
        expectedMsVec);
    // the fourth rank
    id = 3;
    build_engine_check_communicator(
        id, nranks,
        x, v, f, r, m,
        expectedXsVec,
        expectedVsVec,
        expectedFsVec,
        expectedRsVec,
        expectedMsVec);
}

/// <summary>
/// A text with the skin value of 50
/// </summary>

TEST_CASE("Testing updating the ghost atoms in the particles class") {
    std::cout << "Testing updating the ghost atoms in the particles class" << std::endl;
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


    // the communicator configuration
    std::array<int, 3> nranks = { 2,2,1 };
    // first rank
    int id = 0;
    build_engine_check_communicator(
        id, nranks,
        x, v, f, r, m,
        expectedXsVec,
        expectedVsVec,
        expectedFsVec,
        expectedRsVec,
        expectedMsVec,
        skin);
    // the second rank
    id = 1;
    build_engine_check_communicator(
        id, nranks,
        x, v, f, r, m,
        expectedXsVec,
        expectedVsVec,
        expectedFsVec,
        expectedRsVec,
        expectedMsVec,
        skin);
    // the third rank
    id = 2;
    build_engine_check_communicator(
        id, nranks,
        x, v, f, r, m,
        expectedXsVec,
        expectedVsVec,
        expectedFsVec,
        expectedRsVec,
        expectedMsVec,
        skin);
    // the fourth rank
    id = 3;
    build_engine_check_communicator(
        id, nranks,
        x, v, f, r, m,
        expectedXsVec,
        expectedVsVec,
        expectedFsVec,
        expectedRsVec,
        expectedMsVec,
        skin);
}



TEST_CASE("Testing the forward communication") {
    std::cout << "Testing the forward communication" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    // data 
    // number of ranks 
    std::array<int, 3> ranks = { 2,2,1 };
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


    auto engine_ptr1 = set_particles_build_engine(myId1, ranks, x, v, f, r, m, skin);
    auto engine_ptr2 = set_particles_build_engine(myId2, ranks, x, v, f, r, m, skin);
    auto engine_ptr3 = set_particles_build_engine(myId3, ranks, x, v, f, r, m, skin);
    auto engine_ptr4 = set_particles_build_engine(myId4, ranks, x, v, f, r, m, skin);
    // returning the communicators
    auto& communicator1Ref = engine_ptr1->getCommunicator();
    auto& communicator2Ref = engine_ptr2->getCommunicator();
    auto& communicator3Ref = engine_ptr3->getCommunicator();
    auto& communicator4Ref = engine_ptr4->getCommunicator();
    // sending ghosts
    // sending data from rank1 to rank2
    communicator1Ref->sendGhosts(myId2, tranDataRaw12);
    // sending data from rank1 to rank3
    communicator1Ref->sendGhosts(myId3, tranDataRaw13);
    // sending data from rank2 to rank1
    communicator2Ref->sendGhosts(myId1, tranDataRaw21);
    // sending data from rank2 to rank4
    communicator2Ref->sendGhosts(myId4, tranDataRaw24);
    // sending data from rank3 to rank1
    communicator3Ref->sendGhosts(myId1, tranDataRaw31);
    // sending data from rank3 to rank4
    communicator3Ref->sendGhosts(myId4, tranDataRaw34);
    // sending data from rank4 to rank3
    communicator4Ref->sendGhosts(myId3, tranDataRaw43);
    // sending data from rank4 to rank2
    communicator4Ref->sendGhosts(myId2, tranDataRaw42);
    // receiving ghost
    // receiving data from rank2 to rank1
    communicator1Ref->recvGhosts(myId2, tranDataRaw21);
    // receiving data from rank3 to rank1
    communicator1Ref->recvGhosts(myId3, tranDataRaw31);
    // receiving data from rank1 to rank2
    communicator2Ref->recvGhosts(myId1, tranDataRaw12);
    // receiving data from rank4 to rank2
    communicator2Ref->recvGhosts(myId4, tranDataRaw42);
    // receiving data from rank4 to rank3
    communicator3Ref->recvGhosts(myId4, tranDataRaw43);
    // receiving data from rank1 to rank3
    communicator3Ref->recvGhosts(myId1, tranDataRaw13);
    // receiving data from rank3 to rank4
    communicator4Ref->recvGhosts(myId3, tranDataRaw34);
    // receiving data from rank2 to rank4
    communicator4Ref->recvGhosts(myId2, tranDataRaw24);

    // data checker struct
    struct {
        std::unique_ptr<Engine>& engine_ptr;
        std::vector<double> expectedXs, expectedVs, expectedFs, expectedRs, expectedMs;
    } dataCheckerArray[] = {
        {engine_ptr1,expectedXs1,expectedVs1,expectedFs1,expectedRs1,expectedMs1},
        {engine_ptr2,expectedXs2,expectedVs2,expectedFs2,expectedRs2,expectedMs2},
        {engine_ptr3,expectedXs3,expectedVs3,expectedFs3,expectedRs3,expectedMs3},
        {engine_ptr4,expectedXs4,expectedVs4,expectedFs4,expectedRs4,expectedMs4}

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
        // 
        REQUIRE_THAT(myXs, Array3DMatcher(dataCheck.expectedXs.data(), dataCheck.expectedXs.size() / 3, 1e-6));
        REQUIRE_THAT(myVs, Array3DMatcher(dataCheck.expectedVs.data(), dataCheck.expectedVs.size() / 3, 1e-6));
        REQUIRE_THAT(myFs, Array3DMatcher(dataCheck.expectedFs.data(), dataCheck.expectedFs.size() / 3, 1e-6));
        REQUIRE_THAT(myRs, Array1DMatcher(dataCheck.expectedRs.data(), dataCheck.expectedRs.size(), 1e-6));
        REQUIRE_THAT(myMs, Array1DMatcher(dataCheck.expectedMs.data(), dataCheck.expectedMs.size(), 1e-6));
    }

}


TEST_CASE("Testing the destination ranks for each rank")
{
    std::cout << "Testing the destination ranks for each rank" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    std::array<int, 3> ranks1 = { 3,4,5 };
    std::array<int, 3> ranks2 = { 2,4,3 };
    std::array<int, 3> ranks3 = { 4,3,2 };
    std::array<int, 3> ranks4 = { 8,1,1 };
    std::array<int, 3> ranks5 = { 1,8,1 };
    std::array<int, 3> ranks6 = { 1,1,8 };
    std::array<int, 3> ranks7 = { 3,4,5 };
    std::array<int, 3> ranks8 = { 3,4,5 };
    std::array<int, 3> ranks9 = { 3,4,5 };
    std::array<int, 3> ranks10 = { 3,3,1 };

    int myId1 = 2;
    int myId2 = 4;
    int myId3 = 5;
    int myId4 = 6;
    int myId5 = 4;
    int myId6 = 3;
    int myId7 = 16;
    int myId8 = 30;
    int myId9 = 28;
    int myId10 = 1;

    std::array<int, 6> expected1 = { 1,-1,-1,5,-1,14 };
    std::array<int, 6> expected2 = { -1,5,2,6,-1,12 };
    std::array<int, 6> expected3 = { 4,6,1,9,-1,17 };
    std::array<int, 6> expected4 = { 5,7,-1,-1,-1,-1 };
    std::array<int, 6> expected5 = { -1,-1,3,5,-1,-1 };
    std::array<int, 6> expected6 = { -1,-1,-1,-1,2,4 };
    std::array<int, 6> expected7 = { 15,17,13,19,4,28 };
    std::array<int, 6> expected8 = { -1,31,27,33,18,42 };
    std::array<int, 6> expected9 = { 27,29,25,31,16,40 };
    std::array<int, 6> expected10 = { 0,2,-1,4,-1,-1 };

    struct {
        std::array<int, 3> ranks;
        int myId;
        std::array<int, 6> expected;
    } testStruct[] = {
        {ranks1, myId1, expected1},
        {ranks2, myId2, expected2},
        {ranks3, myId3, expected3},
        {ranks4, myId4, expected4},
        {ranks5, myId5, expected5},
        {ranks6, myId6, expected6},
        {ranks7, myId7, expected7},
        {ranks8, myId8, expected8},
        {ranks9, myId9, expected9},
        {ranks10, myId10, expected10}
    };

    for (const auto& test : testStruct)
    {
        // building the engine
        std::vector<double> x = {}, v = {}, f = {}, m = {}, r = {};
        auto engine_ptr = build_engine_set_particles(test.myId, test.ranks, x, v, f, m, r);
        // returning the communicator ref
        auto& communicatorRef = engine_ptr->getCommunicator();
        // checking the reference
        REQUIRE(communicatorRef);
        auto dests = communicatorRef->returnExchangeDests();
        REQUIRE(dests == test.expected);
    }
}

TEST_CASE("Testing the message for sending an out of range particle from a rank")
{
    std::cout << "Testing the message for sending an out of range particle from a rank" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    // Particles
    std::vector<double> x1 = {
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) region 1
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) region 1
        -180.66,   30.77,   14.95,   // particle 2  owned by Q00 (-+ interior) region 3
        -280.66,  130.77,  214.95,   // particle 3  owned by Q00 (-+ interior) region 3
          12.60,  -18.90,  130.10,   // particle 4  owned by Q00 (+- interior) region 2
          35.10, -140.48,   12.66,   // particle 5  owned by Q00 (+- interior) region 2
    };

    std::vector<double> v1 = {
        72.4, -33.1,  95.0,   // particle 0
       -88.6,  14.3, -56.7,   // particle 1
        41.2, -19.8,  63.5,   // particle 2
        51.2, -19.8, 163.5,   // particle 3
       -97.1,  28.9, -74.4,   // particle 4
         3.6,  89.2, -12.5,   // particle 5
    };

    std::vector<double> f1 = {
       12.4, -33.9,  45.2,   // particle 0
      -18.6,   7.3, -41.5,   // particle 1
       29.7,  -5.4,  38.9,   // particle 2
       39.7, -15.4,  18.9,   // particle 3
      -22.1,  16.0, -49.7,   // particle 4
        3.1,  27.6, -14.8,   // particle 5
    };

    std::vector<double> m1 =
    {
        3.4,  // particle 0
        7.8,  // particle 1
        0.9,  // particle 2
        3.0,  // particle 3
        5.6,  // particle 4
        9.1,  // particle 5
    };

    std::vector<double> r1 =
    {
        42.7,  // particle 0
        18.3,  // particle 1
        73.9,  // particle 2
        23.9,  // particle 3
        56.4,  // particle 4
        91.2   // particle 5
    };



    // expected message
    // size
    int expectedMessagesSize[6] = {
        1,
        23,
        1,
        23,
        1,
        1
    };
    // contents
    std::vector<double> expectedMessages[6] = {
        {0.0}, // to xlo --> no xlo
        {2.0,
          12.6, -18.9,130.10,   // x
         -97.1,  28.9, -74.4,   // v
         -22.1,  16.0, -49.7,   // f
           5.6,                 // m
          56.4,                 // r
          35.1,-140.48, 12.66,  // x
           3.6,   89.2, -12.5,  // v
            3.1,  27.6, -14.8,  // f
            9.1,                // m
            91.2                // r
        }, //  to xhi --> to the region 2
        {0.0}, // to ylo --> no ylo
        {2.0,
         -180.66,30.77, 14.95, // x
          41.2, -19.8,  63.5,  // v
          29.7,  -5.4,  38.9,  // f
          0.9,                 // m
          73.9,                // r
        -280.66,  130.77, 214.95,
           51.2,   -19.8, 163.5,
           39.7,   -15.4,  18.9,
           3.0,
           23.9
          }, // to yhi  --> to the region 3
        {0.0},  // to zlo
        {0.0},  // to zhi
    };

    std::vector<double> expectedX1 = {
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) region 1
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) region 1
    };

    std::vector<double> expectedV1 = {
        72.4, -33.1,  95.0,   // particle 0
       -88.6,  14.3, -56.7,   // particle 1
    };

    std::vector<double> expectedF1 = {
       12.4, -33.9,  45.2,   // particle 0
      -18.6,   7.3, -41.5,   // particle 1
    };

    std::vector<double> expectedM1 = {
        3.4,  // particle 0
        7.8,  // particle 1
    };

    std::vector<double> expectedR1 = {
        42.7,  // particle 0
        18.3,  // particle 1
    };

    // expected destinations
    std::array<double, 6> expectedDests = { -1,1,-1,2,-1,-1 };



    int myId = 0;
    std::array<int, 3> ranks = { 2,2,1 };

    // creating the engine_ptr
    std::vector<double> x0 = {}, v0 = {}, f0 = {}, m0 = {}, r0 = {};
    auto engine_ptr = build_engine_set_particles(myId, ranks, x0, v0, f0, r0, m0);

    /* After the engine_ptr is created in the engineFixture
     * the init function is called form every Ref object
     * in the engine one of them is the particles which
     * removes out of range particles.
     * So we need to return a reference to particles
     * from the engine and then reset particles coordinates here
     */
     // returning the particles
    auto& particlesRef = engine_ptr->getParticlesForUpdate();
    // cehcking the pointer
    REQUIRE(particlesRef);
    // convering it to the mockedParticles since I need 
    // the reset_particles  function
    MockedParticles* mockedParticlesConv = dynamic_cast<MockedParticles*>(particlesRef.get());
    // checking the conversion
    if (mockedParticlesConv == nullptr)
        throw std::invalid_argument("Wrong type in the engine_ptr!");
    // resetting particle coordinates
    // copying the x, v, f, r, m values since we 
    // move the inputs to the particles by reference
    // so these would be null after moving into 
    // the mockedParticles object
    std::vector<double> x1Copy = x1, v1Copy = v1, f1Copy = f1, r1Copy = r1, m1Copy = m1;
    int nmax = static_cast<int>(x1Copy.size()) / 3;
    mockedParticlesConv->resetParticles(nmax, x1Copy, v1Copy, f1Copy, r1Copy, m1Copy);


    // returning the communicatorRef from the engine_ptr
    auto& communicator1Ref = engine_ptr->getCommunicator();
    // checking the communicator 
    REQUIRE(communicator1Ref);
    // returning the exchange message
    std::vector<double>* messages1 = communicator1Ref->sendExchangeParticles();

    // getting x,v,f,m and r values
    auto* myX = particlesRef->getXData();
    auto* myV = particlesRef->getVData();
    auto* myF = particlesRef->getFData();
    auto* myR = particlesRef->getRData();
    auto* myM = particlesRef->getMData();

    /*
     * checking the message
     */
    REQUIRE(messages1);
    // checking its size
    for (int i = 0; i < 6; i++) {
        REQUIRE(messages1[i].size() == expectedMessagesSize[i]);
    }
    // checking its contents
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < messages1[i].size(); j++)
            REQUIRE_THAT(messages1[i][j], Catch::Matchers::WithinAbs(expectedMessages[i][j], 1e-6));
    }
    // checking message destinations
    auto exchangeDests = communicator1Ref->returnExchangeDests();
    REQUIRE(exchangeDests == exchangeDests);


    /*
     * checking if the moved particle has deleted correctly
     */

     // checking the returned pointers
    REQUIRE(myX);
    REQUIRE(myV);
    REQUIRE(myF);
    REQUIRE(myR);
    REQUIRE(myM);
    // checking the contents
    REQUIRE_THAT(myX, Array3DMatcher(expectedX1.data(), 2, 1e-6));
    REQUIRE_THAT(myV, Array3DMatcher(expectedV1.data(), 2, 1e-6));
    REQUIRE_THAT(myF, Array3DMatcher(expectedF1.data(), 2, 1e-6));
    REQUIRE_THAT(myR, Array1DMatcher(expectedR1.data(), 2, 1e-6));
    REQUIRE_THAT(myM, Array1DMatcher(expectedM1.data(), 2, 1e-6));

}

TEST_CASE("Testing receiving the particles from another processor")
{
    // the rank2 
    std::vector<double> x2 =
    {
         180.62, -210.44,   60.91,   // particle 5  owned by Q10 (+- interior) region 2
         260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior) region 2
         190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior) region 2
    };

    std::vector<double> v2 =
    {
        54.7, -66.3,  77.1,   // particle 5
       -45.9,  18.0,  92.6,   // particle 6
       -72.5,  44.3, -95.8,   // particle 9
    };

    std::vector<double> f2 =
    {
        44.5, -36.7,  21.4,   // particle 5
        -9.9,   5.6,  31.2,   // particle 6
       -34.8,  25.1, -45.6,   // particle 9
    };

    std::vector<double> m2 =
    {
        2.7,  // particle 5
        6.0,  // particle 6
        4.1,  // particle 9
    };

    std::vector<double> r2 =
    {
        2.7,  // particle 5
        6.0,  // particle 6
        4.1,  // particle 9
    };

    std::vector<double> x3 =
    {
        -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior) region 3
        -270.22,  120.74,  210.46,   // particle 11 owned by Q01 (-+ interior) region 3
        -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior) region 3
         -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior) region 3
    };

    std::vector<double> v3 =
    {
        21.7, -38.4,  68.2,   // particle 10
       -14.9,  87.6, -29.3,   // particle 11
        46.1, -61.5,  33.8,   // particle 12
       -53.7,  27.5, -41.6,   // particle 14
    };

    std::vector<double> f3 =
    {
        14.8,  -7.3,  39.6,   // particle 10
       -21.9,  30.4, -16.5,   // particle 11
        41.2, -29.8,   9.7,   // particle 12
       -25.7,  17.8, -38.6,   // particle 14
    };

    std::vector<double> m3 =
    {
        0.5,  // particle 10
        9.8,  // particle 11
        3.0,  // particle 12
        5.9,  // particle 14
    };

    std::vector<double> r3 =
    {
        0.5,  // particle 10
        9.8,  // particle 11
        3.0,  // particle 12
        5.9,  // particle 14
    };


    // expected results
    std::vector<double> expectedX2 = {
         180.62, -210.44,   60.91,   // particle 5  owned by Q10 (+- interior) region 2
         260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior) region 2
         190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior) region 2

           12.6,  -18.90,  130.10,
           35.1, -140.48,   12.66
    };

    std::vector<double> expectedV2 = {
        54.7, -66.3,  77.1,   // particle 5
       -45.9,  18.0,  92.6,   // particle 6
       -72.5,  44.3, -95.8,   // particle 9


         -97.1,  28.9, -74.4,
           3.6,   89.2, -12.5
    };

    std::vector<double> expectedF2 = {
        44.5, -36.7,  21.4,   // particle 5
        -9.9,   5.6,  31.2,   // particle 6
       -34.8,  25.1, -45.6,   // particle 9


         -22.1,  16.0, -49.7,
           3.1,  27.6, -14.8
    };

    std::vector<double> expectedM2 = {
        2.7,  // particle 5
        6.0,  // particle 6
        4.1,  // particle 9

        5.6,
        9.1
    };

    std::vector<double> expectedR2 = {
        2.7,  // particle 5
        6.0,  // particle 6
        4.1,  // particle 9


        56.4,
        91.2
    };

    std::vector<double> expectedX3 =
    {
        -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior) region 3
        -270.22,  120.74,  210.46,   // particle 11 owned by Q01 (-+ interior) region 3
        -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior) region 3
         -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior) region 3

        -180.66,   30.77,   14.95
    };

    std::vector<double> expectedV3 =
    {
        21.7, -38.4,  68.2,   // particle 10
       -14.9,  87.6, -29.3,   // particle 11
        46.1, -61.5,  33.8,   // particle 12
       -53.7,  27.5, -41.6,   // particle 14

        41.2, -19.8,  63.5
    };

    std::vector<double> expectedF3 =
    {
        14.8,  -7.3,  39.6,   // particle 10
       -21.9,  30.4, -16.5,   // particle 11
        41.2, -29.8,   9.7,   // particle 12
       -25.7,  17.8, -38.6,   // particle 14

        29.7,  -5.4,  38.9
    };

    std::vector<double> expectedM3 =
    {
        0.5,  // particle 10
        9.8,  // particle 11
        3.0,  // particle 12
        5.9,  // particle 14

        0.9
    };

    std::vector<double> expectedR3 =
    {
        0.5,  // particle 10
        9.8,  // particle 11
        3.0,  // particle 12
        5.9,  // particle 14

        73.9
    };


    // messages
    // contents
    std::vector<double> messages[6] = {
        {0.0}, // to xlo --> no xlo
        {2.0,
          12.6, -18.9,130.10,   // x
         -97.1,  28.9, -74.4,   // v
         -22.1,  16.0, -49.7,   // f
           5.6,                 // m
          56.4,                 // r
          35.1,-140.48, 12.66,  // x
           3.6,   89.2, -12.5,  // v
            3.1,  27.6, -14.8,  // f
            9.1,                // m
            91.2                // r
        }, //  to xhi --> to the region 2
        {0.0}, // to ylo --> no ylo
        {1.0,
         -180.66,30.77, 14.95, // x
          41.2, -19.8,  63.5,  // v
          29.7,  -5.4,  38.9,  // f
          0.9,                 // m
          73.9                 // r
          }, // to yhi  --> to the region 3
        {0.0},  // to zlo
        {0.0},  // to zhi
    };


    std::array<int, 3> nranks = { 2,2,1 };
    // engine for the rank2
    int id2 = 1;
    int nmax2 = static_cast<int>(x2.size()) / 3;
    auto engine_ptr2 = build_engine_set_particles(id2, nranks, x2, v2, f2, r2, m2);
    // engine for the rank3
    int id3 = 2;
    int nmax3 = static_cast<int>(x3.size()) / 3;
    auto engine_ptr3 = build_engine_set_particles(id3, nranks, x3, v3, f3, r3, m3);

    // communicator for the engine2
    auto& communicator2Ref = engine_ptr2->getCommunicator();
    // communicator for the engine3
    auto& communicator3Ref = engine_ptr3->getCommunicator();
    // checking the pointers
    REQUIRE(communicator2Ref);
    REQUIRE(communicator3Ref);

    // receiving the messages
    communicator2Ref->recvExchangeParticles(messages[1]);
    // receiving the messages
    communicator3Ref->recvExchangeParticles(messages[3]);
    // the message has to be empty after recvExchangeParticles
    // as the transfer is completed afterwards
    REQUIRE(messages[1].size() == 1);
    REQUIRE(messages[3].size() == 1);
    REQUIRE_THAT(messages[1][0], Catch::Matchers::WithinAbs(0.0, 1e-6));
    REQUIRE_THAT(messages[3][0], Catch::Matchers::WithinAbs(0.0, 1e-6));

    // getting particles
    auto& particles2Ref = engine_ptr2->getParticlesForUpdate();
    auto& particles3Ref = engine_ptr3->getParticlesForUpdate();
    // checking the pointers
    REQUIRE(particles2Ref);
    REQUIRE(particles3Ref);
    // getting x, v, f, m , r values from the engine_ptr2
    auto* myX2 = particles2Ref->getXData();
    auto* myV2 = particles2Ref->getVData();
    auto* myF2 = particles2Ref->getFData();
    auto* myR2 = particles2Ref->getRData();
    auto* myM2 = particles2Ref->getMData();
    // checking the pointers
    REQUIRE(myX2);
    REQUIRE(myV2);
    REQUIRE(myF2);
    REQUIRE(myR2);
    REQUIRE(myM2);
    // getting x, v, f, m, r values from the engine_ptr3
    auto* myX3 = particles3Ref->getXData();
    auto* myV3 = particles3Ref->getVData();
    auto* myF3 = particles3Ref->getFData();
    auto* myR3 = particles3Ref->getRData();
    auto* myM3 = particles3Ref->getMData();
    // checking the pointers
    REQUIRE(myX3);
    REQUIRE(myV3);
    REQUIRE(myF3);
    REQUIRE(myR3);
    REQUIRE(myM3);



    // checking the results
    // the rank2
    REQUIRE_THAT(myX2, Array3DMatcher(expectedX2.data(), 5, 1e-6));
    REQUIRE_THAT(myV2, Array3DMatcher(expectedV2.data(), 5, 1e-6));
    REQUIRE_THAT(myF2, Array3DMatcher(expectedF2.data(), 5, 1e-6));
    REQUIRE_THAT(myR2, Array1DMatcher(expectedR2.data(), 5, 1e-6));
    REQUIRE_THAT(myM2, Array1DMatcher(expectedM2.data(), 5, 1e-6));
    // the rank3
    REQUIRE_THAT(myX3, Array3DMatcher(expectedX3.data(), 5, 1e-6));
    REQUIRE_THAT(myV3, Array3DMatcher(expectedV3.data(), 5, 1e-6));
    REQUIRE_THAT(myF3, Array3DMatcher(expectedF3.data(), 5, 1e-6));
    REQUIRE_THAT(myR3, Array1DMatcher(expectedR3.data(), 5, 1e-6));
    REQUIRE_THAT(myM3, Array1DMatcher(expectedM3.data(), 5, 1e-6));
}