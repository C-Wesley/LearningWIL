#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "../shared/types.h"
#include "../shared/config.h"
#include "../shared/globals.h"

namespace MotorControl
{
    // Initializes motor pins and ESC startup
    void setup();

    // Sets motor speeds directly
    void setSpeed(int leftSpeed, int rightSpeed);

    // Smoothly changes to target speeds
    void updateSpeed(int targetLeft, int targetRight);

    // Used to rotate the boat to match heading
    void rotateToHeading();
}


#endif