// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//Class specific libraries
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    BOARD_Init();
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    //Declare Variables
    float fahr, celsius;
    int lower, upper, step;

    //Initialize Variables
    lower = 0; //lower limit of temperature
    upper = 300; // upper limit of temperature
    step = 20; // step size
    fahr = lower;

    printf("    F     C\n");
    // Print out table
    while (fahr <= upper) {
        celsius = (5.0 / 9.0)* (fahr - 32.0);
        printf("%7.1f %04.0f\n", (double) fahr, (double) celsius);
        fahr = fahr + step;
        
    }
    
    printf("\n");
    printf("    K     F\n");
    float kelvin = lower;
    // Print out table 2
    while (kelvin <= upper) {
        fahr = ((9.0 / 5.0)* (kelvin) - 459.67);
       // fahr = ((9.0 / 5.0)* (kelv-273) + 32);
        printf("%3.3f %5.6f\n", (double) kelvin, (double) fahr);
        kelvin = kelvin + step;
    }
    


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     **************************************************************************************************/

    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}
