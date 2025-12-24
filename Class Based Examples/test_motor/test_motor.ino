#include "config.h"
#include "types.h"
#include "motor.h"

//            7            , 1900            , 1100            , 1500
Motor aMotor(LEFT_MOTOR_PIN, ESC_FULL_FORWARD, ESC_FULL_REVERSE, ESC_STOP_SIGNAL);

void setup()
{
    Serial.begin(9600);
    while(!Serial);
    aMotor.begin();
    delay(5000);        // Give the ESC time to see the motor.
}

void loop()
{
    if (Serial.available() > 0)        // Use the serial monitor to change the speed.
    {
        int speed = Serial.parseInt();
        aMotor.updateSpeed(speed);

        if (Serial.available() > 0)    // Make sure it's fully empty
        {
            Serial.read();
        }
    }
}