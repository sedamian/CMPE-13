// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"


// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "MatrixMath.h"
// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****

int main() {
    BOARD_Init();

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    int t11 = 0;
    int t12 = 0;

    int t13 = 0;
    int t4 = 0;

    int t5 = 0;
    int t6 = 0;

    int t7 = 0;
    int t8 = 0;

    int t9;








    float mat1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    float mat2[3][3] = {
        {1.1, 2.2, 3.3},
        {4.4, 5.5, 6.6},
        {7.7, 8.8, 9.9}
    };
    float mat3[3][3] = {
        {-1.0, 2.0, -3.0},
        {4.0, 5.0, 6.0},
        {7.0, -8.0, -9.0}
    };
    float mat4[3][3] = {
        {-1.5, 2.5, -3.0},
        {4.0, 5.5, 6.0},
        {7.0, -8.0, -9.0}
    };










    float result1[3][3];
    MatrixMultiply(mat1, mat2, result1);

    float test11[3][3] = {
        {33, 39.600002, 46.199996},
        {72.599998, 89.100006, 105.599998},
        {112.199996, 138.600006, 165.000000}
    };

    t11 = t11 + MatrixEquals(result1, test11);

    float test12[3][3] = {
        {28, -12, -18},
        {58, -15, -36},
        {88, -18, -54}
    };
    MatrixMultiply(mat1, mat3, result1);
    t11 = t11 + MatrixEquals(result1, test12);




    //////////MTADD//////////////////////////////////////////////////////////////

    float result2[3][3];
    MatrixAdd(mat1, mat2, result2);


    float test21[3][3] = {
        {2.100000, 4.200000, 6.300000},
        {8.400000, 10.500000, 12.6},
        {14.700000, 16.8, 18.9}
    };

    t12 = t12 + MatrixEquals(result2, test21);

    float test22[3][3] = {
        {0, 4, 0},
        {8, 10, 12},
        {14, 0, 0}
    };
    MatrixAdd(mat1, mat3, result2);

    t12 = t12 + MatrixEquals(result2, test22);





    //////////MTEquals//////////////////////////////////////////////////////////////

    float result3[3][3];
    MatrixAdd(mat1, mat2, result3);


    float test31[3][3] = {
        {2.100000, 4.200000, 6.300000},
        {8.400000, 10.500000, 12.6},
        {14.700000, 16.8, 18.9}
    };


    if (MatrixEquals(result3, test31) == 1) {
        t13 = t13 + 1;
    }

    float test32[3][3] = {
        {235.100000, -4.200000, 926.300000},
        {084.400900, -10.500000, 7812.6},
        {-14.7080000, 16.867, 188.9}
    };

    // MatrixPrint(result3);
    if (MatrixEquals(result3, test32) == 1) {
        t13 = t13;
    } else {
        t13 = t13 + 1;
    }




    //////////SCMult//////////////////////////////////////////////////////////////

    float var41 = 2;
    float var42 = 3;
    float result4[3][3];

    MatrixScalarMultiply(var41, mat1, result4);


    float test41[3][3] = {
        {2.000000, 1.000000, 6.000000},
        {8.000000, 10.000000, 12.000000},
        {14.000000, 16.000000, 18.000000}

    };

    t4 = t4 + MatrixEquals(result4, test41);

    float test42[3][3] = {
        {3.000000, 6.000000, 9.000000},
        {12.000000, 15.000000, 18.000000},
        {21.000000, 24.000000, 27.000000}

    };

    MatrixScalarMultiply(var42, mat1, result4);

    t4 = t4 + MatrixEquals(result4, test42);




    //////////ScalarAdd//////////////////////////////////////////////////////////////

    MatrixScalarAdd(var41, mat1, result4);
    float test51[3][3] = {
        {3.000000, 4.000000, 5.000000},
        {6.000000, 7.000000, 8.000000},
        {9.000000, 10.000000, 11.000000}
    };
    t5 = t5 + MatrixEquals(result4, test51);

    float test52[3][3] = {
        {2.000000, 5.000000, 0.000000},
        {7.000000, 8.000000, 9.000000},
        {10.000000, -5.000000, -6.000000}
    };
    MatrixScalarAdd(var42, mat3, result4);

    t5 = t5 + MatrixEquals(result4, test52);




    //////////MtDeterminant//////////////////////////////////////////////////////////////
    float r61;

    r61 = MatrixDeterminant(mat3);
    float test61 = 354.000000;

    if (test61 == r61) {
        t6 = t6 + 1;
    }
    float r62;
    float test62 = 408.75;
    r62 = MatrixDeterminant(mat4);

    if (test62 == r62) {
        t6 = t6 + 1;
    }


    //////////MtTrace//////////////////////////////////////////////////////////////
    float r71;
    r71 = MatrixTrace(mat3);
    float test71 = -5;

    if (test71 == r71) {
        t7 = t7 + 1;
    }
    float r72;
    float test72 = -5;
    r72 = MatrixTrace(mat4);

    if (test72 == r72) {
        t7 = t7 + 1;
    }

    //////MtTranspose/////////////////////////////////////////////////////////
    MatrixTranspose(mat1, result4);
    float test81[3][3] = {
        {1.000000, 4.000000, 7.000000},
        {2.000000, 5.000000, 8.000000},
        {3.000000, 6.000000, 9.000000}
    };
    t8 = t8 + MatrixEquals(result4, test81);
    float test82[3][3] = {
        {-1.000000, 4.000000, 7.000000},
        {2.000000, 5.000000, -8.000000},
        {-3.000000, 6.000000, -9.000000}
    };
    MatrixTranspose(mat3, result4);

    t8 = t8 + MatrixEquals(result4, test82);





    //////MtTranspose/////////////////////////////////////////////////////////

    float results9[3][3], results91[3][3];
    MatrixInverse(mat4, results9);
    MatrixInverse(mat3, results91);
    float test91[3][3] = {
        { -0.003670, 0.113761, 0.077064},
        { 0.190826, 0.084404, -0.007339},
        { -0.172477, 0.013456, -0.044648}
    };
    t9 = t9 + MatrixEquals(results9, test91);


    float test92[3][3] = {
        { 0.008475, 0.118644, 0.076271},
        {0.220339, 0.084746, -0.016949},
        {-0.189266, 0.016949, -0.036723}
    };



    t9 = t9 + MatrixEquals(results91, test92);







    printf("PASSED (%i/2): MatrixMultiply()\n", t11);
    printf("PASSED (%i/2): MatrixAdd()\n", t12);
    printf("PASSED (%i/2): MatrixEquals()\n", t13);
    printf("PASSED (%i/2): MatrixScalarMultiply()\n", t4);
    printf("PASSED (%i/2): MatrixScalarAdd()\n", t5);
    printf("PASSED (%i/2): MatrixDeterminant()\n", t6);
    printf("PASSED (%i/2): MatrixTrace()\n", t7);
    printf("PASSED (%i/2): MatrixTranspose()\n", t8);
    printf("PASSED (%i/2): MatrixInverse()\n", t9);

    printf("---------------------------------------\n");
    int zz = t11 + t12 + t13 + t4 + t5 + t6 + t7 + t8 + t9;
    int zyz;
    zyz = zz / 18 * 100;
    printf("(%i%%)  \n", zyz);

    printf("Output of MatrixPrint():");
    MatrixPrint(result3);
    printf("\nExpected output of MatrixPrint():\n");

    printf("++++++++++++++++++++++++++++++\n");
    printf(" 2.100000  4.200000  6.300000\n");
    printf(" 8.400000  10.500000  12.600001 \n");
    printf("14.700000  16.799999  18.899999 \n");
    printf("++++++++++++++++++++++++++++++\n");

    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}

