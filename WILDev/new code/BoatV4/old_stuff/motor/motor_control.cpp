#include "motor_control.h"
#include "../control/heading_controller.h"
#include "../control/remote_controller.h"
#include "../imu/bno.h"
#include <Servo.h>

namespace MotorControl
{
    static Servo leftServo;
    static Servo rightServo;
    
    void setup()
    {
        pinMode(LEFT_MOTOR_PIN, OUTPUT);
        pinMode(RIGHT_MOTOR_PIN, OUTPUT);

        leftServo.attach(LEFT_MOTOR_PIN);
        rightServo.attach(RIGHT_MOTOR_PIN);

        leftServo.writeMicroseconds(ESC_STOP_SIGNAL);
        rightServo.writeMicroseconds(ESC_STOP_SIGNAL);

        delay(MOTOR_SETUP_DELAY_MS);
    }

    void setSpeed(int leftSpeed, int rightSpeed)
    {
        leftServo.writeMicroseconds(leftSpeed);
        rightServo.writeMicroseconds(rightSpeed);
    }

    void updateSpeed(int targetLeft, int targetRight)
    {
        while (motorState.leftMotorSpeed != targetLeft || motorState.rightMotorSpeed != targetRight)
        {   
            // Here, I am using terniary operators.
            // (conditional statement) ? (value if true) : (value if false)
            if (motorState.leftMotorSpeed != targetLeft)
                motorState.leftMotorSpeed += (motorState.leftMotorSpeed < targetLeft) ? 1 : -1;
            
                if (motorState.rightMotorSpeed != targetRight)
                motorState.rightMotorSpeed += (motorState.rightMotorSpeed < targetRight) ? 1 : -1;

            setMotorSpeed(motorState.leftMotorSpeed, motorState.rightMotorSpeed);
            delay(ESC_UPDATE_DELAY_MS); // Optional: config.h constant
        }
    }

    void rotateToHeading()
    {
        while (abs(navState.headingError) > ACCEPTABLE_HEADING_ERROR)
        {
            Remote::readController(); // Make sure we don't block our controller from communication
            
            // If the mode changed, then we need to exit.
            if (currentMode == REMOTECONTROL) return;

            if (navState.headingError > 0)
            {
                // Rotate right
                updateMotors(ESC_ROTATE_FORWARD, ESC_ROTATE_REVERSE);
            }
            else
            {
                // Rotate left
                updateMotors(ESC_ROTATE_REVERSE, ESC_ROTATE_FORWARD);
            }

            navState.currentHeading = fixHeading(BNO::getHeading() - navState.averageGPSError);
            navState.headingError = Heading::getHeadingError(navState.currentHeading, navState.targetHeading);
        }

        // Stop rotation
        updateMotors(ESC_STOP_SIGNAL, ESC_STOP_SIGNAL);
    }
}