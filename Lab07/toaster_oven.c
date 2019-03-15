// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

#include "Leds.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

#include "Oled.h"
#include "OledDriver.h"
#include "Adc.h"
#include "Buttons.h"

// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)

// **** Declare any datatypes here ****

//struct AdcResult {
//    uint8_t event;
//    uint16_t value;
//};
//static struct AdcResult dota;

typedef struct ovenStateData {
    uint16_t ckTimeLeft;
    uint16_t ckTimeINIT;
    uint16_t Thermos;
    uint16_t ckMODE;
    uint8_t ckState;
    int bnCount;
    uint16_t inpSel;

} ovenStateData;
static struct ovenStateData aa;

static enum {
    RESET,
    START,
    PENDING_SELECTOR_CHANGE,
    COUNTDOWN,
    PENDING_RESET,
    FLASH_ON,
    FLASH_OFF
} UnitedStates = RESET;

static uint32_t count;
static int lilcount;
static int Flag2Hz;
static uint32_t bigcount;
static uint32_t bgrcount;
static uint16_t buttonsync;
static int ledinc;

static char stering[200];

static uint16_t zoro;
static uint16_t lemask;
static uint16_t vend;
static uint16_t machina;
static int numbo1, numbo2;
static int varmin, varsec;
static int vartem;
static int strtem;
static int newmin, newsec;
static int cntend;
static uint32_t tpled;
static int fixtem;

//static uint16_t bghelp;
//static uint16_t bginfo;
//static uint16_t toGlory;

// **** Define any module-level, global, or external variables here ****
#define TRUE 1
#define FALSE 0
// Configuration Bit settings

