// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>

//CMPE13 Support Library
#include "UnixBOARD.h"
#include "Game.h"
#include "Player.h"
#include "UnixBoard.h"


// User libraries


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****

// **** Declare any function prototypes here ****
void UpdateScreen();
void textColors(int atrib, int fg, int bg);

int main()
{



    /******************************** Your custom code goes below here ********************************/
    PlayerInit();
    GameInit();
    UpdateScreen();
    char input;
    printf("%c{%d;%dH", 27, 40, 80);
    while (1) {
        input = getchar();
        if (input == 'n') {
            GameGoNorth();
            UpdateScreen();
        } else if (input == 'e') {
            GameGoEast();
            UpdateScreen();
        } else if (input == 's') {
            GameGoSouth();
            UpdateScreen();
        } else if (input == 'w') {
            GameGoWest();
            UpdateScreen();
        }
    }
    /**************************************************************************************************/
}

/*
 *
 * Takes in an attribute, foreground color, and background color
 * changes the print to the right color
 * 1 for most of my purposes goes in Attribute (BRIGHT); 0 for RESET
 * 0 = black
 * 1 = Red
 * 2 = Green
 * 3 = Yellow
 * 4 = Blue
 * 5 = Magenta
 * 6 = Cyan
 * 7 = White
 */
void textColors(int atrib, int fg, int bg)
{
    char temp[15];
    sprintf(temp, "^[[%d;%d;%dm", atrib, fg + 30, bg + 40);
    printf("%s", temp);
}

/*
 * This function uses print statements in order to keep the screen updated
 * Handles all of the screen printing using VT100 Commands
 */
static char roomTitle[GAME_MAX_ROOM_TITLE_LENGTH + 1];
static char roomDesc[GAME_MAX_ROOM_DESC_LENGTH + 1];
static uint8_t exits;

void UpdateScreen()
{

    GameGetCurrentRoomTitle(roomTitle);
    GameGetCurrentRoomDescription(roomDesc);
    exits = GameGetCurrentRoomExits();
    //Clear
    printf("%c[2J", 27);

    //Move cursor to top left
    printf("%c[H", 27);

    //Print Title
    textColors(1, 4, 1);
    printf("%s", roomTitle);
    textColors(0, 7, 0);

    //Print Desc
    printf("%s", roomDesc);

    //Next Direction Prompt
    printf("%c[%dB", 27, 39);
    printf("%c[%dC", 27, 40);
    printf("Select Direction by using N, E, S, or W");

    //Compass
    //Exits green if available, red if not available
    printf("%c[8C", 27);
    if (exits & 0b1000) {
        printf("%c[32m", 27);
    } else {
        printf("%c[31m", 27);
    }
    printf("North");
    printf("%cE", 27);
    if (exits & 0b0001) {
        printf("%c[32m", 27);
    } else {
        printf("%c[31m", 27);
    }
    printf("West");
    printf("%c[13C", 27);
    if (exits & 0b0100) {
        printf("%c[32m", 27);
    } else {
        printf("%c[31m", 27);
    }
    printf("East");
    printf("%cE", 27);
    printf("%c[8C", 27);
    if (exits & 0b0010) {
        printf("%c[32m", 27);
    } else {
        printf("%c[31m", 27);
    }
    printf("South");
    printf("%cE", 27);
    printf("%c[37m", 27);
}