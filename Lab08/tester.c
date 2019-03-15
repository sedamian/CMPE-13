//CMPE13 Support Library
#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Morse.h"
#include "Tree.h"

// **** Set any macros or preprocessor directives here ****
// Specify a bit mask for setting/clearing the pin corresponding to BTN4. Should only be used when
// unit testing the Morse event checker.
#define BUTTON4_STATE_FLAG (1 << 7)

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****
static char topLine[42]; //this is for the first line
static MorseEvent eventMorse = 0;
static char decoding = '\0';
static int indexTop = 0; //keeps track of the current char
static int indexBottom = 21; //keeps track of the current char

// **** Declare any function prototypes here ****
static void OLEDTopClear(void);
static void OLEDappendTop(char target);
static void OLEDappendBottom(char target);

int main()
{
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
    //printf("Welcome to the CMPE13 Lab8 blank. Please remove this line.");

    //    Node *head = TreeCreate(3, "ABDECFG");
    //    printf("%c", head->data);
    //    printf("%c", head->leftChild->data);
    //    printf("%c", head->rightChild->data);
    //    printf("%c", head->leftChild->leftChild->data);
    //    printf("%c", head->leftChild->rightChild->data);
    //    printf("%c", head->rightChild->leftChild->data);
    //    printf("%c", head->rightChild->rightChild->data);





    OledInit();
    //using MorseInit
    int morse_rc = MorseInit();
    //displays error on Oled id not initialized 
    if (morse_rc == STANDARD_ERROR) {
        OledDrawString("ERROR: Fail to initialize!");
        FATAL_ERROR();
    }
    //places spaced on the board
    OledDrawString(topLine);
    OledUpdate();


    /******************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     *****************************************************************************/

    //while (1);
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //******** Put your code here *************//

    //using Morse library to read Morse events
    eventMorse = MorseCheckEvents();

    //checks for the button to == DOT
    if (eventMorse == MORSE_EVENT_DOT) {
        //if DOT, send it in to decode it
        MorseDecode(MORSE_CHAR_DOT);
        OLEDappendTop(MORSE_CHAR_DOT);

    }
        ////checks for the button to == dash
    else if (eventMorse == MORSE_EVENT_DASH) {
        //if DASH, send it in to decode it
        MorseDecode(MORSE_CHAR_DASH);
        OLEDappendTop(MORSE_CHAR_DASH);

    }
        //checks if at end of char
    else if (eventMorse == MORSE_EVENT_INTER_LETTER) {
        //clears for new char
        OLEDTopClear();
        decoding = MorseDecode(MORSE_CHAR_END_OF_CHAR); //sends it to decode
        if (decoding != STANDARD_ERROR) {
            //if no error, send at the bottom to print out decoded char
            OLEDappendBottom(decoding);
        }
        else {

            OLEDappendBottom(MORSE_CHAR_END_OF_CHAR);
        }
        MorseDecode(MORSE_CHAR_DECODE_RESET); //needs to reset char holder
    }
        //if it is word
    else if (eventMorse == MORSE_EVENT_INTER_WORD) {
        OLEDTopClear(); //clears for new char
        decoding = MorseDecode(MORSE_CHAR_END_OF_CHAR); //send to decode
        if (decoding != STANDARD_ERROR) {
            //if no error, send at the bottom to print out decoded char
            OLEDappendBottom(decoding);
        }
        else {
            OLEDappendBottom(MORSE_CHAR_END_OF_CHAR);
        }
        MorseDecode(MORSE_CHAR_DECODE_RESET); //needs to reset char holder
    }
    //eventMorse = MORSE_EVENT_NONE; //Reset morseEvents

    // pass eventMorse to DecodeMorse() and print it??
    //char decode = MorseDecode(eventMorse);


}
//clears the top line of the OLED and updates it.

static void OLEDTopClear(void)
{
    int i = 0;
    for (i = 0; i <= 21; i++) {
        topLine[i] = ' ';
    }
    OledDrawString(topLine); //send to print
    OledUpdate();
}

static void OLEDappendTop(char target)
{
    //using 21 for the first Oleds
    if (indexTop <= 21) {
        topLine[indexTop] = target; //using an array to store them in there
        indexTop++;
    }

    OledDrawString(topLine);
    OledUpdate();



}

