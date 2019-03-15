#include "Protocol.h"
#include "BOARD.h"
#include "Field.h"
#include "Agent.h"
#include "Oled.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Buttons.h"
#include <plib.h>
#include <xc.h>
#include <stdbool.h>

typedef enum {
    WAITING, RECORDING, FIRST_CHECKSUM_HALF, SECOND_CHECKSUM_HALF, NEWLINE
} prostate;

typedef struct sdata {
    uint8_t checksum;



} sdata;
//static uint8_t Checksum(char *stcha);
static uint8_t Checksum(char *stcha);
static uint8_t AsciiHexTo(char c);


static sdata sdata2;


static char message4u[PROTOCOL_MAX_MESSAGE_LEN];
static int mesIndex;


static char lodataCoo[PROTOCOL_MAX_PAYLOAD_LEN] = {};
static char lodataHit[PROTOCOL_MAX_PAYLOAD_LEN] = {};
static char lodataCha[PROTOCOL_MAX_PAYLOAD_LEN] = {};
static char lodataDet[PROTOCOL_MAX_PAYLOAD_LEN] = {};
//static uint8_t creearsum(char *Str);


static uint8_t checks;
static uint8_t fromHexTo;
static prostate dState = WAITING;


static uint8_t ChkSumMess;
//static uint8_t CheckSum2;

static int i;


static char *messComp1;
static char *messComp2;
static char *messComp3;

int ProtocolEncodeCooMessage(char *message, const GuessData *data)
{
    //int x,y;

    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
        message[i] = '\0'; //Reset the incoming message pointer to NULL

    }
    sprintf(lodataCoo, PAYLOAD_TEMPLATE_COO, data->row, data->col);
    checks = Checksum(lodataCoo);
    sprintf(message, MESSAGE_TEMPLATE, lodataCoo, checks);
    return strlen(message);
    //return y;
};

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeHitMessage(char *message, const GuessData *data)
{
    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
        message[i] = '\0'; //Reset the incoming message pointer to NULL

    }
    sprintf(lodataHit, PAYLOAD_TEMPLATE_HIT, data->row, data->col, data->hit);
    checks = Checksum(lodataHit);
    sprintf(message, MESSAGE_TEMPLATE, lodataHit, checks);
    return strlen(message);
};

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeChaMessage(char *message, const NegotiationData *data)
{

    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
        message[i] = '\0'; //Reset the incoming message pointer to NULL

    }
    sprintf(lodataCha, PAYLOAD_TEMPLATE_CHA, data->encryptedGuess, data->hash);
    checks = Checksum(lodataCha);
    sprintf(message, MESSAGE_TEMPLATE, lodataCha, checks);
    return strlen(message);
};

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeDetMessage(char *message, const NegotiationData *data)
{

    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
        message[i] = '\0'; //Reset the incoming message pointer to NULL

    }


    sprintf(lodataDet, PAYLOAD_TEMPLATE_DET, data->guess, data->encryptionKey);
    checks = Checksum(lodataDet);
    sprintf(message, MESSAGE_TEMPLATE, lodataDet, checks);
    return strlen(message);
};

