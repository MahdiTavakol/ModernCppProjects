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

        

        MPI_Request requests[6];

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
