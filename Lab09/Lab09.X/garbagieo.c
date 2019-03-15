#include<stdlib.h>
#include<stdio.h>
#include<GenericTypeDefs.h>
#include <stdint.h>
#include<string.h>
#include"Protocol.h"

#define ITERRATION 4
#define SEED 91
#define ONEBYTE 8
//#define TRUE 1
//#define FALSE 0

typedef enum {
    WAITING,
    RECORDING,
    FIRSTCHECKSUM,
    SECONDCHECKSUM,
    NEWLINE,
} PossibleStatus;

PossibleStatus myStatus = WAITING;
char myRecordingData[PROTOCOL_MAX_MESSAGE_LEN];
int indexeofData = 0;
char checkSumRecorder[2];

/**
 * Encodes the coordinate data for a guess into the string `message`. This string must be big
 * enough to contain all of the necessary data. The format is specified in PAYLOAD_TEMPLATE_COO,
 * which is then wrapped within the message as defined by MESSAGE_TEMPLATE. The final length of this
 * message is then returned. There is no failure mode for this function as there is no checking
 * for NULL pointers.
 * @param message The character array used for storing the output. Must be long enough to store the
 *                entire string, see PROTOCOL_MAX_MESSAGE_LEN.
 * @param data The data struct that holds the data to be encoded into `message`.
 * @return The length of the string stored into `message`.
 */
