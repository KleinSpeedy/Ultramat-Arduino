#pragma once

// project includes

// system includes
#include <stdint.h>

/**
 * @brief Exception / Error handling
 */

namespace Errors {

    enum class Type : uint8_t
    {
        Communication = 0,
    };

    /**
     * @brief infinite while loop, blinking error information
     */
    void handleNotRecoverable(Type err);

};
