#ifndef GPS_H
#define GPS_H

    // This file defines how to get data from the GPS module.
    // We are using the Sparkfun ZED-F9P with SMA antenna. 

    #include "types.h"
    #include <Wire.h>

    class GlobalPositioning
    {   
        public: 
            GlobalPositioningData data;

            // The constructor needs to know the I2C address.
            GlobalPositioning(uint8_t add)
            {
                address = add;
            }

            bool begin() 
            {
                // Check if the GPS is there
                // Reply should be a 0.
                Wire.beginTransmission(address);
                int reply = Wire.endTransmission();
                
                // By default the GPS is at register 0xFF
                // The I2C bus sits high. If a device pulls it low, reply will be 0. This indicates a device is at this address.
                // If reply is 0 it indicates success, so we return !0 = 1. !1 = 0, for fail.
                return !reply;  
            }

            int getData()
            {
                // This method will return 1 if it successfully updates the GlobalPositioningData variable. 
                clearBuffer();
                requestPVT();
                int success = read();
                return success; // 1 if success, 0 if fail
            }

        private: 
            uint8_t address;
            const uint8_t PAYLOAD_LENGTH = 92;
            const uint8_t PVT_PACKET_LENGTH = 100;
            uint8_t payload[92];
            
            uint16_t available()
            {
                // This method reads register 0xFD and 0xFD from the GPS. 
                // These two registers contain the number of bytes available to be read from register 0xFF. 
                // If an error occus, we return 0. 

                uint16_t bytesAvailable = 0;
                Wire.beginTransmission(address);
                Wire.write(0xFD);                           // 0xFD (MSB) and 0xFE (LSB) are the registers that contain number of bytes available
                int i2cError = Wire.endTransmission(false); // Always send a restart command. Do not release the bus. 
                
                if (i2cError != 0)
                    return 0; // Sensor did not ACK. Could do better error handling. 

                // Forcing requestFrom to use a restart would be unwise. If bytesAvailable is zero, we want to surrender the bus.
                int bytesReturned = Wire.requestFrom(address, 2);
                if (bytesReturned != 2)
                {
                    return 0; // Sensor did not return 2 bytes
                }
                else          // if (Wire.available() >= 2)
                {
                    uint8_t msb = Wire.read();
                    uint8_t lsb = Wire.read();
                    uint16_t bytesAvailable = (uint16_t)msb << 8 | lsb;
                    return bytesAvailable;
                }
                // Shouldn't be able to make it here, but just in case.
                return 0;
            }

            void clearBuffer()
            {
                // This method clears out the GPS's I2C buffer by first reading from 0xFD and 0xFE
                // to determine how many bytes to read. Then it reads that number of bytes and clears them. 

                uint16_t bytesAvailable = available();

                if (bytesAvailable)
                {
                    Wire.requestFrom(address, bytesAvailable);

                    while(Wire.available() > 0)
                    {
                        Wire.read();
                    }
                }
            }

            void requestPVT()
            {
                // This method will ask the GPS for Position, Velocity, Time data..

                // This array is all the data before the "payload" data
                // u, B, class, id, length (LSB), length (MSB);
                uint8_t header[] = {0xB5, 0x62, 0x01, 0x07, 0x00, 0x00 }; 

                // Start the checksum at 0
                uint8_t CK_A = 0;
                uint8_t CK_B = 0; 

                // Part of the 8-bit fletcher algorithm. 
                // The class and ID must be counted as with the payload
                for (int i=2; i<6; i++)
                {
                    CK_A = CK_A + header[i];
                    CK_B = CK_B + CK_A; 
                }

                // This is an empty PVT packet
                // Per the data sheet: 
                // "messages can be polled by sending the message 
                // required to the receiver but without a payload"
                Wire.beginTransmission(address);
                Wire.write(0xB5); // Header 
                Wire.write(0x62);
                Wire.write(0x01); // Class
                Wire.write(0x07); // ID
                Wire.write(0x00); // Length
                Wire.write(0x00);
                Wire.write(CK_A); // Checksum
                Wire.write(CK_B);
                Wire.endTransmission();
            }

            bool read()
            { 
                // This method request the NAV PVT message and reads it into the NavData data structure.
                // It returns a 1 if the structure has been modified.

                // // Request the data from the GPS
                // requestPVT();

                // Now we expect the GPS to send out a similar
                // packet with it's own header. Everything should be the same as the request
                // except for the length. The length should be 92 -> 0x5C.
                // But first we must ask the GPS if it is there yet.
                uint8_t header[] = {0xB5, 0x62, 0x01, 0x07, 0x00, 0x00 };   
                uint16_t bytesAvailable = 0;

                // We need to wait for the GPS to actually respond, so we will poll the 0xFD, 0xFE register.
                // If we take longer than 2 seconds, assume we failed
                unsigned long timeout = millis(); 
                while(bytesAvailable < PVT_PACKET_LENGTH) //&& (millis() - timeout) < 2000)
                {
                    delay(100);
                    bytesAvailable = available();
                }

                if (bytesAvailable < PVT_PACKET_LENGTH)
                {
                    clearBuffer();
                    return 0; // We failed
                }
                // Cool! So the data should be available now!
                // Let's request the data. 
                // 6 bytes for the header
                // 92 bytes for the payload 
                // 2 bytes for the checksums
                Wire.requestFrom(address, PVT_PACKET_LENGTH);

                for (int i=0; i<6; i++)
                {
                    header[i] = Wire.read();

                    if (DEBUG_PRINT_GPS_DATA)
                    {
                        Serial.print(header[i]); Serial.print(" ");  
                    }
                        
                }
                for (int i=0; i<92; i++)
                {
                    payload[i] = Wire.read();

                    if (DEBUG_PRINT_GPS_DATA)
                    {
                        Serial.print(payload[i]); Serial.print(" ");
                    }
                }
                uint8_t checksumA = Wire.read(); 
                uint8_t checksumB = Wire.read(); 

                // Checking for -> 0xB562, 0x0107, and 0x00C5
                uint16_t head_ = header[0]<<8 | header[1];          //Header
                uint16_t class_id = header[2]<<8 | header[3];       // Class/ID
                uint16_t payLoadLength = header[5] <<8 | header[4]; // Length

                // Here, we check if the data is what we expect:
                // head_         -> 0xB562
                // class_id      -> 0x0107
                // payLoadLength -> 0x005C
                // If so, we can proceed to read the payload 
                if (head_ == 0xB562 && class_id == 0x0107 && payLoadLength == 0x005C)
                {
                    uint8_t CK_A = 0;
                    uint8_t CK_B = 0;
                    // Get the first part of the checksum using the header we read in. 
                    for (int i=2; i<6; i++)
                    {
                        CK_A = CK_A + header[i];
                        CK_B = CK_A + CK_B;
                    }
                    for (int i=0; i<92; i++)
                    {
                        CK_A = CK_A + payload[i];
                        CK_B = CK_A + CK_B;
                    }
                    if (checksumA == CK_A && checksumB == CK_B)
                    {
                        data.date.year   = payload[5]<<8 | payload[4];
                        data.date.month  = payload[6]; 
                        data.date.day    = payload[7]; 
                        data.date.hour   = payload[8];
                        data.date.minute = payload[9];
                        data.date.second = payload[10];

                        data.fixType      = payload[20];
                        data.gnssFixOK    = payload[21] & 0x01; // Grab  the first bit from the "flags" byte. 
                        data.headVehValid = (payload[21] & (1<<5))>>5; // Grab the bit in the 2^5 position from the "flags" byte.
                        
                        data.position.latitude = payload[31] <<24 | payload[30] << 16 | payload[29]<<8 | payload[28]; 
                        data.position.longitude = payload[27] <<24 | payload[26] << 16 | payload[25]<<8 | payload[24];
                        data.SIV = payload[23];
                        
                        data.groundSpeed    = payload[63] << 24 | payload[62] << 16 | payload[61] << 8 | payload[60]; 
                        data.headingMotion  = payload[67] << 24 | payload[66] << 16 | payload[65] << 8 | payload[64];
                        data.headingVehicle = payload[87] << 24 | payload[86] << 16 | payload[85] << 8 | payload[84];
                        data.invalid_llh    = payload[78] & 0x01;  
                        return 1; // Success
                    }
                }
                clearBuffer();
                return 0; // Fail
            }
    }; 

#endif