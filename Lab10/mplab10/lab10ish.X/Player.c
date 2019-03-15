/*
 * File:   Player.c
 * Author: sedamian
 *
 * Created on June 13, 2017, 12:46 AM
 */

#include <stdio.h>

#include <string.h>
#include "Player.h"
#include "UnixBOARD.h"
#include "Game.h"

static struct pStruct {
    uint16_t packback[INVENTORY_SIZE];
    uint16_t ItemSpot;


} pStruct;

/**
 * Adds the specified item to the player's inventory if the inventory isn't full.
 * @param item The item number to be stored: valid values are 0-255.
 * @return SUCCESS if the item was added, STANDARD_ERRROR if the item couldn't be added.
 */
int AddToInventory(uint8_t item)
{

    pStruct.packback[pStruct.ItemSpot] = item; // putting item into inventory

    if (pStruct.packback[pStruct.ItemSpot] != '\0') {
        pStruct.ItemSpot = pStruct.ItemSpot + 1;
        return SUCCESS;
    }
    return STANDARD_ERROR;




}

/**
 * Check if the given item exists in the player's inventory.
 * @param item The number of the item to be searched for: valid values are 0-255.
 * @return SUCCESS if it was found or STANDARD_ERROR if it wasn't.
 */
int FindInInventory(uint8_t item)
{

    // checks the locations for the item.
    int i;
    for (i = 0; pStruct.ItemSpot < INVENTORY_SIZE; i++) {
        if (pStruct.packback[i] == item) {
            return SUCCESS;
        }
    }
    return STANDARD_ERROR;



}










