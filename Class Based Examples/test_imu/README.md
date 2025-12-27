# BNO055 IMU 

The BNO055 inertial measurement unit uses a combination of an accelerometer, gyroscope, and magnetometer to keep tracking headings from some reference system. 

The `BNO` class does the following: 
- Uses I²C to get data from a BNO055. 
- Configures the BNO to use a IMU fusion mode
    - The gyroscope and accelerometer fusion mode is used for WIL 
    - WIL does not use the magnetometer. In our experiments we found that the magnetometer was too susceptible to noise such as power lines. 
- The BNO class reports a heading in degrees. 
- The heading will be relative to the orientation when the system is powered on. 

When using WIL, it is best to align the system to a desired reference direction before powering on the IMU. 

For small experiments you may wish to align the IMU with the edge of a table. Then you can rotate it by 90 degrees and see if the measurement changed by 90. 

## Class Overview 

### Constructor 
```cpp
BNO(uint8_t address); 
```

Creates a BNO interface using the given address. 

### The begin() method 
```cpp
bool begin(); 
``` 
1. Verifies that a device responds at the given I²C address
2. Sets the BNO055 to IMU mode
    - register: OPR_MODE (0x3D)
3. Configures the angle units to degrees 
    - register: UNIT_SEL (0x3B)

Returns true if initialization succeeds.
Returns false if hte device does not respond. 

### The getHeading() method 
```cpp
float getHeading();
``` 
The heading is read from the Euler X registers. The value is a 16-bit quantity split across two 8-bit registers (LSB and MSB)

- LSB is at 0x1A 
- MSB is at 0x1B 

After combining the bytes, we divide by 16 per the data sheet. If the reading is 16 (0b1000 or 0x10) the heading is 1 degree. So each bit represents 1/16 th of a degree. 

## Notes
There is a calibration that we can check. However, the calibration process is not easy to do on WIL since we can't pick WIL up and move him around easily. We have found that the BNO will 'drift' over time. This drift causes our heading error to accumulate. To solve this we use a heading we get from the GPS to correct the BNO over time. That is not presented here, but is good to know. 