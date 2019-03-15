/**
 *Lab09 BattleBoats
 *@Field.c
 *@Aviv Elor
 *@CMPE13L - Section 01 / aelor@ucsc.edu / ID:1482944
 *Collaborated With: Allen Aboytes
 *Field data functions for BattleBoats
 */

#define ENABLE_FIELD //remove this line to disable Field.c
#ifdef ENABLE_FIELD

// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Agent.h"
#include "CircularBuffer.h"
#include "Leds.h"
#include "Oled.h"
#include "Buttons.h"
#include "Protocol.h"
#include "Uart1.h"
#include "Field.h"
#include "OledDriver.h"
#include "FieldOled.h"

// **** Define any module-level, global, or external variables here ****

// **** Set any macros or preprocessor directives here ****
//display error string to OLED and block further execution
#define FATAL_ERROR_WITH_OLED(x)  OledSetDisplayInverted(); \
        OledDrawString(x); \
        OledUpdate(); \
        FATAL_ERROR() 

#define TRUE 1
#define FALSE 0

/**
 * FieldInit() will fill the passed field array with the data specified in positionData. Also the
 * lives for each boat are filled according to the `BoatLives` enum.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     FieldPosition.
 */
void FieldInit(Field *f, FieldPosition p)
{
    if (f) { //if f exists (heap is big enough)
        uint8_t i, j; //temporary holders (i = row, j = column)

        //initialize all lives to the max values
        f->hugeBoatLives = FIELD_BOAT_LIVES_HUGE; //6 lives
        f->largeBoatLives = FIELD_BOAT_LIVES_LARGE; //5 lives
        f->mediumBoatLives = FIELD_BOAT_LIVES_MEDIUM; //4 lives
        f->smallBoatLives = FIELD_BOAT_LIVES_SMALL; //3 lives

        //store the initial FieldPostion via nested loop
        for (i = 0; i < FIELD_ROWS; i++) { // increment the rows
            for (j = 0; j < FIELD_COLS; j++) { //increment the columns
                f->field[i][j] = p; //the FieldPosition to the current [row][column]
            }
        }
    } else { //else f is NULL, check heap size
        FATAL_ERROR_WITH_OLED("\n\tFieldInit() ERROR!"); //output error and block executions
    }
}

/**
 * Retrieves the value at the specified field position.
 * @param f The Field being referenced
 * @param row The row-component of the location to retrieve
 * @param col The column-component of the location to retrieve
 * @return
 */
FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col)
{
    if (f) { //if f exists (heap is big enough)
        return f->field[row][col]; //return the desired position by row and column
    } else { //something went really wrong
        FATAL_ERROR_WITH_OLED("\n\tFieldAt() ERROR!"); //output error and block executions
    }
    return FALSE; //should never reach here, used return to shut MPLABX up (warnings)
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_POSITION_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p)
{
    FieldPosition originalPosition = f->field[row][col]; //get the original position

    if (f) { //if f exists 
        f->field[row][col] = p;
    } else { //something went really wrong
        FATAL_ERROR_WITH_OLED("\n\tFieldSetLocation() ERROR!"); //output error and block executions
    }

    return originalPosition; //return the old value at that field location
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_POSITION_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_SMALL);
 * }
 *
 * should result in a field like:
 *  _ _ _ _ _ _ _ _
 * [ 3 3 3       3 ]
 * [ 4 4 4 4     3 ]
 * [             3 ]
 *  . . . . . . . .
 *
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_POSITION_*_BOAT values from the
 * FieldPosition enum.
 * @return TRUE for success, FALSE for failure
 */
uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type)
{
    FieldPosition p = FIELD_POSITION_EMPTY; //p is the FieldPosition Marker
    int tempRow = row, tempCol = col, bounds = 0; //create bound variables for placing boats
    uint8_t directionNegFlag = 0; //when accounting for NORTH/WEST we have to subtract the boat len
    int testIfEmpty = 0; //for testing to see if the desired location is empty

    //set up the bounds depending on the type of the boat
    if (type == FIELD_BOAT_SMALL) { //if small
        bounds = FIELD_BOAT_LIVES_SMALL; //add the small boat length 
        p = FIELD_POSITION_SMALL_BOAT; //position will be small boat
    } else if (type == FIELD_BOAT_MEDIUM) { //if medium
        bounds = FIELD_BOAT_LIVES_MEDIUM; //add the medium boat length
        p = FIELD_POSITION_MEDIUM_BOAT; //position will be medium boat
    } else if (type == FIELD_BOAT_LARGE) { //if large
        bounds = FIELD_BOAT_LIVES_LARGE; //add the large boat length
        p = FIELD_POSITION_LARGE_BOAT; //position will be large boat
    } else if (type == FIELD_BOAT_HUGE) { //if huge
        bounds = FIELD_BOAT_LIVES_HUGE; //add the huge boat length 
        p = FIELD_POSITION_HUGE_BOAT; //position will be huge boat
    } else { //else type was somehow invalid
        return FALSE; //failure
    }

    if (f) { //if f exists (heap is big enough)
        //finish setting bounds by dir. N/S placed in row directions, W/E placed in col directions
        if (dir == FIELD_BOAT_DIRECTION_NORTH || dir == FIELD_BOAT_DIRECTION_SOUTH) { //if N/S
            if (dir == FIELD_BOAT_DIRECTION_NORTH) { //if direction is north
                directionNegFlag = TRUE; //then the boat length will face in the opposite direction
                bounds *= (-1); //get the negative boat length value
            }
            bounds += row; //add the current boat length value to the initial row
            if (directionNegFlag) { //if we had to account for N/W
                if (bounds >= -1) { //then the bounds will have to be positive or 0
                    testIfEmpty = tempRow;
                    while (testIfEmpty > bounds) {
                        if (f->field[testIfEmpty][col] != FIELD_POSITION_EMPTY) {
                            return FALSE;
                        }
                        testIfEmpty--;
                    }
                    while (tempRow > bounds) { //while the original rows are greater than the bounds
                        if (f->field[tempRow][col] == FIELD_POSITION_EMPTY) { //if open position
                            f->field[tempRow][col] = p; //place the boat type marker
                            tempRow--; //decrement the temporary row
                        } else { //else the field position isn't empty
                            return FALSE; //return Failure
                        }
                    } //end while
                } else { //else the bounds are negative
                    return FALSE; //return failure
                }
                directionNegFlag = FALSE; //clear the directionNegFlag
            } else { //else the direction is South
                if (bounds <= FIELD_ROWS) { //if the bounds are not greater than the field rows
                    testIfEmpty = tempRow;
                    while (testIfEmpty < bounds) {
                        if (f->field[testIfEmpty][col] != FIELD_POSITION_EMPTY) {
                            return FALSE;
                        }
                        testIfEmpty++;
                    }
                    while (tempRow < bounds) { //while the tempRow is less than the bounds
                        if (f->field[tempRow][col] == FIELD_POSITION_EMPTY) { //if empty
                            f->field[tempRow][col] = p; //place the boat type marker
                            tempRow++; //increment the tempRow
                        } else { //else the bounds are greater than the field rows
                            return FALSE; //return failure
                        }
                    } //end while
                } else { //else they are outside of our field rows bound
                    return FALSE; //return failure
                }
            } //end if north or south
        } else if (dir == FIELD_BOAT_DIRECTION_WEST || dir == FIELD_BOAT_DIRECTION_EAST) { //if W/E
            if (dir == FIELD_BOAT_DIRECTION_WEST) { //if direction is west
                directionNegFlag = TRUE; //then the boat length will face in the opposite direction
                bounds *= (-1); //get the negative boat length value
            }
            bounds += col; //add the current boat length value to the initial column
            if (directionNegFlag) { //if we had to account for N/W
                if (bounds >= -1) { //if the bounds aren't negative
                    testIfEmpty = tempCol;
                    while (testIfEmpty > bounds) {
                        if (f->field[row][testIfEmpty] != FIELD_POSITION_EMPTY) {
                            return FALSE;
                        }
                        testIfEmpty--;
                    }
                    while (tempCol > bounds) { //while the columns aren't greater than the bound
                        if (f->field[row][tempCol] == FIELD_POSITION_EMPTY) { //if empty
                            f->field[row][tempCol] = p; //place the boat type position
                            tempCol--; //decrement the temp row
                        } else { //else the position is not empty
                            return FALSE; //return failure
                        }
                    } //end while
                } else { //else bounds are negative
                    return FALSE; //return failure
                }
                directionNegFlag = FALSE; //clear the directionNegFlag
            } else { //else our direction is east
                if (bounds <= FIELD_COLS) { //if the bounds are less than the max field columns 
                    testIfEmpty = tempCol;
                    while (testIfEmpty < bounds) {
                        if (f->field[row][testIfEmpty] != FIELD_POSITION_EMPTY) {
                            return FALSE;
                        }
                        testIfEmpty++;
                    }
                    while (tempCol < bounds) { //while the tempRow is less than the bounds
                        if (f->field[row][tempCol] == FIELD_POSITION_EMPTY) { //if empty
                            f->field[row][tempCol] = p; //set the boat type position
                            tempCol++; //increment the temp Row
                        } else { //else the position isn't empty
                            return FALSE; //return failure
                        }
                    } //end while
                } else { //else the bounds are greater than the max field columns
                    return FALSE; //return failure
                }
            } //end if east or west
        } else { //else direction was somehow invalid
            return FALSE; //failure
        }
        return TRUE; //if no failures were hit, then successful
    } else { //something went really wrong
        FATAL_ERROR_WITH_OLED("\n\tFieldAddBoat() ERROR!"); //output error and block executions
    }
    return FALSE; //should never reach here, return FALSE for failure
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_POSITION_HIT or FIELD_POSITION_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The HIT result is stored in gData->hit as an
 *               output.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData * gData)
{
    FieldPosition originalPosition = f->field[gData->row][gData->col]; //get the original position

    if (f) { //if f exists
        //if the original position is a boat type
        if ((originalPosition < FIELD_POSITION_MISS) && originalPosition != FIELD_POSITION_EMPTY) {
            f->field[gData->row][gData->col] = FIELD_POSITION_HIT; //then mark a hit position
            if (originalPosition == FIELD_POSITION_SMALL_BOAT) { //if ori was small boat
                if (f->smallBoatLives != 0) { //if lives are still remaining
                    f->smallBoatLives--; //decrement the lives
                    if (f->smallBoatLives == 0) { //if zero lives, mark as sunk
                        gData->hit = HIT_SUNK_SMALL_BOAT;
                        return originalPosition;
                    }
                }
            } else if (originalPosition == FIELD_POSITION_MEDIUM_BOAT) { //if ori was medium boat
                if (f->mediumBoatLives != 0) { //if lives are still remaining
                    f->mediumBoatLives--; //decrement the lives
                    if (f->mediumBoatLives == 0) { //if zero lives, mark as sunk
                        gData->hit = HIT_SUNK_MEDIUM_BOAT;
                        return originalPosition;
                    }
                }
            } else if (originalPosition == FIELD_POSITION_LARGE_BOAT) { //if ori was large boat
                if (f->largeBoatLives != 0) { //if lives are still remaining
                    f->largeBoatLives--; //decrement the lives
                    if (f->largeBoatLives == 0) { //if zero lives, mark as sunk
                        gData->hit = HIT_SUNK_LARGE_BOAT;
                        return originalPosition;
                    }
                }
            } else if (originalPosition == FIELD_POSITION_HUGE_BOAT) { //if ori was huge boat
                if (f->hugeBoatLives != 0) { //if lives are still remaining
                    f->hugeBoatLives--; //decrement the lives
                    if (f->hugeBoatLives == 0) { //if zero lives, mark as sunk
                        gData->hit = HIT_SUNK_HUGE_BOAT;
                        return originalPosition;
                    }
                }
            } //end else ifs
            gData->hit = HIT_HIT; //the position was a boat type, so respond data as hit
        } else if (originalPosition == FIELD_POSITION_EMPTY) { //else if empty position
            gData->hit = HIT_MISS; //respond as miss
            f->field[gData->row][gData->col] = FIELD_POSITION_MISS; //set the field position to miss
        }
    } else { //else something went really wrong
        FATAL_ERROR_WITH_OLED("\n\tFieldRegisterEnemyAttack() ERROR!"); //error, block executions
    }

    return originalPosition; //return the original position
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_POSITION_HIT at that position. If it was a miss, display a FIELD_POSITION_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * HIT_SUNK_*_BOAT.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
FieldPosition FieldUpdateKnowledge(Field *f, const GuessData * gData)
{
    FieldPosition originalPosition = f->field[gData->row][gData->col]; //get the original position

    if (f) { //if f exists
        if (gData->hit == HIT_MISS) { //if miss
            f->field[gData->row][gData->col] = FIELD_POSITION_EMPTY; //position is empty
        } else if (gData->hit != HIT_MISS) { //hit miss == 0
            f->field[gData->row][gData->col] = FIELD_POSITION_HIT; //hit mark the position
            if (gData->hit == HIT_SUNK_SMALL_BOAT) { //if hit & small boat sunk
                f->smallBoatLives = 0; //boat sunk, so no lives remaining
            } else if (gData->hit == HIT_SUNK_MEDIUM_BOAT) { //if hit & medium boat sunk
                f->mediumBoatLives = 0; //boat sunk, so no lives remaining
            } else if (gData->hit == HIT_SUNK_LARGE_BOAT) { //if hit & large boat sunk
                f->largeBoatLives = 0; //boat sunk, so no lives remaining
            } else if (gData->hit == HIT_SUNK_HUGE_BOAT) { //if hit & huge boat sunk
                f->hugeBoatLives = 0; //boat sunk, so no lives remaining
            }
        }
    } else { //else something went really wrong
        FATAL_ERROR_WITH_OLED("\n\tFieldUpdateKnowlege() ERROR!"); //output error, block executions
    }

    return originalPosition; //return the previous value of that coordinate position
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field * f)
{
    //initialize allBoatStates to 0 on every call
    uint8_t allBoatStates = 0; //4-bit value of boat states (1 alive, 0 dead)

    if (f) { //if f exists
        if (f->smallBoatLives != 0) { //if smallBoatLives are not 0
            allBoatStates |= FIELD_BOAT_STATUS_SMALL; //show small boat still lives | 0b0001
        }

        if (f->mediumBoatLives != 0) { //if mediumBoatLives are not 0
            allBoatStates |= FIELD_BOAT_STATUS_MEDIUM; //show medium boat still lives | 0b0010
        }

        if (f->largeBoatLives != 0) { //if largeBoatLives are not 0
            allBoatStates |= FIELD_BOAT_STATUS_LARGE; //show large boat still lives | 0b0100
        }

        if (f->hugeBoatLives != 0) { //if hugeBoatLives are not 0
            allBoatStates |= FIELD_BOAT_STATUS_HUGE; //show huge boat still lives | 0b1000
        }
    } else { //else something went really wrong
        FATAL_ERROR_WITH_OLED("\n\tFieldGetBoatStates() ERROR!"); //output error, block executions
    }

    return allBoatStates; //return the results of allBoatStates
}

#endif //ENABLE_FIELD