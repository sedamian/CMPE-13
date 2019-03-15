#include "Field.h"
#include "BOARD.h"
 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
 
#include <xc.h>
#include <plib.h>
 
#define TRUE 1
#define FALSE 0
 
 
 
/**
 * FieldInit() will fill the passed field array with the data specified in positionData. Also the
 * lives for each boat are filled according to the `BoatLives` enum.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     FieldPosition.
 */
void FieldInit(Field *f, FieldPosition p){
    if (f) {       
        int r,c;
        // This will store the position of the field
        for (r = 0;r < FIELD_ROWS;r++ ){
            for (c= 0;c < FIELD_COLS; c++){
                
                f->field[r][c] = p;
                
            }
        }   
        // Boats number of lives.
        f->hugeBoatLives = FIELD_BOAT_LIVES_HUGE;
        f->largeBoatLives = FIELD_BOAT_LIVES_LARGE;
        f->mediumBoatLives = FIELD_BOAT_LIVES_MEDIUM;
        f->smallBoatLives = FIELD_BOAT_LIVES_SMALL;
    }
};
 
/**
 * Retrieves the value at the specified field position.
 * @param f The Field being referenced
 * @param row The row-component of the location to retrieve
 * @param col The column-component of the location to retrieve
 * @return
 */
FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col){
    
    return f->field[row][col]; // take the data from field
};
 
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
FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p){
  //p=FieldAt(f,row,col);// first positon
    f->field[row][col]=p;  // second position
    return p;
};
 
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
uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type){
    int i; // use for loops
    int prove=FALSE; // Flag
 
    // Switch for diffent types of boats
    switch(type){
        case FIELD_BOAT_SMALL:
            switch (dir){// Switch for direction of the boat
                case FIELD_BOAT_DIRECTION_NORTH:
                    for(i=0;i<3;i++){
                        if(row>=2 && f->field[row - i][col]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
 
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<3;i++){
                            f->field[row - i][col]= FIELD_POSITION_SMALL_BOAT;
                        }
                        prove=FALSE;
                    }            
                    break;
                case FIELD_BOAT_DIRECTION_EAST:
                    for(i=0;i<3;i++){
                        if(col<=7 && f->field[row][col + i]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<3;i++){
                            f->field[row][col + i]= FIELD_POSITION_SMALL_BOAT;
                        }
                        prove=FALSE;
                        //return TRUE; 
                    }//return TRUE;      
                    break;
                case FIELD_BOAT_DIRECTION_SOUTH:
                    for(i=0;i<3;i++){
                        if(row<=3 && f->field[row + i][col]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<3;i++){
                            f->field[row + i][col]= FIELD_POSITION_SMALL_BOAT;
                        }
                        prove=FALSE; 
                        //return TRUE; 
                    };      
                    break;
                case FIELD_BOAT_DIRECTION_WEST:
                    for(i=0;i<3;i++){
                        if(col>=2 && f->field[row][col-i]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<3;i++){
                            f->field[row][col-i]= FIELD_POSITION_SMALL_BOAT;
                        }
                        prove=FALSE; 
                        //return TRUE; 
                    }//return TRUE;      
                    break;
//               default:
//                   return FALSE;
//                    //printf("No valid orientation");
//                    break;
            }
            break;
        case FIELD_BOAT_MEDIUM:
            switch (dir){
                case FIELD_BOAT_DIRECTION_NORTH:
                    for(i=0;i<4;i++){
                        if(col>=3 && f->field[row - i][col]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<4;i++){
                            f->field[row - i][col]= FIELD_POSITION_MEDIUM_BOAT;
                        }
                        prove=FALSE;
                        //return TRUE; 
                    }//return TRUE;                    
                    break;
                case FIELD_BOAT_DIRECTION_EAST:
                    for(i=0;i<4;i++){
                        if(col<=6 && f->field[row][col + i]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<4;i++){
                            f->field[row][col + i]= FIELD_POSITION_MEDIUM_BOAT;
                        }
                        prove=FALSE; 
                        //return TRUE; 
                    }//return TRUE;      
                    break;
                case FIELD_BOAT_DIRECTION_SOUTH:
                    for(i=0;i<4;i++){
                        if(col<=2 && f->field[row + i][col]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<4;i++){
                            f->field[row + i][col]= FIELD_POSITION_MEDIUM_BOAT;
                        }
                        prove=FALSE; 
                        //return TRUE; 
                    } //return TRUE;     
                    break;
                case FIELD_BOAT_DIRECTION_WEST:
                    for(i=0;i<4;i++){
                        if(col>=3 && f->field[row][col-i]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<4;i++){
                            f->field[row][col-i]= FIELD_POSITION_MEDIUM_BOAT;
                        }
                        prove=FALSE; 
                        //return TRUE; 
                    } //return TRUE;     
                    break;
//                default:
//                    return FALSE;
//                    //printf("No valid orientation");
//                    break;
            }//return TRUE;
            break;
        case FIELD_BOAT_LARGE:
            switch (dir){
                case FIELD_BOAT_DIRECTION_NORTH:
                    for(i=0;i<5;i++){
                        if(col>=4 && f->field[row - i][col]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<5;i++){
                            f->field[row - i][col]= FIELD_POSITION_LARGE_BOAT;
                        }
                        prove=FALSE;
                        //return TRUE; 
                    } //return TRUE;                   
                    break;
                case FIELD_BOAT_DIRECTION_EAST:
                    for(i=0;i<5;i++){
                        if(col<=5 && f->field[row][col + i]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<5;i++){
                            f->field[row][col + i]= FIELD_POSITION_LARGE_BOAT;
                        }
                        prove=FALSE; 
                        //return TRUE; 
                    }//return TRUE;      
                    break;
                case FIELD_BOAT_DIRECTION_SOUTH:
                    for(i=0;i<5;i++){
                        if(col<=1 && f->field[row + i][col]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<5;i++){
                            f->field[row + i][col]= FIELD_POSITION_LARGE_BOAT;
                        }
                        prove=FALSE;
                        //return TRUE; 
                    }//return TRUE;      
                    break;
                case FIELD_BOAT_DIRECTION_WEST:
                    for(i=0;i<5;i++){
                        if(col>=4 && f->field[row][col-i]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<5;i++){
                            f->field[row][col-i]= FIELD_POSITION_LARGE_BOAT;
                        }
                        prove=FALSE;
                        //return TRUE; 
                    } //return TRUE;     
                    break;
//                default:  
//                    return FALSE;
//                    //printf("No valid orientation");
//                    break;
            }//return TRUE;
            break;
        case FIELD_BOAT_HUGE:
            switch (dir){
                case FIELD_BOAT_DIRECTION_NORTH:
                    for(i=0;i<6;i++){
                        if(col>=5 && f->field[row - i][col]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<6;i++){
                            f->field[row - i][col]= FIELD_POSITION_HUGE_BOAT;
                        }
                       prove=FALSE;
                        //return TRUE; 
                    } //return TRUE;                   
                    break;
                case FIELD_BOAT_DIRECTION_EAST:
                    for(i=0;i<6;i++){
                        if(col<=3 && f->field[row][col + i]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<6;i++){
                            f->field[row][col + i]= FIELD_POSITION_HUGE_BOAT;
                        }
                        prove=FALSE;
                        //return TRUE; 
                    }  //return TRUE;    
                    break;
                case FIELD_BOAT_DIRECTION_SOUTH:
                    for(i=0;i<6;i++){
                        if(col<=0 && f->field[row + i][col]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<6;i++){
                            f->field[row + i][col]= FIELD_POSITION_HUGE_BOAT;
                        }
                        prove=FALSE;
                        //return TRUE; 
                    }  //return TRUE;    
                    break;
                case FIELD_BOAT_DIRECTION_WEST:
                    for(i=0;i<6;i++){
                        if(col>=5 && f->field[row][col-i]==FIELD_POSITION_EMPTY){
                            prove=TRUE;
                            //return prove;
                   
                        }else{
                            return FALSE;
                        }  
                    }if (prove==TRUE){
                        for(i=0;i<6;i++){
                            f->field[row][col-i]= FIELD_POSITION_HUGE_BOAT;
                        }
                        prove=FALSE;
                        //return TRUE; 
                    }//return TRUE;      
                    break; 
            }// end of 
            //return TRUE;
            break;    
    }//end of the switch statement
    return TRUE;
};
 
 
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
FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData *gData){
   FieldPosition posicion;// a variable to manipulate position.
    posicion=FieldAt(f,gData->row,gData->row);// position
    if(posicion!=FIELD_POSITION_EMPTY && posicion < FIELD_POSITION_MISS){
        switch (posicion){// switch statement for boats hit
            case FIELD_POSITION_SMALL_BOAT:             
              if(f->smallBoatLives > 1){// test is the boat is sunk already
                    gData->hit= HIT_HIT;
                    f->smallBoatLives --;// decrement the lives of the boat
                    
              }else{
                f->smallBoatLives --;
                gData->hit =HIT_SUNK_SMALL_BOAT; 
                
              }
            break;
        case FIELD_POSITION_MEDIUM_BOAT:
            if(f->smallBoatLives > 1){// test is the boat is sunk already
                    gData->hit= HIT_HIT;
                    f->smallBoatLives --;// decrement the lives of the boat
                    
              }else{
                f->smallBoatLives --;
                gData->hit=HIT_SUNK_SMALL_BOAT; 
                
              }//FieldSetLocation(f,gData->row,gData->col, FIELD_POSITION_HIT);
            break;
        case FIELD_POSITION_LARGE_BOAT:
            if(f->smallBoatLives > 1){// test is the boat is sunk already
                    gData->hit= HIT_HIT;
                    f->smallBoatLives --;// decrement the lives of the boat
                    
              }else{
                f->smallBoatLives --;
                gData->hit =HIT_SUNK_SMALL_BOAT; 
                
              }//FieldSetLocation(f,gData->row,gData->col, FIELD_POSITION_HIT);
            break;
        case FIELD_POSITION_HUGE_BOAT:
            if(f->smallBoatLives > 1){// test is the boat is sunk already
                    gData->hit= HIT_HIT;
                    f->smallBoatLives --;// decrement the lives of the boat
                   
              }else{
                f->smallBoatLives --;
                gData->hit =HIT_SUNK_SMALL_BOAT; 
                
              }//FieldSetLocation(f,gData->row,gData->col, FIELD_POSITION_HIT);
            break;
//        case FIELD_POSITION_EMPTY:
//            gData->hit=HIT_MISS;
//             f->field[gData->row][gData->col] = FIELD_POSITION_MISS;
//             return posicion;
//            break;
        default:
            posicion =f->field[gData->row][gData->col];
            posicion =f->field[gData->row][gData->col]=FIELD_POSITION_HIT;
            return posicion;
            break;
      
            
        
    }return posicion;
  //}
   }else{
        gData->hit=HIT_MISS;
        posicion =f->field[gData->row][gData->col];
        posicion =f->field[gData->row][gData->col]=FIELD_POSITION_HIT;
//        FieldSetLocation(f,gData->row,gData->col, FIELD_POSITION_MISS);
      return posicion;  
   }
    return posicion;
};
 
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
FieldPosition FieldUpdateKnowledge(Field *f, const GuessData *gData){
    FieldPosition oldposicion;// variable to manipulate field position
    oldposicion=FieldAt(f,gData->col,gData->row); // set the field position.
    //if(gData->hit==HIT_HIT){
        //f->field[gData->row][gData->col]=FIELD_POSITION_HIT;
        //FieldSetLocation(f,gData->row,gData->col,FIELD_POSITION_HIT);
        switch(gData->hit){// switch statment to show if the boats sunk.
            case HIT_HIT:
                f->field[gData->row][gData->col]=FIELD_POSITION_HIT;
                return oldposicion;
            case HIT_SUNK_SMALL_BOAT:
                f->field[gData->row][gData->col]=FIELD_POSITION_HIT;
                f->smallBoatLives=0;
                return oldposicion;
                
                break;
            case HIT_SUNK_MEDIUM_BOAT:
                f->field[gData->row][gData->col]=FIELD_POSITION_HIT;
                f->mediumBoatLives=0;
                return oldposicion;
                break;
            case HIT_SUNK_LARGE_BOAT:
                f->field[gData->row][gData->col]=FIELD_POSITION_HIT;
                f->largeBoatLives=0;
                return oldposicion;
                break;
            case HIT_SUNK_HUGE_BOAT:
                f->field[gData->row][gData->col]=FIELD_POSITION_HIT;
                f->hugeBoatLives=0;
                return oldposicion;
                break;
            default:
                FieldSetLocation(f,gData->row,gData->col,FIELD_POSITION_EMPTY);
                return oldposicion;
                break;
        }
    //}else if(gData->hit!=HIT_MISS){
        //FieldSetLocation(f,gData->row,gData->col,FIELD_POSITION_EMPTY);
        //return oldposicion;
    return oldposicion; // return the old positions
};
 
/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f){
    uint8_t state = 0; 
    
    if(f->smallBoatLives!= 0){// verify of the boat is no 0
        state=state | FIELD_BOAT_STATUS_SMALL;        
    }
    if(f->mediumBoatLives!= 0){
        state=state | FIELD_BOAT_STATUS_MEDIUM;
    }
    if(f->largeBoatLives!= 0){
        state=state | FIELD_BOAT_STATUS_LARGE;
    }if(f->hugeBoatLives!= 0){
        state=state | FIELD_BOAT_STATUS_HUGE;
    }
 
    return state; // return the results
    
           
}
 
 
 
