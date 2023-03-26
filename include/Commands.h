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
                   stepperY_{sY} {};

    ~CommandHandler();
    
    /**
     * @brief Execute the specified command
     * @param rxBuffer recieve buffer
    */
    void execute(uint8_t *rxBuffer);

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

    void Cmd_StopMove();

    /**
     * @brief Establish Connection with base-station and send ok
    */
    void Cmd_HelloThere();

    /**
     * @brief Disable motors and send ok
    */
    void Cmd_HappyLanding();

private:

    // Stepper Control for X-Axis
    AccelStepper *stepperX_;
    // Stepper Control for Y-Axis
    AccelStepper *stepperY_;
    // recieve buffer
    const uint8_t *rxBuffer_;
    // transmit buffer
    uint8_t *txBuffer_;

    void setStepperSpeedAndAcceleration(
        AccelStepper *stepper,
        float speed,
        float acceleration);

    uint32_t decodePosition();

    bool writeSerialData(uint8_t *data);

    /**
     * @brief byte values representing each command
    */
    enum CMD : uint8_t
    {
        /* "setup" commands */
        helloThere      = 0xE0, /* initial hello */
        happyLanding    = 0xE1, /* end connection "nicely" */

        /* "normal" commands */
        homing_X_Axis   = 0xC0, /* homing X-axis */
        homing_Y_Axis   = 0xC1, /* homing Y-axis */
        changePosition  = 0xC2, /* move to specific position */
        fillGlass       = 0xC3, /* up and down motion */
        stop            = 0xC4  /* "emergency" stop */
    };
};