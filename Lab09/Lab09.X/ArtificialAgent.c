/*
 * File:   ArtificialAgent.c
 * Author: sedamian
 *
 * Created on June 8, 2017, 4:24 PM
 */


#include "xc.h"
#include "Agent.h"
#include "Protocol.h"
#include "Field.h"
#include "Oled.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>






static ProtocolParserStatus Inbox;
static NegotiationData ClassifiedInfo;
static GuessData enemyGData = {0, 0, 0};
static NegotiationData NData;

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts. This can include things like initialization of the field, placement of the boats,
 * etc. The agent can assume that stdlib's rand() function has been seeded properly in order to
 * use it safely within.
 */
void AgentInit(void) {



}

/**
 * The Run() function for an Agent takes in a single character. It then waits until enough
 * data is read that it can decode it as a full sentence via the Protocol interface. This data
 * is processed with any output returned via 'outBuffer', which is guaranteed to be 255
 * characters in length to allow for any valid NMEA0183 messages. The return value should be
 * the number of characters stored into 'outBuffer': so a 0 is both a perfectly valid output and
 * means a successful run.
 * @param in The next character in the incoming message stream.
 * @param outBuffer A string that should be transmit to the other agent. NULL if there is no
 *                  data.
 * @return The length of the string pointed to by outBuffer (excludes \0 character).
 */
int AgentRun(char in, char *outBuffer) {

    if (in) {
        Inbox = ProtocolDecode(in, &NData, &enemyGData);
    }


    switch (AgentState) {
        case AGENT_STATE_GENERATE_NEG_DATA:


            AgentState = AGENT_STATE_SEND_CHALLENGE_DATA;
            ProtocolGenerateNegotiationData(&ClassifiedInfo);
            return ProtocolEncodeChaMessage(outBuffer, &ClassifiedInfo);
            break;

        case AGENT_STATE_SEND_CHALLENGE_DATA:

            if (Inbox == PROTOCOL_PARSED_CHA_MESSAGE){

            AgentState = AGENT_STATE_DETERMINE_TURN_ORDER;
            
            
            
            }
            break;

        case AGENT_STATE_DETERMINE_TURN_ORDER:

            if (Inbox == PROTOCOL_PARSED_DET_MESSAGE){
                
                
                
                
                
                if  (){
                    
                }
                else if (){
                    
                }
                else if (){
                    
                } 
                
                
                
                
                
                
                
            }

            break;


        case AGENT_STATE_SEND_GUESS:



            break;

        case AGENT_STATE_WAIT_FOR_HIT:



            break;

        case AGENT_STATE_WAIT_FOR_GUESS:





            break;

        case AGENT_STATE_INVALID:


            break;

        case AGENT_STATE_LOST:



            break;

        case AGENT_STATE_WON:

            break;

    }


}

/**
 * StateCheck() returns a 4-bit number indicating the status of that agent's ships. The smallest
 * ship, the 3-length one, is indicated by the 0th bit, the medium-length ship (4 tiles) is the
 * 1st bit, etc. until the 3rd bit is the biggest (6-tile) ship. This function is used within
 * main() to update the LEDs displaying each agents' ship status. This function is similar to
 * Field::FieldGetBoatStates().
 * @return A bitfield indicating the sunk/unsunk status of each ship under this agent's control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetStatus(void) {



}

/**
 * This function returns the same data as `AgentCheckState()`, but for the enemy agent.
 * @return A bitfield indicating the sunk/unsunk status of each ship under the enemy agent's
 *         control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetEnemyStatus(void) {



}