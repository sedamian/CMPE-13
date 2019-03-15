// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Buttons.h"
#include "Leds.h"

// **** Set macros and preprocessor directives ****

// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****
static uint8_t buttonEvents;


// **** Declare function prototypes ****

int main(void) {
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);

    // Set up the timer interrupt with a priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/



    ButtonsInit();
    LEDS_INIT();
    BOARD_Init();

    uint8_t bEv;
    uint8_t Luke;

    //Luke = ButtonsCheckEvents();
    while (1) {

        Luke = ButtonsCheckEvents();
        if (buttonEvents) { //Given, used to check button.c
            if (buttonEvents & BUTTON_EVENT_1UP) {
                puts("BUTTON_EVENT_1UP");
            }
            if (buttonEvents & BUTTON_EVENT_1DOWN) {
                puts("BUTTON_EVENT_1DOWN");
            }
            buttonEvents = BUTTON_EVENT_NONE;
        }










        //                    uint8_t bEv = (((BUTTON_STATES() * BUTTON_STATES())<<1)+(BUTTON_STATES() * BUTTON_STATES()));
        //                    LEDS_SET(bEv);    


    }

    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

    while (1);
}

/**
 * This is the interrupt for the Timer1 peripheral. It checks for button events and stores them in a
 * module-level variable.
 */
void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void) {
    // Clear the interrupt flag.
    INTClearFlag(INT_T1);

    ButtonsCheckEvents();
}














