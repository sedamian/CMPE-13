/*
 * @File:   Protocol.c
 * @Author: Allen Aboytes (1478650)
 * @Section CMPE13L-01 (62374)
 * @Description: Protocol Message Enc/Dec format
 * 
 * Collaborator: Aviv Elor
 */

#define ENABLE_PROTOCOL //remove this line to disable Protocol.c
#ifdef ENABLE_PROTOCOL

//Standard Libraries
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
// User libraries
#include "Protocol.h"
#include "Agent.h"

// **** Set any macros or preprocessor directives here ****
#define TRUE 1
#define FALSE 0
#define NIBBLE 4
#define BYTE 8
#define HEX_BASE 16
#define BIT_MASK 0x01
#define BYTE_MASK 0xFF
#define HEX_BASE_MASK 0xFFFF 
// **** Declare any data types here ****

//ProtocolDecode FSM State enums

typedef enum {
    WAITING,
    RECORDING,
    FIRST_HALF_CHECKSUM,
    SECOND_HALF_CHECKSUM,
    NEWLINE
} ProtocolParsingState; //Initialized to waiting

typedef struct ProtocolData {
    char message[PROTOCOL_MAX_PAYLOAD_LEN]; //String holds incomming NMEA0183 messages
    uint8_t currentIndex; //Holds current index of string to be stored
    ProtocolParsingState state; //State of ProtocolDecode FSM
    uint8_t checkSum; //Checksum value
} ProtocolData;
// **** Define any module-level, global, or external variables here ****
static uint8_t msgHexVal; //Value of passed Hex char
static uint8_t functionCheckSum; //Holds value to verify recorded Check Sum
static int invalidMID_bool, chaBool, detBool, cooBool, hitBool; //Boolean values for messages
static ProtocolData decodeData = {"\0", 0, WAITING, 0}; //Initialize ProtocolData struct
static char *msgData, *data1, *data2, *data3; //Pointers used to store data in strings
// **** Declare any function prototypes here ****
static uint8_t CheckSum(char *protocolString);
static uint8_t AsciiToHex(char asciiVal);
static void ClearString(char *incomingMessage);

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

