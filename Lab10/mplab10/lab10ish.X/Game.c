/*
 * File:   Game.c
 * Author: sedamian
 *
 * Created on June 13, 2017, 12:46 AM
 */


#include <string.h>
#include "Game.h"
#include "Player.h"
#include "UnixBOARD.h"
#include <stdio.h>

static struct dStruct {
    char descript[GAME_MAX_ROOM_DESC_LENGTH + 1];
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    uint8_t item;
    char roomInventory[4];
    uint8_t nextRooms[4];
    char file[24];
    uint8_t roomCurrent;
    uint32_t keyMaster;
    char secCode[20];
} dStruct;
static uint8_t Hotel = 0;

FILE *fp;
static int lengthOfTitle;
static int lengthOfDesc;
static int lengthOfItRq;
static int lengthOfItCn;
static int lengthOfExit;
//static int YoureGarbage;
static char c;
static int ii = 0;
static int iii = 0;
static int iv = 0;

static int itemReq;
static int itemCon;


#define FILE_WORK "RoomFiles/room%d.txt"



void decodeMessage(void);

/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void)
{

    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_NORTH_EXISTS) {
        dStruct.roomCurrent = dStruct.nextRooms[3];
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void)
{
    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_EAST_EXISTS) {
        dStruct.roomCurrent = dStruct.nextRooms[2];
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void)
{

    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_SOUTH_EXISTS) {
        dStruct.roomCurrent = dStruct.nextRooms[1];
        return SUCCESS;
    }
    return STANDARD_ERROR;


}

/**
 * @see GameGoNorth
 */
int GameGoWest(void)
{

    if (GameGetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS) {
        dStruct.roomCurrent = dStruct.nextRooms[0];
        return SUCCESS;
    }
    return STANDARD_ERROR;




}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void)
{

    dStruct.roomCurrent = STARTING_ROOM;


    if (dStruct.roomCurrent != STARTING_ROOM) {
        return SUCCESS;
    }
    return STANDARD_ERROR;
}

/**
 * Copies the current room title as a NULL-terminated string into the provided character array.
 * Only a NULL-character is copied if there was an error so that the resultant output string
 * length is 0.
 * @param title A character array to copy the room title into. Should be GAME_MAX_ROOM_TITLE_LENGTH+1
 *             in length in order to allow for all possible titles to be copied into it.
 * @return The length of the string stored into `title`. Note that the actual number of chars
 *         written into `title` will be this value + 1 to account for the NULL terminating
 *         character.
 */
int GameGetCurrentRoomTitle(char *title)
{

    decodeMessage();

    strcpy(title, dStruct.title);
    if (title == NULL) {
        title = '\0';
    }


    return strlen(title);
}

/**
 * GetCurrentRoomDescription() copies the description of the current room into the argument desc as
 * a C-style string with a NULL-terminating character. The room description is guaranteed to be less
 * -than-or-equal to GAME_MAX_ROOM_DESC_LENGTH characters, so the provided argument must be at least
 * GAME_MAX_ROOM_DESC_LENGTH + 1 characters long. Only a NULL-character is copied if there was an
 * error so that the resultant output string length is 0.
 * @param desc A character array to copy the room description into.
 * @return The length of the string stored into `desc`. Note that the actual number of chars
 *          written into `desc` will be this value + 1 to account for the NULL terminating
 *          character.
 */
int GameGetCurrentRoomDescription(char *desc)
{



    strcpy(desc, dStruct.descript);
    if (desc == NULL) {
        desc = '\0';
    }

    return strlen(desc);



}

/**
 * This function returns the exits from the current room in the lowest-four bits of the returned
 * uint8 in the order of NORTH, EAST, SOUTH, and WEST such that NORTH is in the MSB and WEST is in
 * the LSB. A bit value of 1 corresponds to there being a valid exit in that direction and a bit
 * value of 0 corresponds to there being no exit in that direction. The GameRoomExitFlags enum
 * provides bit-flags for checking the return value.
 *
 * @see GameRoomExitFlags
 *
 * @return a 4-bit bitfield signifying which exits are available to this room.
 */
uint8_t GameGetCurrentRoomExits(void)
{


    Hotel = 0b0000;

    // != 0 because it can be any number , zero if room doesnt exist.

    if (dStruct.nextRooms[0] != 0) {
        Hotel = Hotel | GAME_ROOM_EXIT_WEST_EXISTS;
    }
    if (dStruct.nextRooms[2] != 0) {
        Hotel = Hotel | GAME_ROOM_EXIT_EAST_EXISTS;
    }
    if (dStruct.nextRooms[1] != 0) {
        Hotel = Hotel | GAME_ROOM_EXIT_SOUTH_EXISTS;
    }
    if (dStruct.nextRooms[3] != 0) {
        Hotel = Hotel | GAME_ROOM_EXIT_NORTH_EXISTS;
    }


    return Hotel;


}

void decodeMessage(void)
{
    dStruct.keyMaster = 0;
    dStruct.keyMaster = dStruct.roomCurrent + 122;


    sprintf(dStruct.secCode, FILE_WORK, dStruct.roomCurrent);
    fp = fopen(dStruct.secCode, "rb");
    if (fp == NULL) {

        FATAL_ERROR(); // if file open fails to open;
    }




    // DECODATION

    lengthOfTitle = fgetc(fp) ^ dStruct.keyMaster;

    printf("TITLE LENGTH:	%i\n", lengthOfTitle);

    for (ii = 0; ii < lengthOfTitle; ii++) {
        c = fgetc(fp);

        dStruct.title[ii] = c ^ dStruct.keyMaster;

        printf("TITLE:	%c\n", dStruct.title[ii]);

        dStruct.title[ii + 1] = '\0';

    }


    lengthOfItRq = fgetc(fp) ^ dStruct.keyMaster;

    printf("INIT REQ: %i\n", lengthOfItRq);

    for (ii = 0; ii < lengthOfItRq; ii++) {
        c = fgetc(fp);

        itemReq = c ^ dStruct.keyMaster;

        printf("item req:	%c\n", itemReq);
    }




    lengthOfDesc = fgetc(fp) ^ dStruct.keyMaster;

    printf("Decp Length: %i\n", lengthOfDesc);

    for (iii = 0; iii < lengthOfDesc; iii++) {
        c = fgetc(fp);

        dStruct.descript[iii] = c ^ dStruct.keyMaster;
        dStruct.descript[iii + 1] = '\0';
    }





    lengthOfItCn = fgetc(fp) ^ dStruct.keyMaster;

    printf("Length of it cn: %i\n", lengthOfItCn);


    for (iii = 0; iii < lengthOfItCn; iii++) {
        c = fgetc(fp);

        itemCon = c ^ dStruct.keyMaster;

        printf("item contained:	%i\n", itemCon);
    }




    lengthOfExit = fgetc(fp) ^ dStruct.keyMaster;

    printf("Length of it exits: %i\n", lengthOfExit);

    //	printf("%i\n", YoureGarbage);
    for (iv = 0; iv < 4; iv++) {
        c = fgetc(fp);

        dStruct.nextRooms[iv] = c ^ dStruct.keyMaster;
        printf("nextroom:	%i	\n", dStruct.nextRooms[iv]);

    }




}








