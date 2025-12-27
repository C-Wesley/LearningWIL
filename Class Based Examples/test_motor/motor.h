#ifndef MOTOR_H
#define MOTOR_H

    #include "types.h"
    #include <Servo.h> 

    // This class is intended to be used with an ElectronicSpeedController.
    // We are using the BlueRobotics ESC. 
    // In that case, a more appropriate name may be ESC or the full name.
    // However, in our abstraction for the Boat, the ESC is effectively the motor
    // so I've decided to name the class motor. 
    class Motor
    {
        public:
            Motor(uint8_t pwm, int forward_limit, int reverse_limit, int stop)
            {
                // The ESC has a PWM pin for speed control.
                // The PWM has a 20 us period and valid speeds are typically between 1100 and 1900.
                // For us the stoping value is around 1500. 
                // Anything lower down to 1100 is reverse and anything great up to 1900 is forward.
                // But I've made them modular in case we use a difference ESC that works differently one day.

                pwmPin = pwm;
                MAX_FORWARD = forward_limit;
                MAX_REVERSE = reverse_limit;
                STOP = stop;
            }
            
            void begin()
            {
                // To provide the PWM signal, Arduino has the Servo class.
                // We are basically building a fancy wrapper for the servo class...
                motor.attach(pwmPin);
                currentSpeed = STOP; 
                motor.writeMicroseconds(STOP);
            }

            void updateSpeed(int targetSpeed)
            {   
                targetSpeed = clampSpeed(targetSpeed);
                while(currentSpeed != targetSpeed)
                {   
                    // The right hand side of this line of code is called 
                    // a ternary operator. 
                    //              (condition) ? (value if true)  : (value if false)
                    currentSpeed += (currentSpeed < targetSpeed) ? (1) : (-1);
                    motor.writeMicroseconds(currentSpeed);
                    delayMicroseconds(500);
                }
            }

        private:
            uint8_t pwmPin; 
            int currentSpeed;
            Servo motor;

            int STOP;
            int MAX_FORWARD;
            int MAX_REVERSE;

            int clampSpeed(int speed)
            {
                if (speed > MAX_FORWARD)
                {
                    return MAX_FORWARD;
                }
                else if (speed < MAX_REVERSE)
                {
                    return MAX_REVERSE;
                }
                else
                {
                    return speed;
                }
            }

    };

#endif