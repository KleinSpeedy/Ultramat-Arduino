#pragma once

#include "CommandEnum.h"
#include "MessageBuffer.h"
#include "util/RingBuffer.h"

#include <stdint.h>

namespace Ultramat {
    
class SerialComms
{
public:

    SerialComms()
    {
        inQueue_.clear();
        outQueue_.clear();
    }

    bool start();

    void process();

    bool getNextInMessage(MessageBuffer &msg);

    bool addDoneMessage(const MessageBuffer &msg);

private:
    // TODO: Look at Ultramat Implementation
    static constexpr const auto BUFFER_SIZE = 128;
    // Baudrate for serial communication
    static constexpr const auto BAUDRATE = 115200;
    
    static constexpr const auto QUEUE_SIZE = 20;

    uint8_t rxBuffer_[BUFFER_SIZE] = {0};
    RingBuffer<MessageBuffer, QUEUE_SIZE> inQueue_;
    RingBuffer<MessageBuffer, QUEUE_SIZE> outQueue_;

    uint8_t idCounter_ = 0;

    // index in rxBuffer on active decoding
    uint8_t bufIndex_ = 0;
    // recieved length of payload
    uint8_t dataLength_ = 0;
    // computed checksum
    uint16_t checksum_ = 0;
    // recieved checksum from message
    uint16_t recievedChecksum_ = 0;

    bool secondChecksumByte_ = false;
    bool inMsgComplete_ = false;

    enum class RecieverState : uint8_t
    {
        Start,
        Id,
        Command,
        DataLength,
        Data,
        Checksum
    };
    RecieverState decodingState_ = RecieverState::Start;

    enum class MsgFrame : uint8_t
    {
        Start = 0,
        Id,
        Command,
        DataLength,
        Data
    };
    static inline constexpr auto msgFrameAsByte(const MsgFrame val)
    {
        return static_cast<uint8_t>(val);
    }

    void decodeMessage();
    void sendMessageToBase(const CommandEnum type,
                           const uint8_t payloadId) const;
};

} // namespace Ultramat
