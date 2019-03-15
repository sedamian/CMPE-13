/* ************************************************************************** */
/** Descriptive File Name
 Samuel Damian 
 * Leds.h
 */
/* ************************************************************************** */

#include "BOARD.h"
#include <xc.h>
#ifndef LEDS_H    /* Guard against multiple inclusion */
#define LEDS_H

#define LEDS_INIT() do {\       // initialize
TRISE = 0;\
    LATE = 0;\
    }
while (0)
#define LEDS_GET() (return LATE)        // get

#define LEDS_SET(x)(LATE = (x))         // sets values into leds

#endif