int main() {
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR1 to F_PB / 256 / 2 yields a 0.5s timer.
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, BOARD_GetPBClock() / 256 / 2);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T1);
    INTSetVectorPriority(INT_TIMER_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T1, INT_ENABLED);

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    BOARD_Init();
    ButtonsInit();
    AdcInit();
    OledInit();
    LEDS_INIT();
    lemask = 0x3fc;
    cntend = 0;
    fixtem = 500; // This is just me initializing values and everything
    tpled = 0;
    ledinc = 0;
    vartem = 350;

    //    bginfo = 0xFF00;

    aa.ckState = 0;
    aa.ckMODE = 0; // initializing struct
    aa.inpSel = 0;
    aa.Thermos = 350;

    while (1) {

        //        toGlory = buttonsync;
        //        bghelp = toGlory;
        zoro = AdcRead();
        vend = zoro & lemask; // masking the pot with xFC
        machina = vend >> 2; // me putting the top three bits into machina

        if (machina <= 60) { //this is my method of getting the minutes and seconds. separated
            numbo1 = machina;
            numbo2 = 0;
        } else if ((machina >= 60) && (machina < 120)) {
            numbo1 = machina - 60;
            numbo2 = 1;
        } else if ((machina < 180) && (machina >= 120)) {
            numbo1 = machina - 120;
            numbo2 = 2;
        } else if ((machina >= 180) && (machina < 240)) {
            numbo1 = machina - 180;
            numbo2 = 3;
        } else if (machina >= 240) {
            numbo1 = machina - 240;
            numbo2 = 4;
        }

        if (Flag2Hz == TRUE) { // my lilcounter
            Flag2Hz = FALSE;
            lilcount++;
        }

        //        if (bginfo != bghelp){
        //            
        //            
        //            bghelp = bginfo;
        //            bigcount++;
        //        }
        //      bigcount = count;



        // this makes it so that it can change the time in certain modes.
        if ((aa.inpSel == 0) || (aa.ckMODE == 1) || (aa.ckMODE == 2)) {
            varmin = numbo2;
            varsec = numbo1 + 1;
            aa.ckTimeINIT = machina;
            // if pointing to the temp, change temp
        } else if (aa.inpSel == 1) {
            vartem = machina + 300;
            aa.Thermos = vartem;

        }


        switch (UnitedStates) {

            case RESET:
                aa.inpSel = 0;
                UnitedStates = START;
                //   vartem = 350;

                OledUpdate();
                break;

            case START:
                // These are all of the different possible outputs in the wirst part. 
                if ((aa.ckMODE == 0) && (aa.inpSel == 0)) { // 0 = BAKE
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Bake \n|     | >TIME: %i:%02i \n|-----|  TEMP: %i\xF8 \n|\x04\x04\x04\x04\x04| \n", varmin, varsec, vartem);
                } else if ((aa.ckMODE == 0) && (aa.inpSel == 1)) {
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Bake \n|     |  TIME: %i:%02i \n|-----| >TEMP: %i\xF8 \n|\x04\x04\x04\x04\x04| \n", varmin, varsec, vartem);
                } else if ((aa.ckMODE == 1) && (aa.inpSel == 0)) { // 1 = Toast
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Toast \n|     |  TIME: %i:%02i \n|-----|              \n|\x04\x04\x04\x04\x04| \n", varmin, varsec);
                } else if ((aa.ckMODE == 1) && (aa.inpSel == 1)) { // 1 = Toast
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Toast \n|     |  TIME: %i:%02i \n|-----|              \n|\x04\x04\x04\x04\x04| \n", varmin, varsec);
                } else if ((aa.ckMODE == 2)) { // 2 = Broil
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Broil\n|     |  TIME: %i:%02i \n|-----|  TEMP: 500\xF8 \n|\x04\x04\x04\x04\x04| \n", varmin, varsec);
                }


                // This checks if pressing buttons to go to next state
                if (buttonsync & BUTTON_EVENT_3DOWN) { // if button 3 pressed

                    count = 0; // reset  counter
                    UnitedStates = PENDING_SELECTOR_CHANGE; // change state
                    Flag2Hz = FALSE;
                    OledUpdate();
                } else if (buttonsync & BUTTON_EVENT_4DOWN) { //if button 4 pressed
                    // Store Time
                    strtem = vartem;
                    newmin = varmin;
                    newsec = varsec;
                    
                    aa.ckTimeLeft = aa.ckTimeINIT;
                    
                    // newtem = vartem;
                    bigcount = 0;
                    LEDS_SET(0xFF);
                    UnitedStates = COUNTDOWN; // Change state to countdown
                    OledUpdate();
                }


                OledUpdate();
                break;




            case PENDING_SELECTOR_CHANGE:
                // This makes it change Selector if more than 1s
                if ((count >= 0x05)) {
                    if (aa.inpSel == 0) {
                        aa.inpSel = 1;
                        //  break;
                    } else if (aa.inpSel == 1) {
                        aa.inpSel = 0;
                        //  break;
                    }
                    UnitedStates = START;
                    OledUpdate();

                }// This makes it change MODE if LESS than 1s
                else if (((count < 0x05) && (ButtonsCheckEvents() & BUTTON_EVENT_3UP))) {
                    if (aa.ckMODE == 0) {
                        aa.ckMODE = 1;
                        aa.inpSel = 0;
                        //break;
                    } else if (aa.ckMODE == 1) {
                        aa.ckMODE = 2;
                        aa.inpSel = 0;
                        //break;
                    } else if (aa.ckMODE == 2) {
                        aa.ckMODE = 0;
                        //break;
                    }
                    UnitedStates = START;
                    OledUpdate();
                }
                //UnitedStates = START;
                break;

            case COUNTDOWN:
                // this is my countdown mechanism.
                // it takes the value of the last time
                // the time would go through a series of checks 
                // and time will decrement.
                if ((lilcount >= 2) && ((newmin > 0) || (newsec > 0))) {
                    if (newmin >= 4) {
                        newsec--;
                        if (newsec < 0) {
                            newmin--;
                            newsec = 59;
                        }
                    } else if ((newmin >= 3) && (newmin < 4)) {
                        newsec--;
                        if (newsec < 0) {
                            newmin--;
                            newsec = 59;
                        }
                    } else if ((newmin >= 2) && (newmin < 3)) {
                        newsec--;
                        if (newsec < 0) {
                            newmin--;
                            newsec = 59;
                        }
                    } else if ((newmin >= 1) && (newmin < 2)) {
                        newsec--;
                        if (newsec < 0) {
                            newmin--;
                            newsec = 59;
                        }
                    } else if ((newmin < 1)) {
                        newsec--;
                        if (newsec == 0) {
                            cntend = 1;
                        }
                    }
                    lilcount = 0;
                }



                // This is turning on the Oven 
                // Certain case for certain modes.
                if ((aa.ckMODE == 0) && (aa.inpSel == 0)) { // 0 = BAKE
                    sprintf(stering, "|\x01\x01\x01\x01\x01|  MODE: Bake \n|     |  TIME: %i:%02i  \n|-----|  TEMP: %i\xF8 \n|\x03\x03\x03\x03\x03| \n", newmin, newsec, strtem);
                } else if ((aa.ckMODE == 0) && (aa.inpSel == 1)) {
                    sprintf(stering, "|\x01\x01\x01\x01\x01|  MODE: Bake \n|     |  TIME: %i:%02i \n|-----|  TEMP: %i\xF8 \n|\x03\x03\x03\x03\x03| \n", newmin, newsec, strtem);
                } else if (aa.ckMODE == 1) { // 1 = Toast
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Toast\n|     |  TIME: %i:%02i \n|-----| \n|\x03\x03\x03\x03\x03| \n", newmin, newsec);
                } else if ((aa.ckMODE == 2)) { // 2 = Broil
                    sprintf(stering, "|\x01\x01\x01\x01\x01|  MODE: Broil\n|     |  TIME: %i:%02i \n|-----|  TEMP: %i\xF8 \n|\x04\x04\x04\x04\x04| \n", newmin, newsec, fixtem);
                }

                OledUpdate();


                // This is where I do my LED thing
                tpled = aa.ckTimeLeft * 250; // I do this to make it a mult of 8

                bgrcount = bigcount * 20;
                // This was before when I though you couldnt use divides. 

                // Check if time reaches numerical multiple
                // if so turn on the next led. in sync
                if ((bgrcount >= tpled) && (ledinc == 0)) {
                    LEDS_SET(0xFE);
                    ledinc = 1;
                    bigcount = 0;
                } else if ((bgrcount >= tpled) && (ledinc == 1)) {
                    LEDS_SET(0xFC);
                    ledinc = 2;
                    bigcount = 0;
                } else if ((bgrcount >= tpled) && (ledinc == 2)) {
                    LEDS_SET(0xF8);
                    ledinc = 3;
                    bigcount = 0;
                } else if ((bgrcount >= tpled) && (ledinc == 3)) {
                    LEDS_SET(0xF0);
                    ledinc = 4;
                    bigcount = 0;
                } else if ((bgrcount >= tpled) && (ledinc == 4)) {
                    LEDS_SET(0xE0);
                    ledinc = 5;
                    bigcount = 0;
                } else if ((bgrcount >= tpled) && (ledinc == 5)) {
                    LEDS_SET(0xC0);
                    ledinc = 6;
                    bigcount = 0;
                } else if ((bgrcount >= tpled) && (ledinc == 6)) {
                    LEDS_SET(0x80);
                    ledinc = 7;
                    bigcount = 0;
                } else if ((bgrcount >= tpled) && (ledinc == 7)) {
                    LEDS_SET(0x00);
                    ledinc = 8;
                }



                // change state if button 4 pressed
                // it will check if its long or not.
                // also if time ends. 
                if (buttonsync & BUTTON_EVENT_4DOWN) {
                    count = 0;

                    buttonsync = 0x0000;
                    UnitedStates = PENDING_RESET; // change state
                    OledUpdate();
                }

                // if time up, turn off led just in case and change state
                if (cntend == 1) {
                    cntend = 0;
                    LEDS_SET(0x00);
                    UnitedStates = FLASH_ON;
                }

                break;

            case PENDING_RESET:


                if ((lilcount >= 2) && ((newmin > 0) || (newsec > 0))) {
                    if (newmin >= 4) {
                        newsec--;
                        if (newsec < 0) {
                            newmin--;
                            newsec = 59;
                        }
                    } else if ((newmin >= 3) && (newmin < 4)) {
                        newsec--;
                        if (newsec < 0) {
                            newmin--;
                            newsec = 59;
                        }
                    } else if ((newmin >= 2) && (newmin < 3)) {
                        newsec--;
                        if (newsec < 0) {
                            newmin--;
                            newsec = 59;
                        }
                    } else if ((newmin >= 1) && (newmin < 2)) {
                        newsec--;
                        if (newsec < 0) {
                            newmin--;
                            newsec = 59;
                        }
                    } else if ((newmin < 1)) {
                        newsec--;
                        if (newsec == 0) {
                            cntend = 1;
                        }
                    }
                    lilcount = 0;
                }

                // turns on elements on 
                if ((aa.ckMODE == 0) && (aa.inpSel == 0)) { // 0 = BAKE
                    sprintf(stering, "|\x01\x01\x01\x01\x01|  MODE: Bake \n|     |  TIME: %i:%02i  \n|-----|  TEMP: %i\xF8 \n|\x03\x03\x03\x03\x03| \n", newmin, newsec, strtem);
                } else if ((aa.ckMODE == 0) && (aa.inpSel == 1)) {
                    sprintf(stering, "|\x01\x01\x01\x01\x01|  MODE: Bake \n|     |  TIME: %i:%02i \n|-----|  TEMP: %i\xF8 \n|\x03\x03\x03\x03\x03| \n", newmin, newsec, strtem);
                } else if (aa.ckMODE == 1) { // 1 = Toast
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Toast\n|     |  TIME: %i:%02i \n|-----| \n|\x03\x03\x03\x03\x03| \n", newmin, newsec);
                } else if ((aa.ckMODE == 2)) { // 2 = Broil
                    sprintf(stering, "|\x01\x01\x01\x01\x01|  MODE: Broil\n|     |  TIME: %i:%02i \n|-----|  TEMP: %i\xF8 \n|\x04\x04\x04\x04\x04| \n", newmin, newsec, fixtem);
                }

                OledUpdate();






                if ((count >= 0x05)) {

                    buttonsync = 0;
                    UnitedStates = RESET;
                    vartem = strtem;
                    count = 0;
                    cntend = 0;
                    ledinc = 0;
                    tpled = 0;
                    OledDriverSetDisplayNormal();
                    LEDS_SET(0x00);
                    OledUpdate();
                    aa.inpSel = 0;
                }



                if (buttonsync & BUTTON_EVENT_4UP) {
                    UnitedStates = COUNTDOWN;
                }
                if ((cntend == 1)) {
                    cntend = 0;
                    UnitedStates = FLASH_ON;
                }


                break;

            case FLASH_ON:


                //This turns off oven when time up
                if ((aa.ckMODE == 0) && (aa.inpSel == 0)) { // 0 = BAKE
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Bake \n|     |  TIME: %i:%02i  \n|-----|  TEMP: %i\xF8 \n|\x04\x04\x04\x04\x04| \n", newmin, newsec, strtem);
                } else if ((aa.ckMODE == 0) && (aa.inpSel == 1)) {
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Bake \n|     |  TIME: %i:%02i \n|-----|  TEMP: %i\xF8 \n|\x04\x04\x04\x04\x04| \n", newmin, newsec, strtem);
                } else if (aa.ckMODE == 1) { // 1 = Toast
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Toast\n|     |  TIME: %i:%02i \n|-----| \n|\x04\x04\x04\x04\x04| \n", newmin, newsec);
                } else if ((aa.ckMODE == 2)) { // 2 = Broil
                    sprintf(stering, "|\x02\x02\x02\x02\x02|  MODE: Broil\n|     |  TIME: %i:%02i \n|-----|  TEMP: %i\xF8 \n|\x04\x04\x04\x04\x04| \n", newmin, newsec, fixtem);
                }

                OledUpdate();



                // This checks if button is pressed if not keep cycling
                if ((lilcount >= 1)) {
                    OledSetDisplayInverted();
                    lilcount = 0;
                    UnitedStates = FLASH_OFF;
                }
                if ((buttonsync & BUTTON_EVENT_4DOWN)) {
                    buttonsync = 0;
                    UnitedStates = RESET;
                    vartem = strtem;
                    aa.inpSel = 0;
                    cntend = 0;
                    ledinc = 0; // initializes when needs to reset
                    tpled = 0;
                    OledDriverSetDisplayNormal();
                    LEDS_SET(0x00);


                }



                break;

            case FLASH_OFF:


                // This doesnt need new oled string because flash on was first.  
                if ((lilcount >= 1)) {
                    OledDriverSetDisplayNormal();
                    //                  nitelite = 0;
                    lilcount = 0;
                    UnitedStates = FLASH_ON;
                }

                // This will make it escape if there is btn4 press
                if ((buttonsync & BUTTON_EVENT_4DOWN)) {

                    buttonsync = 0;
                    UnitedStates = RESET;
                    vartem = strtem;
                    cntend = 0;
                    ledinc = 0; // re initialize everything
                    tpled = 0;
                    OledDriverSetDisplayNormal();
                    LEDS_SET(0x00);
                    aa.inpSel = 0;


                }



                break;




        }

        OledDrawString(stering);

        // OledSetDisplayInverted();
        OledUpdate();
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/
    while (1);
}

void __ISR(_TIMER_1_VECTOR, ipl4auto) TimerInterrupt2Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 4;
    Flag2Hz = TRUE;
}

void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    count++;
}

void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    buttonsync = ButtonsCheckEvents();
    bigcount++;

}