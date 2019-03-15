// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <math.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries


// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
double Round(double operand);

double myabs(double operand);

double myFtoC(double operand);

double myCtoF(double operand);

double myAvg(double operand1, double operand2);

double myTan(double operand);

// If this code is used for testing, rename main to something we can call from our testing code.
#ifndef LAB2_TESTING

int main(void) {
    BOARD_Init();
#else

int their_main(void) {
#endif // LAB2_TESTING

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/


    printf("Greetings!");

    while (1) {

        char g;
        double m;
        double p;
        double ly;
        char gar;

        printf("\nEnter a mathematical operation to perform (*,/,+,-,v,a,c,f,t,r): ");
        scanf("%c", &g);

        if (g == '*') {
            printf("\nEnter the first operand:");
            scanf("%f", &m);

            printf("Enter the second operand:");
            scanf("%f", &p);

            ly = m * p;

            printf("Result of (%f %c %f): %f \n", m, g, p, ly);
            scanf("%c", &gar);
        } else if (g == '/') {
            printf("\nEnter the first operand:");
            scanf("%f", &m);

            printf("Enter the second operand:");
            scanf("%f", &p);

            ly = m / p;

            printf("Result of (%f %c %f): %f \n", m, g, p, ly);
            scanf("%c", &gar);
        } else if (g == '+') {
            printf("\nEnter the first operand:");
            scanf("%f", &m);

            printf("Enter the second operand:");
            scanf("%f", &p);

            ly = m + p;

            printf("Result of (%f %c %f): %f \n", m, g, p, ly);
            scanf("%c", &gar);
        } else if (g == '-') {
            printf("\nEnter the first operand:");
            scanf("%f", &m);

            printf("Enter the second operand:");
            scanf("%f", &p);

            ly = m - p;

            printf("Result of (%f %c %f): %f \n", m, g, p, ly);
            scanf("%c", &gar);
        } else if (g == 'v') {
            printf("\nEnter the first operand:");
            scanf("%f", &m);
            printf("Enter the second operand:");
            scanf("%f", &p);

            ly = myAvg(m, p);
            printf("Result of (%f %c %f): %f \n", m, g, p, ly);
            scanf("%c", &gar);

        } else if (g == 'a') {
            printf("\nEnter an operand:");
            scanf("%f", &m);

            ly = myabs(m);
            printf("Result of (%c %f): %f \n", g, m, ly);
            scanf("%c", &gar);
        } else if (g == 'c') {
            printf("\nEnter an operand:");
            scanf("%f", &m);

            ly = myFtoC(m);
            printf("Result of (%c %f): %f \n", g, m, ly);
            scanf("%c", &gar);

        } else if (g == 'f') {
            printf("\nEnter an operand:");
            scanf("%f", &m);


            ly = myCtoF(m);
            printf("Result of (%c %f): %f \n", g, m, ly);
            scanf("%c", &gar);

        } else if (g == 't') {
            printf("\nEnter an operand:");
            scanf("%f", &m);
            ly = myTan(m);

            printf("Result of (%c %f): %f \n", g, m, ly);
            scanf("%c", &gar);

        }
        else if (g == 'r') {
            printf("\nEnter an operand:");
            scanf("%f", &m);
            ly = Round(m);
            printf("Result of (%c %f): %f \n", g, m, ly);
            scanf("%c", &gar);

        }
        else {
            printf("\ninvalid operator");

        }




    }



    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     *****************************************************************************/
    while (1);
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double myabs(double operand) {
    double ret;
    if (operand < 0) {
        ret = operand * (-1);
    } else {
        ret = operand;
    };

    return ret;

}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double myFtoC(double operand) {
    double ret;
    ret = (5.0 / 9.0)* (operand - 32.0);
    return ret;
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double myCtoF(double operand) {
    double ret;
    ret = (9.0 / 5.0)* (operand + 32.0);
    return ret;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double myAvg(double operand1, double operand2) {
    double ret;
    ret = (operand1 + operand2) / 2;
    return ret;

}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees (Note: Your tangent 
 * function relies on a call from the tangent function of math.h which uses 
 * radians).
 ********************************************************************************/
double myTan(double operand) {
    double ret;
    operand = operand * (3.14159 / 180);
    ret = tan(operand);

    return ret;
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand) {
    double ret;
    if (operand > 0) {
        if (operand >= 0.5) {
            ret = 0;
            while (operand > 1) {
                operand = operand - 1;
                ret = ret + 1;

            }
            if (operand >= 0.5) {
                ret = ret + 1;

            } else {
                ret = ret + 0;
            }
        } else {
            ret = 0;
        }
    } else {

        if (operand < -0.5) {
            ret = 0;
            while (operand <= -1.0) {
                operand = operand + 1;
                ret = ret - 1;

            }
            if (operand < -0.5) {
                ret = ret - 1;

            } else {
                ret = ret + 0;
            }
        } else {
            ret = 0;
        }
    }


    return ret;
}

