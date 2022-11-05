/*  
Motor Homing code using AccelStepper and the Serial Monitor
Created by Yvan / https://Brainy-Bits.com
*/

#include <Arduino.h>
#include <AccelStepper.h>

// AccelStepper Setup
AccelStepper stepperX(1, 2, 5); // 1 = Easy Driver interface
                                // Pin 2 connected to STEP pin of Easy Driver X (Y)
                                // Pin 5 connected to DIR pin of Easy Driver X (Y)
                                // Pin 4 connected to STEP pin of Easy Driver Z
                                // Pin 7 connected to DIR pin of Easy Driver Z
AccelStepper stepperZ(1, 4, 7);

#define HOME_SWITCH 9 // Pin 9 connected to Home Switch (MicroSwitch)

// Stepper Travel Variables
long TravelX;             // Used to store the X value entered in the Serial Monitor
int x_move_finished = 1;  // Used to check if move is completed
long initial_homing = -1; // Used to Home Stepper at startup

void setupHoming()
{
    pinMode(HOME_SWITCH, INPUT_PULLUP);

    delay(5); // Wait for EasyDriver wake up

    // Homing speed and acceleration
    stepperX.setMaxSpeed(1000.0);
    stepperX.setAcceleration(100.0);

    Serial.print("Stepper is Homing . . . . . . . . . . . ");

    while (digitalRead(HOME_SWITCH))
    {                                    // Make the Stepper move CCW until the switch is activated
        stepperX.moveTo(initial_homing); // Set the position to move to
        initial_homing--;                // Decrease by 1 for next move if needed
        stepperX.run();                  // Start moving the stepper
        delay(1);
    }

    stepperX.setCurrentPosition(0);  // Set the current position as zero for now
    stepperX.setMaxSpeed(100.0);     // Set Max Speed of Stepper (Slower to get better accuracy)
    stepperX.setAcceleration(100.0); // Set Acceleration of Stepper

    initial_homing = 1;

    while (!digitalRead(HOME_SWITCH))
    { // Make the Stepper move CW until the switch is deactivated
        stepperX.moveTo(initial_homing);
        stepperX.run();
        initial_homing++;
        delay(3);
    }

    stepperX.setCurrentPosition(0);
    Serial.println("Homing Completed");
    Serial.println("");
    stepperX.setMaxSpeed(100000.0);    // Set Max Speed of Stepper (Faster for regular movements)
    stepperX.setAcceleration(10000.0); // Set Acceleration of Stepper
                                       /*
                                           stepperZ.setMaxSpeed(10000.0);
                                           stepperZ.setAcceleration(1000.0);
                                           stepperZ.setCurrentPosition(0);
                                       */
    Serial.println("Enter Position 1 to 8 to travel ");
}

/* takes Serial Input to move on X(Y) - Axis -> stepperX */
void loopHoming()
{

    while (Serial.available() > 0)
    { // Check if values are available in the Serial Buffer

        x_move_finished = 0; // Set variable for checking move of the Stepper

        TravelX = Serial.parseInt(); // Put numeric value from buffer in TravelX variable
        if (TravelX < 0 || TravelX > 32000)
        { // Make sure the position entered is not beyond the HOME or MAX position
            Serial.println("");
            Serial.println("Please enter a value greater than zero and smaller or equal to 1350.....");
            Serial.println("");
        }
        else
        {
            Serial.print("Moving stepper into position: ");
            Serial.println(TravelX);
            stepperX.enableOutputs();
            delay(5);
            stepperX.moveTo(TravelX); // Set new moveto position of Stepper

            delay(500); // Wait 1 seconds before moving the Stepper
        }
    }

    if (TravelX >= 0 && TravelX <= 32000)
    {

        // Check if the Stepper has reached desired position
        if ((stepperX.distanceToGo() != 0))
        {
            stepperX.run(); // Move Stepper into position
        }

        // If move is completed display message on Serial Monitor
        if ((x_move_finished == 0) && (stepperX.distanceToGo() == 0))
        {
            Serial.println("COMPLETED!");
            Serial.println("");
            Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
            x_move_finished = 1; // Reset move variable
            stepperX.disableOutputs();
        }
    }
}

void changePosition(int posNumber)
{

    // change position data to actual travel data
    long travelPosition = 1600 + ((posNumber - 1) * 4000);
    x_move_finished = 0;

    stepperX.enableOutputs();
    stepperX.moveTo(travelPosition);

    while (stepperX.distanceToGo() != 0)
    {
        stepperX.run();
    }

    x_move_finished = 1;
    stepperX.disableOutputs();

    if (x_move_finished == 1 && stepperX.distanceToGo() == 0)
    {
        Serial.println("X Move finished");
    }

    if (posNumber == 1 || posNumber == 8)
        return;
    else {
        Serial.println("Starting Z move");

        stepperZ.setMaxSpeed(1500.0);
        stepperZ.setAcceleration(1000.0);
        stepperZ.setCurrentPosition(0);

        stepperZ.enableOutputs();
        stepperZ.moveTo(65000);

        while (stepperZ.distanceToGo() != 0)
        {
            stepperZ.run();
        }
        stepperZ.disableOutputs();
        delay(2000);

        stepperZ.enableOutputs();
        stepperZ.moveTo(0);

        while (stepperZ.distanceToGo() != 0)
        {
            stepperZ.run();
        }
        Serial.println("Z move done");
        stepperZ.disableOutputs();
    }
}