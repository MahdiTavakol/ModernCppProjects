#include "catch_amalgamated.hpp"
#include "test_communicator_helpers.hpp"
#include "test.hpp"
#include "MPI_comm_strategy.h"

TEST_CASE("Testing the movement of particles between processors without skin (2X2X1)","[mpi]")
{
    printRankZero("Testing the movement of particles between processors without skin (2X2X1)");
    printRankZero(std::string(80, '='));


    std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();
    int rank = comm_strategy->getRank();
    int size = comm_strategy->getSize();

    // checking the min required number of ranks
    minRanksRequirement(comm_strategy,4);
    // skipping the extra ranks
    comm_strategy = skipExtraRanks(comm_strategy, 4);

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
    std::array<int, 6> dests = communicatorRef->returnExchangeDests();

    // transfered data
    std::vector<double> sendBuff;
    std::vector<double> recvBuff;
    std::vector<double> sendVec;

    // the maximum number of particles
    const int dataPerParticle = Particles::dataPerParticle;
    const int nmax = 50;
    const int bufferSize = nmax * dataPerParticle;

    sendBuff.resize(bufferSize);
    recvBuff.resize(bufferSize);


    int nDestLocal = 0, nDestTotal = 0;
    int nAttemptsLocal = 0, nAttemptsTotal = 0;

    constexpr int maxAttempts = 4;

    do {
        nDestLocal = 0;
        nAttemptsLocal++;

        /// <summary>
        /// the dst orders is xlo, xhi, ylo, yhi, zlo, zhi
        /// so for the dst this would be their xhi, xlo, yhi, ylo, zhi,zlo
        /// </summary>
        std::vector<int> srcTags = { 1,0,3,2,5,4 };
        std::vector<int> dstTags = { 0,1,2,3,4,5 };

        std::vector<std::array<int, 2>> SrcDst = { {0,1},{1,0},{2,3},{3,2},{ 4,5 },{5,4} };
        std::vector<int> tags = { 0,1,2,3,4,5 };

        struct
        {
            std::array<int, 2> indx;
            int tag;
        } loopStruct[6] =
        {
            {{0,1},0}, // send in xlo recv in xhi
            {{1,0},1}, // send in xhi recv in xlo
            {{2,3},2}, // send in ylo recv in yhi
            {{3,2},3}, // send in yhi recv in ylo
            {{4,5},4}, // send in zlo recv in zhi
            {{5,4},5}  // send in zhi recv in zlo
        };

        for (const auto& info : loopStruct)
        {
            int tgtIndx = info.indx[0];
            int srcIndx = info.indx[1];
            int tag = info.tag;

            nDestLocal += communicatorRef->sendParticles(dests[tgtIndx], sendVec);
            std::copy_n(sendVec.data(), sendVec.size(), sendBuff.data());
            comm_strategy->send(sendBuff.data(), bufferSize, dests[tgtIndx], tag);
            sendVec.clear();

            comm_strategy->recv(recvBuff.data(), bufferSize, dests[srcIndx], tag);
            communicatorRef->recvParticles(recvBuff);
            recvBuff.resize(bufferSize);

            comm_strategy->waitAll();
        }


        // MPI_Allreduce to get nDestsTotal
        comm_strategy->reduceAll(&nDestLocal, &nDestTotal, 1);
        comm_strategy->reduceAll(&nAttemptsLocal, &nAttemptsTotal, 1);


    } while (nDestTotal > 0 && nAttemptsTotal < maxAttempts);




    checking_communicator(
        rank, engine.get(),
        expectedXs,
        expectedVs,
        expectedFs,
        expectedRs,
        expectedMs
    );
}


TEST_CASE("Testing the movement of particles between processors without skin (3X3X1)", "[mpi]")
{
    printRankZero("Testing the movement of particles between processors without skin (3X3X1)");
    printRankZero(std::string(80, '='));

    std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();
    int rank = comm_strategy->getRank();
    int size = comm_strategy->getSize();

    // checking the min required number of ranks
    minRanksRequirement(comm_strategy, 9);
    // skipping the extra ranks
    comm_strategy = skipExtraRanks(comm_strategy, 9);


    // moving particles
    std::vector<double> newX;
    std::vector<double> expectedX;

    if (rank == 0) {
        newX =
        {
            -220.0, -220.0,   40.0,   // particle 1 stays in Q00    rank0
            -150.0, -180.0,  -60.0,   // particle 2 stays in Q00    rank0
             -80.0, -210.0,   15.0,   // particle 3 -> Q10 (x high) rank1
            -170.0,  -80.0,   90.0,   // particle 4 -> Q01 (y high) rank3
        };

        // expected values

        // Q00 rank0
        expectedX =
        {
            -220.0, -220.0,   40.0,   // particle 1 stays in Q00    rank0
            -150.0, -180.0,  -60.0,   // particle 2 stays in Q00    rank0
            -130.0, -170.0,   10.0,   // particle 7 -> Q00 (x low)  rank0
            -180.0, -140.0,   80.0    // particle 17 -> Q00 (y low) rank0
        };
    }
    else if (rank == 1) {
        newX =
        {
             -20.0, -220.0,   35.0,   // particle 5 stays in Q10    rank1
              60.0, -150.0,  -80.0,   // particle 6 stays in Q10    rank1
            -130.0, -170.0,   10.0,   // particle 7 -> Q00 (x low)  rank0
             130.0, -190.0,  -25.0,   // particle 8 -> Q20 (x high) rank2
              40.0,  -70.0,   55.0,   // particle 9 -> Q11 (y high) rank4
        };

        // Q10 rank1
        expectedX =
        {
            -80.0, -210.0,   15.0,   // particle 3 -> Q10 (x high) rank1
            -20.0, -220.0,   35.0,   // particle 5 stays in Q10    rank1
             60.0, -150.0,  -80.0,   // particle 6 stays in Q10    rank1
             80.0, -160.0,   70.0,   // particle 12 -> Q10 (x low) rank1
             10.0, -130.0,   45.0    // particle 23 -> Q10 (y low) rank1
        };

       
    }
    else if (rank == 2) {
        newX =
        {
             180.0, -220.0,   10.0,   // particle 10 stays in Q20    rank2
             240.0, -120.0,  -95.0,   // particle 11 stays in Q20    rank2
              80.0, -160.0,   70.0,   // particle 12 -> Q10 (x low)  rank1
             170.0,  -90.0,  -40.0,   // particle 13 -> Q21 (y high) rank5
        };

        // Q20 rank2
        expectedX =
        {
            130.0, -190.0,  -25.0,   // particle 8 -> Q20 (x high) rank2
            180.0, -220.0,   10.0,   // particle 10 stays in Q20   rank2
            240.0, -120.0,  -95.0,   // particle 11 stays in Q20   rank2
            180.0, -170.0,  -15.0    // particle 28 -> Q20 (y low) rank2
        };
    }
    else if (rank == 3) {
        newX =
        {
            -240.0,  -20.0,   50.0,   // particle 14 stays in Q01    rank3
            -160.0,   60.0,  -20.0,   // particle 15 stays in Q01    rank3
             -90.0,   30.0,   15.0,   // particle 16 -> Q11 (x high) rank4
            -180.0, -140.0,   80.0,   // particle 17 -> Q00 (y low)  rank0
            -210.0,  140.0,  -35.0,   // particle 18 -> Q02 (y high) rank6
        };

        // Q01 rank3
        expectedX =
        {
            -170.0,  -80.0,   90.0,   // particle 4 -> Q01 (y high) rank3
            -240.0,  -20.0,   50.0,   // particle 14 stays in Q01   rank3
            -160.0,   60.0,  -20.0,   // particle 15 stays in Q01   rank3
            -140.0,   10.0,   75.0,   // particle 21 -> Q01 (x low) rank3
            -190.0,   80.0,   95.0    // particle 33 -> Q01 (y low) rank3
        };
    }
    else if (rank == 4) {
        newX =
        {
             -40.0,  -30.0,   20.0,   // particle 19 stays in Q11    rank4
              70.0,   80.0,  -60.0,   // particle 20 stays in Q11    rank4
            -140.0,   10.0,   75.0,   // particle 21 -> Q01 (x low)  rank3
             140.0,  -20.0,  -10.0,   // particle 22 -> Q21 (x high) rank5
              10.0, -130.0,   45.0,   // particle 23 -> Q10 (y low)  rank1
             -70.0,  160.0,   90.0,   // particle 24 -> Q12 (y high) rank7
        };

        // Q11 rank4
        expectedX =
        {
             40.0,  -70.0,   55.0,   // particle 9 -> Q11 (y high)  rank4
            -90.0,   30.0,   15.0,   // particle 16 -> Q11 (x high) rank4
            -40.0,  -30.0,   20.0,   // particle 19 stays in Q11    rank4
             70.0,   80.0,  -60.0,   // particle 20 stays in Q11    rank4
             60.0,   20.0,   35.0,   // particle 27 -> Q11 (x low)  rank4
             30.0,   70.0,   75.0    // particle 38 -> Q11 (y low)  rank4
        };

    }
    else if (rank == 5) {
        newX =
        {
             160.0,  -40.0,   25.0,   // particle 25 stays in Q21    rank5
             250.0,   70.0,  -85.0,   // particle 26 stays in Q21    rank5
              60.0,   20.0,   35.0,   // particle 27 -> Q11 (x low)  rank4
             180.0, -170.0,  -15.0,   // particle 28 -> Q20 (y low)  rank2
             220.0,  150.0,   55.0,   // particle 29 -> Q22 (y high) rank8
        };


        // Q21 rank5
        expectedX =
        {
            170.0,  -90.0,  -40.0,   // particle 13 -> Q21 (y high) rank5
            140.0,  -20.0,  -10.0,   // particle 22 -> Q21 (x high) rank5
            160.0,  -40.0,   25.0,   // particle 25 stays in Q21    rank5
            250.0,   70.0,  -85.0,   // particle 26 stays in Q21    rank5
            220.0,   90.0,  -35.0    // particle 42 -> Q21 (y low)  rank5
        };
    }
    else if (rank == 6) {
        newX =
        {
            -260.0,  180.0,   40.0,   // particle 30 stays in Q02    rank6
            -140.0,  260.0,  -70.0,   // particle 31 stays in Q02    rank6
             -80.0,  210.0,   15.0,   // particle 32 -> Q12 (x high) rank7
            -190.0,   80.0,   95.0,   // particle 33 -> Q01 (y low)  rank3
        };

        // Q02 rank6
        std::vector<double> expectedX =
        {
            -210.0,  140.0,  -35.0,   // particle 18 -> Q02 (y high) rank6
            -260.0,  180.0,   40.0,   // particle 30 stays in Q02    rank6
            -140.0,  260.0,  -70.0,   // particle 31 stays in Q02    rank6
            -130.0,  150.0,   60.0,   // particle 36 -> Q02 (x low)  rank6
        };
    }
    else if (rank ==7) {
        newX =
        {
             -20.0,  160.0,   35.0,   // particle 34 stays in Q12    rank7
              80.0,  220.0,  -55.0,   // particle 35 stays in Q12    rank7
            -130.0,  150.0,   60.0,   // particle 36 -> Q02 (x low)  rank6
             140.0,  180.0,  -20.0,   // particle 37 -> Q22 (x high) rank8
              30.0,   70.0,   75.0,   // particle 38 -> Q11 (y low)  rank4
        };

        // Q12 rank7
        std::vector<double> expectedX =
        {
            -70.0,  160.0,   90.0,   // particle 24 -> Q12 (y high) rank7
            -80.0,  210.0,   15.0,   // particle 32 -> Q12 (x high) rank7
            -20.0,  160.0,   35.0,   // particle 34 stays in Q12    rank7
             80.0,  220.0,  -55.0,   // particle 35 stays in Q12    rank7
             90.0,  170.0,   45.0    // particle 41 -> Q12 (x low) rank7
        };

    }
    else if (rank == 8) {
        newX =
        {
             180.0,  160.0,   10.0,   // particle 39 stays in Q22   rank8
             260.0,  240.0,  -90.0,   // particle 40 stays in Q22   rank8
              90.0,  170.0,   45.0,   // particle 41 -> Q12 (x low) rank7
             220.0,   90.0,  -35.0,   // particle 42 -> Q21 (y low) rank5
        };


        // Q22 rank8
        expectedX =
        {
            220.0,  150.0,   55.0,   // particle 29 -> Q22 (y high) rank8
            140.0,  180.0,  -20.0,   // particle 37 -> Q22 (x high) rank8
            180.0,  160.0,   10.0,   // particle 39 stays in Q22   rank8
            260.0,  240.0,  -90.0,   // particle 40 stays in Q22   rank8
        };
    }

    
    std::vector<double> newV(newX.size(), 0.0), newF(newX.size(), 0.0);
    std::vector<double> newM(newX.size() / 3, 0.0), newR(newX.size() / 3, 0.0);


    std::vector<double> expectedV(expectedX.size(), 0.0), expectedF(expectedX.size(), 0.0);
    std::vector<double> expectedM(expectedX.size() / 3, 0.0), expectedR(expectedX.size() / 3, 0.0);


    // running configuration
    std::array<int, 3> ranks = { 3,3,1 };
    // 
    int nranks = ranks[0] * ranks[1] * ranks[2];

    std::unique_ptr<Engine> engine;

    engine = build_engine_set_particles(rank, ranks, newX, newV, newF, newR, newM);


    auto& communicatorRef = engine->getCommunicator();
    std::array<int, 6> dests = communicatorRef->returnExchangeDests();

    // transfered data
    std::vector<double> sendBuff;
    std::vector<double> recvBuff;
    std::vector<double> sendVec;

    // the maximum number of particles
    const int dataPerParticle = Particles::dataPerParticle;
    const int nmax = 50;
    const int bufferSize = nmax * dataPerParticle;

    sendBuff.resize(bufferSize);
    recvBuff.resize(bufferSize);


    int nDestLocal = 0, nDestTotal = 0;
    int nAttemptsLocal = 0, nAttemptsTotal = 0;

    constexpr int maxAttempts = 4;

    do {
        nDestLocal = 0;
        nAttemptsLocal++;



        struct
        {
            std::array<int, 2> indx;
            int tag;
        } loopStruct[6] =
        {
            {{0,1},0}, // send in xlo recv in xhi
            {{1,0},1}, // send in xhi recv in xlo
            {{2,3},2}, // send in ylo recv in yhi
            {{3,2},3}, // send in yhi recv in ylo
            {{4,5},4}, // send in zlo recv in zhi
            {{5,4},5}  // send in zhi recv in zlo
        };

        for (const auto& info : loopStruct)
        {
            int tgtIndx = info.indx[0];
            int srcIndx = info.indx[1];
            int tag = info.tag;

            nDestLocal += communicatorRef->sendParticles(dests[tgtIndx], sendVec);
            std::copy_n(sendVec.data(), sendVec.size(), sendBuff.data());
            comm_strategy->send(sendBuff.data(), bufferSize, dests[tgtIndx], tag);
            sendVec.clear();

            comm_strategy->recv(recvBuff.data(), bufferSize, dests[srcIndx], tag);
            communicatorRef->recvParticles(recvBuff);
            recvBuff.resize(bufferSize);

            comm_strategy->waitAll();
        }


        // MPI_Allreduce to get nDestsTotal
        comm_strategy->reduceAll(&nDestLocal, &nDestTotal, 1);
        comm_strategy->reduceAll(&nAttemptsLocal, &nAttemptsTotal, 1);


    } while (nDestTotal > 0 && nAttemptsTotal < maxAttempts);


    checking_communicator(
        rank, engine.get(),
        expectedX,
        expectedV,
        expectedF,
        expectedR,
        expectedM
    );


}



