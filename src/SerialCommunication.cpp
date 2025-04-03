#include "SerialCommunication.h"
#include "CommandEnum.h"
#include "MessageBuffer.h"
#include "util/ErrorHandling.h"

#include <Arduino.h>
#include <HardwareSerial.h>

namespace Ultramat {

bool SerialComms::start()
{
    Serial.begin(BAUDRATE);
    if(!Serial)
    {
        // try again after 100 ms, dont use while(!Serial)
        delay(100);
        return Serial;
    }

    return true;
}

void SerialComms::process()
{
    if(!inMsgComplete_)
    {
        decodeMessage();
    }
    else
    {
        MessageBuffer msg = MessageBuffer{rxBuffer_};
        const bool success = inQueue_.append(msg);
        if(!success)
        {
            // TODO: Queue is full, wait till we can append
            return;
        }
        inMsgComplete_ = true;
        sendMessageToBase(CommandEnum::Value::Acknowledge, msg.getMessageId());

        // TODO: Reset all state and message values
    }
    MessageBuffer doneMsg;
    if(outQueue_.get(doneMsg))
    {
        sendMessageToBase(CommandEnum::Value::Done, doneMsg.getMessageId());
    }
}

void SerialComms::decodeMessage()
{
    if(!(Serial.available() > 0))
    {
        return;
    }

    uint8_t recievedData = (uint8_t)Serial.read();
    uint8_t xorValue = 0x00;

    if(recievedData == frameValAsByte(FrameVal::Xor))
    {
        xorValue = frameValAsByte(FrameVal::Xor);
        // TODO: Correct?
        recievedData ^= xorValue;
        xorValue = 0x00;
    }

    switch(decodingState_)
    {
        case RecieverState::Start:
        {
            if(recievedData == frameValAsByte(FrameVal::Start))
            {
                bufIndex_ = 0;
                rxBuffer_[bufIndex_++] = recievedData;
                checksum_ = recievedData;
                decodingState_  = RecieverState::Id;
            }
            break;
        }
        case RecieverState::Id:
        {
            rxBuffer_[bufIndex_++] = recievedData;
            checksum_ += recievedData;
            decodingState_ = RecieverState::Command;
            break;
        }
        case RecieverState::Command:
        {
            rxBuffer_[bufIndex_++] = recievedData;
            checksum_ += recievedData;
            decodingState_ = RecieverState::DataLength;
            break;
        }
        case RecieverState::DataLength:
        {
            dataLength_ = recievedData;
            if(dataLength_ > 0)
            {
                rxBuffer_[bufIndex_++] = recievedData;
                checksum_ += recievedData;
                decodingState_ = RecieverState::Data;
            }
            else
            {
                decodingState_ = RecieverState::Start;
            }
            break;
        }
        case RecieverState::Data:
        {
            rxBuffer_[bufIndex_] = recievedData;
            checksum_ += recievedData;
            if(--dataLength_ == 0)
            {
                decodingState_ = RecieverState::Checksum;
            }
            break;
        }
        case RecieverState::Checksum:
        {
            // first byte
            if(!secondChecksumByte_)
            {
                checksum_ += recievedData;
                recievedChecksum_ = (recievedData >> 8) & 0xFF;
                break;
            }
            // second byte
            checksum_ += recievedData;
            recievedChecksum_ += recievedData;

            if(recievedChecksum_ != checksum_)
            {
                // Always fail on malformed package
                // TODO: Find better solution
                Errors::handleNotRecoverable(Errors::Type::CorruptedMsg);
            }
            decodingState_ = RecieverState::Start;
            break;
        }
    }
}

void SerialComms::sendMessageToBase(const CommandEnum type,
                                    const uint8_t payloadId) const
{
    auto ackMsg = MessageBuffer{type, idCounter_, payloadId};
    Serial.write(ackMsg.serialize());
}

bool SerialComms::getNextInMessage(MessageBuffer &msg)
{
    return inQueue_.get(msg);
}

bool SerialComms::addDoneMessage(const MessageBuffer &msg)
{
    return outQueue_.append(msg);
}

} // namespace Ultramat
