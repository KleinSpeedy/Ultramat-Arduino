/*===== INCLUDES =====*/
#include <Arduino.h>
#include "Commands.h"

/*===== DEFINES =====*/

/*-- BUFFER INDEX --*/

enum BufferIndex : uint8_t
{
    start = 0,
    cmd = 1
};

/*-- COMMANDS --*/

enum CMD : uint8_t
{
    homing_X_Axis   = 0x15,
    homing_Y_Axis   = 0x16,
    changePosition  = 0x17,
    fillGlass       = 0x18,
    stop            = 0x19
};

/*-- DATA FRAME --*/

enum Frame : uint8_t
{
    start       = 0x10,
    XOR_char    = 0x11,
    escapeChar  = 0x20
};

/*-- RECIEVER STATUS --*/

enum class RecieverStatus : uint8_t
{
    idle = 1,
    command,
    byte_count,
    data,
    checksum
};

/*===== VARIABLES =====*/

uint8_t g_checksum;    // Checksum
uint8_t g_xorValue;    // XOR Value for Escape Char
uint8_t g_bufferIndex; // Current byte in buffer

uint8_t g_recieveBuffer[32];  // Recieve Buffer
uint8_t g_transmitBuffer[32]; // Transmit Buffer

RecieverStatus g_protocolStatus; // Current protocol status
uint8_t g_byteCount;             // Number of Bytes (Data Length)

CommandHandler *g_cmdHandle; // Command Handler Instance

AccelStepper g_stepperX(1, 2, 5); // TODO: INIT
AccelStepper g_stepperY(1, 4, 7); // TODO: INIT

/*===== METHODS =====*/

void executeCommand(); // execute recieved Command

/*===== IMPLEMENTATION =====*/

void setup()
{
    Serial.begin(115200);
    delay(50);
}

void loop()
{
    if (Serial.available() > 0)
    {
        uint8_t recievedData = (uint8_t)Serial.read();

        /* Handle Escape Frame*/
        if (recievedData == Frame::escapeChar)
            g_xorValue = Frame::XOR_char;
        else
        {
            recievedData ^= g_xorValue;
            g_xorValue = 0x00;

            switch (g_protocolStatus)
            {

                case RecieverStatus::idle:
                {
                    if (recievedData == Frame::start)
                    {
                        g_bufferIndex = 0;
                        g_recieveBuffer[g_bufferIndex++] = recievedData;
                        g_checksum = recievedData;
                        g_protocolStatus = RecieverStatus::command;
                    }
                }

                case RecieverStatus::command:
                {
                    g_recieveBuffer[g_bufferIndex++] = recievedData;
                    g_checksum += recievedData;
                    g_protocolStatus = RecieverStatus::byte_count;
                }

                case RecieverStatus::byte_count:
                {
                    g_byteCount = recievedData;
                    if (g_byteCount > 0)
                    {
                        g_recieveBuffer[g_bufferIndex++] = recievedData;
                        g_checksum += recievedData;
                        g_protocolStatus = RecieverStatus::data;
                    }
                    else
                    {
                        g_protocolStatus = RecieverStatus::idle;
                    }
                }

                case RecieverStatus::data:
                {
                    g_recieveBuffer[g_bufferIndex++] = recievedData;
                    g_checksum += recievedData;
                    if (--g_byteCount == 0)
                    {
                        g_protocolStatus = RecieverStatus::checksum;
                    }
                }

                case RecieverStatus::checksum:
                {
                    if (recievedData == g_checksum)
                    {
                        g_protocolStatus = RecieverStatus::idle;
                        g_recieveBuffer[g_bufferIndex] = recievedData;

                        executeCommand();
                    }
                    else
                    {
                        // Error handling
                        break;
                    }
                }
            }
        }
    }
}

void executeCommand()
{
    switch (g_recieveBuffer[BufferIndex::cmd])
    {
        case CMD::homing_X_Axis:
        {
            g_cmdHandle->Cmd_HomingX();
        }
        case CMD::homing_Y_Axis:
        {
            g_cmdHandle->Cmd_HomingY();
        }
        case CMD::changePosition:
        {
            g_cmdHandle->Cmd_ChangePosition();
        }
        case CMD::fillGlass:
        {
            g_cmdHandle->Cmd_FillGlass();
        }
        case CMD::stop:
        {
            g_cmdHandle->Cmd_Stop();
        }
    }
}