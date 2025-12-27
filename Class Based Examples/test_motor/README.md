# Motor Class for the T200 motors and ESCs

This code provides a simple class for controlling an Electronic Speed Controller (ESC) using a PWM signal. It is written to work with typical RC-style ESCs (such as the BlueRobotics ESC). 

I've named this class Motor even though we are really only talking to the ESC. The ESC handles the motor control. So to our program the ESC is essentially the motor.  

## Big Picture of the Class
- Outputs a PWM signal using the Arduino Servo library 
- Controls motor speed by writing pulse widths in microseconds
- Smoothly ramps motor speed to the target value 
- Clamps commands to proper range 

## ESC Signal 

| Speed (us) | Direction | 
|---         |---        |
|1100-1475   | Reverse   |
|1475-1525   | Stopped   |
|1525-1900   | Forward   |

## Constructor

```cpp
Motor(uint8_t pwmPin, int forward_limit, int reverse_limit, int stop);
```

Parameters:
- pwmPin - Arduino pin connected to the ESC signal. (Look for a ~ on the Arduino). 
- forward_limit - Maximum Forward PWM value in us. (1900 for the Blue Robotics ESC)
- reverse_limit - Maximum reverse PWM value (us) (1100 for Blue Robotics ESC)
- stop - neutral/stop PWM value (1500 for Blue Robotics ESC)

## begin() Method
```cpp
void begin()
```

- Attaches the ESC signal pin using the Servo library. 
- Sends the stop signal 

## updateSpeed() method 
```cpp 
void updateSpeed(int targetSpeed);
```
- Clamps the requested speed to a range that is valid 
- Gradually ramps the current speed to the target speed. This prevents sudden jerks on the motor that sound unpleasant and will damage the motors over time. 
