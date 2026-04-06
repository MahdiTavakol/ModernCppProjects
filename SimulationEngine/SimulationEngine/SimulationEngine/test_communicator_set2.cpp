#include "catch_amalgamated.hpp"
#include "test_communicator_helpers.hpp"


TEST_CASE("Testing the movement of particles between processors without skin (2X2X1)")
{
    std::cout << "Testing the movement of particles between processors without skin (2X2X1)" << std::endl;
    std::cout << std::string(80, '=') << std::endl;


    // moving particles
    std::vector<int> newId1 = {
        0,1,2,3,4
    };
    std::vector<double> newX1 =
    {
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) region 1
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) region 1
        -180.66,   30.77,   14.95,   // particle 2  owned by Q00 (-+ interior) region 3
          12.60,  -18.90,  130.10,   // particle 3  owned by Q00 (+- interior) region 2
          35.10, -140.48,   12.66,   // particle 4  owned by Q00 (+- interior) region 2
    };

    std::vector<double> newV1 =
    {
        72.4, -33.1,  95.0,   // particle 0
       -88.6,  14.3, -56.7,   // particle 1
        41.2, -19.8,  63.5,   // particle 2
       -97.1,  28.9, -74.4,   // particle 3
         3.6,  89.2, -12.5,   // particle 4
    };

    std::vector<double> newF1 =
    {
       12.4, -33.9,  45.2,   // particle 0
      -18.6,   7.3, -41.5,   // particle 1
       29.7,  -5.4,  38.9,   // particle 2
      -22.1,  16.0, -49.7,   // particle 3
        3.1,  27.6, -14.8,   // particle 4
    };

    std::vector<double> newR1 =
    {
     12.5,  // particle 0 region 1
     89.3,  // particle 1 region 1
     45.7,  // particle 2 region 3
     3.9,   // particle 3 region 2
     67.1,  // particle 4 region 2
    };

    std::vector<double> newM1 =
    {
        3.4,  // particle 0 region 1
        7.8,  // particle 1 region 1
        0.9,  // particle 2 region 3
        5.6,  // particle 3 region 2
        9.1,  // particle 4 region 2
    };

    std::vector<int> newId2 = {
        5,6,7,8,9
    };
    std::vector<double> newX2 =
    {
         180.62, -210.44,   60.91,   // particle 5  owned by Q10 (+- interior) region 2
         260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior) region 2
         -25.60, -160.15,  -55.27,   // particle 7  owned by Q10 (-- interior) region 1
         -18.40,  +22.10,   95.50,   // particle 8  owned by Q10 (-+ interior) region 3
         190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior) region 2
    };

    std::vector<double> newV2 =
    {
        54.7, -66.3,  77.1,   // particle 5
       -45.9,  18.0,  92.6,   // particle 6
       -11.4,  36.8, -83.2,   // particle 7
        59.7, -24.6,  10.9,   // particle 8
       -72.5,  44.3, -95.8,   // particle 9
    };

    std::vector<double> newF2 =
    {
        44.5, -36.7,  21.4,   // particle 5
        -9.9,   5.6,  31.2,   // particle 6
       -12.7,  18.9, -28.4,   // particle 7
        47.3, -19.5,   6.2,   // particle 8
       -34.8,  25.1, -45.6,   // particle 9
    };

    std::vector<double> newM2 =
    {
        2.7,  // particle 5 region 2
        6.0,  // particle 6 region 2
        1.3,  // particle 7 region 1
        8.4,  // particle 8 region 3
        4.1,  // particle 9 region 2
    };

    std::vector<double> newR2 =
    {
     24.6,  // particle 5 region 2
     98.0,  // particle 6 region 2
     31.4,  // particle 7 region 1
     76.2,  // particle 8 region 3
     54.3,  // particle 9 region 2
    };

    std::vector<int> newId3 = {
        10,11,12,13,14
    };
    std::vector<double> newX3 =
    {
        -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior) region 3
        -270.22,  120.74,  210.46,   // particle 11 owned by Q01 (-+ interior) region 3
        -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior) region 3
          22.80,   18.10, -140.25,   // particle 13 owned by Q01 (++ interior) region 4
         -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior) region 3
    };

    std::vector<double> newV3 =
    {
        21.7, -38.4,  68.2,   // particle 10
       -14.9,  87.6, -29.3,   // particle 11
        46.1, -61.5,  33.8,   // particle 12
       -79.2,  12.4,  99.1,   // particle 13
       -53.7,  27.5, -41.6,   // particle 14
    };

    std::vector<double> newF3 =
    {
        14.8,  -7.3,  39.6,   // particle 10
       -21.9,  30.4, -16.5,   // particle 11
        41.2, -29.8,   9.7,   // particle 12
       -48.1,  12.6,  33.9,   // particle 13
       -25.7,  17.8, -38.6,   // particle 14
    };

    std::vector<double> newM3 =
    {
        0.5,  // particle 10 region 3
        9.8,  // particle 11 region 3
        3.0,  // particle 12 region 3
        7.4,  // particle 13 region 4
        5.9,  // particle 14 region 3
    };

    std::vector<double> newR3 =
    {
     8.7,   // particle 10 region 3
     63.5,  // particle 11 region 3
     19.8,  // particle 12 region 3
     92.4,  // particle 13 region 4
     37.6,  // particle 14 region 3
    };

    std::vector<int> newId4 = {
        15,16,17,18,19
    };
    std::vector<double> newX4 =
    {
         210.83,  220.41, -150.88,   // particle 15 owned by Q01 (++ interior) region 4
         140.62,  160.33,  198.21,   // particle 16 owned by Q01 (++ interior) region 4
         -30.25,  -22.40,   10.66,   // particle 17 owned by Q01 (-- interior) region 1
         -40.11,  170.25,  204.67,   // particle 18 owned by Q01 (-+ interior) region 3
         160.81,  -35.42,  289.73,   // particle 19 owned by Q01 (+- interior) region 2
    };

    std::vector<double> newV4 =
    {
        64.0, -22.8,  15.2,   // particle 15
       -90.3,  48.6,  -7.4,   // particle 16
        81.9, -35.0,  24.1,   // particle 17
        57.8, -69.2,  39.4,   // particle 18
       -16.7,  73.3, -58.9    // particle 19
    };

    std::vector<double> newF4 =
    {
        26.4, -11.3,   4.9,   // particle 15
       -43.2,  22.5,  -6.8,   // particle 16
        35.0, -18.1,  13.7,   // particle 17
        28.6, -31.4,  19.3,   // particle 18
        -8.2,  46.7, -24.5    // particle 19
    };

    std::vector<double> newM4 =
    {
        2.1,  // particle 15 region 4
        6.7,  // particle 16 region 4
        8.2,  // particle 17 region 1
        1.8,  // particle 18 region 3
        4.6   // particle 19 region 2
    };

    std::vector<double> newR4 =
    {
     71.0,  // particle 15 region 4
     14.2,  // particle 16 region 4
     83.1,  // particle 17 region 1
     58.9,  // particle 18 region 3
     26.4   // particle 19 region 2
    };

    // expected values for each core
    // core 1
    std::vector<int> expectedIds1 = {
        0,1,7,17
    };
    std::vector<double> expectedXs1 = {
        -220.40, -180.10,   80.55,   // particle 0  owned by Q00 (-- interior) region 1
        -140.75, -260.33, -120.18,   // particle 1  owned by Q00 (-- interior) region 1
         -25.60, -160.15,  -55.27,   // particle 7  owned by Q10 (-- interior) region 1
         -30.25,  -22.40,   10.66,   // particle 17 owned by Q01 (-- interior) region 1
    };
    std::vector<double> expectedVs1 = {
      72.4, -33.1,  95.0,   // particle 0
     -88.6,  14.3, -56.7,   // particle 1
     -11.4,  36.8, -83.2,   // particle 7
      81.9, -35.0,  24.1,   // particle 17
    };
    std::vector<double> expectedFs1 = {
       12.4, -33.9,  45.2,   // particle 0
      -18.6,   7.3, -41.5,   // particle 1
      -12.7,  18.9, -28.4,   // particle 7
       35.0, -18.1,  13.7,   // particle 17
    };
    std::vector<double> expectedMs1 = {
         3.4,  // particle 0
         7.8,  // particle 1
         1.3,  // particle 7
         8.2,  // particle 17
    };

    std::vector<double> expectedRs1 = {
        12.5,  // particle 0
        89.3,  // particle 1
        31.4,  // particle 7
        83.1,  // particle 17
    };

    // core 2
    std::vector<int> expectedIds2 = {
        3,4,5,6,9,19
    };
    std::vector<double> expectedXs2 = {
          12.60,  -18.90,  130.10,   // particle 3  owned by Q00 (+- interior) region 2
          35.10, -140.48,   12.66,   // particle 4  owned by Q00 (+- interior) region 2
         180.62, -210.44,   60.91,   // particle 5  owned by Q10 (+- interior) region 2
         260.11, -140.88, -200.30,   // particle 6  owned by Q10 (+- interior) region 2
         190.27,  -40.22, -236.70,   // particle 9  owned by Q10 (+- interior) region 2
         160.81,  -35.42,  289.73,   // particle 19 owned by Q01 (+- interior) region 2
    };
    std::vector<double> expectedVs2 = {
        -97.1,  28.9, -74.4,   // particle 3
          3.6,  89.2, -12.5,   // particle 4
         54.7, -66.3,  77.1,   // particle 5
        -45.9,  18.0,  92.6,   // particle 6
        -72.5,  44.3, -95.8,   // particle 9
        -16.7,  73.3, -58.9    // particle 19

    };
    std::vector<double> expectedFs2 = {
        -22.1,  16.0, -49.7,   // particle 3
          3.1,  27.6, -14.8,   // particle 4
         44.5, -36.7,  21.4,   // particle 5
         -9.9,   5.6,  31.2,   // particle 6
        -34.8,  25.1, -45.6,   // particle 9
         -8.2,  46.7, -24.5    // particle 19
    };
    std::vector<double> expectedMs2 = {
        5.6,  // particle 3
        9.1,  // particle 4
        2.7,  // particle 5
        6.0,  // particle 6
        4.1,  // particle 9
        4.6   // particle 19
    };
    std::vector<double> expectedRs2 = {
         3.9,   // particle 3
        67.1,  // particle 4
        24.6,  // particle 5
        98.0,  // particle 6
        54.3,  // particle 9
        26.4   // particle 19
    };

    // core 3
    std::vector<int> expectedIds3 = {
        2,8,10,11,12,14,18
    };
    std::vector<double> expectedXs3 = {
        -180.66,   30.77,   14.95,   // particle 2  owned by Q00 (-+ interior) region 3
         -18.40,   22.10,   95.50,   // particle 8  owned by Q10 (-+ interior) region 3
        -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior) region 3
        -270.22,  120.74,  210.46,   // particle 11 owned by Q01 (-+ interior) region 3
        -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior) region 3
         -28.20,  180.66,   66.03,   // particle 14 owned by Q01 (-+ interior) region 3
         -40.11,  170.25,  204.67,   // particle 18 owned by Q01 (-+ interior) region 3
    };
    std::vector<double> expectedVs3 = {
     41.2, -19.8,  63.5,   // particle 2
     59.7, -24.6,  10.9,   // particle 8
     21.7, -38.4,  68.2,   // particle 10
    -14.9,  87.6, -29.3,   // particle 11
     46.1, -61.5,  33.8,   // particle 12
    -53.7,  27.5, -41.6,   // particle 14
     57.8, -69.2,  39.4,   // particle 18

    };
    std::vector<double> expectedFs3 = {
     29.7,  -5.4,  38.9,   // particle 2
     47.3, -19.5,   6.2,   // particle 8
     14.8,  -7.3,  39.6,   // particle 10
    -21.9,  30.4, -16.5,   // particle 11
     41.2, -29.8,   9.7,   // particle 12
    -25.7,  17.8, -38.6,   // particle 14
     28.6, -31.4,  19.3,   // particle 18

    };
    std::vector<double> expectedMs3 = {
     0.9,  // particle 2
     8.4,  // particle 8
     0.5,  // particle 10
     9.8,  // particle 11
     3.0,  // particle 12
     5.9,  // particle 14
     1.8,  // particle 18

    };

    std::vector<double> expectedRs3 = {
     45.7,  // particle 2
     76.2,  // particle 8
     8.7,   // particle 10
     63.5,  // particle 11
     19.8,  // particle 12
     37.6,  // particle 14
     58.9,  // particle 18
    };

    // core 4
    std::vector<int> expectedIds4 = {
        13,15,16
    };
    std::vector<double> expectedXs4 = {
        22.80,   18.10,  -140.25,   // particle 13 owned by Q01 (++ interior) region 4 
        210.83,  220.41, -150.88,   // particle 15 owned by Q01 (++ interior) region 4
        140.62,  160.33,  198.21,   // particle 16 owned by Q01 (++ interior) region 4
    };

    std::vector<double> expectedVs4 = {
    -79.2,  12.4,  99.1,   // particle 13
     64.0, -22.8,  15.2,   // particle 15
    -90.3,  48.6,  -7.4,   // particle 16
    };

    std::vector<double> expectedFs4 = {
    -48.1,  12.6,  33.9,   // particle 13
     26.4, -11.3,   4.9,   // particle 15
    -43.2,  22.5,  -6.8,   // particle 16
    };

    std::vector<double> expectedMs4 = {
     7.4,  // particle 13
     2.1,  // particle 15
     6.7,  // particle 16
    };

    std::vector<double> expectedRs4 = {
     92.4,  // particle 13
     71.0,  // particle 15
     14.2,  // particle 16
    };

    std::vector<std::vector<int>> expectedIdsVec = {
        expectedIds1, expectedIds2, expectedIds3, expectedIds3
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

    std::vector<std::vector<int>> newIdVec = {
        newId1, newId2, newId3, newId4
    };
    std::vector<std::vector<double>> newXVec =
    {
        newX1, newX2, newX3, newX4
    };
    std::vector<std::vector<double>> newVVec =
    {
        newV1, newV2, newV3, newV4
    };
    std::vector<std::vector<double>> newFVec = 
    {
        newF1, newF2, newF3, newF4
    };
    std::vector<std::vector<double>> newRVec =
    {
        newR1, newR2, newR3, newR4
    };
    std::vector<std::vector<double>> newMVec = 
    {
        newM1, newM2,newM3, newM4
    };


    // running configuration
    std::array<int, 3> ranks = { 2,2,1 };
    //
    int nranks = ranks[0] * ranks[1] * ranks[2];
    // ids 
    std::vector<int> ids;
    for (int i = 0; i < nranks; i++)
        ids.push_back(i);

    std::vector<unique_ptr<Engine>> engineArray;

    for (int i = 0; i < nranks; i++)
    {
        auto engine_ptr =
            build_engine_set_particles(ids[i], ranks, newIdVec[i], newXVec[i], newVVec[i], newFVec[i], newRVec[i], newMVec[i]);
        engineArray.push_back(std::move(engine_ptr));
    }

    std::vector<Communicator*> communicatorArray;

    for (int i = 0; i < nranks; i++) {
        communicatorArray.push_back(engineArray[i]->getCommunicator().get());
    }

    std::vector<std::array<int, 6>> exchangeDestArray(nranks, std::array<int, 6>{});

    std::vector<double>* messagesArray[4];


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
        nDestsTotal = 0;
        numberOfAttempts++;

        for (int i = 0; i < 4; i++) {
            exchangeDestArray[i] = communicatorArray[i]->returnExchangeDests();
            // an array of vector<double> is returned for each communicatoriRef
            // bugging ===>>> it is a possible bug
            messagesArray[i] = communicatorArray[i]->sendExchangeParticles();
        }

        // getting the nDests values
        for (int i = 0; i < 4; i++)
            nDestsTotal += communicatorArray[i]->getNDests();

        // ranks
        for (int i = 0; i < 4; i++) {
            // directions xlo, xhi, ylo, yhi, zlo, zhi
            for (int j = 0; j < 6; j++) {
                // ref is very important
                // since the recvExchangeParticles
                // after setting the new particle 
                // needs to reset the message
                auto& message = messagesArray[i][j];
                if (exchangeDestArray[i][j] < 0)
                    continue;

                
                communicatorArray[exchangeDestArray[i][j]]->recvExchangeParticles(message);
            }
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


TEST_CASE("Testing the movement of particles between processors without skin (3X3X1)")
{
    std::cout << "Testing the movement of particles between processors without skin (3X3X1)" << std::endl;
    std::cout << std::string(80, '=') << std::endl;


    // moving particles
    std::vector<double> newX1 =
    {
        -220.0, -220.0,   40.0,   // particle 1 stays in Q00    rank0
        -150.0, -180.0,  -60.0,   // particle 2 stays in Q00    rank0
         -80.0, -210.0,   15.0,   // particle 3 -> Q10 (x high) rank1
        -170.0,  -80.0,   90.0,   // particle 4 -> Q01 (y high) rank3
    };


    std::vector<double> newX2 =
    {
         -20.0, -220.0,   35.0,   // particle 5 stays in Q10    rank1
          60.0, -150.0,  -80.0,   // particle 6 stays in Q10    rank1
        -130.0, -170.0,   10.0,   // particle 7 -> Q00 (x low)  rank0
         130.0, -190.0,  -25.0,   // particle 8 -> Q20 (x high) rank2
          40.0,  -70.0,   55.0,   // particle 9 -> Q11 (y high) rank4
    };


    std::vector<double> newX3 =
    {
         180.0, -220.0,   10.0,   // particle 10 stays in Q20    rank2
         240.0, -120.0,  -95.0,   // particle 11 stays in Q20    rank2
          80.0, -160.0,   70.0,   // particle 12 -> Q10 (x low)  rank1
         170.0,  -90.0,  -40.0,   // particle 13 -> Q21 (y high) rank5
    };

    std::vector<double> newX4 =
    {
        -240.0,  -20.0,   50.0,   // particle 14 stays in Q01    rank3
        -160.0,   60.0,  -20.0,   // particle 15 stays in Q01    rank3
         -90.0,   30.0,   15.0,   // particle 16 -> Q11 (x high) rank4
        -180.0, -140.0,   80.0,   // particle 17 -> Q00 (y low)  rank0
        -210.0,  140.0,  -35.0,   // particle 18 -> Q02 (y high) rank6
    };

    std::vector<double> newX5 =
    {
         -40.0,  -30.0,   20.0,   // particle 19 stays in Q11    rank4
          70.0,   80.0,  -60.0,   // particle 20 stays in Q11    rank4
        -140.0,   10.0,   75.0,   // particle 21 -> Q01 (x low)  rank3
         140.0,  -20.0,  -10.0,   // particle 22 -> Q21 (x high) rank5
          10.0, -130.0,   45.0,   // particle 23 -> Q10 (y low)  rank1
         -70.0,  160.0,   90.0,   // particle 24 -> Q12 (y high) rank7
    };

    std::vector<double> newX6 =
    {
         160.0,  -40.0,   25.0,   // particle 25 stays in Q21    rank5
         250.0,   70.0,  -85.0,   // particle 26 stays in Q21    rank5
          60.0,   20.0,   35.0,   // particle 27 -> Q11 (x low)  rank4
         180.0, -170.0,  -15.0,   // particle 28 -> Q20 (y low)  rank2
         220.0,  150.0,   55.0,   // particle 29 -> Q22 (y high) rank8
    };

    std::vector<double> newX7 =
    {
        -260.0,  180.0,   40.0,   // particle 30 stays in Q02    rank6
        -140.0,  260.0,  -70.0,   // particle 31 stays in Q02    rank6
         -80.0,  210.0,   15.0,   // particle 32 -> Q12 (x high) rank7
        -190.0,   80.0,   95.0,   // particle 33 -> Q01 (y low)  rank3
    };

    std::vector<double> newX8 =
    {
         -20.0,  160.0,   35.0,   // particle 34 stays in Q12    rank7
          80.0,  220.0,  -55.0,   // particle 35 stays in Q12    rank7
        -130.0,  150.0,   60.0,   // particle 36 -> Q02 (x low)  rank6
         140.0,  180.0,  -20.0,   // particle 37 -> Q22 (x high) rank8
          30.0,   70.0,   75.0,   // particle 38 -> Q11 (y low)  rank4
    };

    std::vector<double> newX9 =
    {
         180.0,  160.0,   10.0,   // particle 39 stays in Q22   rank8
         260.0,  240.0,  -90.0,   // particle 40 stays in Q22   rank8
          90.0,  170.0,   45.0,   // particle 41 -> Q12 (x low) rank7
         220.0,   90.0,  -35.0,   // particle 42 -> Q21 (y low) rank5
    };

    // expected values

    // Q00 rank0
    std::vector<double> expectedX1 =
    {
        -220.0, -220.0,   40.0,   // particle 1 stays in Q00    rank0
        -150.0, -180.0,  -60.0,   // particle 2 stays in Q00    rank0
        -130.0, -170.0,   10.0,   // particle 7 -> Q00 (x low)  rank0
        -180.0, -140.0,   80.0    // particle 17 -> Q00 (y low) rank0
    };

    // Q10 rank1
    std::vector<double> expectedX2 =
    {
        -80.0, -210.0,   15.0,   // particle 3 -> Q10 (x high) rank1
        -20.0, -220.0,   35.0,   // particle 5 stays in Q10    rank1
         60.0, -150.0,  -80.0,   // particle 6 stays in Q10    rank1
         80.0, -160.0,   70.0,   // particle 12 -> Q10 (x low) rank1
         10.0, -130.0,   45.0    // particle 23 -> Q10 (y low) rank1
    };

    // Q20 rank2
    std::vector<double> expectedX3 =
    {
        130.0, -190.0,  -25.0,   // particle 8 -> Q20 (x high) rank2
        180.0, -220.0,   10.0,   // particle 10 stays in Q20   rank2
        240.0, -120.0,  -95.0,   // particle 11 stays in Q20   rank2
        180.0, -170.0,  -15.0    // particle 28 -> Q20 (y low) rank2
    };

    // Q01 rank3
    std::vector<double> expectedX4 =
    {
        -170.0,  -80.0,   90.0,   // particle 4 -> Q01 (y high) rank3
        -240.0,  -20.0,   50.0,   // particle 14 stays in Q01   rank3
        -160.0,   60.0,  -20.0,   // particle 15 stays in Q01   rank3
        -140.0,   10.0,   75.0,   // particle 21 -> Q01 (x low) rank3
        -190.0,   80.0,   95.0    // particle 33 -> Q01 (y low) rank3
    };

    // Q11 rank4
    std::vector<double> expectedX5 =
    {
         40.0,  -70.0,   55.0,   // particle 9 -> Q11 (y high)  rank4
        -90.0,   30.0,   15.0,   // particle 16 -> Q11 (x high) rank4
        -40.0,  -30.0,   20.0,   // particle 19 stays in Q11    rank4
         70.0,   80.0,  -60.0,   // particle 20 stays in Q11    rank4
         60.0,   20.0,   35.0,   // particle 27 -> Q11 (x low)  rank4
         30.0,   70.0,   75.0    // particle 38 -> Q11 (y low)  rank4
    };

    // Q21 rank5
    std::vector<double> expectedX6 =
    {
        170.0,  -90.0,  -40.0,   // particle 13 -> Q21 (y high) rank5
        140.0,  -20.0,  -10.0,   // particle 22 -> Q21 (x high) rank5
        160.0,  -40.0,   25.0,   // particle 25 stays in Q21    rank5
        250.0,   70.0,  -85.0,   // particle 26 stays in Q21    rank5
        220.0,   90.0,  -35.0    // particle 42 -> Q21 (y low)  rank5
    };

    // Q02 rank6
    std::vector<double> expectedX7 =
    {
        -210.0,  140.0,  -35.0,   // particle 18 -> Q02 (y high) rank6
        -260.0,  180.0,   40.0,   // particle 30 stays in Q02    rank6
        -140.0,  260.0,  -70.0,   // particle 31 stays in Q02    rank6
        -130.0,  150.0,   60.0,   // particle 36 -> Q02 (x low)  rank6
    };

    // Q12 rank7
    std::vector<double> expectedX8 =
    {
        -70.0,  160.0,   90.0,   // particle 24 -> Q12 (y high) rank7
        -80.0,  210.0,   15.0,   // particle 32 -> Q12 (x high) rank7
        -20.0,  160.0,   35.0,   // particle 34 stays in Q12    rank7
         80.0,  220.0,  -55.0,   // particle 35 stays in Q12    rank7
         90.0,  170.0,   45.0    // particle 41 -> Q12 (x low) rank7
    };

    // Q22 rank8
    std::vector<double> expectedX9 =
    { 
        220.0,  150.0,   55.0,   // particle 29 -> Q22 (y high) rank8
        140.0,  180.0,  -20.0,   // particle 37 -> Q22 (x high) rank8
        180.0,  160.0,   10.0,   // particle 39 stays in Q22   rank8
        260.0,  240.0,  -90.0,   // particle 40 stays in Q22   rank8
    };


    std::vector<double> newV1(newX1.size(), 0.0), newF1(newX1.size(), 0.0);
    std::vector<double> newM1(newX1.size() / 3, 0.0), newR1(newX1.size() / 3, 0.0);
    std::vector<double> newV2(newX2.size(), 0.0), newF2(newX2.size(), 0.0);
    std::vector<double> newM2(newX2.size() / 3, 0.0), newR2(newX2.size() / 3, 0.0);
    std::vector<double> newV3(newX3.size(), 0.0), newF3(newX3.size(), 0.0);
    std::vector<double> newM3(newX3.size() / 3, 0.0), newR3(newX3.size() / 3, 0.0);
    std::vector<double> newV4(newX4.size(), 0.0), newF4(newX4.size(), 0.0);
    std::vector<double> newM4(newX4.size() / 3, 0.0), newR4(newX4.size() / 3, 0.0);
    std::vector<double> newV5(newX5.size(), 0.0), newF5(newX5.size(), 0.0);
    std::vector<double> newM5(newX5.size() / 3, 0.0), newR5(newX5.size() / 3, 0.0);
    std::vector<double> newV6(newX6.size(), 0.0), newF6(newX6.size(), 0.0);
    std::vector<double> newM6(newX6.size() / 3, 0.0), newR6(newX6.size() / 3, 0.0);
    std::vector<double> newV7(newX7.size(), 0.0), newF7(newX7.size(), 0.0);
    std::vector<double> newM7(newX7.size() / 3, 0.0), newR7(newX7.size() / 3, 0.0);
    std::vector<double> newV8(newX8.size(), 0.0), newF8(newX8.size(), 0.0);
    std::vector<double> newM8(newX8.size() / 3, 0.0), newR8(newX8.size() / 3, 0.0);
    std::vector<double> newV9(newX9.size(), 0.0), newF9(newX9.size(), 0.0);
    std::vector<double> newM9(newX9.size() / 3, 0.0), newR9(newX9.size() / 3, 0.0);


    std::vector<std::vector<double>> newXVec =
    {
        newX1, newX2, newX3, newX4, newX5, newX6, newX7, newX8, newX9
    };
    std::vector<std::vector<double>> newVVec =
    {
        newV1, newV2, newV3, newV4, newV5, newV6, newV7, newV8, newV9
    };
    std::vector<std::vector<double>> newFVec =
    {
        newF1, newF2, newF3, newF4, newF5, newF6, newF7, newF8, newF9
    };
    std::vector<std::vector<double>> newRVec =
    {
        newR1, newR2, newR3, newR4, newR5, newR6, newR7, newR8, newR9
    };
    std::vector<std::vector<double>> newMVec =
    {
        newM1, newM2, newM3, newM4, newM5, newM6, newM7, newM8, newM9
    };

    std::vector<double> expectedV1(expectedX1.size(), 0.0), expectedF1(expectedX1.size(), 0.0);
    std::vector<double> expectedM1(expectedX1.size() / 3, 0.0), expectedR1(expectedX1.size() / 3, 0.0);
    std::vector<double> expectedV2(expectedX2.size(), 0.0), expectedF2(expectedX2.size(), 0.0);
    std::vector<double> expectedM2(expectedX2.size() / 3, 0.0), expectedR2(expectedX2.size() / 3, 0.0);
    std::vector<double> expectedV3(expectedX3.size(), 0.0), expectedF3(expectedX3.size(), 0.0);
    std::vector<double> expectedM3(expectedX3.size() / 3, 0.0), expectedR3(expectedX3.size() / 3, 0.0);
    std::vector<double> expectedV4(expectedX4.size(), 0.0), expectedF4(expectedX4.size(), 0.0);
    std::vector<double> expectedM4(expectedX4.size() / 3, 0.0), expectedR4(expectedX4.size() / 3, 0.0);
    std::vector<double> expectedV5(expectedX5.size(), 0.0), expectedF5(expectedX5.size(), 0.0);
    std::vector<double> expectedM5(expectedX5.size() / 3, 0.0), expectedR5(expectedX5.size() / 3, 0.0);
    std::vector<double> expectedV6(expectedX6.size(), 0.0), expectedF6(expectedX6.size(), 0.0);
    std::vector<double> expectedM6(expectedX6.size() / 3, 0.0), expectedR6(expectedX6.size() / 3, 0.0);
    std::vector<double> expectedV7(expectedX7.size(), 0.0), expectedF7(expectedX7.size(), 0.0);
    std::vector<double> expectedM7(expectedX7.size() / 3, 0.0), expectedR7(expectedX7.size() / 3, 0.0);
    std::vector<double> expectedV8(expectedX8.size(), 0.0), expectedF8(expectedX8.size(), 0.0);
    std::vector<double> expectedM8(expectedX8.size() / 3, 0.0), expectedR8(expectedX8.size() / 3, 0.0);
    std::vector<double> expectedV9(expectedX9.size(), 0.0), expectedF9(expectedX9.size(), 0.0);
    std::vector<double> expectedM9(expectedX9.size() / 3, 0.0), expectedR9(expectedX9.size() / 3, 0.0);



    std::vector<std::vector<double>> expectedXsVec = {
        expectedX1, expectedX2, expectedX3, expectedX4,
        expectedX5, expectedX6, expectedX7, expectedX8,
        expectedX9
    };
    std::vector<std::vector<double>> expectedVsVec = {
        expectedV1, expectedV2, expectedV3, expectedV4,
        expectedV5, expectedV6, expectedV7, expectedV8,
        expectedV9
    };
    std::vector<std::vector<double>> expectedFsVec = {
        expectedF1, expectedF2, expectedF3, expectedF4,
        expectedF5, expectedF6, expectedF7, expectedF8,
        expectedF9
    };
    std::vector<std::vector<double>> expectedRsVec = {
        expectedR1, expectedR2, expectedR3, expectedR4,
        expectedR5, expectedR6, expectedR7, expectedR8,
        expectedR9
    };
    std::vector<std::vector<double>> expectedMsVec = {
        expectedM1, expectedM2, expectedM3, expectedM4,
        expectedM5, expectedM6, expectedM7, expectedM8,
        expectedM9
    };




    // running configuration
    std::array<int, 3> ranks = { 3,3,1 };
    // 
    int nranks = ranks[0] * ranks[1] * ranks[2];
    // ids 
    std::vector<int> ids;
    ids.reserve(nranks);
    // 
    for (int i = 0; i < nranks; i++)
        ids.push_back(i);



    // Engine vector
    std::vector<std::unique_ptr<Engine>> engineArray;

    // creating engine_ptrs
    for (int i = 0; i < nranks; i++) {
        auto engine_ptr = build_engine_set_particles(
            ids[i], ranks, newXVec[i], newVVec[i], newFVec[i], newRVec[i], newMVec[i]);
        engineArray.push_back(std::move(engine_ptr));
    }


    std::vector<Communicator*> communicatorArray;

    for (auto& engine_ptr : engineArray) {
        auto& communicatorRef = engine_ptr->getCommunicator();
        communicatorArray.push_back(communicatorRef.get());
   }

    std::vector<std::array<int, 6>> exchangeDestArray(nranks, std::array<int, 6>{});



    std::vector<double>* messagesArray[9];

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
        nDestsTotal = 0;
        numberOfAttempts++;

        for (int i = 0; i < nranks; i++) {
            exchangeDestArray[i] = communicatorArray[i]->returnExchangeDests();
   
            // an array of vector<double> is returned for each communicatoriRef
            messagesArray[i] = communicatorArray[i]->sendExchangeParticles();

        }

        // getting the nDests values
        for (int i = 0; i < nranks; i++)
            nDestsTotal += communicatorArray[i]->getNDests();


        // ranks
        for (int i = 0; i < nranks; i++) {
            // directions xlo, xhi, ylo, yhi, zlo, zhi
            for (int j = 0; j < 6; j++) {
                // ref is very important
                // since the recvExchangeParticles
                // after setting the new particle 
                // needs to reset the message
                auto& message = messagesArray[i][j];
                if (exchangeDestArray[i][j] < 0)
                    continue;
                communicatorArray[exchangeDestArray[i][j]]->recvExchangeParticles(message);
            }
        }

    } while (nDestsTotal > 0 && numberOfAttempts < maxAttempts);


    // ranks
    for (int i = 0; i < nranks; i++) {
        int id = i;
        Engine* engine = engineArray[id].get();
        checking_communicator(
            id, engine,
            expectedXsVec,
            expectedVsVec,
            expectedFsVec,
            expectedRsVec,
            expectedMsVec
        );
    }
}


TEST_CASE("Testing the movement of particles between processors without skin (3X3X3)")
{
    std::cout << "Testing the movement of particles between processors without skin (3X3X3)" << std::endl;
    std::cout << std::string(80, '=') << std::endl;



    // rank4
    std::vector<double> newX5 =
    {
         -20.0,  -70.0, -235.0,   // particle 1 stays in rank4
          60.0,  -30.0, -180.0,   // particle 2 stays in rank4
          70.0,  -60.0,  -90.0,   // particle 3 ->  (z high) rank13
          40.0,  -70.0,   55.0,   // particle 4 ->  (z high) rank13
    };

    // rank10
    std::vector<double> newX11 =
    {
         -40.0,  -240.0,   50.0,   // particle 9  stays in rank10
          60.0,  -160.0,  -20.0,   // particle 10 stays in rank10
         -75.0,   -40.0,   80.0,   // particle 11 -> (y high) rank13
          90.0,    60.0,  -35.0,   // particle 12 -> (y high) rank13
    };

    // rank12
    std::vector<double> newX13 =
    {
        -280.0,  -20.0,   10.0,   // particle 5 stays in rank12
        -140.0,   80.0,  -95.0,   // particle 6 stays in rank12
          80.0,  -60.0,   70.0,   // particle 7 -> (x high)  rank13
         -70.0,  -90.0,  -40.0,   // particle 8 -> (x high) rank13
    };

    // rank13
    std::vector<double> newX14 = {
        -65.0, -85.0, 95.0,  // rank13
        -12.0, -66.0, -79.0  // rank13
    };

    // rank14
    std::vector<double> newX15 =
    {
         230.0,  -30.0,   20.0,   // particle 13 stays in rank14
         270.0,   80.0,  -60.0,   // particle 14 stays in rank14
          10.0,  -30.0,   45.0,   // particle 15 -> (x low)  rank13
         -70.0,   60.0,   90.0,   // particle 16 -> (x low)  rank13
    };

    // rank16
    std::vector<double> newX17 =
    {
         -40.0,  160.0,   25.0,   // particle 17 stays in rank17
          70.0,  250.0,  -85.0,   // particle 18 stays in rank17
          60.0,   20.0,   35.0,   // particle 19 -> (y low)  rank13
          18.0,  -17.0,  -15.0,   // particle 20 -> (y low)  rank13
    };
    
    // rank17
    std::vector<double> newX18 = {
        -30.0, 95.0, 45.0 // rank 13
    };

    // rank22
    std::vector<double> newX23 =
    {
        -26.0,   80.0,   140.0,   // particle 21 stays in rank22
        -14.0,   60.0,   170.0,   // particle 22 stays in rank22
         -80.0,  21.0,    15.0,   // particle 23 -> (z low) rank13
         -19.0,  80.0,    95.0,   // particle 24 -> (z low) rank13
    };

    // expected values

    // rank4
    std::vector<double> expectedX5 =
    {
         -20.0,  -70.0, -235.0,   // particle 1 stays in rank4
          60.0,  -30.0, -180.0,   // particle 2 stays in rank4
    };

    // rank10
    std::vector<double> expectedX11 =
    {
         -40.0,  -240.0,   50.0,   // particle 9  stays in rank10
          60.0,  -160.0,  -20.0,   // particle 10 stays in rank10
    };

    // rank12
    std::vector<double> expectedX13 =
    {
        -280.0,  -20.0,   10.0,   // particle 5 stays in rank12
        -140.0,   80.0,  -95.0,   // particle 6 stays in rank12
    };

    // rank13
    std::vector<double> expectedX14 = {
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
        -80.0,  21.0,  15.0,  // particle 23 -> (z low) rank13
        -19.0,  80.0,  95.0,   // particle 24 -> (z low) rank13
        -30.0,  95.0,  45.0  
    };

    // rank14
    std::vector<double> expectedX15 =
    {
         230.0,  -30.0,   20.0,   // particle 13 stays in rank14
         270.0,   80.0,  -60.0,   // particle 14 stays in rank14
    };

    // rank16
    std::vector<double> expectedX17 =
    {
         -40.0,  160.0,   25.0,   // particle 17 stays in rank17
          70.0,  250.0,  -85.0,   // particle 18 stays in rank17
    };

    std::vector<double> expectedX18 = {};

    // rank22
    std::vector<double> expectedX23 =
    {
        -26.0,   80.0,   140.0,   // particle 21 stays in rank22
        -14.0,   60.0,   170.0,   // particle 22 stays in rank22
    };


    std::vector<double> newV5(newX5.size(), 0.0), newF5(newX5.size(), 0.0);
    std::vector<double> newM5(newX5.size() / 3, 0.0), newR5(newX5.size() / 3, 0.0);
    std::vector<double> newV11(newX11.size(), 0.0), newF11(newX11.size(), 0.0);
    std::vector<double> newM11(newX11.size() / 3, 0.0), newR11(newX11.size() / 3, 0.0);
    std::vector<double> newV13(newX13.size(), 0.0), newF13(newX13.size(), 0.0);
    std::vector<double> newM13(newX13.size() / 3, 0.0), newR13(newX13.size() / 3, 0.0);
    std::vector<double> newV14(newX14.size(), 0.0), newF14(newX14.size(), 0.0);
    std::vector<double> newM14(newX14.size() / 3, 0.0), newR14(newX14.size() / 3, 0.0);
    std::vector<double> newV15(newX15.size(), 0.0), newF15(newX15.size(), 0.0);
    std::vector<double> newM15(newX15.size() / 3, 0.0), newR15(newX15.size() / 3, 0.0);
    std::vector<double> newV17(newX17.size(), 0.0), newF17(newX17.size(), 0.0);
    std::vector<double> newM17(newX17.size() / 3, 0.0), newR17(newX17.size() / 3, 0.0);
    std::vector<double> newV18(newX18.size(), 0.0), newF18(newX18.size(), 0.0);
    std::vector<double> newM18(newX18.size() / 3, 0.0), newR18(newX18.size() / 3, 0.0);
    std::vector<double> newV23(newX23.size(), 0.0), newF23(newX23.size(), 0.0);
    std::vector<double> newM23(newX23.size() / 3, 0.0), newR23(newX23.size() / 3, 0.0);

    std::vector<double> expectedV5(newX5.size(), 0.0), expectedF5(newX5.size(), 0.0);
    std::vector<double> expectedM5(newX5.size()/3, 0.0), expectedR5(newX5.size()/3, 0.0);
    std::vector<double> expectedV11(newX11.size(), 0.0), expectedF11(newX11.size(), 0.0);
    std::vector<double> expectedM11(newX11.size() / 3, 0.0), expectedR11(newX11.size() / 3, 0.0);
    std::vector<double> expectedV13(newX13.size(), 0.0), expectedF13(newX13.size(), 0.0);
    std::vector<double> expectedM13(newX13.size() / 3, 0.0), expectedR13(newX13.size() / 3, 0.0);
    std::vector<double> expectedV14(newX14.size(), 0.0), expectedF14(newX14.size(), 0.0);
    std::vector<double> expectedM14(newX14.size() / 3, 0.0), expectedR14(newX13.size() / 3, 0.0);
    std::vector<double> expectedV15(newX15.size(), 0.0), expectedF15(newX15.size(), 0.0);
    std::vector<double> expectedM15(newX15.size() / 3, 0.0), expectedR15(newX15.size() / 3, 0.0);
    std::vector<double> expectedV17(newX17.size(), 0.0), expectedF17(newX17.size(), 0.0);
    std::vector<double> expectedM17(newX17.size() / 3, 0.0), expectedR17(newX17.size() / 3, 0.0);
    std::vector<double> expectedV18(newX18.size(), 0.0), expectedF18(newX18.size(), 0.0);
    std::vector<double> expectedM18(newX18.size() / 3, 0.0), expectedR18(newX18.size() / 3, 0.0);
    std::vector<double> expectedV23(newX23.size(), 0.0), expectedF23(newX23.size(), 0.0);
    std::vector<double> expectedM23(newX23.size() / 3, 0.0), expectedR23(newX23.size() / 3, 0.0);


    std::vector<std::vector<double>> newXVec =
    {
        {}, {}, {}, {}, newX5,
        {}, {}, {}, {}, {},
        newX11, {}, newX13, newX14, newX15, 
        {}, newX17, newX18, {}, {},
        {}, {}, newX23, {}, {},
        {}, {}
    };
    std::vector<std::vector<double>> newVVec =
    {
        {}, {}, {}, {}, newV5,
        {}, {}, {}, {}, {},
        newV11, {}, newV13, newV14, newV15,
        {}, newV17, newV18, {}, {},
        {}, {}, newV23, {}, {},
        {}, {}
    };
    std::vector<std::vector<double>> newFVec =
    {
        {}, {}, {}, {}, newF5,
        {}, {}, {}, {}, {},
        newF11, {}, newF13, newF14, newF15,
        {}, newF17, newF18, {}, {},
        {}, {}, newF23, {}, {},
        {}, {}
    };
    std::vector<std::vector<double>> newRVec =
    {
        {}, {}, {}, {}, newR5,
        {}, {}, {}, {}, {},
        newR11, {}, newR13, newR14, newR15,
        {}, newR17, newR18, {}, {},
        {}, {}, newR23, {}, {},
        {}, {}
    };
    std::vector<std::vector<double>> newMVec =
    {
        {}, {}, {}, {}, newM5,
        {}, {}, {}, {}, {},
        newM11, {}, newM13, newM14, newM15,
        {}, newM17, newM18, {}, {},
        {}, {}, newM23, {}, {},
        {}, {}
    };


    std::vector<std::vector<double>> expectedXVec =
    {
        {}, {}, {}, {}, expectedX5,
        {}, {}, {}, {}, {},
        expectedX11, {}, expectedX13, expectedX14, expectedX15,
        {}, expectedX17, expectedX18, {}, {},
        {}, {}, expectedX23, {}, {},
        {}, {}
    };

    std::vector<std::vector<double>> expectedVVec =
    {
        {}, {}, {}, {}, expectedV5,
        {}, {}, {}, {}, {},
        expectedV11, {}, expectedV13, expectedV14, expectedV15,
        {}, expectedV17, expectedV18, {}, {},
        {}, {}, expectedV23, {}, {},
        {}, {}
    };
    std::vector<std::vector<double>> expectedFVec =
    {
        {}, {}, {}, {}, expectedF5,
        {}, {}, {}, {}, {},
        expectedF11, {}, expectedF13, expectedF14, expectedF15,
        {}, expectedF17, expectedF18, {}, {},
        {}, {}, expectedF23, {}, {},
        {}, {}
    };
    std::vector<std::vector<double>> expectedRVec =
    {
        {}, {}, {}, {}, expectedR5,
        {}, {}, {}, {}, {},
        expectedR11, {}, expectedR13, expectedR14, expectedR15,
        {}, expectedR17, expectedR18, {}, {},
        {}, {}, expectedR23, {}, {},
        {}, {}
    };
    std::vector<std::vector<double>> expectedMVec =
    {
        {}, {}, {}, {}, expectedM5,
        {}, {}, {}, {}, {},
        expectedM11, {}, expectedM13, expectedM14, expectedM15,
        {}, expectedM17, expectedM18, {}, {},
        {}, {}, expectedM23, {}, {},
        {}, {}
    };




    // running configuration
    std::array<int, 3> ranks = { 3,3,3 };
    // 
    int nranks = ranks[0]*ranks[1]*ranks[2];
    // ids 
    std::vector<int> ids;
    ids.reserve(nranks);
    // 
    for (int i = 0; i < nranks; i++)
        ids.push_back(i);



    // Engine vector
    std::vector<std::unique_ptr<Engine>> engineArray;

    // creating engine_ptrs
    for (int i = 0; i < nranks; i++) {
        auto engine_ptr = build_engine_set_particles(
            ids[i], ranks, newXVec[i], newVVec[i], newFVec[i], newRVec[i], newMVec[i]);
        engineArray.push_back(std::move(engine_ptr));
    }


    std::vector<Communicator*> communicatorArray;

    for (auto& engine_ptr : engineArray) {
        auto& communicatorRef = engine_ptr->getCommunicator();
        communicatorArray.push_back(communicatorRef.get());
    }

    std::vector<std::array<int, 6>> exchangeDestArray(nranks, std::array<int, 6>{});



    std::vector<double>* messagesArray[27];

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
        nDestsTotal = 0;
        numberOfAttempts++;

        for (int i = 0; i < nranks; i++) {
            exchangeDestArray[i] = communicatorArray[i]->returnExchangeDests();

            // an array of vector<double> is returned for each communicatoriRef
            messagesArray[i] = communicatorArray[i]->sendExchangeParticles();

        }

        // getting the nDests values
        for (int i = 0; i < nranks; i++)
            nDestsTotal += communicatorArray[i]->getNDests();


        // ranks
        for (int i = 0; i < nranks; i++) {
            // directions xlo, xhi, ylo, yhi, zlo, zhi
            for (int j = 0; j < 6; j++) {
                // ref is very important
                // since the recvExchangeParticles
                // after setting the new particle 
                // needs to reset the message
                auto& message = messagesArray[i][j];
                if (exchangeDestArray[i][j] < 0)
                    continue;
                communicatorArray[exchangeDestArray[i][j]]->recvExchangeParticles(message);
            }
        }

    } while (nDestsTotal > 0 && numberOfAttempts < maxAttempts);


    // ranks
    for (int i = 0; i < nranks; i++) {
        int id = i;
        Engine* engine = engineArray[id].get();
        checking_communicator(
            id, engine,
            expectedXVec,
            expectedVVec,
            expectedFVec,
            expectedRVec,
            expectedMVec
        );
    }
}