TEST_CASE("Testing the movement of particles between processors without skin (3X3X3)","[mpi]")
{
    printRankZero("Testing the movement of particles between processors without skin (3X3X3)");
    printRankZero(std::string(80, '='));


    std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();
    int rank = comm_strategy->getRank();
    int size = comm_strategy->getSize();

    // checking the min required number of ranks
    minRanksRequirement(comm_strategy, 27);
    // skipping the extra ranks
    comm_strategy = skipExtraRanks(comm_strategy, 27);


    // moving particles
    std::vector<double> newX = {};
    std::vector<double> expectedX = {};


    // rank4
    if (rank == 4) {
        newX =
        {
             -20.0,  -70.0, -235.0,   // particle 1 stays in rank4
              60.0,  -30.0, -180.0,   // particle 2 stays in rank4
              70.0,  -60.0,  -90.0,   // particle 3 ->  (z high) rank13
              40.0,  -70.0,   55.0,   // particle 4 ->  (z high) rank13
        };

        expectedX =
        {
             -20.0,  -70.0, -235.0,   // particle 1 stays in rank4
              60.0,  -30.0, -180.0,   // particle 2 stays in rank4
        };
    }
    else if (rank == 10) {
        // rank10
        newX =
        {
             -40.0,  -240.0,   50.0,   // particle 9  stays in rank10
              60.0,  -160.0,  -20.0,   // particle 10 stays in rank10
             -75.0,   -40.0,   80.0,   // particle 11 -> (y high) rank13
              90.0,    60.0,  -35.0,   // particle 12 -> (y high) rank13
        };
        // rank10
        expectedX =
        {
             -40.0,  -240.0,   50.0,   // particle 9  stays in rank10
              60.0,  -160.0,  -20.0,   // particle 10 stays in rank10
        };
    }
    else if (rank == 12)
    {
        // rank12
        newX =
        {
            -280.0,  -20.0,   10.0,   // particle 5 stays in rank12
            -140.0,   80.0,  -95.0,   // particle 6 stays in rank12
              80.0,  -60.0,   70.0,   // particle 7 -> (x high)  rank13
             -70.0,  -90.0,  -40.0,   // particle 8 -> (x high) rank13
        };

        // rank12
        expectedX =
        {
            -280.0,  -20.0,   10.0,   // particle 5 stays in rank12
            -140.0,   80.0,  -95.0,   // particle 6 stays in rank12
        };
    }
    else if (rank == 13)
    {
        // rank13
        newX = {
            -65.0, -85.0, 95.0,  // rank13
            -12.0, -66.0, -79.0  // rank13
        };

        // rank13
        expectedX = {
            -65.0, -85.0,  95.0,  // rank13
            -12.0, -66.0, -79.0,  // rank13
             70.0, -60.0, -90.0,  // particle 3 ->  (z high) rank13
             40.0, -70.0,  55.0,  // particle 4 ->  (z high) rank13
            -75.0, -40.0,  80.0,  // particle 11 -> (y high) rank13
             90.0,  60.0, -35.0,  // particle 12 -> (y high) rank13
             80.0, -60.0,  70.0,  // particle 7 -> (x high)  rank13
            -70.0, -90.0, -40.0,  // particle 8 -> (x high) rank13
             10.0, -30.0,  45.0,  // particle 15 -> (x low)  rank13
            -70.0,  60.0,  90.0,  // particle 16 -> (x low)  rank13
             60.0,  20.0,  35.0,  // particle 19 -> (y low)  rank13
             18.0, -17.0, -15.0,  // particle 20 -> (y low)  rank13
            -80.0,  21.0,  15.0,  // particle 23 -> (z low)  rank13
            -19.0,  80.0,  95.0,  // particle 24 -> (z low) rank13
            -30.0,  95.0,  45.0
        };
    }
    else if (rank == 14)
    {
        newX =
        {
             230.0,  -30.0,   20.0,   // particle 13 stays in rank14
             270.0,   80.0,  -60.0,   // particle 14 stays in rank14
              10.0,  -30.0,   45.0,   // particle 15 -> (x low)  rank13
             -70.0,   60.0,   90.0,   // particle 16 -> (x low)  rank13
        };

        // rank14
        expectedX =
        {
             230.0,  -30.0,   20.0,   // particle 13 stays in rank14
             270.0,   80.0,  -60.0,   // particle 14 stays in rank14
        };
    }
    else if (rank == 16)
    {
        // rank16
        newX =
        {
             -40.0,  160.0,   25.0,   // particle 17 stays in rank17
              70.0,  250.0,  -85.0,   // particle 18 stays in rank17
              60.0,   20.0,   35.0,   // particle 19 -> (y low)  rank13
              18.0,  -17.0,  -15.0,   // particle 20 -> (y low)  rank13
        };

        expectedX =
        {
             -40.0,  160.0,   25.0,   // particle 17 stays in rank17
              70.0,  250.0,  -85.0,   // particle 18 stays in rank17
        };
    }
    else if (rank == 17)
    {
        newX = 
        {
            -30.0, 95.0, 45.0 // rank 13
        };

        expectedX = {};
    }
    else if (rank == 22)
    {
        newX =
        {
            -26.0,   80.0,   140.0,   // particle 21 stays in rank22
            -14.0,   60.0,   170.0,   // particle 22 stays in rank22
             -80.0,  21.0,    15.0,   // particle 23 -> (z low) rank13
             -19.0,  80.0,    95.0,   // particle 24 -> (z low) rank13
        };

        expectedX =
        {
            -26.0,   80.0,   140.0,   // particle 21 stays in rank22
            -14.0,   60.0,   170.0,   // particle 22 stays in rank22
        };
    }
    

    std::vector<double> newV(newX.size(), 0.0), newF(newX.size(), 0.0);
    std::vector<double> newM(newX.size() / 3, 0.0), newR(newX.size() / 3, 0.0);


    std::vector<double> expectedV(newX.size(), 0.0), expectedF(newX.size(), 0.0);
    std::vector<double> expectedM(newX.size() / 3, 0.0), expectedR(newX.size() / 3, 0.0);

    // running configuration
    std::array<int, 3> ranks = { 3,3,3 };
    // 
    int nranks = ranks[0] * ranks[1] * ranks[2];
    // Engine
    std::unique_ptr<Engine> engine = 
        build_engine_set_particles(
            rank, ranks, newX, newV,
            newF, newR, newM);
    auto& communicatorRef = engine->getCommunicator();
    std::array<int, 6> dests = communicatorRef->returnExchangeDests();

    // transfered data
    std::vector<double> sendBuff;
    std::vector<double> recvBuff;
    std::vector<double> sendVec;

    // the maximum number of particles
    const int dataPerParticle = Particles::dataPerParticle;
    const int nmax = 50;
    const int bufferSize = nmax * dataPerParticle;

    sendBuff.resize(bufferSize);
    recvBuff.resize(bufferSize);


    int nDestLocal = 0, nDestTotal = 0;
    int nAttemptsLocal = 0, nAttemptsTotal = 0;

    constexpr int maxAttempts = 4;

    do {
        nDestLocal = 0;
        nAttemptsLocal++;


        struct
        {
            std::array<int, 2> indx;
            int tag;
        } loopStruct[6] =
        {
            {{0,1},0}, // send to xlo recv from xhi
            {{1,0},1}, // send to xhi recv from xlo
            {{2,3},2}, // send to ylo recv from yhi
            {{3,2},3}, // send to yhi recv from ylo
            {{4,5},4}, // send to zlo recv from zhi
            {{5,4},5}  // send to zhi recv from zlo
        };

        for (const auto& info : loopStruct)
        {
            int tgtIndx = info.indx[0];
            int srcIndx = info.indx[1];
            int tag = info.tag;

            nDestLocal += communicatorRef->sendParticles(dests[tgtIndx], sendVec);
            std::copy_n(sendVec.data(), sendVec.size(), sendBuff.data());
            comm_strategy->send(sendBuff.data(), bufferSize, dests[tgtIndx], tag);
            sendVec.clear();

            comm_strategy->recv(recvBuff.data(), bufferSize, dests[srcIndx], tag);
            communicatorRef->recvParticles(recvBuff);
            recvBuff.resize(bufferSize);

            comm_strategy->waitAll();
        }


        // MPI_Allreduce to get nDestsTotal
        comm_strategy->reduceAll(&nDestLocal, &nDestTotal, 1);
        comm_strategy->reduceAll(&nAttemptsLocal, &nAttemptsTotal, 1);


    } while (nDestTotal > 0 && nAttemptsTotal < maxAttempts);


    checking_communicator(
        rank, engine.get(),
        expectedX,
        expectedV,
        expectedF,
        expectedR,
        expectedM
    );
}


