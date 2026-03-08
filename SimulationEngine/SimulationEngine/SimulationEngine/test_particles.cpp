#include "test.hpp"

// shared values
std::vector<double> x = {
    // Global: x,y,z in [-300,300], skin=0
    -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior)
    -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior)
    -180.66,  -30.77,   14.95,   // particle 2  owned by Q00 (-- interior)
     -12.60,  -18.90,  130.10,   // particle 3  owned by Q00 (-- interior)
     -35.10, -140.48,   12.66,   // particle 4  owned by Q00 (-- interior)

     180.62, -210.44,   60.91,   // particle 5  owned by Q10 (+- interior)
     260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior)
      25.60, -160.15,  -55.27,   // particle 7  owned by Q10 (+- interior)
      18.40,  -22.10,   95.50,   // particle 8  owned by Q10 (+- interior)
     190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior)

    -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior)
    -270.22,  120.74,  210.46,   // particle 11 owned by Q01 (-+ interior)
    -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior)
     -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior)
     -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior)

     210.83,  220.41, -150.88,   // particle 15 owned by Q01 (++ interior)
     140.62,  160.33,  198.21,   // particle 16 owned by Q01 (++ interior)
      30.25,   22.40,   10.66,   // particle 17 owned by Q01 (++ interior)
      40.11,  170.25,  204.67,   // particle 18 owned by Q01 (++ interior)
     160.81,   35.42,  289.73,   // particle 19 owned by Q01 (++ interior)
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


