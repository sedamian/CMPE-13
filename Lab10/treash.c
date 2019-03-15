#include "UnixBOARD.h"
#include "UnixBoard.h"
#include "Game.h"
#include "Player.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define FILE_TEMPLATE "RoomFiles/room%d.txt"

/**
 * This enum defines flags for checking the return values of GetCurrentRoomExits(). Usage is as
 * follows:
 *
 * if (GetCurrentRoomExits() & GAME_ROOM_EXIT_WEST_EXISTS) {
 *   // The current room has a west exit.
 * }
 *
 * @see GetCurrentRoomExits
 */


//Room Struct

static struct room {
    char title[GAME_MAX_ROOM_TITLE_LENGTH + 1];
    char desc[GAME_MAX_ROOM_DESC_LENGTH + 1];
    uint8_t roomNumber;
    char file[24];
    uint8_t nextRooms[4];
    uint8_t item;
    char roomInventory[4];
} room;

//File Declaration
static FILE *file;

int UpdateRoomInfo();

/**
 * These function transitions between rooms. Each call should return SUCCESS if the current room has
 * an exit in the correct direction and the new room was able to be loaded, and STANDARD_ERROR
 * otherwise.
 * @return SUCCESS if the room CAN be navigated to and changing the current room to that new room
 *         succeeded.
 */
int GameGoNorth(void)
{
    if (GAME_ROOM_EXIT_NORTH_EXISTS & GameGetCurrentRoomExits()) {
        room.roomNumber = room.nextRooms[0];
        UpdateRoomInfo();
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * @see GameGoNorth
 */
int GameGoEast(void)
{
    if (GAME_ROOM_EXIT_EAST_EXISTS & GameGetCurrentRoomExits()) {
        room.roomNumber = room.nextRooms[1];
        UpdateRoomInfo();
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * @see GameGoNorth
 */
int GameGoSouth(void)
{
    if (GAME_ROOM_EXIT_SOUTH_EXISTS & GameGetCurrentRoomExits()) {
        room.roomNumber = room.nextRooms[2];
        UpdateRoomInfo();
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * @see GameGoNorth
 */
int GameGoWest(void)
{
    if (GAME_ROOM_EXIT_WEST_EXISTS & GameGetCurrentRoomExits()) {
        room.roomNumber = room.nextRooms[3];
        UpdateRoomInfo();
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * This function sets up anything that needs to happen at the start of the game. This is just
 * setting the current room to STARTING_ROOM and loading it. It should return SUCCESS if it succeeds
 * and STANDARD_ERROR if it doesn't.
 * @return SUCCESS or STANDARD_ERROR
 */
int GameInit(void)
{
    room.title[0] = '\0';
    room.desc[0] = '\0';
    room.nextRooms[0] = STARTING_ROOM;
    room.nextRooms[1] = 0;
    room.nextRooms[2] = 0;
    room.nextRooms[3] = 0;
    room.roomNumber = 0;
    room.file[0] = '\0';
    return GameGoNorth();
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
    sprintf(title, "%s", room.title);
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
    sprintf(desc, "%s", room.desc);
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
    uint8_t exits = 0;
    if (room.nextRooms[0] != 0) {
        exits |= GAME_ROOM_EXIT_NORTH_EXISTS;
    }
    if (room.nextRooms[1] != 0) {
        exits |= GAME_ROOM_EXIT_EAST_EXISTS;
    }
    if (room.nextRooms[2] != 0) {
        exits |= GAME_ROOM_EXIT_SOUTH_EXISTS;
    }
    if (room.nextRooms[3] != 0) {
        exits |= GAME_ROOM_EXIT_WEST_EXISTS;
    }
    return exits;
}

/*
 *This function opens the file for other functions to utilize
 *
 */
void OpenFile()
{
    sprintf(room.file, FILE_TEMPLATE, room.roomNumber);
    file = fopen(room.file, "rb");
}

void CloseFile()
{
    fclose(file);
}

/*
 *Updates the room's info after a new room has been selected
 *This function checks to see Items in the player's inventory, also loads the correct data into the room struct
 * and Player struct in the included class...
 * 
 *
 */

char* garbage;

int UpdateRoomInfo()
{
    //Variables
    uint8_t length = 0;

    //clearing the old DATA
    memset(room.desc, 0, sizeof (room.desc));
    memset(room.title, 0, sizeof (room.title));

    //Open File
    OpenFile();

    if (file == NULL) {
        FATAL_ERROR();
    }

    //Title
    fseek(file, 0, SEEK_SET);
    fread(&length, 1, 1, file);
    fread(room.title, 1, length, file);

    //Check for item
    fread(&length, 1, 1, file);
    if (length != '\0') {
        fread(room.roomInventory, 1, length, file);

        if (!(FindInInventory(room.roomInventory[0]) || FindInInventory(room.roomInventory[1])
                || FindInInventory(room.roomInventory[2]) || FindInInventory(room.roomInventory[3]))) {
            //ITERATION OF ROOM WITHOUT ITEM

            //Description
            fread(&length, 1, 1, file);
            fread(room.desc, 1, length, file);

            //Items
            fread(&length, 1, 1, file);
            if (length != '\0') {
                fread(room.roomInventory, 1, length, file);
                AddToInventory(room.roomInventory[0]);
                AddToInventory(room.roomInventory[1]);
                AddToInventory(room.roomInventory[2]);
                AddToInventory(room.roomInventory[3]);
            }

            fread(room.nextRooms, 1, 4, file);
            return SUCCESS;
        } else if (FindInInventory(room.roomInventory[0]) || FindInInventory(room.roomInventory[1])
                || FindInInventory(room.roomInventory[2]) || FindInInventory(room.roomInventory[3])) {
            //Literally copy pasted this from above, just changed array into garbage array
            fread(&length, 1, 1, file);
            fread(garbage, 1, length, file);
            fread(&length, 1, 1, file);
            fread(garbage, 1, length, file);
            fread(garbage, 1, 4, file);
            fread(garbage, 1, 1, file);

            //SECOND VERSION BEGINS HERE
            fread(&length, 1, 1, file);
            fread(room.desc, 1, length, file);
            fread(&length, 1, 1, file);
            if (length != '\0') {
                fread(room.roomInventory, 1, length, file);
                AddToInventory(room.roomInventory[0]);
                AddToInventory(room.roomInventory[1]);
                AddToInventory(room.roomInventory[2]);
                AddToInventory(room.roomInventory[3]);
            }
            fread(room.nextRooms, 1, 4, file);
            return SUCCESS;
        }
    }
    fread(&length, 1, 1, file);
    fread(room.desc, 1, length, file);
    fread(&length, 1, 1, file);
    if (length != '\0') {
        fread(room.roomInventory, 1, length, file);
        AddToInventory(room.roomInventory[0]);
        AddToInventory(room.roomInventory[1]);
        AddToInventory(room.roomInventory[2]);
        AddToInventory(room.roomInventory[3]);
    }
    fread(room.nextRooms, 1, 4, file);
    CloseFile();
    return SUCCESS;
}