TEST_CASE("Testing the sendGhost function to transfer the interior particles", "[mpi]") {
    printRankZero("Testing the sendGhost function to transfer the interior particles");
    printRankZero(std::string(80, '='));

    std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();
    int rank = comm_strategy->getRank();
    int size = comm_strategy->getSize();

    // checking the min required number of ranks
    minRanksRequirement(comm_strategy, 8);
    // skipping the extra ranks
    comm_strategy = skipExtraRanks(comm_strategy, 8);


    double skin = 50.0;

    // x values
    // everthing here belongs to the rank3
    // so no need for ghost transfer.. just
    // wanted to check the interior particles (ghosts for other ranks) detection 

    int expectednParticles = 0;
    std::vector<double> newX = {}, newV = {}, newF = {}, newR = {}, newM = {};
    std::vector<double> expectedX = {}, expectedV = {}, expectedF = {}, expectedR = {}, expectedM = {};

    // region 4
    if (rank == 3) {
        expectednParticles = 5;
        // since the order of particles is not known in advance we
        // need to check X, V, ... separately
        expectedX = {
             -28.20,  180.66,  -66.03,   // particle 14 owned by Q01 (-+ interior) region 3 ghost for region 4
             -18.40,  +22.10,  -95.50,   // particle 8  owned by Q10 (-+ interior) region 3 ghost for region 4
             -40.11,  170.25, -204.67,   // particle 18 owned by Q01 (-+ interior) region 3 ghost for region 4
             -28.20,  150.66,  -36.03,   // particle 20 owneb by Q01 (-+ interior) region 3 ghost for region 4
             -18.15,   30.89,  -12.09    // particle 22 owned by Q01 (-+ interior) region 3 ghost for region 4
        };
        expectedV = {
           -53.7,  27.5, -41.6,   // particle 14
            59.7, -24.6,  10.9,   // particle 8
            57.8, -69.2,  39.4,   // particle 18
            31.5, -75.3,  25.6,   // particle 20
            66.2,  -8.1,  47.3    // particle 22
        };
        expectedF = {
           -25.7,  17.8, -38.6,   // particle 14
            47.3, -19.5,   6.2,   // particle 8
            28.6, -31.4,  19.3,   // particle 18
            17.8, -39.2,  19.4,   // particle 20
            21.5, -35.3,  15.6,   // particle 22
        };
        expectedR = {
            37.6,  // particle 14 region 3
            76.2,  // particle 8  region 3
            58.9,  // particle 18 region 3
            14.9,  // particle 20 region 3 
            22.6,  // particle 22 region 3
        };
        expectedM = {
            5.9,  // particle 14 region 3
            8.4,  // particle 8  region 3
            1.8,  // particle 18 region 3
           18.6,  // particle 20 region 3
           10.8,  // particle 22 region 3
        };
    }
    else if (rank == 0)
    {
        expectednParticles = 6;

        expectedX = {
            -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior) region 3 ghost for region 1
            -180.66,   30.77,  -74.95,   // particle 2  owned by Q00 (-+ interior) region 3 ghost for region 1
             -18.40,  +22.10,  -95.50,   // particle 8  owned by Q10 (-+ interior) region 3 ghost for region 1
            -150.33,   20.18,  -25.05,   // particle 21 owned by Q01 (-+ interior) region 3 ghost for region 1
             -18.15,   30.89,  -12.09,   // particle 22 owned by Q01 (-+ interior) region 3 ghost for region 1
            -190.99,   27.46, -150.00    // particle 23 owned by Q01 (-+ interior) region 3 ghost for region 1
        };
        expectedV = {
            46.1, -61.5,  33.8,   // particle 12
            41.2, -19.8,  63.5,   // particle 2
            59.7, -24.6,  10.9,   // particle 8
           -72.4,  18.7, -54.9,   // particle 21
            66.2,  -8.1,  47.3,   // particle 22
            21.7, -38.4,  68.2    // particle 23
        };
        expectedF = {
            41.2, -29.8,   9.7,   // particle 12
            29.7,  -5.4,  38.9,   // particle 2
            47.3, -19.5,   6.2,   // particle 8
             4.5,  24.3,  12.9,   // particle 21
            21.5, -35.3,  15.6,   // particle 22
            21.7, -38.4,  18.2,   // particle 23
        };
        expectedR = {
            19.8,  // particle 12 region 3
            45.7,  // particle 2  region 3
            76.2,  // particle 8  region 3
            69.1,  // particle 21 region 3
            22.6,  // particle 22 region 3
            12.5   // particle 23 region 3
        };

        expectedM = {
            3.0,  // particle 12 region 3
            0.9,  // particle 2  region 3
            8.4,  // particle 8  region 3
            3.1,  // particle 21 region 3
           10.8,  // particle 22 region 3
            8.5   // particle 23 region 3
        };
    }
    else if (rank == 6)
    {
        expectednParticles = 5;
        expectedX = {
            -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior) region 3 ghost for region 7
            -128.20,  130.66,  -46.03,   // particle 19 owneb by Q01 (-+ interior) region 3 ghost for region 7
             -28.20,  150.66,  -36.03,   // particle 20 owneb by Q01 (-+ interior) region 3 ghost for region 7
            -150.33,   20.18,  -25.05,   // particle 21 owned by Q01 (-+ interior) region 3 ghost for region 7
             -18.15,   30.89,  -12.09    // particle 22 owned by Q01 (-+ interior) region 3 ghost for region 7
        };
        expectedV = {
            21.7, -38.4,  68.2,   // particle 10
             4.5,  64.3,  12.9,   // particle 19
            31.5, -75.3,  25.6,   // particle 20
           -72.4,  18.7, -54.9,   // particle 21
            66.2,  -8.1,  47.3    // particle 22
        };
        expectedF = {
            14.8,  -7.3,  39.6,   // particle 10
            29.7, -24.6,  10.9,   // particle 19
            17.8, -39.2,  19.4,   // particle 20
             4.5,  24.3,  12.9,   // particle 21
            21.5, -35.3,  15.6,   // particle 22
        };
        expectedR = {
             8.7,  // particle 10 region 3
            57.8,  // particle 19 region 3 
            14.9,  // particle 20 region 3 
            69.1,  // particle 21 region 3
            22.6,  // particle 22 region 3 
        };
        expectedM = {
            0.5,  // particle 10 region 3
            7.2,  // particle 19 region 3
            18.6,  // particle 20 region 3
            3.1,  // particle 21 region 3
            10.8,  // particle 22 region 3
        };

    }
    else if (rank == 2) {

        newX = {
            -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior) region 3 ghost for region 7
            -270.22,  120.74, -210.46,   // particle 11 owned by Q01 (-+ interior) region 3
            -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior) region 3 ghost for region 1
             -28.20,  180.66,  -66.03,   // particle 14 owned by Q01 (-+ interior) region 3 ghost for region 4
            -180.66,   30.77,  -74.95,   // particle 2  owned by Q00 (-+ interior) region 3 ghost for region 1
             -18.40,  +22.10,  -95.50,   // particle 8  owned by Q10 (-+ interior) region 3 ghost for regions 1, 2, 4
             -40.11,  170.25, -204.67,   // particle 18 owned by Q01 (-+ interior) region 3 ghost for region 4
            -128.20,  130.66,  -46.03,   // particle 19 owneb by Q01 (-+ interior) region 3 ghost for region 7
             -28.20,  150.66,  -36.03,   // particle 20 owneb by Q01 (-+ interior) region 3 ghost for regions 4, 7, 8
            -150.33,   20.18,  -25.05,   // particle 21 owned by Q01 (-+ interior) region 3 ghost for regions 1, 5, 7
             -18.15,   30.89,  -12.09,   // particle 22 owned by Q01 (-+ interior) region 3 ghost for regions 1, 2, 4, 5, 6, 7, 8
            -190.99,   27.46, -150.00    // particle 23 owned by Q01 (-+ interior) region 3 ghost for region 1
        };
        // v values 
        newV = {
            21.7, -38.4,  68.2,   // particle 10
           -14.9,  87.6, -29.3,   // particle 11
            46.1, -61.5,  33.8,   // particle 12
           -53.7,  27.5, -41.6,   // particle 14
            41.2, -19.8,  63.5,   // particle 2
            59.7, -24.6,  10.9,   // particle 8
            57.8, -69.2,  39.4,   // particle 18
             4.5,  64.3,  12.9,   // particle 19
            31.5, -75.3,  25.6,   // particle 20
           -72.4,  18.7, -54.9,   // particle 21
            66.2,  -8.1,  47.3,   // particle 22
            21.7, -38.4,  68.2    // particle 23
        };
        // f values
        newF = {
            14.8,  -7.3,  39.6,   // particle 10
           -21.9,  30.4, -16.5,   // particle 11
            41.2, -29.8,   9.7,   // particle 12
           -25.7,  17.8, -38.6,   // particle 14
            29.7,  -5.4,  38.9,   // particle 2
            47.3, -19.5,   6.2,   // particle 8
            28.6, -31.4,  19.3,   // particle 18
            29.7, -24.6,  10.9,   // particle 19
            17.8, -39.2,  19.4,   // particle 20
             4.5,  24.3,  12.9,   // particle 21
            21.5, -35.3,  15.6,   // particle 22
            21.7, -38.4,  18.2,   // particle 23
        };
        // r values
        newR = {
          8.7,  // particle 10 region 3
         63.5,  // particle 11 region 3
         19.8,  // particle 12 region 3
         37.6,  // particle 14 region 3
         45.7,  // particle 2  region 3
         76.2,  // particle 8  region 3
         58.9,  // particle 18 region 3
         57.8,  // particle 19 region 3 
         14.9,  // particle 20 region 3 
         69.1,  // particle 21 region 3
         22.6,  // particle 22 region 3
         12.5   // particle 23 region 3
        };
        // m values
         newM = {
            0.5,  // particle 10 region 3
            9.8,  // particle 11 region 3
            3.0,  // particle 12 region 3
            5.9,  // particle 14 region 3
            0.9,  // particle 2  region 3
            8.4,  // particle 8  region 3
            1.8,  // particle 18 region 3
            7.2,  // particle 19 region 3
           18.6,  // particle 20 region 3
            3.1,  // particle 21 region 3
           10.8,  // particle 22 region 3
            8.5   // particle 23 region 3
        };
    }




    // rank configuration
    std::array<int, 3> ranks = { 2,2,2 };


    // transfered data
    std::vector<double> sendBuff;
    std::vector<double> recvBuff;

    // the maximum number of particles
    const int dataPerParticle = Particles::dataPerParticle;
    const int nmax = 50;
    const int bufferSize = nmax * dataPerParticle;

    sendBuff.resize(bufferSize);
    recvBuff.resize(bufferSize);

    // sending the interior particles
    std::vector<double> sendData[8];

    // building the engine just for the rank == 2
     auto engine_ptr = set_particles_build_engine(
            rank, ranks, newX, newV, newF, newR, newM, skin);
    auto* communicator_ptr = engine_ptr->getCommunicator().get();


    if (rank == 2) {
        // sending to the rank4
        // xhi
        communicator_ptr->sendGhosts(3, sendData[3]);
        // sending to the rank1
        // ylo
        communicator_ptr->sendGhosts(0, sendData[0]);
        // sending to the rank7
        // zhi
        communicator_ptr->sendGhosts(6, sendData[6]);
    }

    // sending from rank 2 to rank 3
    if (rank == 2) {
        std::copy_n(sendData[3].data(), sendData[3].size(), sendBuff.data());
        comm_strategy->send(sendBuff.data(), bufferSize, 3, 3);
    }
    if (rank == 3) {
        comm_strategy->recv(recvBuff.data(), bufferSize, 2, 3);
    }
    comm_strategy->waitAll();
    
    // sending from rank 2 to rank 0
    if (rank == 2)
    {
        std::copy_n(sendData[0].data(), sendData[0].size(), sendBuff.data());
        comm_strategy->send(sendBuff.data(), bufferSize, 0, 0);
    }
    else if (rank == 0)
    {
        comm_strategy->recv(recvBuff.data(), bufferSize, 2, 0);
    }
    comm_strategy->waitAll();

    // sending from rank 2 to rank 6
    if (rank == 2)
    {
        std::copy_n(sendData[6].data(), sendData[6].size(), sendBuff.data());
        comm_strategy->send(sendBuff.data(), bufferSize, 6, 6);
    }
    else if (rank == 6)
    {
        comm_strategy->recv(recvBuff.data(), bufferSize, 2, 6);
    }
    comm_strategy->waitAll();


    // checking the message 
    // at this test the message is just sent to the immediate neighboring ranks
    // 1, 4, 7
    // It is the job of neighboring ranks to resend the ghost partilces
    // to the non-immediate neighboring ranks of 2,5,6,8
    
    int nParticles = static_cast<int>(recvBuff[0]);


    // checking the number of particles
    REQUIRE(nParticles == expectednParticles);


    std::vector<double> X, V, F, R, M;
    X.reserve(3 * nParticles); V.reserve(3 * nParticles); F.reserve(3 * nParticles);
    R.reserve(nParticles); M.reserve(nParticles);


    int dataLoc = 1;
    for (int i = 0; i < nParticles; i++)
    {
        // the id
        dataLoc++;
        X.push_back(recvBuff[dataLoc++]);
        X.push_back(recvBuff[dataLoc++]);
        X.push_back(recvBuff[dataLoc++]);
        V.push_back(recvBuff[dataLoc++]);
        V.push_back(recvBuff[dataLoc++]);
        V.push_back(recvBuff[dataLoc++]);
        F.push_back(recvBuff[dataLoc++]);
        F.push_back(recvBuff[dataLoc++]);
        F.push_back(recvBuff[dataLoc++]);
        M.push_back(recvBuff[dataLoc++]);
        R.push_back(recvBuff[dataLoc++]);
    }


    // checking the results
    REQUIRE_THAT(X.data(), Array3DMatcher(expectedX.data(), nParticles, 1e-6));
    REQUIRE_THAT(V.data(), Array3DMatcher(expectedV.data(), nParticles, 1e-6));
    REQUIRE_THAT(F.data(), Array3DMatcher(expectedF.data(), nParticles, 1e-6));
    REQUIRE_THAT(R.data(), Array1DMatcher(expectedR.data(), nParticles, 1e-6));
    REQUIRE_THAT(M.data(), Array1DMatcher(expectedM.data(), nParticles, 1e-6));
}



