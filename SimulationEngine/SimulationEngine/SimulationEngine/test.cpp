#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>

#include "test.hpp"



TEST_CASE("Testing the movement of particles between processors without skin" ,"[.][ignore for now]")
{
    std::cout << "Testing the movement of particles between processors without skin" << std::endl;


    // Particles
    // Particles
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

    // moving particles
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
        3.4,  // particle 0
        7.8,  // particle 1
        0.9,  // particle 2
        5.6,  // particle 3
        9.1,  // particle 4
    };

    std::vector<double> newM1 =
    {
        3.4,  // particle 0
        7.8,  // particle 1
        0.9,  // particle 2
        5.6,  // particle 3
        9.1,  // particle 4
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
        2.7,  // particle 5
        6.0,  // particle 6
        1.3,  // particle 7
        8.4,  // particle 8
        4.1,  // particle 9
    };

    std::vector<double> newR2 = 
    {
        2.7,  // particle 5
        6.0,  // particle 6
        1.3,  // particle 7
        8.4,  // particle 8
        4.1,  // particle 9
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
        0.5,  // particle 10
        9.8,  // particle 11
        3.0,  // particle 12
        7.4,  // particle 13
        5.9,  // particle 14
    };

    std::vector<double> newR3 =
    {
        0.5,  // particle 10
        9.8,  // particle 11
        3.0,  // particle 12
        7.4,  // particle 13
        5.9,  // particle 14
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
       2.1,  // particle 15
        6.7,  // particle 16
        8.2,  // particle 17
        1.8,  // particle 18
        4.6   // particle 19
    };

    std::vector<double> newR4 =
    {
        2.1,  // particle 15
        6.7,  // particle 16
        8.2,  // particle 17
        1.8,  // particle 18
        4.6   // particle 19
    };

    // expected values for each core
    // core 1
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




    auto return_engine_ptr = [&](const int& myId_,
                                 std::array<int, 3> ranks_,
                                 std::vector<double>& x_,
                                 std::vector<double>& v_,
                                 std::vector<double>& f_,
                                 std::vector<double>& r_,
                                 std::vector<double>& m_) -> std::unique_ptr<Engine>
    {
            std::unique_ptr<Communicator> communicator = std::make_unique<Communicator>(myId_, ranks_);

            // box dimensions
            array<double, 3> min = { -300.0,-300.0,-300.0 };
            array<double, 3> max = { 300.0, 300.0, 300.0 };
            std::unique_ptr<Box> box = std::make_unique<Box>(min, max);

            // mocked particles
            std::unique_ptr<Particles> mockedParticles =
                std::make_unique<MockedParticles>();


            // calculating the nmax values
            int nmax = x_.size() / 3;
            // converting the pointer back

            std::vector<std::unique_ptr<Ref>> inputs;
            inputs.push_back(std::move(mockedParticles));
            inputs.push_back(std::move(box));
            inputs.push_back(std::move(communicator));


            // creating the engine fixture
            EngineFixture engineFixture(inputs);
            // getting the engine_ptr
            auto engine_ptr = engineFixture.returnEngine();
            /* After the engine_ptr is created in the engineFixture
             * the init function is called form every Ref object
             * in the engine one of them is the particles which
             * removes out of range particles.
             * So we need to return a reference to particles
             * from the engine and then reset particles coordinates here
             */
            // returning the particles
            auto& particlesRef = engine_ptr->getParticlesForUpdate();
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
            std::vector<double> xCopy = x_, vCopy = v_, fCopy = f_, rCopy = r_, mCopy = m_;
            mockedParticlesConv->resetParticles(nmax, xCopy, vCopy, fCopy, rCopy, mCopy);
            // returning the engine
            return engine_ptr;
    };



    // running configuration
    std::array<int, 3> ranks = { 2,2,1 };
    // ids 
    int ids[4] = {0,1,2,3};

    auto engine_ptr1 = return_engine_ptr(ids[0], ranks, newX1, newV1, newF1, newR1, newM1);
    auto engine_ptr2 = return_engine_ptr(ids[1], ranks, newX2, newV2, newF2, newR2, newM2);
    auto engine_ptr3 = return_engine_ptr(ids[2], ranks, newX3, newV3, newF3, newR3, newM3);
    auto engine_ptr4 = return_engine_ptr(ids[3], ranks, newX4, newV4, newF4, newR4, newM4);

    Engine* engineArray[] = {
        engine_ptr1.get(),
        engine_ptr2.get(),
        engine_ptr3.get(),
        engine_ptr4.get()
    };


    auto& particles1Ref = engine_ptr1->getParticles();
    auto& particles2Ref = engine_ptr2->getParticles();
    auto& particles3Ref = engine_ptr3->getParticles();
    auto& particles4Ref = engine_ptr4->getParticles();

    auto& communicator1Ref = engine_ptr1->getCommunicator();
    auto& communicator2Ref = engine_ptr2->getCommunicator();
    auto& communicator3Ref = engine_ptr3->getCommunicator();
    auto& communicator4Ref = engine_ptr4->getCommunicator();

    Communicator* communicatorArray[] = {
        communicator1Ref.get(),
        communicator2Ref.get(),
        communicator3Ref.get(),
        communicator4Ref.get()
    };



    // number of destinations
    int nDestsTotal = 0;

    int numberOfAttemps = 0;


    constexpr int maxAtempts = 4;
    // repeating the particle reassginement until there is no
    // outside particles in each communicator
    do {
        nDestsTotal = 0;

        std::cout << "The attemp #" << ++numberOfAttemps << std::endl;

        
        auto exchangeDests1 = communicator1Ref->returnExchangeDests();
        auto exchangeDests2 = communicator2Ref->returnExchangeDests();
        auto exchangeDests3 = communicator3Ref->returnExchangeDests();
        auto exchangeDests4 = communicator4Ref->returnExchangeDests();

        // an array of double is returned for each communicatoriRef
        std::vector<double>* messages1 = communicator1Ref->sendExchangeParticles();
        std::vector<double>* messages2 = communicator2Ref->sendExchangeParticles();
        std::vector<double>* messages3 = communicator3Ref->sendExchangeParticles();
        std::vector<double>* messages4 = communicator4Ref->sendExchangeParticles();
        


        // getting the nDests values
        for (int i = 0; i < 4; i++)
            nDestsTotal += communicatorArray[i]->getNDests();


        for (int i = 0; i < 6; i++) {
            auto message = messages1[i];
            if (exchangeDests1[i] < 0 || exchangeDests1[i] >= 4)
                continue;
            communicatorArray[exchangeDests1[i]]->recvExchangeParticles(message);
        }
        for (int i = 0; i < 6; i++) {
            auto message = messages2[i];
            if (exchangeDests2[i] < 0 || exchangeDests2[i] >= 4)
                continue;
            communicatorArray[exchangeDests2[i]]->recvExchangeParticles(message);
        }
        for (int i = 0; i < 6; i++) {
            auto message = messages3[i];
            if (exchangeDests3[i] < 0 || exchangeDests3[i] >= 4)
                continue;
            communicatorArray[exchangeDests3[i]]->recvExchangeParticles(message);
        }
        for (int i = 0; i < 6; i++) {
            std::cout << "4" << std::endl;
            std::cout << exchangeDests4[i] << std::endl;
            auto message = messages4[i];
            if (exchangeDests4[i] < 0 || exchangeDests4[i] >= 4)
                continue;
            communicatorArray[exchangeDests4[i]]->recvExchangeParticles(message);
        }


    } while (nDestsTotal > 0 && numberOfAttemps < maxAtempts);


    auto checking_communicator = [&](const int& myId_, Engine* engine_) {
        // the number of particles
        int nmax, nlocal;
        // getting the particlesRef
        auto& particlesRef = engine_->getParticlesForUpdate();
        // getting xs, vs, fs, rs and ms
        double* myXs = particlesRef->getXData();
        double* myVs = particlesRef->getVData();
        double* myFs = particlesRef->getFData();
        double* myRs = particlesRef->getRData();
        double* myMs = particlesRef->getMData();
        // getting the expected values
        auto& expXs = expectedXsVec[myId_];
        auto& expVs = expectedVsVec[myId_];
        auto& expFs = expectedFsVec[myId_];
        auto& expRs = expectedRsVec[myId_];
        auto& expMs = expectedMsVec[myId_];
        // getting number of particles 
        particlesRef->getNmaxNlocal(nmax, nlocal);
        // checking the number of particles
        //REQUIRE(3 * nlocal == expXs.size());
        //REQUIRE(3 * nlocal == expVs.size());
        //REQUIRE(3 * nlocal == expFs.size());
        //REQUIRE(nlocal == expRs.size());
        //REQUIRE(nlocal == expMs.size());
        // checking per particle data
        for (int i = 0; i < 3 * nlocal; i++) {
            std::cout << myXs[i];
            if (i % 3 == 2)
                std::cout << std::endl;
            else
                std::cout << ",";
            //REQUIRE_THAT(expXs[i], Catch::Matchers::WithinAbs(myXs[i], 1e-6));
            //REQUIRE_THAT(expVs[i], Catch::Matchers::WithinAbs(myVs[i], 1e-6));
            //REQUIRE_THAT(expFs[i], Catch::Matchers::WithinAbs(myFs[i], 1e-6));
        }
        for (int i = 0; i < nlocal; i++) {
            //REQUIRE_THAT(expRs[i], Catch::Matchers::WithinAbs(myRs[i], 1e-6));
            //REQUIRE_THAT(expMs[i], Catch::Matchers::WithinAbs(myMs[i], 1e-6));
        }

    };

    // testing four ranks
    for (int i = 0; i < 4; i++) {
        std::cout << "Rank - " << i << std::endl;
        checking_communicator(ids[i], engineArray[i]);
    }
}
