/*===== INCLUDES =====*/
#include <Arduino.h>
#include "Commands.h"

/*===== DEFINES =====*/

/*-- COMMANDS --*/

static constexpr const uint8_t ERROR = 0xEE;

/*-- DATA FRAME --*/

/**
 * @brief message frame values
*/
enum Frame : uint8_t
{
    startValue      = 0x10, /* start of frame */
    XOR_Value       = 0x11, /* XOR char */
    escape_Value    = 0x20  /* escape char */
};

/*-- RECIEVER STATUS --*/

/**
 * @brief State of communication protocol
*/
enum class RecieverStatus : uint8_t
{
    idle = 1,   /* nothing happens */
    command,    /* decode command value */
    byte_count, /* number of data bytes */
    data,       /* n bytes of data, e.g. position values */
    checksum    /* check if message is valid */
};

/*===== VARIABLES =====*/

static uint8_t checksum;    // Checksum value
static uint8_t xorValue;    // XOR Value for Escape Char
static uint8_t bufferIndex; // Current position in buffer

uint8_t g_recieveBuffer[128];  // Recieve Buffer
uint8_t g_transmitBuffer[128]; // Transmit Buffer

RecieverStatus g_protocolStatus = RecieverStatus::idle; // Current protocol status
static uint8_t byteCount;                               // Number of Bytes (Data Length)

CommandHandler *g_cmdHandle; // Command Handler Instance

AccelStepper g_stepperX(1, 2, 5); // TODO: INIT
AccelStepper g_stepperY(1, 4, 7); // TODO: INIT

/*===== IMPLEMENTATION =====*/

void setup()
{
    Serial.begin(115200);
    delay(50);
}

void loop()
{
    // TODO: use timers and/or interrupts for reading serial data
    if(Serial.available() > 0)
    {
        uint8_t recievedData = (uint8_t)Serial.read();
        /* Handle Escape Frame */
        if(recievedData == Frame::escape_Value)
        {
            xorValue = Frame::XOR_Value;
        }
        else
        {
            recievedData ^= xorValue;
            xorValue = 0x00;

            switch (g_protocolStatus)
            {
                case RecieverStatus::idle:
                {
                    if(recievedData == Frame::startValue)
                    {
                        bufferIndex = 0;
                        g_recieveBuffer[bufferIndex++] = recievedData;
                        checksum = recievedData;
                        g_protocolStatus = RecieverStatus::command;
                    }
                    break;
                }

                case RecieverStatus::command:
                {
                    g_recieveBuffer[bufferIndex++] = recievedData;
                    checksum += recievedData;
                    g_protocolStatus = RecieverStatus::byte_count;
                    break;
                }

                case RecieverStatus::byte_count:
                {
                    byteCount = recievedData;
                    if(byteCount > 0)
                    {
                        g_recieveBuffer[bufferIndex++] = recievedData;
                        checksum += recievedData;
                        g_protocolStatus = RecieverStatus::data;
                    }
                    else
                    {
                        g_protocolStatus = RecieverStatus::idle;
                    }
                    break;
                }

                case RecieverStatus::data:
                {
                    g_recieveBuffer[bufferIndex++] = recievedData;
                    checksum += recievedData;
                    if(--byteCount == 0)
                    {
                        g_protocolStatus = RecieverStatus::checksum;
                    }
                    break;
                }

                case RecieverStatus::checksum:
                {
                    if(recievedData == checksum)
                    {
                        g_protocolStatus = RecieverStatus::idle;
                        g_recieveBuffer[bufferIndex] = recievedData;
                        // TODO: Do this outside if statement
                        g_cmdHandle->execute(g_recieveBuffer);
                    }
                    else
                    {
                        // TODO: Send meaningful codes if possible
                        g_cmdHandle->Cmd_SendError();
                    }
                    break;
                }
            }
        }
    }
}