TEST_CASE("Testing the recvGhost function to receive the ghost particles","[mpi]")
{
    printRankZero("Testing the recvGhost function to receive the ghost particles");
    printRankZero(std::string(80, '='));

    std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();
    int rank = comm_strategy->getRank();
    int size = comm_strategy->getSize();

    // checking the min required number of ranks
    minRanksRequirement(comm_strategy, 27);
    // skipping the extra ranks
    comm_strategy = skipExtraRanks(comm_strategy, 27);

    // the particles should be checked if they are not among the 
    // xlo, ylo, ... of the destination rank they should be added otherwise
    // they should be updated

    /// 
    /// 
    /// 
    /// Important
    /// 
    /// In sending ghost in the oriental direction 
    /// each particle receives its ghost particles
    /// then based on some criteria which I am not sure at the moment
    /// it decides if it is the ghost for its neighbor or not
    /// For this to work each rank should check if the received ghost is not
    /// its own local particles
    /// 
    /// Important2
    /// There should be a test to see whether a particle
    /// is no longer a ghost in this rank
    /// So I would suggest those ghost particles not updated
    /// be removed 
    /// 
    ///
    std::array<int, 3> ranks = { 3,3,3 };

    // the skin parameter
    double skin = 30.0;

    int nlocal = 0, nghost = 0;
    std::vector<int> newId;
    std::vector<double> newX;

    // expected results
    int expectedNLocal = 0;
    int expectedNGhost = 0;

    std::vector<int> expectedIds;
    std::vector<double> expectedX, expectedV, expectedF, expectedM, expectedR;


    // rank 13
    if (rank == 13) {
        nlocal = 3;
        nghost = 3;
        newId = {
            1, 25, 26, 5, 6, 7
        };

        newX = {
            // ---- local particles (inside domain) ----
             42.7, -68.78,  64.20,  // particle 1  (local)
            -68.3,  49.09, -33.10,  // particle 25  (local)
            -91.4, -55.55,  98.09,  // particle 26  (local)

            // ---- ghost particles (>= 30 outside [-100,100]) ----
            124.6,   84.08,  63.02,  // particle 5  (ghost)
           -127.2,   46.05, -78.01,  // particle 6  (ghost)
            112.9, -105.05,  12.05   // particle 7  (ghost)
        };


        // expected results
        expectedNLocal = 3;
        expectedNGhost = 17;
        expectedIds = {
            1,25,26,
            5,6,7,3,12,8,4,6,11,14,13,15,18,16,17,19,21,22
        };
        expectedX = {
            // ---- local particles (inside domain) ----
             42.7, -68.78,  64.20,    // particle 1   (local)
            -68.3,  49.09, -33.10,    // particle 25  (local)
            -91.4, -55.55,  98.09,    // particle 26  (local)

            // ---- ghost particles (>= 30 outside [-100,100]) ----
            // original ghosts (5 and 6 are updated)
           -120.4, -104.10,  116.7,   // Particle 5 (ghost)
           -111.6,  -122.8,  118.5,   // Particle 6 (ghost)
           -113.5,  109.20, -125.1,   // particle 7 (ghost)
           // received ghosts
            118.4, -121.70,  102.6,   // Particle 3  (ghost)
            126.8,  111.20, -118.3,   // Particle 12 (ghost)
            126.1, -117.60,  128.3,   // Particle 8  (ghost)
            121.9,   127.3, -120.7,   // Particle 4  (ghost)
            119.0,  -126.4,  109.8,   // Particle 11 (ghost)
           -121.7,   110.8,  125.9,   // Particle 14 (ghost)
            106.3,  -119.4,  124.5,   // Particle 13 (ghost)
           -126.8,   122.1, -110.4,   // Particle 15 (ghost)
            119.5,  -125.7,  127.6,   // Particle 18 (ghost) 
           -129.5,   118.9,  102.4,   // Particle 16 (ghost)
            125.7,  -103.3, -121.6,   // Particle 17 (ghost)
           -118.6,   129.4, -124.7,   // Particle 19 (ghost)
            111.8,  -124.6,  126.2,   // Particle 21 (ghost)
           -107.5,  121.3,  -129.7,   // Particle 22 (ghost)
        };

        expectedV = {
            0.0,  0.0,  0.0, // particle 1  (local)
            0.0,  0.0,  0.0, // particle 25 (local)
            0.0,  0.0,  0.0, // particle 26 (local)

            // -------- ghost particles -----------
            // original ghosts 
           11.3,   62.5, -37.9, // particle 5  (ghost)
            74.1, -66.3,  39.7, // Particle 6  (ghost)
           -66.1,  43.7, -92.8, // particle 7  (ghost)

            54.3, -87.2,  11.5, // Particle 3  (ghost)
            72.9, -11.6,  48.4, // Particle 12 (ghost)
           -93.4,  25.7,  61.2, // Particle 8  (ghost)
           -55.8,  89.2, -13.4, // Particle 4  (ghost)
            17.6,  58.4, -91.1, // Particle 11 (ghost)
           -71.5,  63.1,  29.4, // Particle 14 (ghost)
            47.2, -22.9,  93.8, // Particle 13 (ghost)
           -83.7,  51.3, -14.6, // Particle 15 (ghost)
            68.4, -73.2,  33.5, // Particle 18 (ghost)
           -14.8,  72.6, -55.1, // Particle 16 (ghost)
            83.5, -48.9,  61.7, // Particle 17 (ghost)
           -67.3,  38.5, -90.2, // Particle 19 (ghost)
            45.2, -72.1,  18.9, // Particle 21 (ghost)
           -52.6,  14.8,  83.1, // Particle 22 (ghost)
        };


        expectedF = {
            0.0,  0.0,  0.0, // particle 1  (local)
            0.0,  0.0,  0.0, // particle 25 (local)
            0.0,  0.0,  0.0, // particle 26 (local)

            // -------- ghost particles -----------
            // original ghosts 
           -12.7,   28.4,  19.6, // particle 5  (ghost)
            -21.8,  26.2, -37.5, // Particle 6  (ghost)
            19.6, -31.4,  27.5,  // particle 7  (ghost)

           -23.4,   14.8,  -9.2, // Particle 3  (ghost)
           -45.7,    3.1, -18.9, // Particle 12 (ghost)
            41.6,  -22.8,   7.3, // Particle 8  (ghost)
            32.5,  -10.7,  17.9, // Particle 4  (ghost)
             11.2, -45.6,   4.3, // Particle 11 (ghost)
            -16.9, -34.2,  12.5, // Particle 14 (ghost)
            -42.1,  15.4, -27.6, // Particle 13 (ghost)
             25.8, -39.5,  18.2, // Particle 15 (ghost)
            -11.4,  44.8,  -6.7, // Particle 18 (ghost)
            -19.7,  36.4,  21.3, // Particle 16 (ghost)
             14.6, -23.8, -32.1, // Particle 17 (ghost)
            -35.4,  11.9,  42.7, // Particle 19 (ghost)
             -7.4,  33.6, -28.5, // Particle 21 (ghost)
             29.7, -41.2,   6.5, // Particle 22 (ghost)
        };

        expectedM = {
            0.0,   // Particle 1
            0.0,   // Particle 25
            0.0,   // Particle 26

            // --- ghost particles ------------
            17.2,  // Particle  5
            15.8,  // Particle  6
             4.8,  // Particle  7


            12.6,  // Particle  3
            10.4,  // Particle 12
             6.4,  // Particle  8
             3.7,  // Particle  4
             7.9,  // Particle 11
             8.2,  // Particle 14
            12.7,  // Particle 13
             5.9,  // Particle 15
            16.1,  // Particle 18
             9.8,  // Particle 16
             4.1,  // Particle 17
            13.6,  // Particle 19
            11.7,  // Particle 21
             5.3   // Particle 22
        };

        expectedR = {
              0.0,   // Particle 1
              0.0,   // Particle 25
              0.0,   // Particle 26

              // --- ghost particles ------------
               11.5,   // Particle  5
                2.4,   // Particle  6
               16.2,   // Particle  7

                7.3,   // Particle 3
               22.7,   // Particle 12
                5.1,   // Particle 8
                9.6,   // Particle 4
               18.5,   // Particle 11
               27.4,   // Particle 14
                6.3,   // Particle 13
               21.8,   // Particle 15
                3.7,   // Particle 18
               17.5,   // Particle 16
                8.2,   // Particle 17
               26.9,   // Particle 19
                 9.4,  // Particle 21
                21.6,  // Particle 22
        };
    }

    std::vector<double> newV(newX.size(), 0.0);
    std::vector<double> newF(newX.size(), 0.0);
    std::vector<double> newR(newX.size() / 3, 0.0);
    std::vector<double> newM(newX.size() / 3, 0.0);


    // transfered data
    std::vector<double> sendVec;
    std::vector<double> sendBuff;
    std::vector<double> recvBuff;

    // the maximum number of particles
    const int dataPerParticle = Particles::dataPerParticle;
    const int nmax = 50;
    const int bufferSize = nmax * dataPerParticle;

    sendBuff.resize(bufferSize);
    recvBuff.resize(bufferSize);

    if (rank == 12)
    {
        // xlo 
        // rank 12
        sendVec = {
            3.0,  // number of particles

            3.0,  // particle id
            118.4, -121.7, 102.6,   // x, y, z
             54.3,  -87.2,  11.5,   // vx, vy, vz
            -23.4,   14.8,  -9.2,   // fx, fy, fz
             12.6,                 // mass
              7.3,                 // radius

            7.0,  // particle id
           -113.5, 109.2, -125.1,   // x, y, z
            -66.1,  43.7, -92.8,    // vx, vy, vz
             19.6, -31.4,  27.5,    // fx, fy, fz
              4.8,                 // mass
             16.2,                 // radius

            12.0, // particle id
            126.8, 111.2, -118.3,  // x, y, z
             72.9, -11.6,  48.4,   // vx, vy, vz
            -45.7,   3.1, -18.9,   // fx, fy, fz
             10.4,                // mass
             22.7,                // radius
        };
    }
    else if (rank == 14) {
        sendVec = {
            2.0,

            5.0,
           -120.4, -104.1, 116.7,   // x, y, z
             11.3,   62.5, -37.9,   // vx, vy, vz
            -12.7,   28.4,  19.6,   // fx, fy, fz
             17.2,
             11.5,

            8.0,
            126.1, -117.6, 128.3,   // x, y, z
            -93.4,   25.7,  61.2,   // vx, vy, vz
             41.6,  -22.8,   7.3,   // fx, fy, fz
              6.4,
              5.1,

        };
    }
    else if (rank == 10) {
        sendVec = {
            3.0,

            4.0,
            121.9, 127.3, -120.7,  // x, y, z
            -55.8,  89.2, -13.4,   // vx, vy, vz
             32.5, -10.7,  17.9,   // fx, fy, fz
              3.7,
              9.6,

            6.0,
           -111.6, -122.8, 118.5,  // x, y, z
             74.1, -66.3,  39.7,   // vx, vy, vz
            -21.8,  26.2, -37.5,   // fx, fy, fz
             15.8,
              2.4,

            11.0,
            119.0, -126.4, 109.8,  // x, y, z
             17.6,  58.4, -91.1,   // vx, vy, vz
             11.2, -45.6,   4.3,   // fx, fy, fz
              7.9,
             18.5,
        };
    }
    else if (rank == 16) {
        sendVec = {
            1.0,


            14.0,
           -121.7, 110.8, 125.9,  // x, y, z
            -71.5,  63.1,  29.4,  // vx, vy, vz
            -16.9, -34.2,  12.5,  // fx, fy, fz
              8.2,
             27.4,
        };
    }
    else if (rank == 4) {
        sendVec = {
            3.0,

            13.0,
            106.3, -119.4, 124.5,  // x, y, z
             47.2, -22.9,  93.8,   // vx, vy, vz
            -42.1,  15.4, -27.6,   // fx, fy, fz
             12.7,
              6.3,

            15.0,
           -126.8, 122.1, -110.4,  // x, y, z
            -83.7,  51.3, -14.6,   // vx, vy, vz
             25.8, -39.5,  18.2,   // fx, fy, fz
              5.9,
             21.8,

            18.0,
            119.5, -125.7, 127.6,  // x, y, z
             68.4, -73.2,  33.5,   // vx, vy, vz
            -11.4,  44.8,  -6.7,   // fx, fy, fz
             16.1,
              3.7,
        };
    }
    else if (rank == 22) {
        sendVec = {
            5.0,  // number of particles

            16.0,
           -129.5, 118.9, 102.4,  // x, y, z
            -14.8,  72.6, -55.1,  // vx, vy, vz
            -19.7,  36.4,  21.3,  // fx, fy, fz
              9.8,
             17.5,

            17.0,
            125.7, -103.3, -121.6,  // x, y, z
             83.5, -48.9,  61.7,    // vx, vy, vz
             14.6, -23.8, -32.1,    // fx, fy, fz
              4.1,
              8.2,

            19.0,
           -118.6, 129.4, -124.7,  // x, y, z
            -67.3,  38.5, -90.2,   // vx, vy, vz
            -35.4,  11.9,  42.7,   // fx, fy, fz
             13.6,
             26.9,

             21.0,
             111.8, -124.6, 126.2,  // x, y, z
              45.2, -72.1,  18.9,   // vx, vy, vz
              -7.4,  33.6, -28.5,   // fx, fy, fz
              11.7,
               9.4,

             22.0,
            -107.5, 121.3, -129.7,  // x, y, z
             -52.6,  14.8,  83.1,   // vx, vy, vz
              29.7, -41.2,   6.5,   // fx, fy, fz
               5.3,
             21.6,
        };
    }


    // building the engine
    auto engine = set_particles_build_engine(rank, ranks, newId, newX, newV, newF, newR, newM, skin);
    // returning the communicatorRef
    auto& communicatorRef = engine->getCommunicator();
    
    
    struct {
        int tag, src, dst;
    } loopArgs[6] = {
        {12,12,13},
        {14,14,13},
        {10,10,13},
        {16,16,13},
        {4,4,13},
        {22,22,13}
    };

    // sending data
    for (const auto& arg : loopArgs)
    {
        if (rank == arg.src)
        {
            std::copy_n(sendVec.data(), sendVec.size(), sendBuff.data());
            comm_strategy->send(sendBuff.data(), sendBuff.size(), arg.dst, arg.tag);
        }
        if (rank == arg.dst)
        {
            comm_strategy->recv(recvBuff.data(), recvBuff.size(), arg.src, arg.tag);
            communicatorRef->recvGhosts(recvBuff);
            recvBuff.resize(bufferSize);
        }
        comm_strategy->waitAll();
    }

    
    // getting the particles
    auto& particlesRef = engine->getParticles();
    // checking the particles Ref
    REQUIRE(particlesRef);
    // checking the results
    int* mId = particlesRef->getIdData();
    auto* myX = particlesRef->getXData();
    auto* myV = particlesRef->getVData();
    auto* myF = particlesRef->getFData();
    auto* myM = particlesRef->getMData();
    auto* myR = particlesRef->getRData();



    //REQUIRE_THAT(mId, Array1DMatcherInt(expectedIds.data(), expectedNLocal, expectedNGhost));
    REQUIRE_THAT(myX, Array3DMatcher(expectedX.data(), expectedNLocal, expectedNGhost, 1e-6));
    REQUIRE_THAT(myV, Array3DMatcher(expectedV.data(), expectedNLocal, expectedNGhost, 1e-6));
    REQUIRE_THAT(myF, Array3DMatcher(expectedF.data(), expectedNLocal, expectedNGhost, 1e-6));
    REQUIRE_THAT(myM, Array1DMatcher(expectedM.data(), expectedNLocal, expectedNGhost, 1e-6));
    REQUIRE_THAT(myR, Array1DMatcher(expectedR.data(), expectedNLocal, expectedNGhost, 1e-6));

}


