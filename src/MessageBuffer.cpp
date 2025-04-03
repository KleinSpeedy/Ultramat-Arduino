#include "MessageBuffer.h"
#include "CommandEnum.h"

#include <string.h>

namespace Ultramat {

MessageBuffer::MessageBuffer(const CommandEnum type, const uint8_t id, const uint8_t payloadId)
    : id_{id}, cmd_{type}
{
    // 1 byte payload size
    size_ = 1;
    // ack/done ID is payload
    payload_[0] = payloadId;
    computeChecksum();
}

MessageBuffer::MessageBuffer(const uint8_t *input)
{
    if(nullptr == input)
    {
        // TODO: Bad thing happended here
        return;
    }

    // TODO: Magic number
    cmd_ = CommandEnum(input[1]);

    // TODO: Magic number
    size_ = input[2];
    if(size_ == 0)
    {
        // TODO: Bad thing happended here, what in case of empty payload?
        return;
    }

    extractPayload(&input[3]);
    computeChecksum();
}

const char* MessageBuffer::serialize(void)
{
    // TODO: get rid of magic numbers
    buffer_[0] = 0x10;
    buffer_[1] = cmd_.byteValue();
    buffer_[2] = size_;
    // payload_ starts at byte 3, checksum comes after payload
    buffer_[3 + size_] = (checksum_ >> 8) & 0xFF; // most significant
    buffer_[3 + size_ + 1] = checksum_ & 0xFF; // least significant

    return (const char *)buffer_;
}

bool MessageBuffer::extractPayload(const uint8_t * const buffer)
{
    // leave place for message start frame, cmd type and payload size byte
    // TODO: Magic number
    payload_ = &buffer_[3];
    return (memcpy(payload_, buffer, size_) != nullptr);
}

void MessageBuffer::computeChecksum(void)
{
    // TODO: Replace magic number with enum value of message start byte
    checksum_ = frameValAsByte(FrameVal::Start);
    checksum_ += id_;
    checksum_ += cmd_.byteValue();
    checksum_ += size_;
    for(uint16_t i = 0; i < size_; ++i)
    {
        checksum_ += buffer_[i];
    }
}

} // namespace Ultramat
