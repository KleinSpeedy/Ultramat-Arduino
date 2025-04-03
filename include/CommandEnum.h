#pragma once

#include <stdint.h>

namespace Ultramat {

class CommandEnum
{
public:
    enum class Value : uint8_t
    {
        Invalid = 0,

        Start = 0xA0,
        Stop = 0xA1,
        HomingX = 0xA2,
        HomingY = 0xA3,

        Acknowledge,
        Done
    };

    CommandEnum() = default;
    constexpr CommandEnum(const uint8_t val)
        : value_{eValue(val)}
    {}

    constexpr CommandEnum(const CommandEnum::Value val)
        : value_{val}
    {}

    // byte value of enum class
    constexpr uint8_t byteValue() const { return static_cast<uint8_t>(value_); }

    constexpr operator Value() const { return value_; }
    explicit operator bool() const = delete;

private:
    enum Value value_;

    struct KeyVal
    {
        const enum Value eVal;  // enum value
        const uint8_t iVal;     // integer value
    };

    // TODO: Put this in PROGMEM
    #define BYTE(x) static_cast<uint8_t>(x)
    static constexpr struct KeyVal map_[] = {
        {Value::Start, BYTE(Value::Start)},
        {Value::Stop, BYTE(Value::Stop)},
        {Value::HomingX, BYTE(Value::HomingX)},
        {Value::HomingY, BYTE(Value::HomingY)},
    };
    #undef BYTE

    // check for valid enum value using key value map
    static constexpr enum Value eValue(const uint8_t val)
    {
        for(const auto &pair : map_)
        {
            if(pair.iVal == val)
                return pair.eVal;
        }
        return Value::Invalid;
    }
};

} // namespace Ultramat