TEST_CASE("Creating particles from the x, v, f, r and m lists")
{
    std::cout << "Creating particles from the x, v, f, r and m lists" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    int nmax = 20;
    int nlocal = 20;

    // expected results
    std::vector<double> expectedXs = {
        // Global: x,y,z in [-300,300], skin=0
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior)
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior)
        -180.66,  -30.77,   14.95,   // particle 2  owned by Q00 (-- interior)
         -12.60,  -18.90,  130.10,   // particle 3  owned by Q00 (-- interior)
         -35.10, -140.48,   12.66,   // particle 4  owned by Q00 (-- interior)

         180.62, -210.44,   60.91,   // particle 5  owned by Q10 (+- interior)
         260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior)
          25.60, -160.15,  -55.27,   // particle 7  owned by Q10 (+- interior)
          18.40,  -22.10,   95.50,   // particle 8  owned by Q10 (+- interior)
         190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior)

        -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior)
        -270.22,  120.74,  210.46,   // particle 11 owned by Q01 (-+ interior)
        -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior)
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior)
         -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior)

         210.83,  220.41, -150.88,   // particle 15 owned by Q01 (++ interior)
         140.62,  160.33,  198.21,   // particle 16 owned by Q01 (++ interior)
          30.25,   22.40,   10.66,   // particle 17 owned by Q01 (++ interior)
          40.11,  170.25,  204.67,   // particle 18 owned by Q01 (++ interior)
         160.81,   35.42,  289.73,   // particle 19 owned by Q01 (++ interior)
    };

    std::vector<double> expectedVs = {
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
    std::vector<double> expectedFs = {
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
    std::vector<double> expectedMs = {
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

    std::vector<double> expectedRs = {
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

    // creating a copy of x, v, f, r, m
    // since I use std::move to put them into 
    // the particles
    std::vector<double> xCopy = x, vCopy = v, fCopy = f, rCopy = r, mCopy = m;
    // creating particles
    auto particles = std::make_unique<Particles>(nmax, xCopy, vCopy, fCopy, mCopy, rCopy);


    // getting the x, v, f, r, m values
    auto* myXs = particles->getXData();
    auto* myVs = particles->getVData();
    auto* myFs = particles->getFData();
    auto* myRs = particles->getRData();
    auto* myMs = particles->getMData();
    // checking the results
    REQUIRE_THAT(myXs, Array3DMatcher(expectedXs.data(), 20, 1e-6));
    REQUIRE_THAT(myVs, Array3DMatcher(expectedVs.data(), 20, 1e-6));
    REQUIRE_THAT(myFs, Array3DMatcher(expectedFs.data(), 20, 1e-6));
    REQUIRE_THAT(myRs, Array1DMatcher(expectedRs.data(), 20, 1e-6));
    REQUIRE_THAT(myMs, Array1DMatcher(expectedMs.data(), 20, 1e-6));

}


TEST_CASE("Adding particle to the particle list")
{
    std::cout << "Adding particles to the particle list" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    int nmax = 20;
    int nlocal = 20;

    // expected results
    std::vector<double> expectedXs = {
        // Global: x,y,z in [-300,300], skin=0
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior)
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior)
        -180.66,  -30.77,   14.95,   // particle 2  owned by Q00 (-- interior)
         -12.60,  -18.90,  130.10,   // particle 3  owned by Q00 (-- interior)
         -35.10, -140.48,   12.66,   // particle 4  owned by Q00 (-- interior)

         180.62, -210.44,   60.91,   // particle 5  owned by Q10 (+- interior)
         260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior)
          25.60, -160.15,  -55.27,   // particle 7  owned by Q10 (+- interior)
          18.40,  -22.10,   95.50,   // particle 8  owned by Q10 (+- interior)
         190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior)

        -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior)
        -270.22,  120.74,  210.46,   // particle 11 owned by Q01 (-+ interior)
        -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior)
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior)
         -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior)

         210.83,  220.41, -150.88,   // particle 15 owned by Q01 (++ interior)
         140.62,  160.33,  198.21,   // particle 16 owned by Q01 (++ interior)
          30.25,   22.40,   10.66,   // particle 17 owned by Q01 (++ interior)
          40.11,  170.25,  204.67,   // particle 18 owned by Q01 (++ interior)
         160.81,   35.42,  289.73,   // particle 19 owned by Q01 (++ interior)


         118.47,  148.92,  214.36,   // particle 20
          52.83,   18.77,   26.41,   // particle 21
          36.59,  182.14,  237.88,   // particle 22
         171.26,   41.93,  276.54    // particle 23
    };

    std::vector<double> expectedVs = {
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
    -16.7,  73.3, -58.9,   // particle 19

     18.6, -42.7,  71.4,   // particle 20
    -11.3,  92.1, -25.8,   // particle 21
     39.8, -57.2,  28.6,   // particle 22
    -83.5,  16.9,  94.7,   // particle 23
    };
    std::vector<double> expectedFs = {
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
     -8.2,  46.7, -24.5,   // particle 19

    -47.6,  19.3,  -9.4,   // particle 20
     31.8, -21.7,  16.2,   // particle 21
     24.9, -28.6,  22.7,   // particle 22
    -11.5,  43.1, -27.9    // particle 23
    };
    std::vector<double> expectedMs = {
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
     4.6,  // particle 19

     5.9,  // particle 20
     7.4,  // particle 21
     2.6,  // particle 22
     3.8   // particle 23
    };

    std::vector<double> expectedRs = {
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
     26.4,  // particle 19

     11.8,  // particle 20
     79.6,  // particle 21
     62.3,  // particle 22
     29.7   // particle 23
    };


    // creaing the particles
    std::vector<double> xCopy = x, vCopy = v, fCopy = f, rCopy = r, mCopy = m;
    auto particles = std::make_unique<Particles>(nmax, xCopy, vCopy, fCopy, mCopy, rCopy);

    // adding new particles
    std::array<double, 3> newX1 = {
        118.47, 148.92, 214.36   // particle 20
    };
    std::array<double, 3> newV1 = {
     18.6, -42.7,  71.4   // particle 20
    };
    std::array<double, 3> newF1 = {
        -47.6,  19.3,  -9.4   // particle 20
    };
    double newM1 = 5.9;  // particle 20
    double newR1 = 11.8;  // particle 20
    // adding the particle
    particles->addParticle(newX1,newV1,newF1, newM1,newR1);


    std::array<double, 3> newX2 = {
        52.83, 18.77, 26.41   // particle 21
    };
    std::array<double, 3> newV2 = {
    -11.3,  92.1, -25.8   // particle 21
    };
    std::array<double, 3> newF2 = {
        31.8, -21.7,  16.2   // particle 21
    };
    double newM2 = 7.4;  // particle 21
    double newR2 = 79.6;  // particle 21
    // adding the particle
    particles->addParticle(newX2, newV2, newF2, newM2, newR2);

    std::array<double, 3> newX3 = {
        36.59, 182.14, 237.88   // particle 22
    };
    std::array<double, 3> newV3 = {
     39.8, -57.2,  28.6   // particle 22
    };
    std::array<double, 3> newF3 = {
        24.9, -28.6,  22.7   // particle 22
    };
    double newM3 = 2.6;  // particle 22
    double newR3 = 62.3;  // particle 22
    // adding the particle
    particles->addParticle(newX3, newV3, newF3, newM3, newR3);

    std::array<double, 3> newX4 = {
        171.26, 41.93, 276.54    // particle 23
    };
    std::array<double, 3> newV4 = {
        -83.5,  16.9,  94.7   // particle 23
    };
    std::array<double, 3> newF4 = {
        -11.5,  43.1, -27.9    // particle 23
    }; 
    double newM4 = 3.8;  // particle 23
    double newR4 = 29.7;  // particle 23
    // adding the particle
    particles->addParticle(newX4, newV4, newF4, newM4, newR4);


    // checking the results

    // getting the x, v, f, r, m values
    auto* myXs = particles->getXData();
    auto* myVs = particles->getVData();
    auto* myFs = particles->getFData();
    auto* myMs = particles->getMData();
    auto* myRs = particles->getRData();

    REQUIRE_THAT(myXs, Array3DMatcher(expectedXs.data(), 24, 1e-6));
    REQUIRE_THAT(myVs, Array3DMatcher(expectedVs.data(), 24, 1e-6));
    REQUIRE_THAT(myFs, Array3DMatcher(expectedFs.data(), 24, 1e-6));
    REQUIRE_THAT(myRs, Array1DMatcher(expectedRs.data(), 24, 1e-6));
    REQUIRE_THAT(myMs, Array1DMatcher(expectedMs.data(), 24, 1e-6));
}

