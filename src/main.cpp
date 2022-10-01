/*===== INCLUDES =====*/

#include <Arduino.h>

/*===== DEFINES =====*/

/*-- DATA FRAME --*/

#define FRAME_START         0x10
#define FRAME_XOR_CHAR      0x11
#define FRAME_ESCAPE_CHAR   0x20

/*-- RECIEVER STATUS --*/

enum class RecieverStatus : uint8_t
{
  RCV_ST_IDLE = 1,
  RCV_ST_CMD,
  RCV_ST_BYTE_COUNT,
  RCV_ST_DATA,
  RCV_ST_CHECKSUM
};

/*===== VARIABLES =====*/

uint8_t g_checksum;         // Checksum
uint8_t g_xorValue;         // XOR Value for Escape Char
uint8_t g_bufferIndex;      // Current byte in buffer

uint8_t *g_rxBuffer;        // Recieve Buffer
uint8_t *g_txBuffer;        // Transmit Buffer

RecieverStatus g_status;    // Current protocol status
uint8_t g_byteCount;        // Number of Bytes (Data Length)

/*===== METHODS =====*/


/*===== IMPLEMENTATION =====*/

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available() > 0)
    {
        uint8_t recievedData = (uint8_t)Serial.read();
        
        /* Handle Escape Frame*/
        if (recievedData == FRAME_ESCAPE_CHAR)
            g_xorValue = FRAME_XOR_CHAR;
        else
        {
            recievedData ^= g_xorValue;
            g_xorValue = 0x00;

            switch (g_status)
            {
            case RecieverStatus::RCV_ST_IDLE:
            {

            }
            case RecieverStatus::RCV_ST_CMD:
            {

            }
            case RecieverStatus::RCV_ST_BYTE_COUNT:
            {

            }
            case RecieverStatus::RCV_ST_DATA:
            {
            
            }
            case RecieverStatus::RCV_ST_CHECKSUM:
            {
                
            }
            default:
                break;
            }
        }
    }
}
