#include "catch_amalgamated.hpp"
#include "test_communicator_helpers.hpp"
#include "test.hpp"
#include "MPI_comm_strategy.h"

TEST_CASE("Testing the movement of particles between processors without skin (2X2X1)","[mpi]")
{
    std::cout << "Testing the movement of particles between processors without skin (2X2X1)" << std::endl;
    std::cout << std::string(80, '=') << std::endl;


    std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();
    int rank = comm_strategy->getRank();
    int size = comm_strategy->getSize();

    // checking the min required number of ranks
    minRanksRequirement(comm_strategy,4);
    // skipping the extra ranks
    skipExtraRanks(comm_strategy, 4);

    std::vector<int> newId;
    std::vector<double> newX, newV, newF, newM, newR;
    std::vector<int> expectedIds;
    std::vector<double> expectedXs, expectedVs, expectedFs, expectedMs, expectedRs;


    // moving particles
    if (rank == 0) {
        newId = {
            0,1,2,3,4
        };
        newX =
        {
            -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) region 1
            -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) region 1
            -180.66,   30.77,   14.95,   // particle 2  owned by Q00 (-+ interior) region 3
              12.60,  -18.90,  130.10,   // particle 3  owned by Q00 (+- interior) region 2
              35.10, -140.48,   12.66,   // particle 4  owned by Q00 (+- interior) region 2
        };

        newV =
        {
            72.4, -33.1,  95.0,   // particle 0
           -88.6,  14.3, -56.7,   // particle 1
            41.2, -19.8,  63.5,   // particle 2
           -97.1,  28.9, -74.4,   // particle 3
             3.6,  89.2, -12.5,   // particle 4
        };

        newF =
        {
           12.4, -33.9,  45.2,   // particle 0
          -18.6,   7.3, -41.5,   // particle 1
           29.7,  -5.4,  38.9,   // particle 2
          -22.1,  16.0, -49.7,   // particle 3
            3.1,  27.6, -14.8,   // particle 4
        };

        newR =
        {
         12.5,  // particle 0 region 1
         89.3,  // particle 1 region 1
         45.7,  // particle 2 region 3
         3.9,   // particle 3 region 2
         67.1,  // particle 4 region 2
        };

        newM =
        {
            3.4,  // particle 0 region 1
            7.8,  // particle 1 region 1
            0.9,  // particle 2 region 3
            5.6,  // particle 3 region 2
            9.1,  // particle 4 region 2
        };

        // expected values for each core
        // core 1
        expectedIds = {
            0,1,7,17
        };
        expectedXs = {
            -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) region 1
            -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) region 1
             -25.60, -160.15,  -55.27,   // particle 7  owned by Q10 (-- interior) region 1
             -30.25,  -22.40,   10.66,   // particle 17 owned by Q01 (-- interior) region 1
        };
        expectedVs = {
          72.4, -33.1,  95.0,   // particle 0
         -88.6,  14.3, -56.7,   // particle 1
         -11.4,  36.8, -83.2,   // particle 7
          81.9, -35.0,  24.1,   // particle 17
        };
        expectedFs = {
           12.4, -33.9,  45.2,   // particle 0
          -18.6,   7.3, -41.5,   // particle 1
          -12.7,  18.9, -28.4,   // particle 7
           35.0, -18.1,  13.7,   // particle 17
        };
        expectedMs = {
             3.4,  // particle 0
             7.8,  // particle 1
             1.3,  // particle 7
             8.2,  // particle 17
        };

        expectedRs = {
            12.5,  // particle 0
            89.3,  // particle 1
            31.4,  // particle 7
            83.1,  // particle 17
        };
    }
    else if (rank == 1) {

        newId = {
            5,6,7,8,9
        };
        newX =
        {
             180.62, -210.44,   60.91,   // particle 5  owned by Q10 (+- interior) region 2
             260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior) region 2
             -25.60, -160.15,  -55.27,   // particle 7  owned by Q10 (-- interior) region 1
             -18.40,  +22.10,   95.50,   // particle 8  owned by Q10 (-+ interior) region 3
             190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior) region 2
        };

        newV =
        {
            54.7, -66.3,  77.1,   // particle 5
           -45.9,  18.0,  92.6,   // particle 6
           -11.4,  36.8, -83.2,   // particle 7
            59.7, -24.6,  10.9,   // particle 8
           -72.5,  44.3, -95.8,   // particle 9
        };

        newF =
        {
            44.5, -36.7,  21.4,   // particle 5
            -9.9,   5.6,  31.2,   // particle 6
           -12.7,  18.9, -28.4,   // particle 7
            47.3, -19.5,   6.2,   // particle 8
           -34.8,  25.1, -45.6,   // particle 9
        };

        newM =
        {
            2.7,  // particle 5 region 2
            6.0,  // particle 6 region 2
            1.3,  // particle 7 region 1
            8.4,  // particle 8 region 3
            4.1,  // particle 9 region 2
        };

        newR =
        {
         24.6,  // particle 5 region 2
         98.0,  // particle 6 region 2
         31.4,  // particle 7 region 1
         76.2,  // particle 8 region 3
         54.3,  // particle 9 region 2
        };


        // core 2
        expectedIds = {
            3,4,5,6,9,19
        };
        expectedXs = {
              12.60,  -18.90,  130.10,   // particle 3  owned by Q00 (+- interior) region 2
              35.10, -140.48,   12.66,   // particle 4  owned by Q00 (+- interior) region 2
             180.62, -210.44,   60.91,   // particle 5  owned by Q10 (+- interior) region 2
             260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior) region 2
             190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior) region 2
             160.81,  -35.42,  289.73,   // particle 19 owned by Q01 (+- interior) region 2
        };
        expectedVs = {
            -97.1,  28.9, -74.4,   // particle 3
              3.6,  89.2, -12.5,   // particle 4
             54.7, -66.3,  77.1,   // particle 5
            -45.9,  18.0,  92.6,   // particle 6
            -72.5,  44.3, -95.8,   // particle 9
            -16.7,  73.3, -58.9    // particle 19

        };
        expectedFs = {
            -22.1,  16.0, -49.7,   // particle 3
              3.1,  27.6, -14.8,   // particle 4
             44.5, -36.7,  21.4,   // particle 5
             -9.9,   5.6,  31.2,   // particle 6
            -34.8,  25.1, -45.6,   // particle 9
             -8.2,  46.7, -24.5    // particle 19
        };
        expectedMs = {
            5.6,  // particle 3
            9.1,  // particle 4
            2.7,  // particle 5
            6.0,  // particle 6
            4.1,  // particle 9
            4.6   // particle 19
        };
        expectedRs = {
             3.9,   // particle 3
            67.1,  // particle 4
            24.6,  // particle 5
            98.0,  // particle 6
            54.3,  // particle 9
            26.4   // particle 19
        };
    }
    else if (rank == 2) {

        newId = {
            10,11,12,13,14
        };
        newX =
        {
            -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior) region 3
            -270.22,  120.74,  210.46,   // particle 11 owned by Q01 (-+ interior) region 3
            -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior) region 3
              22.80,   18.10, -140.25,   // particle 13 owned by Q01 (++ interior) region 4
             -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior) region 3
        };

        newV =
        {
            21.7, -38.4,  68.2,   // particle 10
           -14.9,  87.6, -29.3,   // particle 11
            46.1, -61.5,  33.8,   // particle 12
           -79.2,  12.4,  99.1,   // particle 13
           -53.7,  27.5, -41.6,   // particle 14
        };

        newF =
        {
            14.8,  -7.3,  39.6,   // particle 10
           -21.9,  30.4, -16.5,   // particle 11
            41.2, -29.8,   9.7,   // particle 12
           -48.1,  12.6,  33.9,   // particle 13
           -25.7,  17.8, -38.6,   // particle 14
        };

        newM =
        {
            0.5,  // particle 10 region 3
            9.8,  // particle 11 region 3
            3.0,  // particle 12 region 3
            7.4,  // particle 13 region 4
            5.9,  // particle 14 region 3
        };

        newR =
        {
         8.7,   // particle 10 region 3
         63.5,  // particle 11 region 3
         19.8,  // particle 12 region 3
         92.4,  // particle 13 region 4
         37.6,  // particle 14 region 3
        };


        // core 3
        expectedIds = {
            2,8,10,11,12,14,18
        };
        expectedXs = {
            -180.66,   30.77,   14.95,   // particle 2  owned by Q00 (-+ interior) region 3
             -18.40,   22.10,   95.50,   // particle 8  owned by Q10 (-+ interior) region 3
            -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior) region 3
            -270.22,  120.74,  210.46,   // particle 11 owned by Q01 (-+ interior) region 3
            -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior) region 3
             -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior) region 3
             -40.11,  170.25,  204.67,   // particle 18 owned by Q01 (-+ interior) region 3
        };
        expectedVs = {
         41.2, -19.8,  63.5,   // particle 2
         59.7, -24.6,  10.9,   // particle 8
         21.7, -38.4,  68.2,   // particle 10
        -14.9,  87.6, -29.3,   // particle 11
         46.1, -61.5,  33.8,   // particle 12
        -53.7,  27.5, -41.6,   // particle 14
         57.8, -69.2,  39.4,   // particle 18

        };
        expectedFs = {
         29.7,  -5.4,  38.9,   // particle 2
         47.3, -19.5,   6.2,   // particle 8
         14.8,  -7.3,  39.6,   // particle 10
        -21.9,  30.4, -16.5,   // particle 11
         41.2, -29.8,   9.7,   // particle 12
        -25.7,  17.8, -38.6,   // particle 14
         28.6, -31.4,  19.3,   // particle 18

        };
        expectedMs = {
         0.9,  // particle 2
         8.4,  // particle 8
         0.5,  // particle 10
         9.8,  // particle 11
         3.0,  // particle 12
         5.9,  // particle 14
         1.8,  // particle 18

        };

        expectedRs = {
         45.7,  // particle 2
         76.2,  // particle 8
         8.7,   // particle 10
         63.5,  // particle 11
         19.8,  // particle 12
         37.6,  // particle 14
         58.9,  // particle 18
        };
    } 
    else if (rank == 3) {

        newId = {
            15,16,17,18,19
        };
        newX =
        {
             210.83,  220.41, -150.88,   // particle 15 owned by Q01 (++ interior) region 4
             140.62,  160.33,  198.21,   // particle 16 owned by Q01 (++ interior) region 4
             -30.25,  -22.40,   10.66,   // particle 17 owned by Q01 (-- interior) region 1
             -40.11,  170.25,  204.67,   // particle 18 owned by Q01 (-+ interior) region 3
             160.81,  -35.42,  289.73,   // particle 19 owned by Q01 (+- interior) region 2
        };

        newV =
        {
            64.0, -22.8,  15.2,   // particle 15
           -90.3,  48.6,  -7.4,   // particle 16
            81.9, -35.0,  24.1,   // particle 17
            57.8, -69.2,  39.4,   // particle 18
           -16.7,  73.3, -58.9    // particle 19
        };

        newF =
        {
            26.4, -11.3,   4.9,   // particle 15
           -43.2,  22.5,  -6.8,   // particle 16
            35.0, -18.1,  13.7,   // particle 17
            28.6, -31.4,  19.3,   // particle 18
            -8.2,  46.7, -24.5    // particle 19
        };

        newM =
        {
            2.1,  // particle 15 region 4
            6.7,  // particle 16 region 4
            8.2,  // particle 17 region 1
            1.8,  // particle 18 region 3
            4.6   // particle 19 region 2
        };

        newR =
        {
         71.0,  // particle 15 region 4
         14.2,  // particle 16 region 4
         83.1,  // particle 17 region 1
         58.9,  // particle 18 region 3
         26.4   // particle 19 region 2
        };

        // core 4
        expectedIds = {
            13,15,16
        };
        expectedXs = {
            22.80,   18.10,  -140.25,   // particle 13 owned by Q01 (++ interior) region 4 
            210.83,  220.41, -150.88,   // particle 15 owned by Q01 (++ interior) region 4
            140.62,  160.33,  198.21,   // particle 16 owned by Q01 (++ interior) region 4
        };

        expectedVs = {
        -79.2,  12.4,  99.1,   // particle 13
         64.0, -22.8,  15.2,   // particle 15
        -90.3,  48.6,  -7.4,   // particle 16
        };

        expectedFs = {
        -48.1,  12.6,  33.9,   // particle 13
         26.4, -11.3,   4.9,   // particle 15
        -43.2,  22.5,  -6.8,   // particle 16
        };

        expectedMs = {
         7.4,  // particle 13
         2.1,  // particle 15
         6.7,  // particle 16
        };

        expectedRs = {
         92.4,  // particle 13
         71.0,  // particle 15
         14.2,  // particle 16
        };

    }



    // running configuration
    std::array<int, 3> ranks = { 2,2,1 };
    //
    int nranks = ranks[0] * ranks[1] * ranks[2];
  

    std::unique_ptr<Engine> engine = build_engine_set_particles(
        rank, ranks, newId, newX, newV, newF, newR, newM);

    auto& communicatorRef = engine->getCommunicator();

    std::array<int, 6> destArray;


    int nDestLocal = 0, nDestTotal;
    int numberOfAttempts;

    constexpr int maxAttempts = 4;

    do {

    }



    std::vector<std::array<int, 6>> exchangeDestArray(nranks, std::array<int, 6>{});





    // number of destinations
    int nDestsTotal = 0;

    int numberOfAttempts = 0;

    constexpr int maxAttempts = 4;
    // repeating the particle reassginement until there is no
    // outside particles in each communicator
    // The reason for repeating it is that it is a rare possibility 
    // that x, y, and z values of a particle are all out of the
    // rank dimensions. In this case at first attempt the particle is 
    // moved in the rank which is neighboring the current rank 
    // in the x dimension. The second time in the y dimension
    // and finally in the z dimension... This way there is a need
    // for knowing 6 neighboring ranks in contrast to the general 
    // case of 26 neighboring ranks!
    do {
        std::vector<double> messageVec[4];

        nDestsTotal = 0;
        numberOfAttempts++;

        for (int i = 0; i < 4; i++) {
            exchangeDestArray[i] = communicatorArray[i]->returnExchangeDests();

            // xlo, xhi, ylo, yhi, ...
            for (int j = 0; j < 6; j++)
            {
                // not sure about this yet.
                int dest = exchangeDestArray[i][j];
                if (dest < 0)
                    continue;
                // 
                // may be I need to call some reset function to reset exterXLo, ...
                nDestsTotal += communicatorArray[i]->sendParticles(dest, messageVec[dest]);

            }
        }


        // ranks
        for (int i = 0; i < 4; i++) {
            communicatorArray[i]->recvParticles(messageVec[i]);
        }

    } while (nDestsTotal > 0 && numberOfAttempts < maxAttempts);



    for (int i = 0; i < nranks; i++) {
        int id = ids[i];
        Engine* engine_ptr = engineArray[i].get();

        checking_communicator(
            id, engine_ptr,
            expectedXsVec,
            expectedVsVec,
            expectedFsVec,
            expectedRsVec,
            expectedMsVec
        );
    }

}