TEST_CASE("Testing the movement of particles for the case with the skin value of 50","[mpi]")
{
    printRankZero("Testing the movement of particles for the case with the skin value of 50");
    printRankZero(std::string(80, '='));

    std::unique_ptr<Comm_strategy> comm_strategy = std::make_unique<MPI_comm_strategy>();
    int rank = comm_strategy->getRank();
    int size = comm_strategy->getSize();

    // checking the min required number of ranks
    minRanksRequirement(comm_strategy, 4);
    // skipping the extra ranks
    comm_strategy = skipExtraRanks(comm_strategy, 4);

    double skin = 50.0;

    int newNGhost = 0;
    std::vector<int> newId;
    std::vector<double> newX, newV, newF, newM, newR;

    int expectedNGhost = 0;
    std::vector<int> expectedId;
    std::vector<double> expectedX, expectedV, expectedF, expectedM, expectedR;

    if (rank == 0)
    {
        // id values
        newId = {
            0,1,2,3,4
        };
        //  x values
        newX =
        {
            -220.40, -180.10,   80.55,   // particle 0  (-- interior) region 1
            -140.75, -260.33, -120.18,   // particle 1  (-- interior) region 1
            -180.66,   30.77,   14.95,   // particle 2  (-+ interior) region 3
              12.60,  -18.90,  130.10,   // particle 3  (+- interior) region 2
              35.10, -140.48,   12.66,   // particle 4  (+- interior) region 2
        };

        newV =
        {
            72.4, -33.1,  95.0,   // particle 0
           -88.6,  14.3, -56.7,   // particle 1
            41.2, -19.8,  63.5,   // particle 2
           -97.1,  28.9, -74.4,   // particle 3
             3.6,  89.2, -12.5,   // particle 4
        };

        // force values
        newF =
        {
           12.4, -33.9,  45.2,   // particle 0
          -18.6,   7.3, -41.5,   // particle 1
           29.7,  -5.4,  38.9,   // particle 2
          -22.1,  16.0, -49.7,   // particle 3
            3.1,  27.6, -14.8,   // particle 4
        };

        // radius values
        newR =
        {
         12.5,  // particle 0 region 1
         89.3,  // particle 1 region 1
         45.7,  // particle 2 region 3
         3.9,   // particle 3 region 2
         67.1,  // particle 4 region 2
        };

        // mass values
        newM =
        {
            3.4,  // particle 0 region 1
            7.8,  // particle 1 region 1
            0.9,  // particle 2 region 3
            5.6,  // particle 3 region 2
            9.1,  // particle 4 region 2
        };

        newNGhost = 0;

        // expected id values
        expectedId = {
            0,1,7,17,
            3,4,12,2,8,13
        };

        // expected x values
        expectedX = {
            -220.40, -180.10,   80.55,   // particle 0  (-- interior) region 1
            -140.75, -260.33, -120.18,   // particle 1  (-- interior) region 1
             -25.60, -160.15,  -55.27,   // particle 7  (-- interior) region 1 ghost for region 2
             -30.25,  -22.40,   10.66,   // particle 17 (-- interior) region 1 ghost for regions 2, 3, 4

             // ghosts 
             12.60,  -18.90,  130.10,   // particle 3  (+- interior) region 2
             35.10, -140.48,   12.66,   // particle 4  (+- interior) region 2
             -120.33,   20.18, -199.05,   // particle 12 (-+ interior) region 3
             -180.66,   30.77,   14.95,   // particle 2  (-+ interior) region 3
             -18.40,  +22.10,   95.50,   // particle 8  (-+ interior) region 3
             22.80,   18.10, -140.25,   // particle 13 (++ interior) region 4
        };

        // expected velocity values
        expectedV = {
            72.4, -33.1,  95.0,   // particle 0
           -88.6,  14.3, -56.7,   // particle 1
           -11.4,  36.8, -83.2,   // particle 7
            81.9, -35.0,  24.1,   // particle 17

           -97.1,  28.9, -74.4,   // particle 3
             3.6,  89.2, -12.5,   // particle 4
            46.1, -61.5,  33.8,   // particle 12
            41.2, -19.8,  63.5,   // particle 2
            59.7, -24.6,  10.9,   // particle 8
           -79.2,  12.4,  99.1,   // particle 13
        };

        // expected force values
        expectedF = {
           12.4, -33.9,  45.2,   // particle 0
          -18.6,   7.3, -41.5,   // particle 1
          -12.7,  18.9, -28.4,   // particle 7
           35.0, -18.1,  13.7,   // particle 17

          -22.1,  16.0, -49.7,   // particle 3
            3.1,  27.6, -14.8,   // particle 4
           41.2, -29.8,   9.7,   // particle 12
           29.7,  -5.4,  38.9,   // particle 2
           47.3, -19.5,   6.2,   // particle 8
          -48.1,  12.6,  33.9,   // particle 13
        };


        // expected radius values
        expectedR = {
            12.5,  // particle 0  region 1
            89.3,  // particle 1  region 1
            31.4,  // particle 7  region 1
            83.1,  // particle 17 region 1

            3.9,   // particle 3 region 2
            67.1,  // particle 4  region 2
            19.8,  // particle 12 region 3
            45.7,  // particle 2  region 3
            76.2,  // particle 8  region 3
            92.4,  // particle 13 region 4
        };

        // expected mass values
        expectedM = {
            3.4,  // particle 0  region 1
            7.8,  // particle 1  region 1
            1.3,  // particle 7  region 1
            8.2,  // particle 17 region 1


            5.6,  // particle 3 region 2
            9.1,  // particle 4  region 2
            3.0,  // particle 12 region 3
            0.9,  // particle 2  region 3
            8.4,  // particle 8  region 3
            7.4,  // particle 13 region 4
        };

        expectedNGhost = 6;

    }
    else if (rank == 1)
    {
        newId = {
            5,6,9,7,8
        };

        newX =
        {
             180.62, -210.44,   60.91,   // particle 5  (+- interior) region 2
             260.11, -140.88, -200.30,   // particle 6  (+- interior) region 2
             190.27,  -40.22, -236.70,   // particle 9  (+- interior) region 2
             -25.60, -160.15,  -55.27,   // particle 7  (-- interior) region 1
             -18.40,  +22.10,   95.50,   // particle 8  (-+ interior) region 3
        };

        newV =
        {
            54.7, -66.3,  77.1,   // particle 5
           -45.9,  18.0,  92.6,   // particle 6
           -72.5,  44.3, -95.8,   // particle 9
           -11.4,  36.8, -83.2,   // particle 7
            59.7, -24.6,  10.9,   // particle 8
        };

        newF =
        {
            44.5, -36.7,  21.4,   // particle 5
            -9.9,   5.6,  31.2,   // particle 6
           -34.8,  25.1, -45.6,   // particle 9
           -12.7,  18.9, -28.4,   // particle 7
            47.3, -19.5,   6.2,   // particle 8
        };

        newR =
        {
            24.6,  // particle 5 region 2
            98.0,  // particle 6 region 2
            54.3,  // particle 9 region 2
            31.4,  // particle 7 region 1
            76.2,  // particle 8 region 3
        };

        newM =
        {
            2.7,  // particle 5 region 2
            6.0,  // particle 6 region 2
            4.1,  // particle 9 region 2
            1.3,  // particle 7 region 1
            8.4,  // particle 8 region 3
        };

        newNGhost = 0;

        expectedId = {
            5,6,9,3,4,19,
            7,17,8,13
        };


        expectedX = {
            180.62, -210.44,   60.91,   // particle 5  (+- interior) region 2
            260.11, -140.88, -200.30,   // particle 6  (+- interior) region 2
            190.27,  -40.22, -236.70,   // particle 9  (+- interior) region 2 ghost for region 4
            12.60,  -18.90,  130.10,   // particle 3  (+- interior) region 2 ghost for regions 1, 3, 4
            35.10, -140.48,   12.66,   // particle 4  (+- interior) region 2 ghost for region 1
            160.81,  -35.42,  289.73,   // particle 19 (+- interior) region 2 ghost for region 4
            
            // ghosts
            -25.60, -160.15,  -55.27,   // particle 7  (-- interior) region 1
            -30.25,  -22.40,   10.66,   // particle 17 (-- interior) region 1
            -18.40,  +22.10,   95.50,   // particle 8  (-+ interior) region 3
            22.80,   18.10, -140.25,   // particle 13 (++ interior) region 4
        };

        expectedV = {
            54.7, -66.3,  77.1,   // particle 5
           -45.9,  18.0,  92.6,   // particle 6
           -72.5,  44.3, -95.8,   // particle 9
           -97.1,  28.9, -74.4,   // particle 3
             3.6,  89.2, -12.5,   // particle 4
           -16.7,  73.3, -58.9,   // particle 19


           -11.4,  36.8, -83.2,   // particle 7
            81.9, -35.0,  24.1,   // particle 17
            59.7, -24.6,  10.9,   // particle 8
           -79.2,  12.4,  99.1,   // particle 13
        };

        expectedF = {
            44.5, -36.7,  21.4,   // particle 5
            -9.9,   5.6,  31.2,   // particle 6
           -34.8,  25.1, -45.6,   // particle 9
           -22.1,  16.0, -49.7,   // particle 3
             3.1,  27.6, -14.8,   // particle 4
            -8.2,  46.7, -24.5,   // particle 19


           -12.7,  18.9, -28.4,   // particle 7
            35.0, -18.1,  13.7,   // particle 17
            47.3, -19.5,   6.2,   // particle 8
           -48.1,  12.6,  33.9,   // particle 13
        };
        
        expectedR = {
            24.6,  // particle 5 region 2
            98.0,  // particle 6 region 2
            54.3,  // particle 9 region 2
             3.9,  // particle 3 region 2
            67.1,  // particle 4 region 2
            26.4,  // particle 19 region 2


            31.4,  // particle 7  region 1
            83.1,  // particle 17 region 1
            76.2,  // particle 8  region 3
            92.4,  // particle 13 region 4
        };

        expectedM = {
            2.7,  // particle 5 region 2
            6.0,  // particle 6 region 2
            4.1,  // particle 9 region 2
            5.6,  // particle 3 region 2
            9.1,  // particle 4 region 2
            4.6,  // particle 19 region 2


            1.3,  // particle 7  region 1
            8.2,  // particle 17 region 1
            8.4,  // particle 8  region 3
            7.4,  // particle 13 region 4
        };

        expectedNGhost = 4;

    }
    else if (rank == 2)
    {
        newId = {
            10,11,12,13,14
        };

        newX =
        {
            -200.90,  190.36,  -40.58,   // particle 10 (-+ interior) region 3
            -270.22,  120.74,  210.46,   // particle 11 (-+ interior) region 3
            -120.33,   20.18, -199.05,   // particle 12 (-+ interior) region 3
              22.80,   18.10, -140.25,   // particle 13 (++ interior) region 4
             -28.20,  180.66,   66.03,   // particle 14 (-+ interior) region 3
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
        
        newR =
        {
            8.7,   // particle 10 region 3
            63.5,  // particle 11 region 3
            19.8,  // particle 12 region 3
            92.4,  // particle 13 region 4
            37.6,  // particle 14 region 3
        };

        newM =
        {
            0.5,  // particle 10 region 3
            9.8,  // particle 11 region 3
            3.0,  // particle 12 region 3
            7.4,  // particle 13 region 4
            5.9,  // particle 14 region 3
        };

        


        newNGhost = 0;


        expectedId = {
            10,11,12,14,2,8,18,
            17,3,13
        };

        expectedX = {
            -200.90,  190.36,  -40.58,   // particle 10 (-+ interior) region 3
            -270.22,  120.74,  210.46,   // particle 11 (-+ interior) region 3
            -120.33,   20.18, -199.05,   // particle 12 (-+ interior) region 3 ghost for region 1
            -28.20,  180.66,   66.03,   // particle 14 (-+ interior) region 3 ghost for region 4
            -180.66,   30.77,   14.95,   // particle 2  (-+ interior) region 3 ghost for region 1
            -18.40,  +22.10,   95.50,   // particle 8  (-+ interior) region 3 ghost for regions 1, 2, 4
            -40.11,  170.25,  204.67,   // particle 18 (-+ interior) region 3 ghost for region 4

            // ghosts
            -30.25,  -22.40,   10.66,   // particle 17 (-- interior) region 1
            12.60,  -18.90,  130.10,   // particle 3  (+- interior) region 2
            22.80,   18.10, -140.25,   // particle 13 (++ interior) region 4
        };

        expectedV = {
            21.7, -38.4,  68.2,   // particle 10
           -14.9,  87.6, -29.3,   // particle 11
            46.1, -61.5,  33.8,   // particle 12
           -53.7,  27.5, -41.6,   // particle 14
            41.2, -19.8,  63.5,   // particle 2
            59.7, -24.6,  10.9,   // particle 8
            57.8, -69.2,  39.4,   // particle 18


            81.9, -35.0,  24.1,   // particle 17
           -97.1,  28.9, -74.4,   // particle 3
           -79.2,  12.4,  99.1,   // particle 13
        };


        expectedF = {
            14.8,  -7.3,  39.6,   // particle 10
           -21.9,  30.4, -16.5,   // particle 11
            41.2, -29.8,   9.7,   // particle 12
           -25.7,  17.8, -38.6,   // particle 14
            29.7,  -5.4,  38.9,   // particle 2
            47.3, -19.5,   6.2,   // particle 8
            28.6, -31.4,  19.3,   // particle 18


            35.0, -18.1,  13.7,   // particle 17
           -22.1,  16.0, -49.7,   // particle 3
           -48.1,  12.6,  33.9,   // particle 13
        };
        
        
        expectedR = {
            8.7,  // particle 10 region 3
            63.5,  // particle 11 region 3
            19.8,  // particle 12 region 3
            37.6,  // particle 14 region 3
            45.7,  // particle 2  region 3
            76.2,  // particle 8  region 3
            58.9,  // particle 18 region 3


            83.1,  // particle 17 region 1
            3.9,  // particle 3  region 2
            92.4,  // particle 13 region 4
        };

        expectedM = {
            0.5,  // particle 10 region 3
            9.8,  // particle 11 region 3
            3.0,  // particle 12 region 3
            5.9,  // particle 14 region 3
            0.9,  // particle 2  region 3
            8.4,  // particle 8  region 3
            1.8,  // particle 18 region 3


            8.2,  // particle 17 region 1
            5.6,  // particle 3  region 2
            7.4,  // particle 13 region 4
        };


        expectedNGhost = 3;

    }
    else if (rank == 3)
    {
        newId = {
            15,16,17,18,19
        };

        newX =
        {
             210.83,  220.41, -150.88,   // particle 15 (++ interior) region 4
             140.62,  160.33,  198.21,   // particle 16 (++ interior) region 4
             -30.25,  -22.40,   10.66,   // particle 17 (-- interior) region 1
             -40.11,  170.25,  204.67,   // particle 18 (-+ interior) region 3
             160.81,  -35.42,  289.73,   // particle 19 (+- interior) region 2
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

        newR =
        {
            71.0,  // particle 15 region 4
            14.2,  // particle 16 region 4
            83.1,  // particle 17 region 1
            58.9,  // particle 18 region 3
            26.4   // particle 19 region 2
        };

        newM =
        {
            2.1,  // particle 15 region 4
            6.7,  // particle 16 region 4
            8.2,  // particle 17 region 1
            1.8,  // particle 18 region 3
            4.6   // particle 19 region 2
        };

        newNGhost = 0;


        expectedId = {
            15,16,13,
            17,9,3,14,8,18,19
        };

        expectedX = {
            210.83,  220.41, -150.88,   // particle 15 (++ interior) region 4 
            140.62,  160.33,  198.21,   // particle 16 (++ interior) region 4
            22.80,   18.10, -140.25,   // particle 13 (++ interior) region 4 ghost for regions 1, 2, 3
            
            
            -30.25,  -22.40,   10.66,   // particle 17 (-- interior) region 1
            190.27,  -40.22, -236.70,   // particle 9  (+- interior) region 2
            12.60,  -18.90,  130.10,   // particle 3  (+- interior) region 2
            -28.20,  180.66,   66.03,   // particle 14 (-+ interior) region 3 ghost for region 4
            -18.40,  +22.10,   95.50,   // particle 8  (-+ interior) region 3 ghost for regions 1, 2, 4
            -40.11,  170.25,  204.67,   // particle 18 (-+ interior) region 3 ghost for region 4
            160.81,  -35.42,  289.73,   // particle 19 (+- interior) region 2
        };

        expectedV = {
            64.0, -22.8,  15.2,   // particle 15
           -90.3,  48.6,  -7.4,   // particle 16
           -79.2,  12.4,  99.1,   // particle 13


            81.9, -35.0,  24.1,   // particle 17
           -72.5,  44.3, -95.8,   // particle 9
           -97.1,  28.9, -74.4,   // particle 3
           -53.7,  27.5, -41.6,   // particle 14
            59.7, -24.6,  10.9,   // particle 8
            57.8, -69.2,  39.4,   // particle 18
           -16.7,  73.3, -58.9    // particle 19
        };

        expectedF = {
            26.4, -11.3,   4.9,   // particle 15
           -43.2,  22.5,  -6.8,   // particle 16
           -48.1,  12.6,  33.9,   // particle 13


            35.0, -18.1,  13.7,   // particle 17
           -34.8,  25.1, -45.6,   // particle 9
           -22.1,  16.0, -49.7,   // particle 3
           -25.7,  17.8, -38.6,   // particle 14
            47.3, -19.5,   6.2,   // particle 8
            28.6, -31.4,  19.3,   // particle 18
            -8.2,  46.7, -24.5    // particle 19
        };



        expectedR = {
            71.0,  // particle 15 region 4
            14.2,  // particle 16 region 4
            92.4,  // particle 13 region 4


            83.1,  // particle 17 region 1
            54.3,  // particle 9  region 2
            3.9,   // particle 3  region 2
            37.6,  // particle 14 region 3
            76.2,  // particle 8  region 3
            58.9,  // particle 18 region 3
            26.4   // particle 19 region 2
        };

        expectedM = {
            2.1,  // particle 15 region 4
            6.7,  // particle 16 region 4
            7.4,  // particle 13 region 4


            8.2,  // particle 17 region 1
            4.1,  // particle 9  region 2
            5.6,  // particle 3  region 2
            5.9,  // particle 14 region 3
            8.4,  // particle 8  region 3
            1.8,  // particle 18 region 3
            4.6   // particle 19 region 2
        };

        expectedNGhost = 7;

    }
    


    // running configuration
    std::array<int, 3> ranks = { 2,2,1 };
    //
    int nranks = ranks[0] * ranks[1] * ranks[2];

    // builing the engine
    auto engine = build_engine_set_particles(rank, ranks,
        newId, newX, newV, newF, newR, newM, newNGhost, skin);

    // getting the communicator
    auto& communicatorRef = engine->getCommunicator();
    std::array<int, 6> dests = communicatorRef->returnExchangeDests();

    // transfered data
    std::vector<double> sendBuff;
    std::vector<double> recvBuff;
    std::vector<double> sendVec;

    // the maximum number of particles
    const int dataPerParticle = Particles::dataPerParticle;
    const int nmax = 50;
    const int bufferSize = nmax * dataPerParticle;

    sendBuff.resize(bufferSize);
    recvBuff.resize(bufferSize);


    int nDestLocal = 0, nDestTotal = 0;
    int nAttemptsLocal = 0, nAttemptsTotal = 0;

    constexpr int maxAttempts = 4;

    do {
        nDestLocal = 0;
        nAttemptsLocal++;

        /// <summary>
        /// the dst orders is xlo, xhi, ylo, yhi, zlo, zhi
        /// so for the dst this would be their xhi, xlo, yhi, ylo, zhi,zlo
        /// </summary>

        std::vector<std::array<int, 2>> SrcDst = { {0,1},{1,0},{2,3},{3,2},{ 4,5 },{5,4} };
        std::vector<int> tags = { 0,1,2,3,4,5 };

        struct
        {
            std::array<int, 2> indx;
            int tag;
        } loopStruct[6] =
        {
            {{0,1},0}, // send in xlo recv in xhi
            {{1,0},1}, // send in xhi recv in xlo
            {{2,3},2}, // send in ylo recv in yhi
            {{3,2},3}, // send in yhi recv in ylo
            {{4,5},4}, // send in zlo recv in zhi
            {{5,4},5}  // send in zhi recv in zlo
        };

        // exchanging particles first
        for (const auto& info : loopStruct)
        {
            int tgtIndx = info.indx[0];
            int srcIndx = info.indx[1];
            int tag = info.tag;

            nDestLocal += communicatorRef->sendParticles(dests[tgtIndx], sendVec);
            std::copy_n(sendVec.data(), sendVec.size(), sendBuff.data());
            comm_strategy->send(sendBuff.data(), bufferSize, dests[tgtIndx], tag);
            sendVec.clear();

            comm_strategy->recv(recvBuff.data(), bufferSize, dests[srcIndx], tag);
            communicatorRef->recvParticles(recvBuff);
            recvBuff.resize(bufferSize);

            comm_strategy->waitAll();
        }

        // sending / receiving ghost particles
        for (const auto& info : loopStruct)
        {
            int tgtIndx = info.indx[0];
            int srcIndx = info.indx[1];
            int tag = info.tag;

            nDestLocal += communicatorRef->sendGhosts(dests[tgtIndx], sendVec);
            std::copy_n(sendVec.data(), sendVec.size(), sendBuff.data());
            comm_strategy->send(sendBuff.data(), bufferSize, dests[tgtIndx], tag);
            sendVec.clear();

            comm_strategy->recv(recvBuff.data(), bufferSize, dests[srcIndx], tag);
            communicatorRef->recvGhosts(recvBuff);
            recvBuff.clear();
            recvBuff.resize(bufferSize);

            comm_strategy->waitAll();
        }


        // MPI_Allreduce to get nDestsTotal
        comm_strategy->reduceAll(&nDestLocal, &nDestTotal, 1);
        comm_strategy->reduceAll(&nAttemptsLocal, &nAttemptsTotal, 1);


    } while (nDestTotal > 0 && nAttemptsTotal < maxAttempts);

    // checking the particles
    checking_communicator(
        rank, engine.get(),
        expectedId,
        expectedX, 
        expectedV, 
        expectedF, 
        expectedR,
        expectedM,
        expectedNGhost);

}