int ProtocolEncodeCooMessage(char *message, const GuessData *data) {
    char temp[PROTOCOL_MAX_MESSAGE_LEN];
    int i = 0;
    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
        temp[i] = '\0';
    }
    //clear the array
    sprintf(temp, PAYLOAD_TEMPLATE_COO, data->row, data->col);

    // I log in the protoco template and add the checksum
    uint8_t checkSum = 0x0000;
    //XOR operation
    i = 0;
    for (;;) {
        checkSum ^= temp[i];
        i += 1;
        if (temp[i] == '\0') {
            break;
        }
    }
    //final Output
    sprintf(message, MESSAGE_TEMPLATE, temp, checkSum);

    return i + 5;
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeHitMessage(char *message, const GuessData *data) {
    char temp[PROTOCOL_MAX_MESSAGE_LEN];
    int i = 0;
    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
        temp[i] = '\0';
    }
    //clear the array

    // I log in the protoco template and add the checksum

    sprintf(temp, PAYLOAD_TEMPLATE_HIT, data->row, data->col, data->hit);

    uint8_t checkSum = 0x00;
    i = 0;
    for (;;) {
        checkSum ^= temp[i];
        i += 1;
        if (temp[i] == '\0') {
            break;
        }
    }




    sprintf(message, MESSAGE_TEMPLATE, temp, checkSum);

    //final Output

    return i + 5;
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeChaMessage(char *message, const NegotiationData *data) {
    char temp[PROTOCOL_MAX_MESSAGE_LEN];
    int i;



    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
        temp[i] = '\0';
    }
    uint8_t checkSum = 0x00;
    //clear the array

    // I log in the protoco template and add the checksum

    sprintf(temp, PAYLOAD_TEMPLATE_CHA, data->encryptedGuess, data->hash);

    i = 0;
    for (;;) {
        checkSum ^= temp[i];
        i += 1;
        if (temp[i] == '\0') {
            break;
        }
    }
    sprintf(message, MESSAGE_TEMPLATE, temp, checkSum);
    //Final outPut

    return i + 5;
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeDetMessage(char *message, const NegotiationData *data) {
    char temp[PROTOCOL_MAX_MESSAGE_LEN];
    int i;
    uint8_t checkSum = 0x00;

    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
        temp[i] = '\0';
    }

    //clear the array

    // I log in the protoco template and add the checksum
    sprintf(temp, PAYLOAD_TEMPLATE_DET, data->guess, data->encryptionKey);
    i = 0;
    for (;;) {
        checkSum ^= temp[i];
        i += 1;
        if (temp[i] == '\0') {
            break;
        }
    }
    //Final output
    sprintf(message, MESSAGE_TEMPLATE, temp, checkSum);
    return i + 5;
}

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
 * @param nData A struct used for storing data if a message is decoded that stores NegotiationData.`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````                    
 * @param gData A struct used for storing data if a message is decoded that stores GuessData.
 * @return A value from the UnpackageDataEnum enum.
 */
ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData) {
    switch (myStatus) {
        case(WAITING):
        {
            //Stage1
            //check if it is the dollar sign
            if (in == '$') {
                myStatus = RECORDING;
                indexeofData = 0;
                for (indexeofData = 0; indexeofData < PROTOCOL_MAX_MESSAGE_LEN; indexeofData++) {
                    myRecordingData[indexeofData] = '\0';
                }

                indexeofData = 0;
                return PROTOCOL_PARSING_GOOD;
            } else {
                return PROTOCOL_WAITING;
            }
            break;
        }
        case(RECORDING):
            //stage 2
            //start recording
        {
            switch (in) {
                case('1'): case('2'): case('3'): case('4'): case('5'):
                case('6'): case('7'): case('8'):case('9'): case('0'):
                    //                case('a'): case('b'):case('c'): case('d'):case('e'):
                case('C'): case('O'):case('H'): case('I'): case('T'):
                case('A'):case('D'):case('E'): case(','):
                {
                    //possible cases
                    myRecordingData[indexeofData] = in;
                    indexeofData += 1;
                    return PROTOCOL_PARSING_GOOD;
                    break;
                }
                case('*'):
                {
                    // if it is a first * we go to the first check sum
                    myStatus = FIRSTCHECKSUM;
                    return PROTOCOL_PARSING_GOOD;
                    break;
                }
                default:
                {
                    myStatus = WAITING;
                    return PROTOCOL_PARSING_FAILURE;
                    break;
                }
            }
            break;
        }
        case(FIRSTCHECKSUM):
        {
            switch (in) {
                case('1'): case('2'): case('3'): case('4'): case('5'):
                case('6'): case('7'): case('8'):case('9'): case('0'):
                case('a'): case('b'):case('c'): case('d'):case('e'):
                case('f'):
                {
                    // if on this stage
                    // store the value and goes to next possible stage
                    checkSumRecorder[0] = in;
                    myStatus = SECONDCHECKSUM;
                    return PROTOCOL_PARSING_GOOD;
                    break;
                }
                default:
                {

                    myStatus = WAITING;
                    return PROTOCOL_PARSING_FAILURE;
                    break;
                }
            }
            break;
        }
        case(SECONDCHECKSUM):
        {
            switch (in) {
                case('1'): case('2'): case('3'): case('4'): case('5'):
                case('6'): case('7'): case('8'):case('9'): case('0'):
                case('a'): case('b'):case('c'): case('d'):case('e'):
                case('f'):
                {
                    // if the possible value matches
                    //record the data and check it
                    checkSumRecorder[1] = in;
                    int i = 0;
                    uint8_t hashingResult, originalHashing = 0x00;
                    hashingResult = xtoi(checkSumRecorder);
                    for (;;) {
                        if (myRecordingData[i] != '\0') {
                            originalHashing ^= myRecordingData[i];
                            i += 1;
                        } else {
                            break;
                        }
                    }
                    if (originalHashing == hashingResult) {
                        myStatus = NEWLINE;
                        return PROTOCOL_PARSING_GOOD;
                    } else {
                        myStatus = WAITING;
                        return PROTOCOL_PARSING_FAILURE;
                    }
                    break;
                }
                default:
                {
                    // else return a parsing failure
                    myStatus = WAITING;
                    return PROTOCOL_PARSING_FAILURE;
                    break;
                }
            }
            break;
        }
        case(NEWLINE):
        {
            if (in == '\n') {
                myStatus = WAITING;
                char *strTest, *str1, *str2, *str3;
                strTest = strtok(myRecordingData, ",");

                // make sure we are using the correct data
                // check what kind of data it is
                if (strcmp(strTest, "CHA") == 0) {
                    str1 = strtok(NULL, ",");
                    str2 = strtok(NULL, "*");
                    nData->encryptedGuess = atoi(str1);
                    nData->hash = atoi(str2);
                    // reurn different kinds of returnVal
                    return PROTOCOL_PARSED_CHA_MESSAGE;
                } else if (strcmp(strTest, "COO") == 0) {
                    str1 = strtok(NULL, ",");
                    str2 = strtok(NULL, "*");
                    gData->row = atoi(str1);
                    gData->col = atoi(str2);
                    // reurn different kinds of returnVal
                    return PROTOCOL_PARSED_COO_MESSAGE;
                } else if (strcmp(strTest, "DET") == 0) {
                    str1 = strtok(NULL, ",");
                    str2 = strtok(NULL, "*");
                    nData->guess = atoi(str1);
                    nData->encryptionKey = atoi(str2);
                    // reurn different kinds of returnVal
                    return PROTOCOL_PARSED_DET_MESSAGE;
                } else if (strcmp(strTest, "HIT") == 0) {
                    //Hit is a little bit different special
                    str1 = strtok(NULL, ",");
                    str2 = strtok(NULL, ",");
                    str3 = strtok(NULL, "*");
                    // change it into a real number
                    gData->row = atoi(str1);
                    gData->col = atoi(str2);
                    gData->hit = atoi(str3);
                    // reurn different kinds of returnVal
                    return PROTOCOL_PARSED_HIT_MESSAGE;
                } else {
                    return PROTOCOL_PARSING_FAILURE;
                }
            } else {
                myStatus = WAITING;
                return PROTOCOL_PARSING_FAILURE;
            }
        }
        default:
            return PROTOCOL_PARSING_FAILURE;

    }

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
void ProtocolGenerateNegotiationData(NegotiationData *data) {

    uint8_t tempData;



    data->guess = (rand()&0x0FFFF);
    data->encryptionKey = (rand()&0x0FFFF);
    // encryption key is 16 bit
    data->encryptedGuess = ((data->guess) ^ (data->encryptionKey));


    tempData = ((data->guess) >> ONEBYTE);
    tempData ^= (data->guess);

    tempData ^= ((data->encryptionKey) >> ONEBYTE);
    tempData ^= ((data->encryptionKey));
    // XOR twice
    data->hash = tempData;
    // log in the hashing
}

/**
 * Validates that the negotiation data within 'data' is correct according to the algorithm given in
 * GenerateNegotitateData(). Used for verifying another agent's supplied negotiation data. There is
 * no checking for NULL pointers within this function. Returns TRUE if the NegotiationData struct
 * is valid or FALSE on failure.
 * @param data A filled NegotiationData struct that will be validated.
 * @return TRUE if the NegotiationData struct is consistent and FALSE otherwise.
 */
uint8_t ProtocolValidateNegotiationData(const NegotiationData *data) {
    // use the same algrothm to chaaecck the validation of the data
    uint8_t hashCheck;
    hashCheck = ((data->guess) >> ONEBYTE);
    hashCheck ^= (data->guess);
    hashCheck ^= ((data->encryptionKey) >> ONEBYTE);
    hashCheck ^= (data-> encryptionKey);
    if (hashCheck == (data->hash)) {
        return TRUE;
    } else {
        return FALSE;
    }


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
TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData) {

    uint16_t indicationData;
    // use the algrothm that provided by lab manual to check the who goes first
    indicationData = (oppData->encryptionKey) ^ (myData->encryptionKey);
    indicationData &= 0x01;
    if (indicationData == 0x01) {
        // if the indication data is 0x01, who is bigger who goes first
        if ((myData->guess) > (oppData->guess)) {
            return TURN_ORDER_START;
        } else if ((myData->guess) < (oppData->guess)) {
            return TURN_ORDER_DEFER;
        } else {
            return TURN_ORDER_TIE;
        }
    } else {
        // else, who is smaller who goes first
        if ((myData->guess) < (oppData->guess)) {
            return TURN_ORDER_START;
        } else if ((myData->guess) > (oppData->guess)) {
            return TURN_ORDER_DEFER;
        } else {
            return TURN_ORDER_TIE;
        }
    }
}