/*
 * File:   Buttons.c
 * Author: Samuel Damian
 *
 * Created on May 17, 2017, 6:25 PM
 */


#include "xc.h"
#include "BOARD.h"
#include "Buttons.h"

/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified so that this library does not interfere with other libraries.
 */
void ButtonsInit(void){
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

/**
 * This function checks the button states and returns any events that have occured since the last
 * call. In the case of the first call to ButtonsCheckEvents() after ButtonsInit(), the function
 * should assume that the buttons start in an off state with value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, BUTTONS_EVENT_NONE should be returned. The
 * events are listed in the ButtonEventFlags enum at the top of this file. This function should be
 * called repeatedly.
 *
 * 
 * 
 * This function also performs debouncing of the buttons. Every time ButtonsCheckEvents() is called,
 * all button values are saved, up to the 4th sample in the past, so 4 past samples and the present
 * values. A button event is triggered if the newest 4 samples are the same and different from the
 * oldest sample. For example, if button 1 was originally down, button 1 must appear to be up for 4
 * samples and the last BTN3 event was BUTTON_EVENT_3DOWN before a BUTTON_EVENT_1UP is triggered. 
 * This eliminates button bounce, where the button may quickly oscillate between the ON and OFF state
 * as it's being pressed or released.
 *
 * NOTE: This will not work properly without ButtonsInit() being called beforehand.
 * @return A bitwise-ORing of the constants in the ButtonEventFlags enum or BUTTON_EVENT_NONE if no
 *         event has occurred.
 */
uint8_t ButtonsCheckEvents(void){
    
//  00000011 = 0x03
//  00001100 = 0x0C
//  00110000 = 0x30
//  11000000 = 0xC0

//    BUTTON_EVENT_NONE = 0x00,
//    BUTTON_EVENT_1UP = 0x01,
//    BUTTON_EVENT_1DOWN = 0x02,
//    BUTTON_EVENT_2UP = 0x04,
//    BUTTON_EVENT_2DOWN = 0x08,
//    BUTTON_EVENT_3UP = 0x10,
//    BUTTON_EVENT_3DOWN = 0x20,
//    BUTTON_EVENT_4UP = 0x40,
//    BUTTON_EVENT_4DOWN = 0x80
    
    
    uint8_t latt;
    uint8_t curr;
    int notfirst = 0;
    
    curr = 0x00;
    
    
    latt = (0x03 & BUTTON_STATES());
    
    if ((0x01 == BUTTON_STATES())&&(curr == 0x00)){
        curr = 0x01;
        return BUTTON_EVENT_1UP;
        
        
        
    }
    if ((0x00 == BUTTON_STATES())&&(curr == 0x01)){
        curr = 0x00;
        return BUTTON_EVENT_1DOWN;
    }
    
    
    else {
        return BUTTON_EVENT_NONE;
    }
    notfirst = 5;
    
    

    

    
        
        
        
}







//    static uint8_t buttonEvents;
//    buttonEvents = BUTTON_STATES();
//        if (buttonEvents){
//            if (buttonEvents & BUTTON_EVENT_1UP){
//                puts("BUTTON_EVENT_1UP");                                 
//            }
//            else if (buttonEvents & BUTTON_EVENT_1DOWN){
//                puts("BUTTON_EVENT_1DOWN");
//            }
/////////////////////////////////////////////////////////////////////////            
//            if (buttonEvents & BUTTON_EVENT_2UP){
//                puts("BUTTON_EVENT_2UP");                                 
//            }
//            else if (buttonEvents & BUTTON_EVENT_2DOWN){
//                puts("BUTTON_EVENT_2DOWN");
//            }
/////////////////////////////////////////////////////////////////////////           
//            if (buttonEvents & BUTTON_EVENT_3UP){
//                puts("BUTTON_EVENT_3UP");                                 
//            }
//            else if (buttonEvents & BUTTON_EVENT_3DOWN){
//                puts("BUTTON_EVENT_3DOWN");
//            }
/////////////////////////////////////////////////////////////////////////                     
//            if (buttonEvents & BUTTON_EVENT_4UP){
//                puts("BUTTON_EVENT_4UP");                                 
//            }
//            else if (buttonEvents & BUTTON_EVENT_4DOWN){
//                puts("BUTTON_EVENT_4DOWN");
//            }
// 
//            buttonEvents = BUTTON_EVENT_NONE;
//        }