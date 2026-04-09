#include "catch_amalgamated.hpp"
#include "test_communicator_helpers.hpp"
#include "Comm_strategy.h"
#include "MPI_comm_strategy.h"

/// <summary>
/// The default value of the skin parameter
/// </summary>

TEST_CASE("Testing the communication class","[mpi]") {
    std::cout << "Testing the communication class" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();
    int rank = comm_strategy->getRank();
    int size = comm_strategy->getSize();

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
    std::vector<double> expectedXs;
    std::vector<double> expectedVs;
    std::vector<double> expectedFs;
    std::vector<double> expectedMs;
    std::vector<double> expectedRs;

    if (rank == 0)
    {
        expectedXs = {
        -122.63,  -187.55,   241.08,    // particle 14
        };
        expectedVs = {
            -53.7,  27.5, -41.6,   // particle 14
        };
        expectedFs = {
            -25.7,  17.8, -38.6,   // particle 14
        };
        expectedMs = {
         5.9,  // particle 14
        };
        expectedRs = {
         37.6,  // particle 14
        };
    }
    else if (rank == 1)
    {
        expectedXs = {
            145.32, -210.77,  89.45,    // particle 0
             12.87,  -98.44, 276.31,    // particle 2
            233.11,   -5.66, 147.82,    // particle 4
             67.59, -243.80,  18.26,    // particle 6
            190.74,  -75.33, -166.48,   // particle 7
             54.03, -132.67, 204.95,    // particle 9
            174.28, -299.99, -63.77,    // particle 11
            287.44,  -18.39,  92.71,    // particle 13
            256.77, -264.44,  11.38,    // particle 16
             83.61,  -54.27, 165.49,    // particle 18
        };

        expectedVs = {
             72.4, -33.1,  95.0,        // particle 0
             41.2, -19.8,  63.5,        // particle 2
              3.6,  89.2, -12.5,        // particle 4
            -45.9,  18.0,  92.6,        // particle 6
            -11.4,  36.8, -83.2,        // particle 7
            -72.5,  44.3, -95.8,        // particle 9
            -14.9,  87.6, -29.3,        // particle 11
            -79.2,  12.4,  99.1,        // particle 13
            -90.3,  48.6,  -7.4,        // particle 16
             57.8, -69.2,  39.4,        // particle 18
        };

        expectedFs = {
             12.4, -33.9,  45.2,        // particle 0
             29.7,  -5.4,  38.9,        // particle 2
              3.1,  27.6, -14.8,        // particle 4
             -9.9,   5.6,  31.2,        // particle 6
            -12.7,  18.9, -28.4,        // particle 7
            -34.8,  25.1, -45.6,        // particle 9
            -21.9,  30.4, -16.5,        // particle 11
            -48.1,  12.6,  33.9,        // particle 13
            -43.2,  22.5,  -6.8,        // particle 16
             28.6, -31.4,  19.3,        // particle 18
        };

        expectedMs = {
            3.4,                        // particle 0
            0.9,                        // particle 2
            9.1,                        // particle 4
            6.0,                        // particle 6
            1.3,                        // particle 7
            4.1,                        // particle 9
            9.8,                        // particle 11
            7.4,                        // particle 13
            6.7,                        // particle 16
            1.8,                        // particle 18
        };

        expectedRs = {
            12.5,                       // particle 0
            45.7,                       // particle 2
            67.1,                       // particle 4
            98.0,                       // particle 6
            31.4,                       // particle 7
            54.3,                       // particle 9
            63.5,                       // particle 11
            92.4,                       // particle 13
            14.2,                       // particle 16
            58.9,                       // particle 18
        };
    }
    else if (rank == 2)
    {
        expectedXs = {
           -178.64,  254.19, -133.58,   // particle 1
            -45.22,  199.73, -287.90,   // particle 3
           -299.45,  120.38,  -44.91,   // particle 5
            -88.17,  281.56,  -12.49,   // particle 8
           -215.90,   33.14,  119.62,   // particle 10
            -31.50,  146.22, -250.84,   // particle 12
           -194.05,   77.80, -299.12,   // particle 17
           -270.33,  295.88,  -21.76    // particle 19
        };

        expectedVs = {
           -88.6,  14.3, -56.7,         // particle 1
           -97.1,  28.9, -74.4,         // particle 3
            54.7, -66.3,  77.1,         // particle 5
            59.7, -24.6,  10.9,         // particle 8
            21.7, -38.4,  68.2,         // particle 10
            46.1, -61.5,  33.8,         // particle 12
            81.9, -35.0,  24.1,         // particle 17
           -16.7,  73.3, -58.9          // particle 19
        };

        expectedFs = {
           -18.6,   7.3, -41.5,         // particle 1
           -22.1,  16.0, -49.7,         // particle 3
            44.5, -36.7,  21.4,         // particle 5
            47.3, -19.5,   6.2,         // particle 8
            14.8,  -7.3,  39.6,         // particle 10
            41.2, -29.8,   9.7,         // particle 12
            35.0, -18.1,  13.7,         // particle 17
            -8.2,  46.7, -24.5          // particle 19
        };

        expectedMs = {
            7.8,                        // particle 1
            5.6,                        // particle 3
            2.7,                        // particle 5
            8.4,                        // particle 8
            0.5,                        // particle 10
            3.0,                        // particle 12
            8.2,                        // particle 17
            4.6                         // particle 19
        };

        expectedRs = {
            89.3,                       // particle 1
            3.9,                        // particle 3
            24.6,                       // particle 5
            76.2,                       // particle 8
            8.7,                        // particle 10
            19.8,                       // particle 12
            83.1,                       // particle 17
            26.4                        // particle 19
        };
        }
    else if (rank == 3)
    {
        expectedXs = {
            39.96, 208.13, -174.29,     // particle 15
        };

        expectedVs = {
            64.0, -22.8, 15.2,          // particle 15
        };

        expectedFs = {
            26.4, -11.3, 4.9,           // particle 15
        };

        expectedMs = {
            2.1,                        // particle 15
        };

        expectedRs = {
            71.0,                       // particle 15
        };
    }


    // the communicator configuration
    std::array<int, 3> nranks = { 2,2,1 };
    std::unique_ptr<Communicator> comm = std::make_unique<Communicator>(rank, nranks);


    
    build_engine_check_communicator(
            rank, nranks,
            x, v, f, r, m,
            expectedXs,
            expectedVs,
            expectedFs,
            expectedRs,
            expectedMs);

}