static void OLEDappendBottom(char target)
{
    //using 42 and 21 for the second row
    if (indexBottom <= 42 && indexBottom >= 21) {
        topLine[indexBottom] = target;
        indexBottom++;

    }

    OledDrawString(topLine);
    OledUpdate();


}



//
//#include "BOARD.h"
//#include "Morse.h"
//#include "Tree.h"
//#include "Buttons.h"
//
////this is the tree char's
//static const char morseString[] = {NULL, 'E', 'I', 'S', 'H', '5', '4', 'V', NULL, '3', 'U', 'F', NULL,
//    NULL, NULL, NULL, '2', 'A', 'R', 'L', NULL, NULL, NULL, NULL, NULL, 'W', 'P', NULL, NULL, 'J', NULL, '1', 'T', 'N', 'D',
//    'B', '6', NULL, 'X', NULL, NULL, 'K', 'C', NULL, NULL, 'Y', NULL, NULL, 'M', 'G', 'Z', '7', NULL, 'Q', NULL, NULL,
//    'O', NULL, '8', NULL, NULL, '9', '0'};
//
//
//static Node *startUp = NULL; //for my state
//static Node *headTree = NULL;
//static uint8_t currentButton = 0;
//static int counter = 0;
//
//static enum {//state machines 
//    WAITING,
//    DOT,
//    DASH,
//    INTER_LETTER
//} currentEvent = WAITING;
//
///**
// * This function initializes the Morse code decoder. This is primarily the generation of the
// * Morse tree: a binary tree consisting of all of the ASCII alphanumeric characters arranged
// * according to the DOTs and DASHes that represent each character. Traversal of the tree is done by
// * taking the left-child if it is a dot and the right-child if it is a dash. If the morse tree is
// * successfully generated, SUCCESS is returned, otherwise STANDARD_ERROR is returned. This function
// * also initializes the Buttons library so that MorseCheckEvents() can work properly.
// * @return Either SUCCESS if the decoding tree was successfully created or STANDARD_ERROR if not.
// */
//int MorseInit(void) {
//    ButtonsInit();
//    startUp = TreeCreate(6, morseString); //6 level binary tree in the making 
//    if (startUp == NULL) {
//        return STANDARD_ERROR;
//    } else {
//        currentEvent = WAITING;
//        headTree = startUp; //head points to the current 
//        return SUCCESS;
//    }
//
//
//
//}
//
///**
// * MorseDecode decodes a Morse string by iteratively being passed MORSE_CHAR_DOT or MORSE_CHAR_DASH.
// * Each call that passes a DOT or DASH returns a SUCCESS if the string could still compose a
// * Morse-encoded character. Passing in the special MORSE_CHAR_END_OF_CHAR constant will terminate
// * decoding and return the decoded character. During that call to MorseDecode() the return value
// * will be the character that was decoded or STANDARD_ERROR if it couldn't be decoded. Another
// * special value exists, MORSE_CHAR_DECODE_RESET, which will clear the stored state. When a
// * MORSE_CHAR_DECODE_RESET is done, SUCCESS will be returned. If the input is not a valid MorseChar
// * then the internal state should be reset and STANDARD_ERROR should be returned.
// * 
// * @param in A value from the MorseChar enum which specifies how to traverse the Morse tree.
// * 
// * @return Either SUCCESS on DECODE_RESET or when the next traversal location is still a valid
// *         character, the decoded character on END_OF_CHAR, or STANDARD_ERROR if the Morse tree
// *         hasn't been initialized, the next traversal location doesn't exist/represent a character,
// *         or `in` isn't a valid member of the MorseChar enum.
// */
//char MorseDecode(MorseChar in) {
//
//    if (startUp) {
//
//        //dot = left child
//        if ((in == MORSE_CHAR_DOT) && (headTree != NULL)) {
//            headTree = headTree->leftChild;
//            return SUCCESS;
//
//
//        }            //dash = right child
//        else if ((in == MORSE_CHAR_DASH)&& (headTree != NULL)) {
//            headTree = headTree->rightChild;
//            return SUCCESS;
//
//
//        }            //returns the decoded char
//        else if ((in == MORSE_CHAR_END_OF_CHAR) && (headTree != NULL)) {
//            printf("decoding has lead you to %c\n", headTree->data);
//            return headTree->data;
//
//        }            //resets for next char
//        else if (in == MORSE_CHAR_DECODE_RESET) {
//            headTree = startUp; //reset
//            return SUCCESS;
//        }
//
//        headTree = startUp; //reset if it didn't before
//        return STANDARD_ERROR;
//
//    }
//    return STANDARD_ERROR; //if startUp is not TRUE, 0 is returned 
//
//}
///*
// MORSE_CHAR_DOT = '.',          /// A Morse DOT.
//    MORSE_CHAR_DASH = '-',         /// A Morse DASH.
//    MORSE_CHAR_END_OF_CHAR = '#',  /// Signifies END_OF_CHAR: that the Morse string is over and the
//                                    // character should be decoded.
//    MORSE_CHAR_DECODE_RESET = '\0'
// 
// 
// */
//
///**
// * This function calls ButtonsCheckEvents() once per call and returns which, if any,
// * of the Morse code events listed in the enum above have been encountered. It checks for BTN4
// * events in its input and should be called at 100Hz so that the timing works. The
// * length that BTN4 needs to be held down for a dot is >= 0.25s and < 0.50s with a dash being a button
// * down event for >= 0.5s. The button uptime various between dots/dashes (>= .5s), letters
// * (>= 1s), and words (>= 2s).
// *
// * @note This function assumes that the buttons are all unpressed at startup, so that the first
// *       event it will see is a BUTTON_EVENT_*DOWN.
// *
// * So pressing the button for 0.1s, releasing it for 0.1s, pressing it for 0.3s, and then waiting
// * will decode the string '.-' (A). It will trigger the following order of events:
// * 9 MORSE_EVENT_NONEs, 1 MORSE_EVENT_DOT, 39 MORSE_EVENT_NONEs, a MORSE_EVENT_DASH, 69
// * MORSE_EVENT_NONEs, a MORSE_EVENT_END_CHAR, and then MORSE_EVENT_INTER_WORDs.
// * 
// * @return The MorseEvent that occurred.
// */
//MorseEvent MorseCheckEvents(void) {
//    currentButton = ButtonsCheckEvents();
//    switch (currentEvent) {
//        case WAITING:
//            //case when state goes into DOT
//            if (currentButton == BUTTON_EVENT_4DOWN) {
//                counter = 0; //reset counter
//                currentEvent = DOT; //next state
//            } else {
//                currentEvent = WAITING; //stays in the same state
//                //return MORSE_EVENT_NONE;
//            }
//            break;
//
//
//
//        case DOT:
//            counter++;
//            if (counter >= MORSE_EVENT_LENGTH_DOWN_DOT) {
//                currentEvent = DASH; //next state
//            }
//            if (currentButton == BUTTON_EVENT_4UP) {
//                counter = 0; //reset the counter
//                currentEvent = INTER_LETTER;
//                return MORSE_EVENT_DOT;
//
//            }
//            break;
//        case DASH:
//            if (currentButton == BUTTON_EVENT_4UP) {
//                counter = 0;
//                currentEvent = INTER_LETTER;
//                return MORSE_EVENT_DASH;
//            }
//            break;
//
//
//        case INTER_LETTER:
//            //case #1
//            counter++;
//            if (counter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER_TIMEOUT) {
//                currentEvent = WAITING;
//                return MORSE_EVENT_INTER_WORD;
//            }                //case #2
//            else if (currentButton == BUTTON_EVENT_4DOWN) {
//                //if TRUE
//                if (counter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER) {
//                    counter = 0;
//                    currentEvent = DOT;
//                    return MORSE_EVENT_INTER_LETTER;
//
//                }                    //if FALSE
//                else {
//                    counter = 0;
//                    currentEvent = DOT;
//                    return MORSE_EVENT_NONE;
//
//                }
//            }
//
//
//
//            break;
//
//            //in the case of all cases not working
//        default:
//            FATAL_ERROR();
//            break;
//    };
//    return MORSE_EVENT_NONE;
//}
//
//
//
//
//
//
//
///*
// * File:   Morse.c
// * Author: sedamian
// *
// * Created on May 30, 2017, 4:45 PM
// */
//
//
//#include <xc.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include "BOARD.h"
//#include "Buttons.h"
//#include "Morse.h"
//#include "Tree.h"
//#include <plib.h>
//
//static Node *TreeOfLife;
//static const char str[63] = {NULL, 'E', 'I', 'S', 'H', '5', '4', 'V', '#', '3',
//    'U', 'F', '#', '#', '#', '#', '2', 'A', 'R', 'L', '#', '#', '#', '#', '#',
//    'W', 'P', '#', '#', 'J', '#', '1', 'T', 'N', 'D', 'B', '6', '#', 'X', '#',
//    '#', 'K', 'C', '#', '#', 'Y', '#', '#', 'M', 'G', 'Z', '7', '#', 'Q', '#',
//    '#', 'O', '#', '8', '#', '#', '9', '0'};
////static char DecChar;
//static Node *MorEmbChar;
//
//static enum {
//    WAITING,
//    DOT,
//    DASH,
//    INTER_LETTER,
//} MorseEvents = WAITING;
////static MorseEvents = WAITING;
//static int counter = 0;
//static uint16_t BtnEvents;
//
///**
// * This function initializes the Morse code decoder. This is primarily the generation of the
// * Morse tree: a binary tree consisting of all of the ASCII alphanumeric characters arranged
// * according to the DOTs and DASHes that represent each character. Traversal of the tree is done by
// * taking the left-child if it is a dot and the right-child if it is a dash. If the morse tree is
// * successfully generated, SUCCESS is returned, otherwise STANDARD_ERROR is returned. This function
// * also initializes the Buttons library so that MorseCheckEvents() can work properly.
// * @return Either SUCCESS if the decoding tree was successfully created or STANDARD_ERROR if not.
// */
//int MorseInit(void) {
//
//    ButtonsInit();
//    TreeOfLife = TreeCreate(6, str);
//
//
//    //printf("inside morse init\n");
//    printf("startup head = %c", TreeOfLife->data);
//    printf("startup left child = %c\n", TreeOfLife->leftChild->data);
//    printf("???? = %c\n", TreeOfLife->leftChild->leftChild->leftChild->leftChild->data);
//    //printf("This should be a V = %c\n", TreeOfLife->leftChild->leftChild->leftChild->leftChild->leftChild->rightChild->rightChild->data);
//    printf("startup right child = %c\n", TreeOfLife->rightChild->data);
//    //printf("%c", TreeOfLife->leftChild->leftChild->rightChild->rightChild->rightChild->rightChild->data);//this prints out an A when is should print out NULL
//    //printf("%c", startUp->leftChild->leftChild->leftChild->rightChild->rightChild->rightChild->data);
//
//    if (TreeOfLife != NULL) {
//        MorseEvents = WAITING;
//        MorEmbChar = TreeOfLife;
//        return SUCCESS;
//    } else {
//        return STANDARD_ERROR;
//    }
//
//}
//
///**
// * MorseDecode decodes a Morse string by iteratively being passed MORSE_CHAR_DOT or MORSE_CHAR_DASH.
// * Each call that passes a DOT or DASH returns a SUCCESS if the string could still compose a
// * Morse-encoded character. Passing in the special MORSE_CHAR_END_OF_CHAR constant will terminate
// * decoding and return the decoded character. During that call to MorseDecode() the return value
// * will be the character that was decoded or STANDARD_ERROR if it couldn't be decoded. Another
// * special value exists, MORSE_CHAR_DECODE_RESET, which will clear the stored state. When a
// * MORSE_CHAR_DECODE_RESET is done, SUCCESS will be returned. If the input is not a valid MorseChar
// * then the internal state should be reset and STANDARD_ERROR should be returned.
// * 
// * @param in A value from the MorseChar enum which specifies how to traverse the Morse tree.
// * 
// * @return Either SUCCESS on DECODE_RESET or when the next traversal location is still a valid
// *         character, the decoded character on END_OF_CHAR, or STANDARD_ERROR if the Morse tree
// *         hasn't been initialized, the next traversal location doesn't exist/represent a character,
// *         or `in` isn't a valid member of the MorseChar enum.
// */
//char MorseDecode(MorseChar in) {
//
//    if (in == MORSE_CHAR_DOT) {
//        if (MorEmbChar == NULL) {
//            return STANDARD_ERROR;
//        } else {
//            MorEmbChar = MorEmbChar->leftChild;
//            return SUCCESS;
//        }
//    } else if (in == MORSE_CHAR_DASH) {
//        if (MorEmbChar == NULL) {
//            return STANDARD_ERROR;
//        } else {
//            MorEmbChar = MorEmbChar->rightChild;
//            return SUCCESS;
//        }
//    } else if (in == MORSE_CHAR_END_OF_CHAR) {
//        if (MorEmbChar == NULL) {
//            return STANDARD_ERROR;
//        }
//        return MorEmbChar->data;
//    } else if (in == MORSE_CHAR_DECODE_RESET) {
//        MorEmbChar = NULL;
//        return SUCCESS;
//    }
//    return STANDARD_ERROR;
//
//
//}
//
///**
// * This function calls ButtonsCheckEvents() once per call and returns which, if any,
// * of the Morse code events listed in the enum above have been encountered. It checks for BTN4
// * events in its input and should be called at 100Hz so that the timing works. The
// * length that BTN4 needs to be held down for a dot is >= 0.25s and < 0.50s with a dash being a button
// * down event for >= 0.5s. The button uptime various between dots/dashes (>= .5s), letters
// * (>= 1s), and words (>= 2s).
// *
// * @note This function assumes that the buttons are all unpressed at startup, so that the first
// *       event it will see is a BUTTON_EVENT_*DOWN.
// *
// * So pressing the button for 0.1s, releasing it for 0.1s, pressing it for 0.3s, and then waiting
// * will decode the string '.-' (A). It will trigger the following order of events:
// * 9 MORSE_EVENT_NONEs, 1 MORSE_EVENT_DOT, 39 MORSE_EVENT_NONEs, a MORSE_EVENT_DASH, 69
// * MORSE_EVENT_NONEs, a MORSE_EVENT_END_CHAR, and then MORSE_EVENT_INTER_WORDs.
// * 
// * @return The MorseEvent that occurred.
// */
//MorseEvent MorseCheckEvents(void) {
//
//    BtnEvents = ButtonsCheckEvents();
//    switch (MorseEvents) {
//
//        case WAITING:
//
//            if (BtnEvents & BUTTON_EVENT_4DOWN) {
//                MorseEvents = DOT;
//                counter = 0;
//            } else {
//                return MORSE_EVENT_NONE;
//            }
//            break;
//        case DOT:
//            counter++;
//            if (BtnEvents & BUTTON_EVENT_4UP) {
//                MorseEvents = INTER_LETTER;
//                counter = 0;
//                return MORSE_EVENT_DOT;
//            } else if (counter >= MORSE_EVENT_LENGTH_DOWN_DOT) {
//                MorseEvents = DASH;
//            } else {
//                return MORSE_EVENT_NONE;
//            }
//            break;
//
//        case DASH:
//
//            if (BtnEvents & BUTTON_EVENT_4UP) {
//                counter = 0;
//                MorseEvents = INTER_LETTER;
//                return MORSE_EVENT_DASH;
//            } else {
//                return MORSE_EVENT_NONE;
//
//            }
//            break;
//
//        case INTER_LETTER:
//            counter++;
//            if (BtnEvents & BUTTON_EVENT_4DOWN) {
//                if (counter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER) {
//                    MorseEvents = DOT;
//                    counter = 0;
//                    return MORSE_EVENT_INTER_LETTER;
//                } else {
//                    MorseEvents = DOT;
//                    counter = 0;
//                    return MORSE_EVENT_NONE;
//                }
//            } else if (counter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER_TIMEOUT) {
//                MorseEvents = WAITING;
//                return MORSE_EVENT_INTER_WORD;
//            } else {
//                return MORSE_EVENT_NONE;
//            }
//
//
//            break;
//        default:
//            FATAL_ERROR();
//            break;
//    }
//    return MORSE_EVENT_NONE;
//
//
//
//
//}
//