int ProtocolEncodeCooMessage(char *message, const GuessData *data)
{
    //PAYLOAD_TEMPLATE_COO "COO,%u,%u"    // Coordinate message: row, col
    //MESSAGE_TEMPLATE "$%s*%02x\n"
    ClearString(message);
    char cooMsg[PROTOCOL_MAX_PAYLOAD_LEN] = {}; //Array to hold payload message
    sprintf(cooMsg, PAYLOAD_TEMPLATE_COO, data->row, data->col); //Store data into payload
    sprintf(message, MESSAGE_TEMPLATE, cooMsg, CheckSum(cooMsg)); //Store payload into message
    return strlen(message);
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeHitMessage(char *message, const GuessData *data)
{
    //PAYLOAD_TEMPLATE_HIT "HIT,%u,%u,%u" // Hit message: row, col, hit (see HitStatus)
    //MESSAGE_TEMPLATE "$%s*%02x\n"
    ClearString(message);
    char hitMsg[PROTOCOL_MAX_PAYLOAD_LEN] = {}; //Array to hold payload message
    sprintf(hitMsg, PAYLOAD_TEMPLATE_HIT, data->row, data->col, data->hit); //Store data
    sprintf(message, MESSAGE_TEMPLATE, hitMsg, CheckSum(hitMsg)); //Store payload into message
    return strlen(message);
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeChaMessage(char *message, const NegotiationData *data)
{
    //PAYLOAD_TEMPLATE_CHA "CHA,%u,%u"    // Challenge message: encryptedGuess, hash
    //MESSAGE_TEMPLATE "$%s*%02x\n"
    ClearString(message);
    char chaMsg[PROTOCOL_MAX_PAYLOAD_LEN] = {}; //Array to hold payload message
    sprintf(chaMsg, PAYLOAD_TEMPLATE_CHA, data->encryptedGuess, data->hash); //Store data
    sprintf(message, MESSAGE_TEMPLATE, chaMsg, CheckSum(chaMsg)); //Store payload into message
    return strlen(message);
}

/**
 * Follows from ProtocolEncodeCooMessage above.
 */
int ProtocolEncodeDetMessage(char *message, const NegotiationData *data)
{
    //PAYLOAD_TEMPLATE_DET "DET,%u,%u"    // Determine message: guess, encryptionKey
    //MESSAGE_TEMPLATE "$%s*%02x\n"
    ClearString(message);
    char detMsg[PROTOCOL_MAX_PAYLOAD_LEN] = {}; //Array to hold payload message
    sprintf(detMsg, PAYLOAD_TEMPLATE_DET, data->guess, data->encryptionKey); //Store data
    sprintf(message, MESSAGE_TEMPLATE, detMsg, CheckSum(detMsg)); //Store payload into message
    return strlen(message);
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
 * @param nData A struct used for storing data if a message is decoded that stores NegotiationData.
 * @param gData A struct used for storing data if a message is decoded that stores GuessData.
 * @return A value from the UnpackageDataEnum enum.
 */
ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData)
{
    switch (decodeData.state) {
    case WAITING:
        if (in == '$') { //Indicates start of string
            ClearString(decodeData.message);
            decodeData.currentIndex = 0; //Reset index
            decodeData.state = RECORDING; //Transition states
            return PROTOCOL_PARSING_GOOD;
        } else if (in != '$') { //Any other value is wrong, waiting
            return PROTOCOL_WAITING;
        }
        break;
    case RECORDING:
        if (in == '*') { //If '*' is reached Check Sum is next "in" value
            decodeData.state = FIRST_HALF_CHECKSUM; //Transition states
            return PROTOCOL_PARSING_GOOD;
        } else { //Else keep recording characters in decoded message struct
            decodeData.message[decodeData.currentIndex] = in; //Record character passed
            decodeData.currentIndex++; //Increment index
            return PROTOCOL_PARSING_GOOD;
        }
        break;
    case FIRST_HALF_CHECKSUM:
        msgHexVal = AsciiToHex(in); //Convert passed character to Hex
        if (msgHexVal < HEX_BASE) {//Max single Hex value 15 ('f' and 'F')
            decodeData.checkSum = msgHexVal << NIBBLE; //Store in top 4 bits into
            decodeData.state = SECOND_HALF_CHECKSUM; //Transition states
            return PROTOCOL_PARSING_GOOD;
        } else {//Greater than 15 (f)
            decodeData.state = WAITING; //Return to waiting
            return PROTOCOL_PARSING_FAILURE;
        }
        break;
    case SECOND_HALF_CHECKSUM:
        msgHexVal = AsciiToHex(in); //Convert passed character to Hex
        decodeData.checkSum |= msgHexVal; //Store last remaining Nibble
        functionCheckSum = CheckSum(decodeData.message); //Calculate Check Sum
        if (msgHexVal > HEX_BASE - 1 || decodeData.checkSum != functionCheckSum) {
            decodeData.state = WAITING; //Transition states
            return PROTOCOL_PARSING_FAILURE;
        } else if (msgHexVal < HEX_BASE && decodeData.checkSum == functionCheckSum) {
            decodeData.message[decodeData.currentIndex] = '\0'; //Store NULL to terminate string
            decodeData.state = NEWLINE; //Transition states
            return PROTOCOL_PARSING_GOOD;
        }
        break;
    case NEWLINE:
        //Generate boolean values, values are zero if equal
        //strncmp compares the first n bytes of arg1 to agr2
        detBool = abs(strncmp(decodeData.message, "DET", 3));
        chaBool = abs(strncmp(decodeData.message, "CHA", 3));
        cooBool = abs(strncmp(decodeData.message, "COO", 3));
        hitBool = abs(strncmp(decodeData.message, "HIT", 3));
        invalidMID_bool = detBool + chaBool + cooBool + hitBool; //Invalid flag sum of other bools
        decodeData.state = WAITING; //Go to waiting for new incoming string
        if (in == '\n') { //Signifies end of string
            msgData = strtok(decodeData.message, ","); // ',' seperates data in Protocol Strings
            if (hitBool == 0) { //If "HIT" Message ID
                // Hit message: row, col, hit (see HitStatus)
                data1 = strtok(NULL, ","); //Tokenize data
                data2 = strtok(NULL, ","); //Tokenize data
                data3 = strtok(NULL, "*"); //Tokenize data
                gData->row = AsciiToHex(*data1); //Store converted string
                gData->col = AsciiToHex(*data2); //Store converted string
                gData->hit = AsciiToHex(*data3); //Store converted string
                return PROTOCOL_PARSED_HIT_MESSAGE;
            } else if (cooBool == 0) { //If "COO" Message ID
                // Coordinate message: row, col
                data1 = strtok(NULL, ","); //Tokenize data
                data2 = strtok(NULL, "*"); //Tokenize data
                gData->row = AsciiToHex(*data1); //Store converted string
                gData->col = AsciiToHex(*data2); //Store converted string
                return PROTOCOL_PARSED_COO_MESSAGE;
            } else if (chaBool == 0) { //If "CHA" Message ID
                // Challenge message: encryptedGuess, hash
                data1 = strtok(NULL, ","); //Tokenize data
                data2 = strtok(NULL, "*"); //Tokenize data
                nData->encryptedGuess = atoi(data1); //Store converted string
                nData->hash = atoi(data2); //Store converted string
                return PROTOCOL_PARSED_CHA_MESSAGE;
            } else if (detBool == 0) { //If "DET" Message ID
                // Determine message: guess, encryptionKey
                data1 = strtok(NULL, ","); //Tokenize data
                data2 = strtok(NULL, "*"); //Tokenize data
                nData->guess = atoi(data1); //Store converted string
                nData->encryptionKey = atoi(data2); //Store converted string
                return PROTOCOL_PARSED_DET_MESSAGE;
            }
        } else if (in != '\n' || invalidMID_bool) { //If passed char is not '\n' or booleans passed
            return PROTOCOL_PARSING_FAILURE;
        }
        break;
    default:
        //Default is a failure error
        return PROTOCOL_PARSING_FAILURE;
        break;
    };
    //Never returns here
    return PROTOCOL_PARSING_FAILURE;
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
void ProtocolGenerateNegotiationData(NegotiationData *data)
{
    //rand() used for random numbers
    data->guess = (rand() & HEX_BASE_MASK); // & 0xFFFF same as mod 2^16 bit range
    data->encryptionKey = (rand() & HEX_BASE_MASK); // & 0xFFFF same as mod 2^16 bit range
    data->encryptedGuess = (data->guess ^ data->encryptionKey); //Encrypt with XOR

    //1 byte hash which XORs all bits from Guess and Encryption key
    uint8_t hash = (data->guess >> BYTE) & BYTE_MASK; //first 8 bits
    hash ^= (data->guess) & BYTE_MASK; //last 8 bits
    hash ^= (data->encryptionKey >> BYTE) & BYTE_MASK; //first 8 bits
    hash ^= (data->encryptionKey) & BYTE_MASK; //last 8 bits
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
uint8_t ProtocolValidateNegotiationData(const NegotiationData *data)
{
    //Encrypting is the same as decrypting because of XOR symmetry
    uint16_t checkGuess = (data->guess ^ data->encryptionKey); //Decrypt guess
    uint8_t newHash = (data->guess >> BYTE) & BYTE_MASK; //first 8 bits
    newHash ^= (data->guess) & BYTE_MASK; //last 8 bits
    newHash ^= (data->encryptionKey >> BYTE) & BYTE_MASK; //first 8 bits
    newHash ^= (data->encryptionKey) & BYTE_MASK; //last 8 bits

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
TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData)
{
    //TURN_ORDER_START myData player
    //TURN_ORDER_DEFER oppData player

    //XOR of randomly generated keys and least bit is masked (0 or 1)
    uint8_t turnPlayer = (myData->encryptionKey ^ oppData->encryptionKey) & BIT_MASK;
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
}

static uint8_t CheckSum(char* protocolString)
{
    uint8_t checkSumValue = 0; //start of Check Sum
    int loopLength = strlen(protocolString); //Iterations is length of string
    while (loopLength) {
        //Every character on the ASCII table is a byte, or 8 bits
        checkSumValue ^= *protocolString++; //XOR every byte
        loopLength--;
    }
    return checkSumValue;
}

static uint8_t AsciiToHex(char asciiVal)
{
    uint8_t hexVal;
    //Assuming protocol message, case insensitive
    if (asciiVal >= 'a' && asciiVal <= 'f') {//acceptable hex a-f
        //ASCII Hex lowercase letters offset by 87 
        hexVal = (uint8_t) asciiVal - 87;
    } else if (asciiVal >= 'A' && asciiVal <= 'F') {//acceptable hex A-F
        //ASCII Hex uppercase letters offset by 55 
        hexVal = (uint8_t) asciiVal - 55;
    } else if (asciiVal >= '0' && asciiVal <= '9') {//acceptable digits 0-9
        //ASCII digits offset by 48
        hexVal = (uint8_t) asciiVal - 48;
    }

    return hexVal;
}

static void ClearString(char *incomingMessage)
{
    //Clears a passed string
    //Assumed start of array passed
    int i;
    for (i = 0; i < PROTOCOL_MAX_MESSAGE_LEN; i++) {
        *incomingMessage = '\0'; //Reset the incoming message pointer to NULL
        incomingMessage++;
    }
}
#endif //PROTOCOL_ENABLE