TEST_CASE("Testing the message for sending an out of range particle from a rank","[.]")
{
    // I need to think about this test!
}

TEST_CASE("Testing receiving the particles from another processor", "[.]")
{
    // I need to think about this test!
}

TEST_CASE("Test sending and recieving ghost particles","[mpi]") {
    std::cout << "Test sending and receiving ghost particles" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    // data 
    // number of ranks 
    std::array<int, 3> ranks = { 2,2,1 };
    std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();
    int rank = comm_strategy->getRank();
    int size = comm_strategy->getSize();


    // the skin values
    double skin = 50.0;

    // the minimum number of ranks
    minRanksRequirement(comm_strategy, 4);
    // skipping the extra ranks
    skipExtraRanks(comm_strategy, 4);


    int nlocal, nghost;
    std::vector<int> ids;
    std::vector<double> xs, vs, fs, ms, rs;
    int expectedNlocal, expectedNghost;
    std::vector<int> expectedIds;
    std::vector<double> expectedXs, expectedVs, expectedFs, expectedMs, expectedRs;
    // new values
    // core 0
    if (rank == 0)
    {
        nlocal = 5;
        nghost = 4;

        ids = {
            0, 1, 11, 15, 19,
            8, 9, 13, 16
        };

        xs = {
           -220.40, -180.10,   80.55,   // particle 0
           -140.75, -260.33, -120.18,   // particle 1
            -35.10, -140.48,   12.66,   // particle 11
           -180.66,  -30.77,   14.95,   // particle 15
            -12.60,  -18.90,  130.10,   // particle 19

              5.60, -130.15,  -15.27,   // particle 8
           -116.33,   14.18, -179.05,   // particle 9
            -13.80,    8.10, -130.25,   // particle 13
             13.40,  -13.10,   90.50,   // particle 16
        };

        vs = {
            72.4, -33.1,  95.0,         // particle 0
           -88.6,  14.3, -56.7,         // particle 1
           -14.9,  87.6, -29.3,         // particle 11
            64.0, -22.8,  15.2,         // particle 15
           -16.7,  73.3, -58.9,         // particle 19

            34.8, -67.2,  12.5,         // particle 8
           -81.4,  53.7, -42.9,         // particle 9
           -15.6,  27.9,  88.3,         // particle 13
           -92.1, -11.4,  46.7,         // particle 16
        };

        fs = {
            12.4, -33.9,  45.2,         // particle 0
           -18.6,   7.3, -41.5,         // particle 1
           -21.9,  30.4, -16.5,         // particle 11
            26.4, -11.3,   4.9,         // particle 15
            -8.2,  46.7, -24.5,         // particle 19

            21.7, -34.9,  12.4,         // particle 8
           -18.3,  27.6, -41.5,         // particle 9
           -45.2,   9.8,  36.1,         // particle 13
           -32.7,  14.3,  -7.9,         // particle 16
        };

        ms = {
            3.4,                        // particle 0
            7.8,                        // particle 1
            9.8,                        // particle 11
            2.1,                        // particle 15
            4.6,                        // particle 19

            8.4,                        // particle 8
            4.1,                        // particle 9
            7.4,                        // particle 13
            6.7,                        // particle 16
        };

        rs = {
            12.5,                       // particle 0
            89.3,                       // particle 1
            63.5,                       // particle 11
            71.0,                       // particle 15
            26.4,                       // particle 19

            76.2,                       // particle 8
            54.3,                       // particle 9
            92.4,                       // particle 13
            14.2,                       // particle 16
        };

        expectedNlocal = 5;
        expectedNghost = 4;

        expectedIds = {
            0, 1, 11, 15, 19,
            8, 9, 13, 16
        };

        expectedXs = {
           -220.40, -180.10,   80.55,   // particle 0
           -140.75, -260.33, -120.18,   // particle 1
            -35.10, -140.48,   12.66,   // particle 11
           -180.66,  -30.77,   14.95,   // particle 15
            -12.60,  -18.90,  130.10,   // particle 19

            25.60, -160.15,  -55.27,    // particle 8
           -120.33,  20.18, -199.05,    // particle 9
            -22.80,  18.10, -140.25,    // particle 13
             18.40, -22.10,   95.50,    // particle 16
        };

        expectedVs = {
            72.4, -33.1,  95.0,         // particle 0
           -88.6,  14.3, -56.7,         // particle 1
           -14.9,  87.6, -29.3,         // particle 11
            64.0, -22.8,  15.2,         // particle 15
           -16.7,  73.3, -58.9,         // particle 19

            59.7, -24.6,  10.9,         // particle 8
           -72.5,  44.3, -95.8,         // particle 9
           -79.2,  12.4,  99.1,         // particle 13
           -90.3,  48.6,  -7.4,         // particle 16
        };

        expectedFs = {
            12.4, -33.9,  45.2,         // particle 0
           -18.6,   7.3, -41.5,         // particle 1
           -21.9,  30.4, -16.5,         // particle 11
            26.4, -11.3,   4.9,         // particle 15
            -8.2,  46.7, -24.5,         // particle 19

            47.3, -19.5,   6.2,         // particle 8
           -34.8,  25.1, -45.6,         // particle 9
           -48.1,  12.6,  33.9,         // particle 13
           -43.2,  22.5,  -6.8,         // particle 16
        };

        expectedMs = {
            3.4,                        // particle 0
            7.8,                        // particle 1
            9.8,                        // particle 11
            2.1,                        // particle 15
            4.6,                        // particle 19

            8.4,                        // particle 8
            4.1,                        // particle 9
            7.4,                        // particle 13
            6.7,                        // particle 16
        };

        expectedRs = {
            12.5,                       // particle 0
            89.3,                       // particle 1
            63.5,                       // particle 11
            71.0,                       // particle 15
            26.4,                       // particle 19

            76.2,                       // particle 8
            54.3,                       // particle 9
            92.4,                       // particle 13
            14.2,                       // particle 16
        };
    }
    else if (rank == 1)
    {
        nlocal = 5;
        nghost = 4;

        ids = {
            2, 3, 8, 16, 18,
            10, 11, 12, 19
        };

        xs = {
            180.62, -210.44,   60.91,   // particle 2
            260.11, -140.88, -200.30,   // particle 3
             25.60, -160.15,  -55.27,   // particle 8
             18.40,  -22.10,   95.50,   // particle 16
            190.27,  -40.22, -236.70,   // particle 18

             10.25,   12.40,    5.66,   // particle 10
            -25.10, -120.48,    7.66,   // particle 11
            130.81,   25.42,  279.73,   // particle 12
            -10.60,  -14.90,  110.10    // particle 19
        };

        vs = {
            41.2, -19.8,  63.5,         // particle 2
           -97.1,  28.9, -74.4,         // particle 3
            59.7, -24.6,  10.9,         // particle 8
           -90.3,  48.6,  -7.4,         // particle 16
            57.8, -69.2,  39.4,         // particle 18

            73.4, -26.1,  14.8,         // particle 10
           -58.7,  39.5, -82.3,         // particle 11
           -67.9,  18.2,  91.6,         // particle 12
           -84.1,  47.2,  -9.3,         // particle 19
        };

        fs = {
            29.7,  -5.4,  38.9,         // particle 2
           -22.1,  16.0, -49.7,         // particle 3
            47.3, -19.5,   6.2,         // particle 8
           -43.2,  22.5,  -6.8,         // particle 16
            28.6, -31.4,  19.3,         // particle 18

           -62.4,  18.7, -33.1,         // particle 10
            41.6, -72.3,  54.8,         // particle 11
            -9.5,  63.2, -47.6,         // particle 12
            78.1, -14.9,  21.7,         // particle 19
        };

        ms = {
            0.9,                        // particle 2
            5.6,                        // particle 3
            8.4,                        // particle 8
            6.7,                        // particle 16
            1.8,                        // particle 18

            2.6,                        // particle 10
            9.3,                        // particle 11
            5.8,                        // particle 12
            1.7,                        // particle 19
        };

        rs = {
            45.7,                       // particle 2
            3.9,                        // particle 3
            76.2,                       // particle 8
            14.2,                       // particle 16
            58.9,                       // particle 18

            38.7,                       // particle 10
            81.5,                       // particle 11
            67.2,                       // particle 12
            23.9,                       // particle 19
        };

        expectedNlocal = 5;
        expectedNghost = 4;

        expectedIds = {
            2, 3, 8, 16, 18,
            10, 11, 12, 19
        };

        expectedXs = {
            180.62, -210.44,   60.91,   // particle 2
            260.11, -140.88, -200.30,   // particle 3
             25.60, -160.15,  -55.27,   // particle 8
             18.40,  -22.10,   95.50,   // particle 16
            190.27,  -40.22, -236.70,   // particle 18

             30.25,   22.40,   10.66,   // particle 10
            -35.10, -140.48,   12.66,   // particle 11
            160.81,   35.42,  289.73,   // particle 12
            -12.60,  -18.90,  130.10    // particle 19
        };

        expectedVs = {
            41.2, -19.8,  63.5,         // particle 2
           -97.1,  28.9, -74.4,         // particle 3
            59.7, -24.6,  10.9,         // particle 8
           -90.3,  48.6,  -7.4,         // particle 16
            57.8, -69.2,  39.4,         // particle 18

            21.7, -38.4,  68.2,         // particle 10
           -14.9,  87.6, -29.3,         // particle 11
            46.1, -61.5,  33.8,         // particle 12
           -16.7,  73.3, -58.9          // particle 19
        };

        expectedFs = {
            29.7,  -5.4,  38.9,         // particle 2
           -22.1,  16.0, -49.7,         // particle 3
            47.3, -19.5,   6.2,         // particle 8
           -43.2,  22.5,  -6.8,         // particle 16
            28.6, -31.4,  19.3,         // particle 18

            14.8,  -7.3,  39.6,         // particle 10
           -21.9,  30.4, -16.5,         // particle 11
            41.2, -29.8,   9.7,         // particle 12
            -8.2,  46.7, -24.5          // particle 19
        };

        expectedMs = {
            0.9,                        // particle 2
            5.6,                        // particle 3
            8.4,                        // particle 8
            6.7,                        // particle 16
            1.8,                        // particle 18

            0.5,                        // particle 10
            9.8,                        // particle 11
            3.0,                        // particle 12
            4.6                         // particle 19
        };

        expectedRs = {
            45.7,                       // particle 2
            3.9,                        // particle 3
            76.2,                       // particle 8
            14.2,                       // particle 16
            58.9,                       // particle 18

            8.7,                        // particle 10
            63.5,                       // particle 11
            19.8,                       // particle 12
            26.4                        // particle 19
        };
    }
    else if (rank == 2)
    {
        nlocal = 5;
        nghost = 4;

        ids = {
            4, 5, 9, 13, 17,
            10, 14, 15, 19
        };

        xs = {
           -200.90,  190.36,  -40.58,   // particle 4
           -270.22,  120.74,  210.46,   // particle 5
           -120.33,   20.18, -199.05,   // particle 9
            -22.80,   18.10, -140.25,   // particle 13
            -28.20,  180.66,   66.03,   // particle 17

              3.25,   12.40,   20.66,   // particle 10
             20.11,  130.25,  194.67,   // particle 14
           -150.66,  -25.77,   34.95,   // particle 15
            -16.60,  -28.90,  110.10    // particle 19
        };

        vs = {
             3.6,  89.2, -12.5,         // particle 4
            54.7, -66.3,  77.1,         // particle 5
           -72.5,  44.3, -95.8,         // particle 9
           -79.2,  12.4,  99.1,         // particle 13
            81.9, -35.0,  24.1,         // particle 17

            34.6, -57.1,  82.4,         // particle 10
           -61.3,  18.9, -36.7,         // particle 14
            72.5, -11.4,  23.8,         // particle 15
           -28.9,  64.7, -49.2          // particle 19
        };

        fs = {
             3.1,  27.6, -14.8,         // particle 4
            44.5, -36.7,  21.4,         // particle 5
           -34.8,  25.1, -45.6,         // particle 9
           -48.1,  12.6,  33.9,         // particle 13
            35.0, -18.1,  13.7,         // particle 17

            63.5, -21.4,  48.7,         // particle 10
           -47.9,  36.2, -72.1,         // particle 14
            18.6, -54.3,  12.8,         // particle 15
           -32.4,  69.5, -15.7          // particle 19
        };

        ms = {
            9.1,                        // particle 4
            2.7,                        // particle 5
            4.1,                        // particle 9
            7.4,                        // particle 13
            8.2,                        // particle 17

            3.7,                        // particle 10
            8.2,                        // particle 14
            1.4,                        // particle 15
            6.9                         // particle 19
        };

        rs = {
            67.1,                       // particle 4
            24.6,                       // particle 5
            54.3,                       // particle 9
            92.4,                       // particle 13
            83.1,                       // particle 17

            42.3,                       // particle 10
            19.8,                       // particle 14
            86.5,                       // particle 15
            63.1                        // particle 19
        };

        expectedNlocal = 5;
        expectedNghost = 4;

        expectedIds = {
            4, 5, 9, 13, 17,
            10, 14, 15, 19
        };

        expectedXs = {
           -200.90,  190.36,  -40.58,   // particle 4
           -270.22,  120.74,  210.46,   // particle 5
           -120.33,   20.18, -199.05,   // particle 9
            -22.80,   18.10, -140.25,   // particle 13
            -28.20,  180.66,   66.03,   // particle 17

            30.25,   22.40,   10.66,    // particle 10
            40.11,  170.25,  204.67,    // particle 14
          -180.66,  -30.77,   14.95,    // particle 15
           -12.60,  -18.90,  130.10     // particle 19
        };

        expectedVs = {
             3.6,  89.2, -12.5,         // particle 4
            54.7, -66.3,  77.1,         // particle 5
           -72.5,  44.3, -95.8,         // particle 9
           -79.2,  12.4,  99.1,         // particle 13
            81.9, -35.0,  24.1,         // particle 17

            21.7, -38.4,  68.2,         // particle 10
           -53.7,  27.5, -41.6,         // particle 14
            64.0, -22.8,  15.2,         // particle 15
           -16.7,  73.3, -58.9          // particle 19
        };

        expectedFs = {
             3.1,  27.6, -14.8,         // particle 4
            44.5, -36.7,  21.4,         // particle 5
           -34.8,  25.1, -45.6,         // particle 9
           -48.1,  12.6,  33.9,         // particle 13
            35.0, -18.1,  13.7,         // particle 17

            14.8,  -7.3,  39.6,         // particle 10
           -25.7,  17.8, -38.6,         // particle 14
            26.4, -11.3,   4.9,         // particle 15
            -8.2,  46.7, -24.5          // particle 19
        };

        expectedMs = {
            9.1,                        // particle 4
            2.7,                        // particle 5
            4.1,                        // particle 9
            7.4,                        // particle 13
            8.2,                        // particle 17

            0.5,                        // particle 10
            5.9,                        // particle 14
            2.1,                        // particle 15
            4.6                         // particle 19
        };

        expectedRs = {
            67.1,                       // particle 4
            24.6,                       // particle 5
            54.3,                       // particle 9
            92.4,                       // particle 13
            83.1,                       // particle 17

            8.7,                        // particle 10
            37.6,                       // particle 14
            71.0,                       // particle 15
            26.4                        // particle 19
        };
        }
    else if (rank == 3)
    {
        nlocal = 5;
        nghost = 4;

        ids = {
            6, 7, 10, 12, 14,
            13, 16, 17, 18
        };

        xs = {
            210.83, 220.41, -150.88,    // particle 6
            140.62, 160.33,  198.21,    // particle 7
             30.25,  22.40,   10.66,    // particle 10
            160.81,  35.42,  289.73,    // particle 12
             40.11, 170.25,  204.67,    // particle 14

            -12.80,   8.10, -130.25,    // particle 13
              8.40, -12.10,   75.50,    // particle 16
            -18.20, 120.66,   56.03,    // particle 17
            175.27, -20.22, -226.70     // particle 18
        };

        vs = {
           -45.9,  18.0,  92.6,         // particle 6
           -11.4,  36.8, -83.2,         // particle 7
            21.7, -38.4,  68.2,         // particle 10
            46.1, -61.5,  33.8,         // particle 12
           -53.7,  27.5, -41.6,         // particle 14

           -63.4,  28.7,  74.5,         // particle 13
           -88.1,  41.3, -12.9,         // particle 16
            69.6, -22.4,  31.8,         // particle 17
            44.7, -57.9,  53.2          // particle 18
        };

        fs = {
            -9.9,   5.6,  31.2,         // particle 6
           -12.7,  18.9, -28.4,         // particle 7
            14.8,  -7.3,  39.6,         // particle 10
            41.2, -29.8,   9.7,         // particle 12
           -25.7,  17.8, -38.6,         // particle 14

           -21.6,  34.2,  17.5,         // particle 13
           -42.8,  19.7, -11.3,         // particle 16
            28.4, -14.6,  22.1,         // particle 17
            13.9, -37.5,  41.6          // particle 18
        };

        ms = {
            6.0,                        // particle 6
            1.3,                        // particle 7
            0.5,                        // particle 10
            3.0,                        // particle 12
            5.9,                        // particle 14

            2.9,                        // particle 13
            9.1,                        // particle 16
            4.6,                        // particle 17
            7.3                         // particle 18
        };

        rs = {
            98.0,                       // particle 6
            31.4,                       // particle 7
            8.7,                        // particle 10
            19.8,                       // particle 12
            37.6,                       // particle 14

            64.8,                       // particle 13
            27.5,                       // particle 16
            83.2,                       // particle 17
            41.9                        // particle 18
        };

        expectedNlocal = 5;
        expectedNghost = 4;

        expectedIds = {
            6, 7, 10, 12, 14,
            13, 16, 17, 18
        };

        expectedXs = {
            210.83, 220.41, -150.88,    // particle 6
            140.62, 160.33,  198.21,    // particle 7
             30.25,  22.40,   10.66,    // particle 10
            160.81,  35.42,  289.73,    // particle 12
             40.11, 170.25,  204.67,    // particle 14

            -22.80,  18.10, -140.25,    // particle 13
             18.40, -22.10,   95.50,    // particle 16
            -28.20, 180.66,   66.03,    // particle 17
            190.27, -40.22, -236.70     // particle 18
        };

        expectedVs = {
           -45.9,  18.0,  92.6,         // particle 6
           -11.4,  36.8, -83.2,         // particle 7
            21.7, -38.4,  68.2,         // particle 10
            46.1, -61.5,  33.8,         // particle 12
           -53.7,  27.5, -41.6,         // particle 14

           -79.2,  12.4,  99.1,         // particle 13
           -90.3,  48.6,  -7.4,         // particle 16
            81.9, -35.0,  24.1,         // particle 17
            57.8, -69.2,  39.4          // particle 18
        };

        expectedFs = {
            -9.9,   5.6,  31.2,         // particle 6
           -12.7,  18.9, -28.4,         // particle 7
            14.8,  -7.3,  39.6,         // particle 10
            41.2, -29.8,   9.7,         // particle 12
           -25.7,  17.8, -38.6,         // particle 14

           -48.1,  12.6,  33.9,         // particle 13
           -43.2,  22.5,  -6.8,         // particle 16
            35.0, -18.1,  13.7,         // particle 17
            28.6, -31.4,  19.3          // particle 18
        };

        expectedMs = {
            6.0,                        // particle 6
            1.3,                        // particle 7
            0.5,                        // particle 10
            3.0,                        // particle 12
            5.9,                        // particle 14

            7.4,                        // particle 13
            6.7,                        // particle 16
            8.2,                        // particle 17
            1.8                         // particle 18
        };

        expectedRs = {
            98.0,                       // particle 6
            31.4,                       // particle 7
            8.7,                        // particle 10
            19.8,                       // particle 12
            37.6,                       // particle 14

            92.4,                       // particle 13
            14.2,                       // particle 16
            83.1,                       // particle 17
            58.9                        // particle 18
        };
    }



    // forward communication

    // transfered data
    std::vector<double> sendBuff;
    std::vector<double> recvBuff;
    std::vector<double> sendVec;

    // the maximum number of particles
    const int dataPerParticle = Particles::dataPerParticle;
    const int nmax = 30;
    const int bufferSize = nmax * dataPerParticle;

    sendBuff.resize(bufferSize);
    recvBuff.resize(bufferSize);

    std::unique_ptr<Engine> engine;
    Communicator* comm;

    engine = set_particles_build_engine(rank, ranks, ids, xs, vs, fs, rs, ms, skin);

    auto& communicatorRef = engine->getCommunicator();

    std::array<int, 6> dests = communicatorRef->returnExchangeDests();

    for (const auto& dst : dests)
    {
        communicatorRef->sendGhosts(dst, sendVec);
        std::copy_n(sendVec.data(), sendVec.size(), sendBuff.data());
        comm_strategy->send(sendBuff.data(), sendBuff.size(), dst, 0);
    }

    for (const auto& src : dests)
    {
        comm_strategy->recv(recvBuff.data(), recvBuff.size(), src, 0);
        communicatorRef->recvGhosts(recvBuff);
    }

    auto& particlesRef = engine->getParticles();

    auto myIs = particlesRef->getIdData();
    auto myXs = particlesRef->getXData();
    auto myVs = particlesRef->getVData();
    auto myFs = particlesRef->getFData();
    auto myRs = particlesRef->getRData();
    auto myMs = particlesRef->getMData();

    REQUIRE_THAT(myIs,
        Array1DMatcherInt(expectedIds.data(), expectedNlocal, expectedNghost));
    REQUIRE_THAT(myXs,
        Array3DMatcher(expectedXs.data(), expectedNlocal, expectedNghost));
    REQUIRE_THAT(myVs,
        Array3DMatcher(expectedVs.data(), expectedNlocal, expectedNghost));
    REQUIRE_THAT(myFs,
        Array3DMatcher(expectedFs.data(), expectedNlocal, expectedNghost));
    REQUIRE_THAT(myRs,
        Array1DMatcher(expectedRs.data(), expectedNlocal, expectedNghost));
    REQUIRE_THAT(myMs,
        Array1DMatcher(expectedMs.data(), expectedNlocal, expectedNghost));

}



