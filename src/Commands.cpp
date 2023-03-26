/*
 * Source Command Handler
 * Homing Code taken from Yvan / https://Brainy-Bits.com
*/

//Include Modul
#include "Commands.h"
#include <util/delay.h>
#include <Arduino.h>

// command byte position in frame
static constexpr const uint8_t CMD_POS = 0x01;

void CommandHandler::setStepperSpeedAndAcceleration(
    AccelStepper *stepper,
    float speed,
    float acceleration)
{
    stepper->setMaxSpeed(speed);
    stepper->setAcceleration(acceleration);
}

bool CommandHandler::writeSerialData(uint8_t *data)
{
    Serial.write(*data);

    return true;
}

void CommandHandler::Cmd_HelloThere()
{
    uint8_t tx = 0xFF;
    bool ret = writeSerialData(&tx);
    (void *) ret;
}

void CommandHandler::Cmd_HappyLanding()
{
    uint8_t tx = 0xEE;
    bool ret = writeSerialData(&tx);
}

void CommandHandler::Cmd_HomingX()
{
    // Pin for X-Axis switch
    static constexpr uint8_t switchX = 9;

    pinMode(switchX, INPUT_PULLUP);
    float homingSpeed = 1000.0;
    float homingAccel = 100.0;

    //setup Stepper Motor
    stepperX_->enableOutputs();
    setStepperSpeedAndAcceleration(stepperX_, homingSpeed, homingAccel);

    int32_t initialHomingPoint = -1;

    while(digitalRead(switchX))
    {
        stepperX_->moveTo(initialHomingPoint);
        initialHomingPoint--;
        stepperX_->run();
        //TODO: replace delay with non-blocking function! delay vs _delay_ms
        _delay_ms(1);
    }

    stepperX_->setCurrentPosition(0);
    setStepperSpeedAndAcceleration(stepperX_, homingSpeed, homingAccel);

    initialHomingPoint = 1;

    while(!(digitalRead(switchX)))
    {
        stepperX_->moveTo(initialHomingPoint);
        initialHomingPoint++;
        stepperX_->run();
        //TODO: replace delay with non-blocking function!
        delay(1);
    }

    stepperX_->setCurrentPosition(0);
    stepperX_->disableOutputs();
}

void CommandHandler::Cmd_HomingY()
{
    //Slightly differnet to X ?
}

void CommandHandler::Cmd_ChangePosition()
{

}

void CommandHandler::Cmd_FillGlass()
{

}

void CommandHandler::Cmd_StopMove()
{

}

void CommandHandler::execute(uint8_t *rxBuffer)
{
    switch(rxBuffer[CMD_POS])
    {
        case CMD::helloThere:
        {
            Cmd_HelloThere();
            break;
        }
        case CMD::happyLanding:
        {
            Cmd_HelloThere();
            break;
        }
        case CMD::homing_X_Axis:
        {
            Cmd_HomingX();
            break;
        }
        case CMD::homing_Y_Axis:
        {
            Cmd_HomingY();
            break;
        }
        case CMD::changePosition:
        {
            Cmd_ChangePosition();
            break;
        }
        case CMD::fillGlass:
        {
            Cmd_FillGlass();
            break;
        }
        case CMD::stop:
        {
            Cmd_StopMove();
            break;
        }
    }
}
