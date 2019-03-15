// **** Include libraries here ****
// Standard C libraries

//heap is: 2048
//CMPE13 Support Library
#include "BOARD.h"



#include "Buttons.h"
#include "Ascii.h"
#include "Oled.h"
#include "OledDriver.h"


// Microchip libraries
#include <xc.h>
#include <plib.h>
#include <stdio.h>
#include "Morse.h"
#include "Tree.h"
// User libraries


// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)
#define TRUE 1
#define FALSE 0
// **** Declare any data types here ****




// **** Define any module-level, global, or external variables here ****
static MorseEvent WhatIs = 0; //checks the states if they are dash, space, dot, and end char
static char mDecode = '\0'; //used to check if decoder works
static int Check; //used for standard error
static int i;
//static int r;
static char toOled[84]; //array of chars
static int arrayAddress = 0; //index of line 1
static int arrayAddress2 = 21; //index of line 2
// **** Declare any function prototypes here ****

static void ClearTopLine(void);
static void AppendBottom(char inChar);
static void AppendTop(char inChar);

static int TimeCheck = 1;

int main() {
    BOARD_Init();


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    OledInit();


    //checks if initialized
    Check = MorseInit();
    if (Check == STANDARD_ERROR) {
        OledDrawString("Fatal Error bro"); //prints to Oled
        FATAL_ERROR();
    }

    OledDrawString(toOled);
    OledUpdate();
    while (1) {

        if (TimeCheck == TRUE) {
            //checks for dash
            if (WhatIs == MORSE_EVENT_DASH) {
                //send to decode
                MorseDecode(MORSE_CHAR_DASH);
                AppendTop(MORSE_CHAR_DASH);
                WhatIs = MORSE_EVENT_NONE;

            }//checks for space
            else if (WhatIs == MORSE_EVENT_INTER_WORD) { // When time is more than two seconds
                ClearTopLine(); //clears the char
                WhatIs = MORSE_EVENT_NONE;
                //send to decode
                mDecode = MorseDecode(MORSE_CHAR_END_OF_CHAR); //send to decode
                if (mDecode == STANDARD_ERROR) {
                    AppendBottom(MORSE_CHAR_END_OF_CHAR);
                } else { //decoded char sent to the bottom line
                    AppendBottom(mDecode);
                    AppendBottom(' '); // places space at end or char
                }
                MorseDecode(MORSE_CHAR_DECODE_RESET); //needs to reset
            }//checks for dot
            else if (WhatIs == MORSE_EVENT_DOT) {
                //send to decode
                MorseDecode(MORSE_CHAR_DOT);
                AppendTop(MORSE_CHAR_DOT);
                WhatIs = MORSE_EVENT_NONE;

            }                //checks to see if at end of char
            else if (WhatIs == MORSE_EVENT_INTER_LETTER) {

                ClearTopLine(); //clears the char
                WhatIs = MORSE_EVENT_NONE;
                //send to decode
                mDecode = MorseDecode(MORSE_CHAR_END_OF_CHAR);
                if (mDecode == STANDARD_ERROR) {
                    AppendBottom(MORSE_CHAR_END_OF_CHAR);
                } else {
                    //decoded char sent to the bottom line
                    AppendBottom(mDecode);
                }
                MorseDecode(MORSE_CHAR_DECODE_RESET); //needs to reset
            }

            TimeCheck = FALSE;
        }

    }






    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/

    while (1);
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    TimeCheck = TRUE;
    //******** Put your code here *************//

    WhatIs = MorseCheckEvents(); //sets this to the checker 



}

static void ClearTopLine(void) {

    // goes through and lawnmowers all the Zombies in the first line.
    for (i = 0; i <= 20; i++) {
        toOled[i] = ' ';
    }
    arrayAddress = 0;
    OledDrawString(toOled); //prints and updates
    OledUpdate();

}

static void AppendTop(char inChar) {

    //using 0 for the Oled first line
    // if it increments to 21 then it needs to stop or else all HELL will 
    // break loose
    // this is before because I needed a way to get the first part in.

    if (arrayAddress == 21) {
        arrayAddress = 0;
    }
    toOled[arrayAddress] = inChar; // places new char into new address spot
    arrayAddress++; // increments to new spot for new char in.

    OledDrawString(toOled); //prints and updates
    OledUpdate();

}

