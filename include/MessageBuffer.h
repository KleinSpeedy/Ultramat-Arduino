#pragma once

#include "CommandEnum.h"

#include <stdint.h>

namespace Ultramat {

enum class FrameVal : uint8_t
{
    Start,
    Xor,
    EscapeChar
};
constexpr inline auto frameValAsByte(const FrameVal val)
{
    return static_cast<uint8_t>(val);
}

class MessageBuffer
{
public:

    // TODO: How to construct ack, done and error msg easily?
    MessageBuffer(const CommandEnum type, const uint8_t id, const uint8_t payloadId);

    MessageBuffer(const uint8_t *input);

    MessageBuffer()
    {
    }

    static constexpr auto maximumBufferSize(void)
    {
        return MAX_BUFFER_SIZE;
    }

    uint8_t getMessageId(void) const
    {
        return id_;
    }

    CommandEnum getCommandType(void) const
    {
        return cmd_;
    }

    const uint8_t * getPayload(void) const
    {
        return payload_;
    }

    uint8_t getPayloadSize(void) const
    {
        return size_;
    }

    const char* serialize(void);

private:

    static constexpr auto MAX_BUFFER_SIZE = 256;
    uint8_t buffer_[MAX_BUFFER_SIZE] = {0};

    uint8_t id_ = 0;
    CommandEnum cmd_;

    uint8_t size_ = 0;
    uint8_t *payload_ = nullptr;

    uint16_t checksum_ = 0;

    bool extractPayload(const uint8_t * const buffer);
    void computeChecksum(void);
};

} // namespace Ultramat
