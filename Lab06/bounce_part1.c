//// **** Include libraries here ****
//// Standard libraries
//
////CMPE13 Support Library
//#include "BOARD.h"
//
//// Microchip libraries
//#include <xc.h>
//#include <plib.h>
//
//// User libraries
//#include "Leds.h"
//
//// **** Set macros and preprocessor directives ****
//#define LEFT 1
//#define RIGHT 0
//
//#define TRUE 1
//#define FALSE 0
//
//// **** Declare any datatypes here ****
//
//struct TimerResult {
//    uint8_t event;
//    unsigned char value;
//};
//// **** Define global, module-level, or external variables here ****
//static struct TimerResult eData;
//
//// **** Declare function prototypes ****
//
//int main(void) {
//    BOARD_Init();
//
//    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
//    // pretty reasonable rate to start.
//    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_8, 0xFFFF);
//
//    // Set up the timer interrupt with a priority of 4.
//    INTClearFlag(INT_T1);
//    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
//    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
//    INTEnable(INT_T1, INT_ENABLED);
//
//    /***************************************************************************************************
//     * Your code goes in between this comment and the following one with asterisks.
//     **************************************************************************************************/
//
//
//
//    //struct TimerResult eData;
//
//
//
//
//    LEDS_INIT();
//
//    //    uint8_t dir = LEFT;
//    //    int ball = 0x01;
//    int dir = LEFT;
//    uint8_t ball = 0x01;
//
//
//    while (1) {
//
//
//        if (eData.event == TRUE) {
//            LEDS_SET(ball);
//            if (ball == 0x80) {
//                dir = RIGHT;
//            } else if (ball == 0x01) {
//                dir = LEFT;
//            }
//            if (dir == LEFT) {
//                ball = ball << 1;
//            } else  {  
//                ball = ball >> 1;
//            }
//        }
//        eData.event = FALSE;
//
//    }
//
//
//    /***************************************************************************************************
//     * Your code goes in between this comment and the preceding one with asterisks
//     **************************************************************************************************/
//
//    while (1);
//}
//
///**
// * This is the interrupt for the Timer1 peripheral. During each call it increments a counter (the
// * value member of a module-level TimerResult struct). This counter is then checked against the
// * binary values of the four switches on the I/O Shield (where SW1 has a value of 1, SW2 has a value
// * of 2, etc.). If the current counter is greater than this switch value, then the event member of a
// * module-level TimerResult struct is set to true and the value member is cleared.
// */
//void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void) {
//    // Clear the interrupt flag.
//    INTClearFlag(INT_T1);
//
//
//    eData.value++;
//    if (eData.value > SWITCH_STATES()) {
//        eData.event = TRUE;
//        eData.value = 0;
//    }
//
//
//}