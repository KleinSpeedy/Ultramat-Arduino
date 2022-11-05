/*
 * Source Command Handler
 * Homing Code taken from Yvan / https://Brainy-Bits.com
*/

//Include Modul
#include "Commands.h"
#include <util/delay.h>

void CommandHandler::setStepperSpeedAndAcceleration(
    AccelStepper *stepper,
    float speed,
    float acceleration)
{
    stepper->setMaxSpeed(speed);
    stepper->setAcceleration(acceleration);
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