static void AppendBottom(char inChar) {

    //using 42 and 21 for the Oled second line
    //    if (arrayAddress2 == 41) {
    //        arrayAddress2 = 21;
    //        for (r = 21; r <= 41; r++) {
    //        toOled[r] = ' ';
    //    }
    //        
    //        
    //    }


    // This is for the extra credit
    // once it reaches the last position, it will stop incrementing
    // Then it will place the newer one in the older and then for the newest 
    // on it places in  inChar
    if (arrayAddress2 == 85) {

        toOled[21] = toOled[22]; // 21
        toOled[22] = toOled[23]; // 22
        toOled[23] = toOled[24]; // 23
        toOled[24] = toOled[25]; // 24
        toOled[25] = toOled[26]; // 25
        toOled[26] = toOled[27]; // 26       
        toOled[27] = toOled[28]; // 27
        toOled[28] = toOled[29]; // 28               
        toOled[29] = toOled[30]; // 29
        toOled[30] = toOled[31]; // 30
        toOled[31] = toOled[32]; // 31
        toOled[32] = toOled[33]; // 32
        toOled[33] = toOled[34]; // 33
        toOled[34] = toOled[35]; // 34
        toOled[35] = toOled[36]; // 35
        toOled[36] = toOled[37]; // 36       
        toOled[37] = toOled[38]; // 37
        toOled[38] = toOled[39]; // 38               
        toOled[39] = toOled[40]; // 39
        toOled[40] = toOled[41]; // 40
        toOled[41] = toOled[42]; // 41

        toOled[42] = toOled[43]; // 42
        toOled[43] = toOled[44]; // 43
        toOled[44] = toOled[45]; // 44
        toOled[45] = toOled[46]; // 45
        toOled[46] = toOled[47]; // 46       
        toOled[47] = toOled[48]; // 47
        toOled[48] = toOled[49]; // 48               
        toOled[49] = toOled[50]; // 49
        toOled[50] = toOled[51]; // 50
        toOled[51] = toOled[52]; // 51
        toOled[52] = toOled[53]; // 52
        toOled[53] = toOled[54]; // 53
        toOled[54] = toOled[55]; // 54
        toOled[55] = toOled[56]; // 55
        toOled[56] = toOled[57]; // 56       
        toOled[57] = toOled[58]; // 57
        toOled[58] = toOled[59]; // 58               
        toOled[59] = toOled[60]; // 59
        toOled[60] = toOled[61]; // 60
        toOled[61] = toOled[62]; // 61
        toOled[62] = toOled[63]; // 62

        toOled[63] = toOled[64]; // 63
        toOled[64] = toOled[65]; // 64
        toOled[65] = toOled[66]; // 65
        toOled[66] = toOled[67]; // 66       
        toOled[67] = toOled[68]; // 67
        toOled[68] = toOled[69]; // 68               
        toOled[69] = toOled[70]; // 69
        toOled[70] = toOled[71]; // 70
        toOled[71] = toOled[72]; // 71
        toOled[72] = toOled[73]; // 72
        toOled[73] = toOled[74]; // 73
        toOled[74] = toOled[75]; // 74
        toOled[75] = toOled[76]; // 75
        toOled[76] = toOled[77]; // 76       
        toOled[77] = toOled[78]; // 77
        toOled[78] = toOled[79]; // 78               
        toOled[79] = toOled[80]; // 79
        toOled[80] = toOled[81]; // 80
        toOled[81] = toOled[82]; // 81
        toOled[82] = toOled[83]; // 82
        toOled[83] = toOled[84]; // 83

        toOled[84] = inChar; // This is for the new last char, places new input


        // This will increment normally until the end
        // where it will stop
    } else if (arrayAddress2 != 85) {
        toOled[arrayAddress2] = inChar; // this is the normal increment. 
        arrayAddress2++;
    }






    OledDrawString(toOled); //prints and updates
    OledUpdate();
}




