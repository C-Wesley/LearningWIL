#ifndef IMU_H
#define IMU_H

    // This file defines how to talk to the BNO055 inertial measurement unit.
    // We configure it to give us a heading relative to the initial power on state. 
    // This involves a "fusion" mode using a combination of the gyroscope and accelerometer. 
    // Make sure to align the boat with north before power on for best results. 

    #include "types.h"
    #include <Wire.h>

    class BNO
    {
        public:
            BNO(uint8_t add)
            {
                address = add;
            }

            bool begin()
            {
                // Here we are going to check if a device is at the specified address.
                // We are assuming the address is correct one for the BNO. 
                
                Wire.beginTransmission(address);
                int reply = Wire.endTransmission();
                
                if (reply == 0) // Success! Device is there!
                {
                    // Configure the BNO to IMU mode
                    // OPR_MODE is 0x3D
                    // IMU setting : xxxx1000b
                    Wire.beginTransmission(address); 
                    Wire.write(OPR_MODE);
                    Wire.write(1<<3);
                    Wire.endTransmission();
                    
                    // Unit Select Register
                    // Set Degrees xxxxx0xxb
                    Wire.beginTransmission(address);
                    Wire.write(UNIT_SEL);
                    Wire.write(0<<2); 
                    Wire.endTransmission();
                    return 1;
                }
                else
                {
                    return 0;
                }
            }


            float getHeading()
            {
                // To read the heading, we read the register named "EULER_ANGLE_X" from the datasheet. 

                // Heading Regisgers 1A LSB , 1B MSB
                Wire.beginTransmission(address);
                Wire.write(HEADING_REGISTER);
                Wire.endTransmission();
                Wire.requestFrom(address, 2); // Go Get 1A, 1B since it reads in order

                uint8_t LSB = Wire.read();
                uint8_t MSB = Wire.read();
            
                // Put humpty dumpty back together and divide by 16
                return (float)(((int)MSB<<8 | LSB))/16.0; 
            }
        private:
            uint8_t address; 
            static constexpr int OPR_MODE = 0x3D;
            static constexpr uint8_t UNIT_SEL = 0x3B;  
            static constexpr uint8_t HEADING_REGISTER = 0x1A;
    };
#endif