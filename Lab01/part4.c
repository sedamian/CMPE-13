/*
 * File:   part4.c
 * Author: sedamian
 *
 * Created on April 12, 2017, 7:57 PM
 */


#include "xc.h"
#include "BOARD.h"

void main(void) 
{
    BOARD_Init();
   // printf("Hello Wurl\n");
    OledInit();
    OledDrawString("Hello Wurl\n");
    OledUpdate();
    while(1); 
}