/**
 * This function decodes a message into either the NegotiationData or GuessData structs depending
 * on what the type of message is. This function receives the message one byte at a time, where the
 * messages are in the format defined by MESSAGE_TEMPLATE, with payloads of the format defined by
 * the PAYLOAD_TEMPLATE_* macros. It returns the type of message that was decoded and also places
 * the decoded data into either the `nData` or `gData` structs depending on what the message held.
 * The onus is on the calling function to make sure the appropriate structs are available (blame the
 * lack of function overloading in C for this ugliness).
 *
 * PROTOCOL_PARSING_FAILURE is returned if there was an error of any kind (though this excludes
 * checking for NULL pointers), while
 * 
 * @param in The next character in the NMEA0183 message to be decoded.
 * @param nData A struct used for storing data if a message is decoded that stores NegotiationData.
 * @param gData A struct used for storing data if a message is decoded that stores GuessData.
 * @return A value from the UnpackageDataEnum enum.
 */
ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData)
{
    char *WhatIs;
    switch (dState) {
    case WAITING:
        if (in == '$') {
            for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
                message4u[i] = '\0'; //Reset message pointer to NULL

            }
            mesIndex = 0;
            dState = RECORDING;
            return PROTOCOL_PARSING_GOOD;
        } else {


            return PROTOCOL_WAITING;
        }
        break;
    case RECORDING:

        if (in == '*') {
            //        message[mesIndex] = '\0';
            dState = FIRST_CHECKSUM_HALF;
            return PROTOCOL_PARSING_GOOD;
        } else {
            message4u[mesIndex] = in;
            mesIndex++;


            return PROTOCOL_PARSING_GOOD;
        }

        break;
    case FIRST_CHECKSUM_HALF:

        //  if not invalid


        fromHexTo = AsciiHexTo(in);

        if ((16 > fromHexTo)) {


            sdata2.checksum = fromHexTo << 4;

            dState = SECOND_CHECKSUM_HALF;

            return PROTOCOL_PARSING_GOOD;


        } else {
            dState = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
        break;
    case SECOND_CHECKSUM_HALF:

        // if not invalid
        fromHexTo = AsciiHexTo(in);
        sdata2.checksum |= fromHexTo;
        ChkSumMess = Checksum(message4u);
        //    CheckSum2
        if ((127 > fromHexTo) && ((sdata2.checksum) == ChkSumMess)) {

            message4u[mesIndex] = '\0';
            dState = NEWLINE;
            return PROTOCOL_PARSING_GOOD;
        } else {
            dState = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
        break;

    case NEWLINE:



        if ((in == '\n')) {

            dState = WAITING;

            WhatIs = strtok(message4u, ",");

            if ((strncmp(WhatIs, "COO", 3)) == 0) {


                messComp1 = strtok(NULL, ",");
                messComp2 = strtok(NULL, "*");


                gData->row = atoi(messComp1);
                gData->col = atoi(messComp2);
                return PROTOCOL_PARSED_COO_MESSAGE;

            } else if ((strncmp(WhatIs, "HIT", 3)) == 0) {

                messComp1 = strtok(NULL, ",");
                messComp2 = strtok(NULL, ",");
                messComp3 = strtok(NULL, "*");


                gData->row = atoi(messComp1);
                gData->col = atoi(messComp2);
                gData->hit = atoi(messComp3);
                return PROTOCOL_PARSED_HIT_MESSAGE;

            } else if ((strncmp(WhatIs, "CHA", 3)) == 0) {

                messComp1 = strtok(NULL, ",");
                messComp2 = strtok(NULL, ",");
                nData->encryptedGuess = atoi(messComp1);
                nData->hash = atoi(messComp2);
                return PROTOCOL_PARSED_CHA_MESSAGE;

            } else if ((strncmp(WhatIs, "DET", 3)) == 0) {

                messComp1 = strtok(NULL, ",");
                messComp2 = strtok(NULL, "*");
                nData->guess = atoi(messComp1);
                nData->encryptionKey = atoi(messComp2);
                return PROTOCOL_PARSED_DET_MESSAGE;
            } else {

                return PROTOCOL_PARSING_FAILURE;
            }
        } else {

            dState = WAITING;
            return PROTOCOL_PARSING_FAILURE;

        }
        break;
    default:
        return PROTOCOL_PARSING_FAILURE;
    }
    return 0;
}

/**
 * This function generates all of the data necessary for the negotiation process used to determine
 * the player that goes first. It relies on the pseudo-random functionality built into the standard
 * library. The output is stored in the passed NegotiationData struct. The negotiation data is
 * generated by creating two random 16-bit numbers, one for the actual guess and another for an
 * encryptionKey used for encrypting the data. The 'encryptedGuess' is generated with an
 * XOR(guess, encryptionKey). The hash is simply an 8-bit value that is the XOR() of all of the
 * bytes making up both the guess and the encryptionKey. There is no checking for NULL pointers
 * within this function.
 * @param data The struct used for both input and output of negotiation data.
 */
void ProtocolGenerateNegotiationData(NegotiationData * data)
//{
//    uint32_t enkey = 0xFF;
//    uint32_t guessra = rand();
//    //    int i;
//    data->guess = guessra;
//    data->encryptionKey = rand();
//    data->encryptedGuess = data->guess ^ data->encryptionKey;
//    data->hash = (guessra >> 8)^(guessra & enkey)^(data->encryptionKey >> 8)^(data->encryptionKey & enkey);
//};
{
    //rand() used for random numbers
    data->guess = (rand() & 0xFFFF); // & 0xFFFF same as mod 2^16 bit range
    data->encryptionKey = (rand() & 0xFFFF); // & 0xFFFF same as mod 2^16 bit range
    data->encryptedGuess = (data->guess ^ data->encryptionKey); //Encrypt with XOR

    //1 byte hash which XORs all bits from Guess and Encryption key
    uint8_t hash = (data->guess >> 8) & 0xFF; //first 8 bits
    hash ^= (data->guess) & 0xFF; //last 8 bits
    hash ^= (data->encryptionKey >> 8) & 0xFF; //first 8 bits
    hash ^= (data->encryptionKey) & 0xFF; //last 8 bits
    data->hash = hash;
}

/**
 * Validates that the negotiation data within 'data' is correct according to the algorithm given in
 * GenerateNegotitateData(). Used for verifying another agent's supplied negotiation data. There is
 * no checking for NULL pointers within this function. Returns TRUE if the NegotiationData struct
 * is valid or FALSE on failure.
 * @param data A filled NegotiationData struct that will be validated.
 * @return TRUE if the NegotiationData struct is consistent and FALSE otherwise.
 */
uint8_t ProtocolValidateNegotiationData(const NegotiationData * data)
//{
//    uint32_t encrip;
//    uint32_t hdata;
//    uint32_t guessra = rand();
//
//    encrip = data->guess ^data ->encryptionKey;
//    //hdata = (guessra >> 8)^(guessra & 0xFF)^(data->encryptionKey >> 8)^(data->encryptionKey & 0xFF);
//    hdata = ((data->encryptionKey & 0x00FF) ^(data->encryptionKey & 0xFF00) ^(data->guess & 0x00FF) ^ (data->guess & 0xFF));
//    if (encrip == data->encryptedGuess && data->guess == hdata) {
//        return TRUE;
//    } else {
//        return FALSE;
//    }
//
//};
{
    //Encrypting is the same as decrypting because of XOR symmetry
    uint16_t checkGuess = (data->guess ^ data->encryptionKey); //Decrypt guess
    uint8_t newHash = (data->guess >> 8) & 0xFF; //first 8 bits
    newHash ^= (data->guess) & 0xFF; //last 8 bits
    newHash ^= (data->encryptionKey >> 8) & 0xFF; //first 8 bits
    newHash ^= (data->encryptionKey) & 0xFF; //last 8 bits

    if (checkGuess != data->encryptedGuess) { //If the guesses do not match based on recorded data
        return FALSE;
    } else if (newHash != data->hash) { //Hashes generated do not mash from rehashing
        return FALSE;
    }

    return TRUE;
}

/**
 * This function returns a TurnOrder enum type representing which agent has won precedence for going
 * first. The value returned relates to the agent whose data is in the 'myData' variable. The turn
 * ordering algorithm relies on the XOR() of the 'encryptionKey' used by both agents. The least-
 * significant bit of XOR(myData.encryptionKey, oppData.encryptionKey) is checked so that if it's a
 * 1 the player with the largest 'guess' goes first otherwise if it's a 0, the agent with the
 * smallest 'guess' goes first. The return value of TURN_ORDER_START indicates that 'myData' won,
 * TURN_ORDER_DEFER indicates that 'oppData' won, otherwise a tie is indicated with TURN_ORDER_TIE.
 * There is no checking for NULL pointers within this function.
 * @param myData The negotiation data representing the current agent.
 * @param oppData The negotiation data representing the opposing agent.
 * @return A value from the TurnOrdering enum representing which agent should go first.
 */
TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData * oppData)
{



    uint8_t turnPlayer = (myData->encryptionKey ^ oppData->encryptionKey) & 0x01;
    if (turnPlayer) {//Choose largest guess 0x01
        if (myData->guess > oppData->guess) { //oppData is smaller
            return TURN_ORDER_START;
        } else { //oppData is greater
            return TURN_ORDER_DEFER;
        }
    } else {//Chooese smallest guess 0x00
        if (myData->guess < oppData->guess) { //oppData is greater
            return TURN_ORDER_START;
        } else {//oppData is smaller
            return TURN_ORDER_DEFER;
        }
    }
    return TURN_ORDER_TIE;















    //    uint8_t whofirst = (myData->encryptionKey^oppData->encryptionKey && 0x01);
    //    if (whofirst) {
    //        if (myData->guess > oppData->guess) {
    //            return TURN_ORDER_START;
    //        }
    //        if (myData->guess < oppData->guess) {
    //            return TURN_ORDER_DEFER;
    //        }
    //    } else {
    //        if (myData->guess < oppData->guess) {
    //            return TURN_ORDER_START;
    //        }
    //        if (myData->guess > oppData->guess) {
    //            return TURN_ORDER_DEFER;
    //        }
    //    }
    //    return TURN_ORDER_TIE;
}

