# BNO055 IMU

This sketch does the following: 

- Scans the I²C bus to confirm the BNO055 address (0x28)
- Configures the BNO055:
- - uses the accelerometer and gyroscope in a fusion mode
- - Sets output units to degrees
- Reads the heading registers (0x1A and 0x1B)
- Combines the raw bytes and converts them to degrees

# Lab Notebook Entries
[Revisiting the BNO055](https://c-wesley.github.io/projects/asv/Notebook_2/8_Revisiting_the_BNO055.pdf)