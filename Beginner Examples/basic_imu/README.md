# BNO055 IMU

This sketch does the following: 

- Scans the I²C bus to confirm the BNO055 address (0x28)
- Configures the BNO055:
- - uses the accelerometer and gyroscope in a fusion mode
- - Sets output units to degrees
- Reads the heading registers (0x1A and 0x1B)
- Combines the raw bytes and converts them to degrees

# Related Lab Notebook Entries

## No-Library-Challenge
[Revisiting the BNO055](https://c-wesley.github.io/projects/asv/Notebook_2/8_Revisiting_the_BNO055.pdf)

## Adafruit_Sensor and Adafruit_BNO055 Library
[BNO055 Testing](https://c-wesley.github.io/projects/asv/Notebook_1/3_BNO055_Testing.pdf)

The following resource was used when making the above: 
[Paul McWhorter IMU Videos](https://toptechboy.com/arduino-based-9-axis-inertial-measurement-unit-imu-based-on-bno055-sensor/)