static uint8_t Checksum(char *stcha)
{

    //    uint8_t checksum2 = 0;
    //    //sdata.checksum
    //    int i;
    //    for (i = 0; stcha[i] != NULL; i++) {
    //        checksum2 ^= stcha[i];
    //    }
    //    return checksum2;


    uint8_t checkSumValue = 0; //start of Check Sum
    int loopLength = strlen(stcha); //Iterations is length of string
    while (loopLength) {
        //Every character on the ASCII table is a byte, or 8 bits
        checkSumValue ^= *stcha++; //XOR every byte
        loopLength--;
    }
    return checkSumValue;



}

static uint8_t AsciiHexTo(char c)
{

    if (c == '0') {
        return (uint8_t) c - 48;
    } else if (c == '1') {
        return (uint8_t) c - 48;
    } else if (c == '2') {
        return (uint8_t) c - 48;
    } else if (c == '3') {
        return (uint8_t) c - 48;
    } else if (c == '4') {
        return (uint8_t) c - 48;
    } else if (c == '5') {
        return (uint8_t) c - 48;
    } else if (c == '6') {
        return (uint8_t) c - 48;
    } else if (c == '7') {
        return (uint8_t) c - 48;
    } else if (c == '8') {
        return (uint8_t) c - 48;
    } else if (c == '9') {
        return (uint8_t) c - 48;


    } else if (c == 'A') {
        return (uint8_t) c - 55;
    } else if (c == 'B') {
        return (uint8_t) c - 55;
    } else if (c == 'C') {
        return (uint8_t) c - 55;
    } else if (c == 'D') {
        return (uint8_t) c - 55;
    } else if (c == 'E') {
        return (uint8_t) c - 55;
    } else if (c == 'F') {
        return (uint8_t) c - 55;


    } else if (c == 'a') {
        return (uint8_t) c - 87;
    } else if (c == 'b') {
        return (uint8_t) c - 87;
    } else if (c == 'c') {
        return (uint8_t) c - 87;
    } else if (c == 'd') {
        return (uint8_t) c - 87;
    } else if (c == 'e') {
        return (uint8_t) c - 87;
    } else if (c == 'f') {
        return (uint8_t) c - 87;
    }

    return 128;

}
