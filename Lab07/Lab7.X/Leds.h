/* ************************************************************************** */
/** Descriptive File Name
 Samuel Damian 
 * Leds.h
 */
/* ************************************************************************** */
#ifndef LEDS_H    /* Guard against multiple inclusion */
#define LEDS_H
#include "BOARD.h"
#include <xc.h>


#define LEDS_INIT() do {\
    TRISE = 0;\
    LATE = 0;\
    } while(0)
#define LEDS_GET() (return LATE)

#define LEDS_SET(x)(LATE = (x))

#endif


