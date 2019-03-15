/*
 * File:   Morse.c
 * Author: sedamian
 *
 * Created on May 30, 2017, 4:45 PM
 */


#include <xc.h>
#include <stdio.h> // Brings in our nations finest fleet
#include <stdlib.h>
#include "BOARD.h"
#include "Buttons.h"
#include "Morse.h"
#include "Tree.h"
#include <plib.h>

//array of char used for tree
// Null because Max said so, '#' because TA suggested
static const char str[] = {NULL, 'E', 'I', 'S', 'H', '5', '4', 'V', '#', '3',
    'U', 'F', '#', '#', '#', '#', '2', 'A', 'R', 'L', '#', '#', '#', '#', '#',
    'W', 'P', '#', '#', 'J', '#', '1', 'T', 'N', 'D', 'B', '6', '#', 'X', '#',
    '#', 'K', 'C', '#', '#', 'Y', '#', '#', 'M', 'G', 'Z', '7', '#', 'Q', '#',
    '#', 'O', '#', '8', '#', '#', '9', '0'};


static Node *MorEmbChar = NULL;
static Node *TreeOfLife = NULL;

static enum {//state machine
    WAITING,
    DOT,
    DASH,
    INTER_LETTER,
} MorseEvents = WAITING;
static int counter = 0;
static uint8_t BtnEvents = 0;


/**
 * This function initializes the Morse code decoder. This is primarily the generation of the
 * Morse tree: a binary tree consisting of all of the ASCII alphanumeric characters arranged
 * according to the DOTs and DASHes that represent each character. Traversal of the tree is done by
 * taking the left-child if it is a dot and the right-child if it is a dash. If the morse tree is
 * successfully generated, SUCCESS is returned, otherwise STANDARD_ERROR is returned. This function
 * also initializes the Buttons library so that MorseCheckEvents() can work properly.
 * @return Either SUCCESS if the decoding tree was successfully created or STANDARD_ERROR if not.
 */
int MorseInit(void) {

    //  ButtonsInit();
    TreeOfLife = TreeCreate(6, str); //setting a tree
    if (TreeOfLife != NULL) {
        MorseEvents = WAITING; //initializes the state machine
        MorEmbChar = TreeOfLife; //restarting the tree
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }

}

/**
 * MorseDecode decodes a Morse string by iteratively being passed MORSE_CHAR_DOT or MORSE_CHAR_DASH.
 * Each call that passes a DOT or DASH returns a SUCCESS if the string could still compose a
 * Morse-encoded character. Passing in the special MORSE_CHAR_END_OF_CHAR constant will terminate
 * decoding and return the decoded character. During that call to MorseDecode() the return value
 * will be the character that was decoded or STANDARD_ERROR if it couldn't be decoded. Another
 * special value exists, MORSE_CHAR_DECODE_RESET, which will clear the stored state. When a
 * MORSE_CHAR_DECODE_RESET is done, SUCCESS will be returned. If the input is not a valid MorseChar
 * then the internal state should be reset and STANDARD_ERROR should be returned.
 * 
 * @param in A value from the MorseChar enum which specifies how to traverse the Morse tree.
 * 
 * @return Either SUCCESS on DECODE_RESET or when the next traversal location is still a valid
 *         character, the decoded character on END_OF_CHAR, or STANDARD_ERROR if the Morse tree
 *         hasn't been initialized, the next traversal location doesn't exist/represent a character,
 *         or `in` isn't a valid member of the MorseChar enum.
 */