TEST_CASE("Testing removing a particle from the particle list") {
    std::cout << "Testing removing a particle from the particle list" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    int nmax = 20;
    int nlocal = 20;


    // expected results
    std::vector<double> expectedXs = {
        // Global: x,y,z in [-300,300], skin=0
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior)
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior)
        -180.66,  -30.77,   14.95,   // particle 2  owned by Q00 (-- interior)
         -12.60,  -18.90,  130.10,   // particle 3  owned by Q00 (-- interior)
         -35.10, -140.48,   12.66,   // particle 4  owned by Q00 (-- interior)


         260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior)
          25.60, -160.15,  -55.27,   // particle 7  owned by Q10 (+- interior)
          18.40,  -22.10,   95.50,   // particle 8  owned by Q10 (+- interior)
         190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior)

        -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior)
        -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior)
         -22.80,   18.10, -140.25,   // particle 13 owned by Q01 (-+ interior)
         -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior)

         210.83,  220.41, -150.88,   // particle 15 owned by Q01 (++ interior)
         140.62,  160.33,  198.21,   // particle 16 owned by Q01 (++ interior)
          30.25,   22.40,   10.66,   // particle 17 owned by Q01 (++ interior)
          40.11,  170.25,  204.67,   // particle 18 owned by Q01 (++ interior)
         160.81,   35.42,  289.73,   // particle 19 owned by Q01 (++ interior)
    };

    std::vector<double> expectedVs = {
     72.4, -33.1,  95.0,   // particle 0
    -88.6,  14.3, -56.7,   // particle 1
     41.2, -19.8,  63.5,   // particle 2
    -97.1,  28.9, -74.4,   // particle 3
      3.6,  89.2, -12.5,   // particle 4
    -45.9,  18.0,  92.6,   // particle 6
    -11.4,  36.8, -83.2,   // particle 7
     59.7, -24.6,  10.9,   // particle 8
    -72.5,  44.3, -95.8,   // particle 9
     21.7, -38.4,  68.2,   // particle 10
     46.1, -61.5,  33.8,   // particle 12
    -79.2,  12.4,  99.1,   // particle 13
    -53.7,  27.5, -41.6,   // particle 14
     64.0, -22.8,  15.2,   // particle 15
    -90.3,  48.6,  -7.4,   // particle 16
     81.9, -35.0,  24.1,   // particle 17
     57.8, -69.2,  39.4,   // particle 18
    -16.7,  73.3, -58.9    // particle 19
    };
    std::vector<double> expectedFs = {
     12.4, -33.9,  45.2,   // particle 0
    -18.6,   7.3, -41.5,   // particle 1
     29.7,  -5.4,  38.9,   // particle 2
    -22.1,  16.0, -49.7,   // particle 3
      3.1,  27.6, -14.8,   // particle 4
     -9.9,   5.6,  31.2,   // particle 6
    -12.7,  18.9, -28.4,   // particle 7
     47.3, -19.5,   6.2,   // particle 8
    -34.8,  25.1, -45.6,   // particle 9
     14.8,  -7.3,  39.6,   // particle 10
     41.2, -29.8,   9.7,   // particle 12
    -48.1,  12.6,  33.9,   // particle 13
    -25.7,  17.8, -38.6,   // particle 14
     26.4, -11.3,   4.9,   // particle 15
    -43.2,  22.5,  -6.8,   // particle 16
     35.0, -18.1,  13.7,   // particle 17
     28.6, -31.4,  19.3,   // particle 18
     -8.2,  46.7, -24.5    // particle 19
    };
    std::vector<double> expectedMs = {
     3.4,  // particle 0
     7.8,  // particle 1
     0.9,  // particle 2
     5.6,  // particle 3
     9.1,  // particle 4
     6.0,  // particle 6
     1.3,  // particle 7
     8.4,  // particle 8
     4.1,  // particle 9
     0.5,  // particle 10
     3.0,  // particle 12
     7.4,  // particle 13
     5.9,  // particle 14
     2.1,  // particle 15
     6.7,  // particle 16
     8.2,  // particle 17
     1.8,  // particle 18
     4.6   // particle 19
    };

    std::vector<double> expectedRs = {
     12.5,  // particle 0
     89.3,  // particle 1
     45.7,  // particle 2
     3.9,   // particle 3
     67.1,  // particle 4
     98.0,  // particle 6
     31.4,  // particle 7
     76.2,  // particle 8
     54.3,  // particle 9
     8.7,   // particle 10
     19.8,  // particle 12
     92.4,  // particle 13
     37.6,  // particle 14
     71.0,  // particle 15
     14.2,  // particle 16
     83.1,  // particle 17
     58.9,  // particle 18
     26.4   // particle 19
    };

    // checking the number of data
    int nXs = static_cast<int>(x.size()) / 3;
    int nVs = static_cast<int>(v.size()) / 3;
    int nFs = static_cast<int>(f.size()) / 3;
    int nRs = static_cast<int>(r.size());
    int nMs = static_cast<int>(m.size());
    REQUIRE(nXs == nVs);
    REQUIRE(nXs == nFs);
    REQUIRE(nXs == nRs);
    REQUIRE(nXs == nMs);

    // creating a copy of x, v, f, r, m
    // since I use std::move to put them into 
    // the particles
    std::vector<double> xCopy = x, vCopy = v, fCopy = f, rCopy = r, mCopy = m;
    // creating particles
    auto particles = std::make_unique<Particles>(nmax, xCopy, vCopy, fCopy, mCopy, rCopy);


    // removing particles #11 and #5
    particles->removeParticle(11);
    particles->removeParticle(5);

    // getting the x, v, f, r, m values
    auto* myX = particles->getXData();
    auto* myV = particles->getVData();
    auto* myF = particles->getFData();
    auto* myR = particles->getRData();
    auto* myM = particles->getMData();

    REQUIRE_THAT(myX, Array3DMatcher(expectedXs.data(), 18, 1e-6));
    REQUIRE_THAT(myV, Array3DMatcher(expectedVs.data(), 18, 1e-6));
    REQUIRE_THAT(myF, Array3DMatcher(expectedFs.data(), 18, 1e-6));
    REQUIRE_THAT(myR, Array1DMatcher(expectedRs.data(), 18, 1e-6));
    REQUIRE_THAT(myM, Array1DMatcher(expectedMs.data(), 18, 1e-6));
}