// module include
#include "ErrorHandling.h"

// project includes
// system includes
#include <avr/io.h>
#include <util/delay.h>

namespace Errors {

// delay times

static constexpr auto LONG_DELAY_MS = 1000;
static constexpr auto SHORT_DELAY_MS = 200;
static constexpr auto BETWEEN_DELAY_MS = 250;

static constexpr auto OFF_DELAY_MS = 2000;

// builtin led configuration

#define LED_DDR                 DDRB
#define LED_PORT                PORTB
#define LED_PIN                 PB5

#define LONG_BLINK              1
#define SHORT_BLINK             0

struct BlinkCode
{
    // how many blinks represent the blink code
    static constexpr auto length = 3;
    // bit representation of blink code
    int value : length;
};

static inline constexpr BlinkCode makeBlinkCode(uint8_t a, uint8_t b, uint8_t c)
{
    return BlinkCode{(a << 0 | b << 1 | c << 2)};
}

static void doBlinking(BlinkCode &code)
{
    for(int i = 0; i < code.length; ++i)
    {
        // LED on
        LED_PORT |= (1 << LED_PIN);

        // check bit value
        if(code.value & (1 << i))
        {
            _delay_ms(LONG_DELAY_MS);
        }
        else
        {
            _delay_ms(SHORT_DELAY_MS);
        }

        // LED off
        LED_PORT &= ~(1 << LED_PIN);
        _delay_ms(BETWEEN_DELAY_MS);
    }
    // turn off between patterns
    LED_PORT &= (1 << LED_PIN);
    _delay_ms(OFF_DELAY_MS);
}

static BlinkCode getBlinkCode(Type err)
{
    switch (err)
    {
        // short short long
        case Type::Communication:
        {
            return makeBlinkCode(SHORT_BLINK, SHORT_BLINK, LONG_BLINK);
        }
    }
}

void handleNotRecoverable(Type err)
{
    // TODO: Put crash data into memory section that can be read using debugger

    // set builtin led to output
    LED_DDR |= (1 << LED_PIN);

    // get blink code for error
    auto code = getBlinkCode(err);

    // infinite blink code
    while(true)
    {
        doBlinking(code);
    }
}

} // namespace Errors
