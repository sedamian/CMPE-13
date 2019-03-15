// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Stack.h"
#include <stdio.h>

// **** Set macros and preprocessor directives ****

// **** Define global, module-level, or external variables here ****

// **** Declare function prototypes ****
/*
 * Function prototype for ProcessBackspaces() - This function should be defined
 * below main after the related comment.
 */
int ProcessBackspaces(char *rpn_sentence);

// If this code is used for testing, rename main to something we can call from our testing code. We
// also skip all processor configuration and initialization code.
#ifndef LAB4_TESTING

int main() {
    BOARD_Init();
#else

int their_main(void) {
#endif // LAB4_TESTING

    /******************************** Your custom code goes below here *******************************/
    struct Stack OREOs;

    printf("Welcome to CMPE13 Lab4 \n");
    float retval;
    char instring[60];
    char *Woo;
    float temp1;
    float temp2;
    float ret;

    while (1) {

        StackInit(&OREOs);
        printf("Enter floats and + - / * \n");


        fgets(instring, 60, stdin);

        retval = 0;




        Woo = strtok(instring, " ");

        while (Woo != NULL) {

            //printf(" is this it \n%s", Woo);
            if (!((atof(Woo) != 0) || (Woo[0] == '+') || (Woo[0] == '-') || (Woo[0] == '*') || (Woo[0] == '/'))) {
                printf("\nERROR: INVALID CHARACTER\n");
                break;
            }
            if ((StackGetSize(&OREOs) <= 2)&&((Woo[0] == '+') || (Woo[0] == '-') || (Woo[0] == '*') || (Woo[0] == '/'))) {
                printf("\nERROR: Not enough operands\n");
                break;
            }
            if (StackIsFull(&OREOs)) {
                printf("\nERROR: STACK FULL\n");
                break;
            } else {



                if (Woo[0] == '+') {

                    printf("\nyou are using: +");

                    StackPop(&OREOs, &temp1);
                    StackPop(&OREOs, &temp2);

                    retval = temp1 + temp2;

                    StackPush(&OREOs, retval);
                } else if (Woo[0] == '-') {
                    StackPop(&OREOs, &temp1);
                    StackPop(&OREOs, &temp2);

                    retval = temp2 - temp1;

                    StackPush(&OREOs, retval);

                } else if (Woo[0] == '*') {
                    StackPop(&OREOs, &temp1);
                    StackPop(&OREOs, &temp2);

                    retval = temp1 * temp2;

                    StackPush(&OREOs, retval);

                } else if (Woo[0] == '/') {
                    StackPop(&OREOs, &temp1);
                    StackPop(&OREOs, &temp2);

                    retval = temp2 / temp1;

                    StackPush(&OREOs, retval);

                } else if (Woo[0] == '0' || atof(Woo) != 0) {
                    StackPush(&OREOs, atof(Woo));
                    printf("\nValue pushed: %f", atof(Woo));
                }



                Woo = strtok(NULL, " ");
            }



        }
        ret = 0;
        StackPop(&OREOs, &retval);
        printf("\nResult: %0.2f \n\n", (double) retval);

    }





    /*************************************************************************************************/

    // You can never return from main() in an embedded system (one that lacks an operating system).
    // This will result in the processor restarting, which is almost certainly not what you want!
    while (1);
}

/**
 * Extra Credit: Define ProcessBackspaces() here - This function should read through an array of
 * characters and when a backspace character is read it should replace the preceding character with
 * the following character. It should be able to handle multiple repeated backspaces and also
 * strings with more backspaces than characters. It should be able to handle strings that are at
 * least 256 characters in length.
 * @param rpn_sentence The string that will be processed for backspaces. This string is modified in
 *                     place.
 * @return Returns the size of the resultant string in `rpn_sentence`.
 */
int ProcessBackspaces(char *rpn_sentence) {
    return 0;
}