TEST_CASE("Testing the sendGhost function to transfer the interior particles") {
    std::cout << "Testing the sendGhost function to transfer the interior particles" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    double skin = 50.0;

    // x values
    // everthing here belongs to the rank3
    // so no need for ghost transfer.. just
    // wanted to check the interior particles (ghosts for other ranks) detection 
    std::vector<double> newX3 = {
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
    std::vector<double> newV3 = {
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
    std::vector<double> newF3 = {
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
    std::vector<double> newR3 = {
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
    std::vector<double> newM3 = {
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


    // expected results
    // rank4
    int expectednParticles1 = 5;
    // rank1
    int expectednParticles2 = 6;
    // rank7
    int expectednParticles5 = 5;
    // rank 4
    // since the order of particles is not known in advance we
    // need to check X, V, ... separately
    std::vector<double> expectedX1 = {
         -28.20,  180.66,  -66.03,   // particle 14 owned by Q01 (-+ interior) region 3 ghost for region 4
         -18.40,  +22.10,  -95.50,   // particle 8  owned by Q10 (-+ interior) region 3 ghost for region 4
         -40.11,  170.25, -204.67,   // particle 18 owned by Q01 (-+ interior) region 3 ghost for region 4
         -28.20,  150.66,  -36.03,   // particle 20 owneb by Q01 (-+ interior) region 3 ghost for region 4
         -18.15,   30.89,  -12.09    // particle 22 owned by Q01 (-+ interior) region 3 ghost for region 4
    };
    std::vector<double> expectedX2 = {
        -120.33,   20.18, -199.05,   // particle 12 owned by Q01 (-+ interior) region 3 ghost for region 1
        -180.66,   30.77,  -74.95,   // particle 2  owned by Q00 (-+ interior) region 3 ghost for region 1
         -18.40,  +22.10,  -95.50,   // particle 8  owned by Q10 (-+ interior) region 3 ghost for region 1
        -150.33,   20.18,  -25.05,   // particle 21 owned by Q01 (-+ interior) region 3 ghost for region 1
         -18.15,   30.89,  -12.09,   // particle 22 owned by Q01 (-+ interior) region 3 ghost for region 1
        -190.99,   27.46, -150.00    // particle 23 owned by Q01 (-+ interior) region 3 ghost for region 1
    };
    std::vector<double> expectedX5 = {
        -200.90,  190.36,  -40.58,   // particle 10 owned by Q01 (-+ interior) region 3 ghost for region 7
        -128.20,  130.66,  -46.03,   // particle 19 owneb by Q01 (-+ interior) region 3 ghost for region 7
         -28.20,  150.66,  -36.03,   // particle 20 owneb by Q01 (-+ interior) region 3 ghost for region 7
        -150.33,   20.18,  -25.05,   // particle 21 owned by Q01 (-+ interior) region 3 ghost for region 7
         -18.15,   30.89,  -12.09    // particle 22 owned by Q01 (-+ interior) region 3 ghost for region 7
    };
    std::vector<double> expectedV1 = {
       -53.7,  27.5, -41.6,   // particle 14
        59.7, -24.6,  10.9,   // particle 8
        57.8, -69.2,  39.4,   // particle 18
        31.5, -75.3,  25.6,   // particle 20
        66.2,  -8.1,  47.3    // particle 22
    };
    std::vector<double> expectedV2 = {
        46.1, -61.5,  33.8,   // particle 12
        41.2, -19.8,  63.5,   // particle 2
        59.7, -24.6,  10.9,   // particle 8
       -72.4,  18.7, -54.9,   // particle 21
        66.2,  -8.1,  47.3,   // particle 22
        21.7, -38.4,  68.2    // particle 23
    };
    std::vector<double> expectedV5 = {
        21.7, -38.4,  68.2,   // particle 10
         4.5,  64.3,  12.9,   // particle 19
        31.5, -75.3,  25.6,   // particle 20
       -72.4,  18.7, -54.9,   // particle 21
        66.2,  -8.1,  47.3    // particle 22
    };
    std::vector<double> expectedF1 = {
       -25.7,  17.8, -38.6,   // particle 14
        47.3, -19.5,   6.2,   // particle 8
        28.6, -31.4,  19.3,   // particle 18
        17.8, -39.2,  19.4,   // particle 20
        21.5, -35.3,  15.6,   // particle 22
    };
    std::vector<double> expectedF2 = {
        41.2, -29.8,   9.7,   // particle 12
        29.7,  -5.4,  38.9,   // particle 2
        47.3, -19.5,   6.2,   // particle 8
         4.5,  24.3,  12.9,   // particle 21
        21.5, -35.3,  15.6,   // particle 22
        21.7, -38.4,  18.2,   // particle 23
    };
    std::vector<double> expectedF5 = {
        14.8,  -7.3,  39.6,   // particle 10
        29.7, -24.6,  10.9,   // particle 19
        17.8, -39.2,  19.4,   // particle 20
         4.5,  24.3,  12.9,   // particle 21
        21.5, -35.3,  15.6,   // particle 22
    };
    std::vector<double> expectedR1 = {
        37.6,  // particle 14 region 3
        76.2,  // particle 8  region 3
        58.9,  // particle 18 region 3
        14.9,  // particle 20 region 3 
        22.6,  // particle 22 region 3
    };
    std::vector<double> expectedR2 = {
        19.8,  // particle 12 region 3
        45.7,  // particle 2  region 3
        76.2,  // particle 8  region 3
        69.1,  // particle 21 region 3
        22.6,  // particle 22 region 3
        12.5   // particle 23 region 3
    };
    std::vector<double> expectedR5 = {
         8.7,  // particle 10 region 3
        57.8,  // particle 19 region 3 
        14.9,  // particle 20 region 3 
        69.1,  // particle 21 region 3
        22.6,  // particle 22 region 3 
    };
    std::vector<double> expectedM1 = {
        5.9,  // particle 14 region 3
        8.4,  // particle 8  region 3
        1.8,  // particle 18 region 3
       18.6,  // particle 20 region 3
       10.8,  // particle 22 region 3
    };
    std::vector<double> expectedM2 = {
        3.0,  // particle 12 region 3
        0.9,  // particle 2  region 3
        8.4,  // particle 8  region 3
        3.1,  // particle 21 region 3
       10.8,  // particle 22 region 3
        8.5   // particle 23 region 3
    };
    std::vector<double> expectedM5 = {
        0.5,  // particle 10 region 3
        7.2,  // particle 19 region 3
       18.6,  // particle 20 region 3
        3.1,  // particle 21 region 3
       10.8,  // particle 22 region 3
    };


    std::vector<int> expectednParticlesVec =
    {
        expectednParticles1, expectednParticles2, expectednParticles5
    };
    std::vector<std::vector<double>> expectedXVec =
    {
        expectedX1, expectedX2, expectedX5
    };
    std::vector<std::vector<double>> expectedVVec =
    {
        expectedV1, expectedV2, expectedV5
    };
    std::vector<std::vector<double>> expectedFVec =
    {
        expectedF1, expectedF2, expectedF5
    };
    std::vector<std::vector<double>> expectedRVec =
    {
        expectedR1, expectedR2, expectedR5
    };
    std::vector<std::vector<double>> expectedMVec =
    {
        expectedM1, expectedM2, expectedM5
    };



    // sending data
    std::array<int, 3> ranks = { 2,2,2 };
    int id = 2;
    // building the engine
    // since every particle is the local particle
    // it is fine to first set the particles
    // prior to building the engine
    auto engine_ptr =
        set_particles_build_engine(id, ranks, newX3, newV3, newF3, newR3, newM3,skin);
    // getting the communicator pointer
    auto* communicator_ptr = engine_ptr->getCommunicator().get();

    // sending the interior particles
    std::vector<double> transData[6];
    // sending to the rank4
    // xhi
    communicator_ptr->sendGhosts(3, transData[1]);
    // sending to the rank1
    // ylo
    communicator_ptr->sendGhosts(0, transData[2]);
    // sending to the rank7
    // zhi
    communicator_ptr->sendGhosts(6, transData[5]);

    // checking the message
    // at first attempt the message is just send to the immediate neighboring ranks
    // 1, 4, 7
    // It is the job of those neighbors to resend the ghost particles
    // to non-immediate neighboring ranks of 2,5,6,8
    // The non-immediate case is tested in the next test
    // That situation is more like a regression test
    int output_indxs[] = { 1,2,5 };


    std::vector<double> X, V, F, R, M;
    for (int i = 0; i < 3; i++) {
        auto transDatai = transData[output_indxs[i]];
        int nParticles = static_cast<int>(transDatai[0]);
        // checking the number of particles
        REQUIRE(nParticles == expectednParticlesVec[i]);
        // outputs 
        X.clear(); V.clear(); F.clear(); R.clear(); M.clear();
        X.reserve(3 * nParticles); V.reserve(3 * nParticles); F.reserve(3 * nParticles);
        R.reserve(nParticles); M.reserve(nParticles);

        int dataLoc = 1;
        for (int j = 0; j < nParticles; j++) {
            // the id
            dataLoc++;
            X.push_back(transDatai[dataLoc++]);
            X.push_back(transDatai[dataLoc++]);
            X.push_back(transDatai[dataLoc++]);
            V.push_back(transDatai[dataLoc++]);
            V.push_back(transDatai[dataLoc++]);
            V.push_back(transDatai[dataLoc++]);
            F.push_back(transDatai[dataLoc++]);
            F.push_back(transDatai[dataLoc++]);
            F.push_back(transDatai[dataLoc++]);
            M.push_back(transDatai[dataLoc++]);
            R.push_back(transDatai[dataLoc++]);
        }



        // checking the results
        REQUIRE_THAT(X.data(), Array3DMatcher(expectedXVec[i].data(), nParticles, 1e-6));
        REQUIRE_THAT(V.data(), Array3DMatcher(expectedVVec[i].data(), nParticles, 1e-6));
        REQUIRE_THAT(F.data(), Array3DMatcher(expectedFVec[i].data(), nParticles, 1e-6));
        REQUIRE_THAT(R.data(), Array1DMatcher(expectedRVec[i].data(), nParticles, 1e-6));
        REQUIRE_THAT(M.data(), Array1DMatcher(expectedMVec[i].data(), nParticles, 1e-6));
    }


}

TEST_CASE("Testing the recvGhost function to receive the ghost particles")
{
    std::cout << "Testing the recvGhost function to receive the ghost particles" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

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
    int myId = 13;
    double skin = 30.0;

    int nlocal = 3;
    int nghost = 3;
    std::vector<int> newId14 = {
        1, 25, 26, 5, 6, 7
    };

    std::vector<double> newX14 = {
        // ---- local particles (inside domain) ----
         42.7, -68.78,  64.20,  // particle 1  (local)
        -68.3,  49.09, -33.10,  // particle 25  (local)
        -91.4, -55.55,  98.09,  // particle 26  (local)

        // ---- ghost particles (>= 30 outside [-100,100]) ----
        124.6,   84.08,  63.02,  // particle 5  (ghost)
       -127.2,   46.05, -78.01,  // particle 6  (ghost)
        112.9, -105.05,  12.05   // particle 7  (ghost)
    };

    std::vector<double> newV14(newX14.size(),0.0);
    std::vector<double> newF14(newX14.size(), 0.0);
    std::vector<double> newR14(newX14.size() / 3, 0.0);
    std::vector<double> newM14(newX14.size() / 3, 0.0);

    std::vector<double> transData[6];

    std::vector<double> transDataVec[6];

    // xlo 
    transDataVec[0] = {
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

    transDataVec[1] = {
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

    transDataVec[2] = {
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

    transDataVec[3] = {
        1.0,


        14.0,
       -121.7, 110.8, 125.9,  // x, y, z
        -71.5,  63.1,  29.4,  // vx, vy, vz
        -16.9, -34.2,  12.5,  // fx, fy, fz
          8.2,
         27.4,
    };

    transDataVec[4] = {
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

    transDataVec[5] = {
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

    // filling transData with the transDataVec elements
    
    for (int i = 0; i < 6; i++)
    {
        int nElements = transDataVec[i].size();
        std::vector<double> tempArray;
        for (int j = 0; j < nElements; j++)
            tempArray.push_back(transDataVec[i][j]);
        // The memory allocated by the
        // tempArray is passed to the transData
        // and later recvGhosts function will deallocate
        // this memory
        transData[i] = tempArray;
    }

    // expected results
    int expectedNLocal = 3;
    int expectedNGhost = 17;
    std::vector<int> expectedIds = {
        1,25,26,
        5,6,7,3,12,8,4,6,11,14,13,15,18,16,17,19,21,22
    };
    std::vector<double> expectedX14 = {
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

    std::vector<double> expectedV14 = {
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


    std::vector<double> expectedF14 = {
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

    std::vector<double> expectedM14 = {
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

    std::vector<double> expectedR14 = {
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

    // building the engine
    auto engine_ptr = set_particles_build_engine(
        myId, ranks, newX14, newV14, newF14, newR14, newM14, skin);

    // getting the reference to the communicator
    auto& communicatorRef = engine_ptr->getCommunicator();
    // checking the reference
    REQUIRE(communicatorRef);
    // receiving the messages
    for (int i = 0; i < 6; i++)
        communicatorRef->recvGhosts(transData[i]);

    // getting the particles
    auto& particlesRef = engine_ptr->getParticles();
    // checking the particles Ref
    REQUIRE(particlesRef);
    // checking the results
    int*  mId  = particlesRef->getIdData();
    auto* myX  = particlesRef->getXData();
    auto* myV  = particlesRef->getVData();
    auto* myF  = particlesRef->getFData();
    auto* myM  = particlesRef->getMData();
    auto* myR  = particlesRef->getRData();



    //REQUIRE_THAT(mId, Array1DMatcherInt(expectedIds.data(), expectedNLocal, expectedNGhost));
    REQUIRE_THAT(myX,  Array3DMatcher(expectedX14.data(), expectedNLocal, expectedNGhost, 1e-6));
    REQUIRE_THAT(myV,  Array3DMatcher(expectedV14.data(), expectedNLocal, expectedNGhost, 1e-6));
    REQUIRE_THAT(myF,  Array3DMatcher(expectedF14.data(), expectedNLocal, expectedNGhost, 1e-6));
    REQUIRE_THAT(myM,  Array1DMatcher(expectedM14.data(), expectedNLocal, expectedNGhost, 1e-6));
    REQUIRE_THAT(myR,  Array1DMatcher(expectedR14.data(), expectedNLocal, expectedNGhost, 1e-6));
}


TEST_CASE("Testing the movement of particles for the case with the skin value of 50")
{
    std::cout << "Testing the movement of particles for the case with the skin value of 50" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    double skin = 50.0;

    // id values
    std::vector<int> newId1 = {
        0,1,2,3,4
    };
    std::vector<int> newId2 = {
        5,6,9,7,8
    };
    std::vector<int> newId3 = {
        10,11,12,13,14
    };
    std::vector<int> newId4 = {
        15,16,17,18,19
    };

    //  x values
    std::vector<double> newX1 =
    {
        -220.40, -180.10,   80.55,   // particle 0  (-- interior) region 1
        -140.75, -260.33, -120.18,   // particle 1  (-- interior) region 1
        -180.66,   30.77,   14.95,   // particle 2  (-+ interior) region 3
          12.60,  -18.90,  130.10,   // particle 3  (+- interior) region 2
          35.10, -140.48,   12.66,   // particle 4  (+- interior) region 2
    };

    int newNGhost1 = 0;

    std::vector<double> newX2 =
    {
         180.62, -210.44,   60.91,   // particle 5  (+- interior) region 2
         260.11, -140.88, -200.30,   // particle 6  (+- interior) region 2
         190.27,  -40.22, -236.70,   // particle 9  (+- interior) region 2
         -25.60, -160.15,  -55.27,   // particle 7  (-- interior) region 1
         -18.40,  +22.10,   95.50,   // particle 8  (-+ interior) region 3
    };

    int newNGhost2 = 0;

    std::vector<double> newX3 =
    {
        -200.90,  190.36,  -40.58,   // particle 10 (-+ interior) region 3
        -270.22,  120.74,  210.46,   // particle 11 (-+ interior) region 3
        -120.33,   20.18, -199.05,   // particle 12 (-+ interior) region 3
          22.80,   18.10, -140.25,   // particle 13 (++ interior) region 4
         -28.20,  180.66,   66.03,   // particle 14 (-+ interior) region 3
    };

    int newNGhost3 = 0;

    std::vector<double> newX4 =
    {
         210.83,  220.41, -150.88,   // particle 15 (++ interior) region 4
         140.62,  160.33,  198.21,   // particle 16 (++ interior) region 4
         -30.25,  -22.40,   10.66,   // particle 17 (-- interior) region 1
         -40.11,  170.25,  204.67,   // particle 18 (-+ interior) region 3
         160.81,  -35.42,  289.73,   // particle 19 (+- interior) region 2
    };

    int newNGhost4 = 0;

    // expected id values
    std::vector<int> expectedIds1 = {
        0,1,7,17,
        3,4,12,2,8,13
    };
    std::vector<int> expectedIds2 = {
        5,6,9,3,4,
        7,17,8,13
    };
    std::vector<int> expectedIds3 = {
        10,11,12,14,2,8,18,
        17,3,13
    };
    std::vector<int> expectedIds4 = {
        15,16,13,
        17,9,3,4,8,18
    };

    // expected x values
    std::vector<double> expectedX1 = {
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

    std::vector<double> expectedX2 = {
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

    std::vector<double> expectedX3 = {
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

    std::vector<double> expectedX4 = {
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

    std::vector<double> newV1 =
    {
        72.4, -33.1,  95.0,   // particle 0
       -88.6,  14.3, -56.7,   // particle 1
        41.2, -19.8,  63.5,   // particle 2
       -97.1,  28.9, -74.4,   // particle 3
         3.6,  89.2, -12.5,   // particle 4
    };

    std::vector<double> newV2 =
    {
        54.7, -66.3,  77.1,   // particle 5
       -45.9,  18.0,  92.6,   // particle 6
       -72.5,  44.3, -95.8,   // particle 9
       -11.4,  36.8, -83.2,   // particle 7
        59.7, -24.6,  10.9,   // particle 8
    };

    std::vector<double> newV3 =
    {
        21.7, -38.4,  68.2,   // particle 10
       -14.9,  87.6, -29.3,   // particle 11
        46.1, -61.5,  33.8,   // particle 12
       -79.2,  12.4,  99.1,   // particle 13
       -53.7,  27.5, -41.6,   // particle 14
    };

    std::vector<double> newV4 =
    {
        64.0, -22.8,  15.2,   // particle 15
       -90.3,  48.6,  -7.4,   // particle 16
        81.9, -35.0,  24.1,   // particle 17
        57.8, -69.2,  39.4,   // particle 18
       -16.7,  73.3, -58.9    // particle 19
    };

    // expected velocity values
    std::vector<double> expectedV1 = {
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

    std::vector<double> expectedV2 = {
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

    std::vector<double> expectedV3 = {
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

    std::vector<double> expectedV4 = {
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

    // force values
    std::vector<double> newF1 =
    {
       12.4, -33.9,  45.2,   // particle 0
      -18.6,   7.3, -41.5,   // particle 1
       29.7,  -5.4,  38.9,   // particle 2
      -22.1,  16.0, -49.7,   // particle 3
        3.1,  27.6, -14.8,   // particle 4
    };

    std::vector<double> newF2 =
    {
        44.5, -36.7,  21.4,   // particle 5
        -9.9,   5.6,  31.2,   // particle 6
       -34.8,  25.1, -45.6,   // particle 9
       -12.7,  18.9, -28.4,   // particle 7
        47.3, -19.5,   6.2,   // particle 8
    };

    std::vector<double> newF3 =
    {
        14.8,  -7.3,  39.6,   // particle 10
       -21.9,  30.4, -16.5,   // particle 11
        41.2, -29.8,   9.7,   // particle 12
       -48.1,  12.6,  33.9,   // particle 13
       -25.7,  17.8, -38.6,   // particle 14
    };

    std::vector<double> newF4 =
    {
        26.4, -11.3,   4.9,   // particle 15
       -43.2,  22.5,  -6.8,   // particle 16
        35.0, -18.1,  13.7,   // particle 17
        28.6, -31.4,  19.3,   // particle 18
        -8.2,  46.7, -24.5    // particle 19
    };



    // expected force values
    std::vector<double> expectedF1 = {
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

    std::vector<double> expectedF2 = {
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

    std::vector<double> expectedF3 = {
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

    std::vector<double> expectedF4 = {
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

    // radius values
    std::vector<double> newR1 =
    {
     12.5,  // particle 0 region 1
     89.3,  // particle 1 region 1
     45.7,  // particle 2 region 3
     3.9,   // particle 3 region 2
     67.1,  // particle 4 region 2
    };

    std::vector<double> newR2 =
    {
     24.6,  // particle 5 region 2
     98.0,  // particle 6 region 2
     54.3,  // particle 9 region 2
     31.4,  // particle 7 region 1
     76.2,  // particle 8 region 3
    };

    std::vector<double> newR3 =
    {
     8.7,   // particle 10 region 3
     63.5,  // particle 11 region 3
     19.8,  // particle 12 region 3
     92.4,  // particle 13 region 4
     37.6,  // particle 14 region 3
    };

    std::vector<double> newR4 =
    {
     71.0,  // particle 15 region 4
     14.2,  // particle 16 region 4
     83.1,  // particle 17 region 1
     58.9,  // particle 18 region 3
     26.4   // particle 19 region 2
    };

    // expected radius values
    std::vector<double> expectedR1 = {
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

    std::vector<double> expectedR2 = {
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

    std::vector<double> expectedR3 = {
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

    std::vector<double> expectedR4 = {
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


    // mass values
    std::vector<double> newM1 =
    {
        3.4,  // particle 0 region 1
        7.8,  // particle 1 region 1
        0.9,  // particle 2 region 3
        5.6,  // particle 3 region 2
        9.1,  // particle 4 region 2
    };

    std::vector<double> newM2 =
    {
        2.7,  // particle 5 region 2
        6.0,  // particle 6 region 2
        4.1,  // particle 9 region 2
        1.3,  // particle 7 region 1
        8.4,  // particle 8 region 3
    };

    std::vector<double> newM3 =
    {
        0.5,  // particle 10 region 3
        9.8,  // particle 11 region 3
        3.0,  // particle 12 region 3
        7.4,  // particle 13 region 4
        5.9,  // particle 14 region 3
    };

    std::vector<double> newM4 =
    {
        2.1,  // particle 15 region 4
        6.7,  // particle 16 region 4
        8.2,  // particle 17 region 1
        1.8,  // particle 18 region 3
        4.6   // particle 19 region 2
    };

    // expected mass values
    std::vector<double> expectedM1 = {
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

    std::vector<double> expectedM2 = {
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

    std::vector<double> expectedM3 = {
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

    std::vector<double> expectedM4 = {
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

    // new  vectors
    std::vector<std::vector<int>> newIdVec = {
        newId1, newId2, newId3, newId4
    };
    std::vector<std::vector<double>> newXVec =
    {
        newX1, newX2, newX3, newX4
    };
    std::vector<int> newNGhostVec = {
        newNGhost1, newNGhost2, newNGhost3, newNGhost4
    };
    std::vector<std::vector<double>> newVVec =
    {
        newV1, newV2, newV3, newV4
    };
    std::vector<std::vector<double>> newFVec =
    {
        newF1, newF2, newF3, newF4
    };
    std::vector<std::vector<double>> newRVec =
    {
        newR1, newR2, newR3, newR4
    };
    std::vector<std::vector<double>> newMVec =
    {
        newM1, newM2, newM3, newM4
    };

    // expected values
    std::vector<std::vector<int>> expectedIdVec = {
        expectedIds1, expectedIds2, expectedIds3, expectedIds4
    };
    std::vector<std::vector<double>> expectedXVec =
    {
        expectedX1, expectedX2, expectedX3, expectedX4
    };
    std::vector<std::vector<double>> expectedVVec =
    {
        expectedV1, expectedV2, expectedV3, expectedV4
    };
    std::vector<std::vector<double>> expectedFVec =
    {
        expectedF1, expectedF2, expectedF3, expectedF4
    };
    std::vector<std::vector<double>> expectedRVec =
    {
        expectedR1, expectedR2, expectedR3, expectedR4
    };
    std::vector<std::vector<double>> expectedMVec =
    {
        expectedM1, expectedM2, expectedM3, expectedM4
    };

    // running configuration
    std::array<int, 3> ranks = { 2,2,1 };
    //
    int nranks = ranks[0] * ranks[1] * ranks[2];
    // ids 
    std::vector<int> ids;
    for (int i = 0; i < nranks; i++)
        ids.push_back(i);

    std::vector<unique_ptr<Engine>> engineArray;

    for (int i = 0; i < nranks; i++)
    {
        auto engine_ptr =
            build_engine_set_particles(
                ids[i], ranks,
                newIdVec[i],
                newXVec[i], newVVec[i], newFVec[i],
                newRVec[i], newMVec[i],
                newNGhostVec[i],
                skin
            );
        engineArray.push_back(std::move(engine_ptr));
    }

    std::vector<Communicator*> communicatorArray;

    for (int i = 0; i < nranks; i++) {
        communicatorArray.push_back(engineArray[i]->getCommunicator().get());
    }

    std::vector<std::array<int, 6>> exchangeDestArray(nranks, std::array<int, 6>{});

    // each std::vector<double>* for each rank has six
    // std::vector<double> vectors
    // So three dimensions
    // NRanks X 6 (directions) X number of moved particles
    std::vector<double>* messagesArray[4];

    // sending the ghost particles for neighbors
    std::vector<double> transData[4];


    // number of partical and ghost exchanges 
    int nExchangeTotal = 0;

    int numberOfAttempts = 0;


    constexpr int maxAttempts = 4;


    // setting the interior particles (ghosts for neighboring particles)
    for (auto& communicator : communicatorArray)
    {
        communicator->setInteriorParticles();
    }

    // sending the interior particles as ghosts
    for (auto& communicator : communicatorArray) {
        //communicator->sendGhosts()
    }

    for (auto& communicator : communicatorArray)
    {
        communicator->resetInterior();
        communicator->clearGhostInterior();
    }

    int nDestsTotal = 0;
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
        nDestsTotal = 0;
        numberOfAttempts++;

        for (int i = 0; i < 4; i++) {
            exchangeDestArray[i] = communicatorArray[i]->returnExchangeDests();
            // an array of vector<double> is returned for each communicatoriRef
            messagesArray[i] = communicatorArray[i]->sendExchangeParticles();
            communicatorArray[i]->resetInterior();
            communicatorArray[i]->resetGhostInterior();
            // directions xlo, xhi, ylo, yhi, zlo and zhi
            for (int j = 0; j < 6; j++)
            {
                //std::cout << "[" << i << "," << j << "] == " << exchangeDestArray[i][j] << std::endl;
                if (exchangeDestArray[i][j] < 0)
                    continue;
                int dest = exchangeDestArray[i][j];
                // sending the ghost particles info
                nDestsTotal += communicatorArray[i]->sendGhosts(dest, transData[dest]);
            }
        }

        // getting the nDests values
        for (int i = 0; i < 4; i++)
            nDestsTotal += communicatorArray[i]->getNDests();


        // ranks
        for (int i = 0; i < 4; i++) {
            // receiving ghosts
            communicatorArray[i]->recvGhosts(transData[i]);
            // directions xlo, xhi, ylo, yhi, zlo, zhi
            for (int j = 0; j < 6; j++) {
                // ref is very important
                // since the recvExchangeParticles
                // after setting the new particle 
                // needs to reset the message
                auto& message = messagesArray[i][j];
                if (exchangeDestArray[i][j] < 0)
                    continue;
                communicatorArray[exchangeDestArray[i][j]]->recvExchangeParticles(message);
            }
        }

    } while ( numberOfAttempts < maxAttempts);

    auto& o = engineArray[3]->getParticles();
    int nl, nm, ng;
    o->getNmaxNlocal(nm, nl);
    o->getNGhosts(ng);
    auto x = o->getXData();




    for (int i = 0; i < nranks; i++) {
        int id = ids[i];
        Engine* engine_ptr = engineArray[i].get();
        checking_communicator(
            id, engine_ptr,
            expectedXVec,
            expectedVVec,
            expectedFVec,
            expectedRVec,
            expectedMVec
        );
    }

}

