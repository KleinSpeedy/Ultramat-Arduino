#include "util/ErrorHandling.h"

#include <avr/io.h>
#include <util/delay.h>

namespace Ultramat::Errors {

// delay times

static constexpr auto LONG_DELAY_MS = 1500;
static constexpr auto SHORT_DELAY_MS = 500;
static constexpr auto BETWEEN_DELAY_MS = 250;

static constexpr auto OFF_DELAY_MS = 3000;

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

static inline void doBlinking(const BlinkCode &code)
{
    for(auto i = 0; i < code.length; ++i)
    {
        LED_PORT |= (1 << LED_PIN);

        // check bit value
        code.value & (LONG_BLINK << i) ?
            _delay_ms(LONG_DELAY_MS) : _delay_ms(SHORT_DELAY_MS);

        LED_PORT &= ~(1 << LED_PIN);
        _delay_ms(BETWEEN_DELAY_MS);
    }
    // turn off between patterns
    LED_PORT &= (1 << LED_PIN);
    _delay_ms(OFF_DELAY_MS);
}

// TODO: make this array with enum type as index
static BlinkCode getBlinkCode(const Type err)
{
    switch (err)
    {
        // long short short
        case Type::StartComms:
        {
            return makeBlinkCode(LONG_BLINK, SHORT_BLINK, SHORT_BLINK);
        }
        // short short long
        case Type::CorruptedMsg:
        {
            return makeBlinkCode(SHORT_BLINK, SHORT_BLINK, LONG_BLINK);
        }
        // short long short
        case Type::UnknownError:
        {
            return makeBlinkCode(SHORT_BLINK, LONG_BLINK, SHORT_BLINK);
        }
    }
    // return unknow error to avoid gcc warning
    return makeBlinkCode(SHORT_BLINK, LONG_BLINK, SHORT_BLINK);
}

void handleNotRecoverable(const Type err)
{
    // TODO: Put crash data into memory section that can be read using debugger
    // if we have crash data

    // set builtin led to output
    LED_DDR |= (1 << LED_PIN);

    // get blink code for error
    const auto code = getBlinkCode(err);

    // infinite blink code
    while(true)
    {
        doBlinking(code);
    }
}

} // namespace Ultramat::Errors
