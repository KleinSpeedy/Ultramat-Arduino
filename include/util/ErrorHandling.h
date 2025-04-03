#pragma once

#include <stdint.h>

// Exception / Error handling
namespace Ultramat::Errors {

    // what type of error were handling
    enum class Type : uint8_t
    {
        UnknownError,   // unknown error occured
        StartComms,     // error starting serial communication
        CorruptedMsg    // recieved corrupted Message
    };

    /**
     * @brief infinite while loop, blinks builtin LED with specific pattern
     */
    void handleNotRecoverable(const Type err);

}; // namespace Ultramat::Errors
