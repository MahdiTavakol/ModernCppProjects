#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include <vector>
#include <array>
#include <memory>
#include <iostream>

#include "test.hpp"



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


    // forward communication
    // rank 1 
    int myId1 = 0, myId2 = 1, myId3 = 2, myId4 = 3;
    // transfered data
    transferedData tranData12;
    transferedData tranData13;
    transferedData tranData21;
    transferedData tranData24;
    transferedData tranData31;
    transferedData tranData34;
    transferedData tranData42;
    transferedData tranData43;
    // communicators
    std::unique_ptr<Communicator> communicator1 = std::make_unique<Communicator>(myId1, nranks, skin);
    std::unique_ptr<Communicator> communicator2 = std::make_unique<Communicator>(myId2, nranks, skin);
    std::unique_ptr<Communicator> communicator3 = std::make_unique<Communicator>(myId3, nranks, skin);
    std::unique_ptr<Communicator> communicator4 = std::make_unique<Communicator>(myId4, nranks, skin);
    // box dimensions
    array<double, 3> min = { -300.0,-300.0,-300.0 };
    array<double, 3> max = { 300.0, 300.0, 300.0 };
    // box
    std::unique_ptr<Box> box1 = std::make_unique<Box>(min, max);
    std::unique_ptr<Box> box2 = std::make_unique<Box>(min, max);
    std::unique_ptr<Box> box3 = std::make_unique<Box>(min, max);
    std::unique_ptr<Box> box4 = std::make_unique<Box>(min, max);
    // particles
    int nmax = 20;
    int nlocal = 20;
    int nghosts = 0;
    // copies since we used the std::move in the mockedParticles class
    std::vector<double> xCopy1 = x, xCopy2 = x, xCopy3 = x, xCopy4 = x;
    std::vector<double> vCopy1 = v, vCopy2 = v, vCopy3 = v, vCopy4 = v;
    std::vector<double> fCopy1 = f, fCopy2 = f, fCopy3 = f, fCopy4 = f;
    std::vector<double> rCopy1 = r, rCopy2 = r, rCopy3 = r, rCopy4 = r;
    std::vector<double> mCopy1 = m, mCopy2 = m, mCopy3 = m, mCopy4 = m;
    // creating the mockedParticles object
    std::unique_ptr<Particles> mockedParticles1 =
        std::make_unique<MockedParticles>(nmax, xCopy1, vCopy1, fCopy1, rCopy1, mCopy1);
    std::unique_ptr<Particles> mockedParticles2 =
        std::make_unique<MockedParticles>(nmax, xCopy2, vCopy2, fCopy2, rCopy2, mCopy2);
    std::unique_ptr<Particles> mockedParticles3 =
        std::make_unique<MockedParticles>(nmax, xCopy3, vCopy3, fCopy3, rCopy3, mCopy3);
    std::unique_ptr<Particles> mockedParticles4 =
        std::make_unique<MockedParticles>(nmax, xCopy4, vCopy4, fCopy4, rCopy4, mCopy4);
    // putting everything into the engine
    std::vector<std::unique_ptr<Ref>> inputs1;
    std::vector<std::unique_ptr<Ref>> inputs2;
    std::vector<std::unique_ptr<Ref>> inputs3;
    std::vector<std::unique_ptr<Ref>> inputs4;
    inputs1.push_back(std::move(mockedParticles1));
    inputs2.push_back(std::move(mockedParticles2));
    inputs3.push_back(std::move(mockedParticles3));
    inputs4.push_back(std::move(mockedParticles4));
    inputs1.push_back(std::move(box1));
    inputs2.push_back(std::move(box2));
    inputs3.push_back(std::move(box3));
    inputs4.push_back(std::move(box4));
    inputs1.push_back(std::move(communicator1));
    inputs2.push_back(std::move(communicator2));
    inputs3.push_back(std::move(communicator3));
    inputs4.push_back(std::move(communicator4));
    // creating the engine fixture
    EngineFixture engineFixture1(inputs1);
    EngineFixture engineFixture2(inputs2);
    EngineFixture engineFixture3(inputs3);
    EngineFixture engineFixture4(inputs4);
    // getting the engine_ptr
    auto engine_ptr1 = engineFixture1.returnEngine();
    auto engine_ptr2 = engineFixture2.returnEngine();
    auto engine_ptr3 = engineFixture3.returnEngine();
    auto engine_ptr4 = engineFixture4.returnEngine();
    // returning the communicators
    auto& communicator1Ref = engine_ptr1->getCommunicator();
    auto& communicator2Ref = engine_ptr2->getCommunicator();
    auto& communicator3Ref = engine_ptr3->getCommunicator();
    auto& communicator4Ref = engine_ptr4->getCommunicator();
    // sending data
    // sending data from rank1 to rank2
    communicator1Ref->forward_particle({1,0,0}, tranData12);
    // sending data from rank1 to rank3
    communicator1Ref->forward_particle({0,1,0}, tranData13);
    // sending data from rank2 to rank1
    communicator2Ref->forward_particle({ -1,0,0 }, tranData21);
    // sending data from rank2 to rank4
    communicator2Ref->forward_particle({ 0,1,0 }, tranData24);
    // sending data from rank3 to rank1
    communicator3Ref->forward_particle({ 0,-1,0 }, tranData31);
    // sending data from rank3 to rank4
    communicator3Ref->forward_particle({ 1,0,0 }, tranData34);
    // sending data from rank4 to rank3
    communicator4Ref->forward_particle({ -1,0,0 }, tranData43);
    // sending data from rank4 to rank2
    communicator4Ref->forward_particle({ 0,-1,0 }, tranData42);
    // receiving data
    auto& particles1Ref = engine_ptr1->getParticlesForUpdate();
    auto& particles2Ref = engine_ptr2->getParticlesForUpdate();
    auto& particles3Ref = engine_ptr3->getParticlesForUpdate();
    auto& particles4Ref = engine_ptr4->getParticlesForUpdate();
    // updating ghosts
    communicator1Ref->updateGhosts({ 1,0,0 },tranData21);
    communicator1Ref->updateGhosts({ 0,1,0 }, tranData31);
    communicator2Ref->updateGhosts({ -1,0,0 }, tranData12);
    communicator2Ref->updateGhosts({ 0,1,0 }, tranData42);
    communicator3Ref->updateGhosts({ 1,0,0 }, tranData43);
    communicator3Ref->updateGhosts({ 0,-1,0 }, tranData13);
    communicator4Ref->updateGhosts({ -1,0,0 }, tranData34);
    communicator4Ref->updateGhosts({ 0,-1,0 },tranData24);
    // checking data
    auto myXs1 = particles1Ref->getXData();
    auto myVs1 = particles1Ref->getVData();
    auto myFs1 = particles1Ref->getFData();
    auto myMs1 = particles1Ref->getMData();
    auto myRs1 = particles1Ref->getRData();
    auto myXs2 = particles2Ref->getXData();
    auto myVs2 = particles2Ref->getVData();
    auto myFs2 = particles2Ref->getFData();
    auto myMs2 = particles2Ref->getMData();
    auto myRs2 = particles2Ref->getRData();
    auto myXs3 = particles3Ref->getXData();
    auto myVs3 = particles3Ref->getVData();
    auto myFs3 = particles3Ref->getFData();
    auto myMs3 = particles3Ref->getMData();
    auto myRs3 = particles3Ref->getRData();
    auto myXs4 = particles4Ref->getXData();
    auto myVs4 = particles4Ref->getVData();
    auto myFs4 = particles4Ref->getFData();
    auto myMs4 = particles4Ref->getMData();
    auto myRs4 = particles4Ref->getRData();
    // natoms
    int natoms1, natoms2, natoms3, natoms4;
    int nlocal1, nlocal2, nlocal3, nlocal4;
    int nghost1, nghost2, nghost3, nghost4;
    particles1Ref->getNmaxNlocal(nmax, nlocal1);
    particles2Ref->getNmaxNlocal(nmax, nlocal2);
    particles3Ref->getNmaxNlocal(nmax, nlocal3);
    particles4Ref->getNmaxNlocal(nmax, nlocal4);
    particles1Ref->getNGhosts(nghost1);
    particles2Ref->getNGhosts(nghost2);
    particles3Ref->getNGhosts(nghost3);
    particles4Ref->getNGhosts(nghost4);
    natoms1 = nlocal1 + nghost1;
    natoms2 = nlocal2 + nghost2;
    natoms3 = nlocal3 + nghost3;
    natoms4 = nlocal4 + nghost4;
    // checking number of particles
    REQUIRE(expectedXs1.size()/3 == natoms1);
    REQUIRE(expectedXs2.size()/3 == natoms2);
    REQUIRE(expectedXs3.size()/3 == natoms3);
    REQUIRE(expectedXs4.size()/3 == natoms4);
    for (int i = 0; i < 3*natoms1; i++) {
        REQUIRE_THAT(expectedXs1[i], Catch::Matchers::WithinAbs(myXs1[i], 1e-6));
        REQUIRE_THAT(expectedVs1[i], Catch::Matchers::WithinAbs(myVs1[i], 1e-6));
        REQUIRE_THAT(expectedFs1[i], Catch::Matchers::WithinAbs(myFs1[i], 1e-6));
    }
    for (int i = 0; i < 3 * natoms2; i++) {
        REQUIRE_THAT(expectedXs2[i], Catch::Matchers::WithinAbs(myXs2[i], 1e-6));
        REQUIRE_THAT(expectedVs2[i], Catch::Matchers::WithinAbs(myVs2[i], 1e-6));
        REQUIRE_THAT(expectedFs2[i], Catch::Matchers::WithinAbs(myFs2[i], 1e-6));
    }
    for (int i = 0; i < 3 * natoms3; i++) {
        REQUIRE_THAT(expectedXs3[i], Catch::Matchers::WithinAbs(myXs3[i], 1e-6));
        REQUIRE_THAT(expectedVs3[i], Catch::Matchers::WithinAbs(myVs3[i], 1e-6));
        REQUIRE_THAT(expectedFs3[i], Catch::Matchers::WithinAbs(myFs3[i], 1e-6));
    }
    for (int i = 0; i < 3 * natoms4; i++) {
        REQUIRE_THAT(expectedXs4[i], Catch::Matchers::WithinAbs(myXs4[i], 1e-6));
        REQUIRE_THAT(expectedVs4[i], Catch::Matchers::WithinAbs(myVs4[i], 1e-6));
        REQUIRE_THAT(expectedFs4[i], Catch::Matchers::WithinAbs(myFs4[i], 1e-6));
    }
    for (int i = 0; i < natoms1; i++) {
        REQUIRE_THAT(expectedRs1[i], Catch::Matchers::WithinAbs(myRs1[i], 1e-6));
        REQUIRE_THAT(expectedMs1[i], Catch::Matchers::WithinAbs(myMs1[i], 1e-6));
    }
    for (int i = 0; i < natoms2; i++) {
        REQUIRE_THAT(expectedRs2[i], Catch::Matchers::WithinAbs(myRs2[i], 1e-6));
        REQUIRE_THAT(expectedMs2[i], Catch::Matchers::WithinAbs(myMs2[i], 1e-6));
    }

    for (int i = 0; i < natoms3; i++) {
        REQUIRE_THAT(expectedRs3[i], Catch::Matchers::WithinAbs(myRs3[i], 1e-6));
        REQUIRE_THAT(expectedMs3[i], Catch::Matchers::WithinAbs(myMs3[i], 1e-6));
    }

    for (int i = 0; i < natoms4; i++) {
        REQUIRE_THAT(expectedRs4[i], Catch::Matchers::WithinAbs(myRs4[i], 1e-6));
        REQUIRE_THAT(expectedMs4[i], Catch::Matchers::WithinAbs(myMs4[i], 1e-6));
    }

}