char MorseDecode(MorseChar in) {

        if (TreeOfLife) {
            //if dot appears, it needs to go to the left child
            if (in == MORSE_CHAR_DOT) {
                if (MorEmbChar == NULL) {
                    return STANDARD_ERROR;
                } else {
                    MorEmbChar = MorEmbChar->leftChild;
                    return SUCCESS;
                }
    
                //if dash appears, it needs to go to the right child
            } else if (in == MORSE_CHAR_DASH) {
                if (MorEmbChar == NULL) {
                    return STANDARD_ERROR;
                } else {
                    MorEmbChar = MorEmbChar->rightChild;
                    return SUCCESS;
                }
                //if at the end and there is a child, returns the child char   
            } else if (in == MORSE_CHAR_END_OF_CHAR && MorEmbChar != NULL) {
                return MorEmbChar->data;
            }//resets the char
            else if (in == MORSE_CHAR_DECODE_RESET) {
                MorEmbChar = TreeOfLife;
                return SUCCESS;
            }
            MorEmbChar = TreeOfLife; //resets the char
        }
    
        return STANDARD_ERROR;


//    switch (in) {
//        case MORSE_CHAR_DOT:
//
//            //if dot appears, it needs to go to the left child
//            if (MorEmbChar == NULL) {
//                return STANDARD_ERROR;
//            } else {
//                MorEmbChar = MorEmbChar->leftChild;
//                return SUCCESS;
//            }
//            break;
//            //if dash appears, it needs to go to the right child
//
//        case MORSE_CHAR_DASH:
//            if (MorEmbChar == NULL) {
//                return STANDARD_ERROR;
//            } else {
//                MorEmbChar = MorEmbChar->rightChild;
//                return SUCCESS;
//            }
//            break;
//
//            //if at the end and there is a child, returns the child char   
//        case MORSE_CHAR_END_OF_CHAR:
//            if (MorEmbChar != NULL) {
//                return MorEmbChar->data;
//            }//resets the char
//            else if (in == MORSE_CHAR_DECODE_RESET) {
//                MorEmbChar = TreeOfLife;
//                return SUCCESS;
//            }
//            MorEmbChar = TreeOfLife; //resets the char
//            break;
//        default:
//            return STANDARD_ERROR;
//            break;
//
//    }
//    return STANDARD_ERROR;



}

/**
 * This function calls ButtonsCheckEvents() once per call and returns which, if any,
 * of the Morse code events listed in the enum above have been encountered. It checks for BTN4
 * events in its input and should be called at 100Hz so that the timing works. The
 * length that BTN4 needs to be held down for a dot is >= 0.25s and < 0.50s with a dash being a button
 * down event for >= 0.5s. The button uptime various between dots/dashes (>= .5s), letters
 * (>= 1s), and words (>= 2s).
 *
 * @note This function assumes that the buttons are all unpressed at startup, so that the first
 *       event it will see is a BUTTON_EVENT_*DOWN.
 *
 * So pressing the button for 0.1s, releasing it for 0.1s, pressing it for 0.3s, and then waiting
 * will decode the string '.-' (A). It will trigger the following order of events:
 * 9 MORSE_EVENT_NONEs, 1 MORSE_EVENT_DOT, 39 MORSE_EVENT_NONEs, a MORSE_EVENT_DASH, 69
 * MORSE_EVENT_NONEs, a MORSE_EVENT_END_CHAR, and then MORSE_EVENT_INTER_WORDs.
 * 
 * @return The MorseEvent that occurred.
 */
MorseEvent MorseCheckEvents(void) {

    BtnEvents = ButtonsCheckEvents();


    // This state machine was basically given to us.
    // one adjustment was that there is a return of none if
    // nothing else is returned.
    switch (MorseEvents) {

        case WAITING:

            if (BtnEvents & BUTTON_EVENT_4DOWN) {//used when pressing btn4
                MorseEvents = DOT; //next state
                counter = 0; //reset counter

            } else {
                MorseEvents = WAITING;
                return MORSE_EVENT_NONE;


            }
            break;
        case DOT:
            counter++;
            if (BtnEvents & BUTTON_EVENT_4UP) {//used when pressing btn4
                MorseEvents = INTER_LETTER; //next state
                counter = 0; //reset counter

                return MORSE_EVENT_DOT;
            }
            if (counter >= MORSE_EVENT_LENGTH_DOWN_DOT) {
                MorseEvents = DASH; //next state
                return MORSE_EVENT_NONE;

            } else {
                return MORSE_EVENT_NONE;
            }
            break;

        case DASH:

            if (BtnEvents & BUTTON_EVENT_4UP) {
                MorseEvents = INTER_LETTER; //next state
                counter = 0;

                return MORSE_EVENT_DASH;
            } else {
                return MORSE_EVENT_NONE;

            }
            break;

        case INTER_LETTER:
            counter++;
            if (BtnEvents & BUTTON_EVENT_4DOWN) {
                if (counter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER) {
                    MorseEvents = DOT; //next state
                    counter = 0;

                    return MORSE_EVENT_INTER_LETTER;
                } else {
                    MorseEvents = DOT; //next state
                    counter = 0;

                    return MORSE_EVENT_NONE;
                }
            } else if (counter >= MORSE_EVENT_LENGTH_UP_INTER_LETTER_TIMEOUT) {
                MorseEvents = WAITING; //next state
                return MORSE_EVENT_INTER_WORD;
            } else {
                return MORSE_EVENT_NONE;
            }


            break;
        default: // The default state. Cause Safety's first
            FATAL_ERROR();
            break;
    };
    return MORSE_EVENT_NONE; //if nothing happens, return none





}