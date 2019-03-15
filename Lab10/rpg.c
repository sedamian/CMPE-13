// **** Include libraries here ****
// Standard libraries
#include <string.h>
#include <math.h>
#include <stdio.h>

//CMPE13 Support Library
#include "UnixBOARD.h"
#include "Game.h"
#include "Player.h"



// User libraries


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any global or external variables here ****
//static uint8_t roomExits = 0;

static char Title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
static char Desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
//static 	char temp[15];
static uint8_t Openings;
// **** Declare any function prototypes here ****

static char intake;
static int IEEE = 0;

int main()
{



    /******************************** Your custom code goes below here ********************************/

    GameInit();



    while (1) {

        printf("\033[2J"); // trying to clear the screen




        //intake = getchar();
        // printf("%c{%d;%dH", 27, 40, 80);

        GameGetCurrentRoomTitle(Title);

        GameGetCurrentRoomDescription(Desc);

        // sprintf(temp, "^[[%d;%d;%dm", 1, 34, 41);
        //    printf("%s", temp);

        printf("\033[2J"); //clears the screen


        printf("%c[%dB", 27, 39);
        printf("%c[%dC", 27, 40);
        printf("%s\n", Title); // prints title of room
        printf("%s\n", Desc); // prints Description of room
        // textColors(0, 7, 0);
        Openings = GameGetCurrentRoomExits();

        //	printf("%c[%dB", 27, 39);
        //    printf("%c[%dC", 27, 40);
        for (IEEE = 0; IEEE < 10; IEEE++) {
            printf("\n");
        }


        // prints green if available, red if not;
        printf("%c[8C", 27);
        if (Openings & 0b1000) {
            printf("%c[32m", 27);
        } else {
            printf("%c[31m", 27);
        }
        printf("NORTH");
        printf("%cE", 27);
        if (Openings & 0b0001) {
            printf("%c[32m", 27);
        } else {
            printf("%c[31m", 27);
        }
        printf("WEST");
        printf("%c[13C", 27);
        if (Openings & 0b0100) {
            printf("%c[32m", 27);
        } else {
            printf("%c[31m", 27);
        }
        printf("EAST");
        printf("%cE", 27);
        printf("%c[8C", 27);
        if (Openings & 0b0010) {
            printf("%c[32m", 27);
        } else {
            printf("%c[31m", 27);
        }
        printf("SOUTH");
        printf("%cE", 27);
        printf("%c[37m", 27);

        intake = getchar();


        if (intake == 'q') {
            break;
        }


            // checks which direction entered
         if (intake == 'n') {
            GameGoNorth();
        } else if (intake == 'e') {
            GameGoEast();
        } else if (intake == 's') {
            GameGoSouth();
        } else if (intake == 'w') {
            GameGoWest();
        }








    }
    /**************************************************************************************************/
}








