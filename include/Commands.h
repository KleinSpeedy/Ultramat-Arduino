/* 
 * Header Commannd Handler
*/
#pragma once

/*===== INCLUDES =====*/
#include <AccelStepper.h>
#include <Arduino.h>

class CommandHandler
{
public:

    CommandHandler(AccelStepper *sX,
                   AccelStepper *sY,
                   uint8_t *rBuffer,
                   uint8_t *tBuffer) :
                   stepperX_{sX},
                   stepperY_{sY},
                   recieveBuffer_{rBuffer},
                   transmitBuffer_{tBuffer} {};

    ~CommandHandler();
    
    /**
     * @brief Set home-position of X-Axis stepper-motor.
     * 
     * Homing is done with a switch. Stepper moves until it reaches Homing-Switch. 
     * After making contact it moves back and sets Homing-Position.
     */
    void Cmd_HomingX();

    void Cmd_HomingY();

    void Cmd_ChangePosition();

    void Cmd_SetParameters();

    void Cmd_FillGlass();

    void Cmd_Stop();

private:

    // Stepper Control for X-Axis
    AccelStepper *stepperX_;
    // Stepper Control for Y-Axis
    AccelStepper *stepperY_;

    const uint8_t *recieveBuffer_;
    uint8_t *transmitBuffer_;

    void setStepperSpeedAndAcceleration(
        AccelStepper *stepper,
        float speed,
        float acceleration);

    uint32_t decodePosition();
};