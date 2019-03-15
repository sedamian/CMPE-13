/*
 * File:   MatrixMath.c
 * Author: sedamian
 *
 * Created on April 25, 2017, 1:13 AM
 */


#include "xc.h"

#include "MatrixMath.h"

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {

    float place;
    place = 0;
    int i;
    int j;
    int k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                place = place + (mat1[i][k] * mat2[k][j]);
            }


            result[i][j] = place;
            place = 0;
        }
    }



}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {

    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat1[i][j] + mat2[i][j];

        }
    }

    //return result;

}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    int ret0;
    ret0 = 0;
    int ret1;
    ret1 = 1;
    int flag;
    flag = 0;
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if ((mat1[i][j] - mat2[i][j]) >= FP_DELTA) {
                // if ((mat1[i][j]-mat2[i][j])>0.01){
                flag = flag + 1;

                // printf ("\n %i \n  %i  \n", i , j);
                //               return 0;

            }
            //if ((mat1[i][j]-mat2[i][j])<0.01){
            if ((-mat1[i][j] + mat2[i][j]) >= FP_DELTA) {
                flag = flag + 1;
            }
        }
    }
    if (flag == 0) {
        return ret1;
    } else {
        return ret0;
    }

    // return 1;

}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {


    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat[i][j] * x;

        }
    }



}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {

    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat[i][j] + x;

        }
    }


}

float MatrixDeterminant(float mat[3][3]) {


    float x, y, z;

    //ret = mat[2][2];

    x = mat[0][0]*(mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
    y = (-mat[0][1])*(mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]);
    z = mat[0][2]*(mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    //    printf ("---- %f \n", x);
    //    printf ("---- %f \n", y);
    //    printf ("---- %f \n", z);
    //    printf ("---- %f \n", x + y + z);

    return x + y + z;
}

float MatrixTrace(float mat[3][3]) {

    float ret;
    ret = 0;
    int i;
    for (i = 0; i < 3; i++) {
        ret = ret + mat[i][i];
    }
    return ret;
}

void MatrixTranspose(float mat[3][3], float result[3][3]) {

    float temp1;
    float temp2;
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            temp1 = mat[i][j];
            temp2 = mat[j][i];
            result[j][i] = temp1;

        }
    }

}

void MatrixInverse(float mat[3][3], float result[3][3]) {


    float overdet;
    float over;
    overdet = MatrixDeterminant(mat);
    over = 1 / overdet;

    float tp[3][3];

    MatrixTranspose(mat, tp);


    float restrap[3][3];

    MatrixAdjugate(mat, restrap);

    //    restrap[0][0] = (mat[1][1]*mat[2][2] - mat[1][2]*mat[2][1]);
    //    restrap[0][1] = (mat[1][2]*mat[2][0] - mat[1][0]*mat[2][2]);
    //    restrap[0][2] = (mat[1][0]*mat[2][1] - mat[1][1]*mat[2][0]);
    //    
    //    restrap[1][0] = (mat[0][2]*mat[2][1] - mat[0][1]*mat[2][2]);
    //    restrap[1][1] = (mat[0][0]*mat[2][2] - mat[0][2]*mat[2][0]);
    //    restrap[1][2] = (mat[0][1]*mat[2][0] - mat[0][0]*mat[2][1]);
    //    
    //    restrap[2][0] = (mat[0][1]*mat[1][2] - mat[0][2]*mat[1][1]);
    //    restrap[2][1] = (mat[0][2]*mat[1][0] - mat[0][0]*mat[1][2]);
    //    restrap[2][2] = (mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0]);

    MatrixTranspose(restrap, tp);

    MatrixScalarMultiply(over, tp, result);


}

void MatrixPrint(float mat[3][3]) {
    float tp;
    int i;
    int j;
    printf("\n++++++++++++++++++++++++++++++\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            tp = mat[i][j];
            printf(" %f ", tp);
        }
        printf("\n");
    }
    printf("++++++++++++++++++++++++++++++\n");

}

void MatrixAdjugate(float mat[3][3], float result[3][3]) {
    int i;
    int j;
    int m = 0;
    int p = 0;
    int k = 0;
    int h = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == 0) {
                m = 1;
                p = 2;
                if (j == 0) {
                    k = 1;
                    h = 2;
                } else if (j == 1) {
                    k = 2;
                    h = 0;
                } else {
                    k = 0;
                    h = 1;
                }
            } else if (i == 1) {
                m = 0;
                p = 2;
                if (j == 0) {
                    k = 2;
                    h = 1;
                } else if (j == 1) {
                    k = 0;
                    h = 2;
                } else {
                    k = 1;
                    h = 0;
                }
            } else if (i == 2) {
                m = 0;
                p = 1;
                if (j == 0) {
                    k = 1;
                    h = 2;
                } else if (j == 1) {
                    k = 2;
                    h = 0;
                } else {
                    k = 0;
                    h = 1;
                }
            }
            result[i][j] = ((mat[m][k] * mat[p][h]) - (mat[m][h] * mat[p][k]));
        }
    }
}










