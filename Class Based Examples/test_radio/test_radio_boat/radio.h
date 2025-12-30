#ifndef RADIO_H
#define RADIO_H

    // We are using the SIK Telemetry Serial Radio.
    // At it's core, the Serial radio is just a remote UART. 
    // I've implemeneted the data processing for the remote here 
    // because we are using it to communicate with the remote controller. 
 
    #include "types.h"

    class RadioControl
    {
        public: 
            ProgramMode mode;        // I'm letting the controller own the program mode because it is responsible for changing it.
                                     // We could let the boat own program mode and have a "received program mode" here... 
            int leftMotorRec;        // Since the radio doesn't own the motors, we will get a "received motor speed" and update the motors separately. 
            int rightMotorRec; 
            bool takeRemoteReading;  // The remote may ask us to record eco data. 
            int MAX_WAIT;            // How long to wait for full message to come in once we see the header of it.

            RadioControl(int maxTimeout)
            {
                leftMotorRec = 1500;
                rightMotorRec = 1500; 
                mode = REMOTECONTROL;
                MAX_WAIT = maxTimeout;
            }

            void begin(int baud)
            {
                // The default baud is 57600.
                // This is configurable. 
                // I've made a variable for this in config, but left the class adjustable. 
                Serial2.begin(baud);
            }

            void read()
            {
                while(Serial2.available() > 0)
                {
                    char start = Serial2.read();
                    
                    switch (start)       // Check for one of our packet headers
                    {
                        case 'S':        // We are receiving a speed 
                            Serial.println("We are recieiving a speed.");
                            readSpeed();
                            break;
                        case 'M':       // We are receiving a mode 
                            Serial.println("We are receiving a mode.");
                            readState();
                            break;
                        case 'R':       // We are asked to make measurements
                            Serial.println("We are being asked for eco data.");
                            handleRemoteReading();
                            break;
                        default:
                            break;
                            // Continue reading chars until we get the character we need.
                    }
                }
            }

            void sendEcoData(const EcoData &data)
            {
                // Convert the EcoData to a pointer to a byte array 
                byte * b = (byte *) &data;

                Serial2.print('R');                // Header 
                Serial2.write(b, sizeof(EcoData)); // Data 
                Serial2.print('E');                // Footer

                // Turn off our remote reading flag. 
                takeRemoteReading = 0;
            }

            void sendAutoData(const NavigationState &state) // should have named this sendNavState /fp 
            {
                // Convert the NavigationState to a pointer to a byte array 
                byte * b = (byte *) &state;

                Serial2.print('C');
                Serial2.write(b, sizeof(NavigationState));
                Serial2.print('E');
            }
        
        private: 
            
            void readSpeed()
            {
                uint8_t packetSize = sizeof(int)*2 + 1; // The speed packet consists of 2 integers and 1 footer byte.
                uint8_t buffer[packetSize];
                unsigned long timeout = millis();

                // Wait for enough bytes or for the timeout
                while(Serial2.available() < (packetSize) ) //&& (millis() - timeout) <MAX_WAIT)
                {}
                // Serial.print("Speed Time: "); Serial.println(millis()-timeout);

                // If there are enough bytes, try to read.
                if (Serial2.available() >= packetSize)
                {
                    for (int ii=0; ii<packetSize; ii++)
                    {
                        buffer[ii] = Serial2.read();
                    }
                    // Last byte is the footer
                    char footer = buffer[packetSize-1]; 
                    if (footer == 'E')
                    {
                        // Expected message format (9 bytes total):
                        // Byte 0–3: leftMotorRec (int)
                        // Byte 4–7: rightMotorRec (int)
                        // Byte 8:   footer ('E')
                        // 'buffer' -> memory address of start of leftMotorRec
                        // 'buffer + sizeof(int)' -> memory address of the start of rightMotorRec
                        memcpy(&leftMotorRec, buffer, sizeof(int));
                        memcpy(&rightMotorRec, buffer+sizeof(int), sizeof(int));
                    }
                }
            }

            void readState()
            {
                uint8_t packetSize = sizeof(char) + 1; // Mode Character + Footer
                unsigned long timeout = millis();
                
                // Wait for enough bytes or for the timeout
                while(Serial2.available() < (packetSize))// && (millis() - timeout) <MAX_WAIT)
                {}
                Serial.print("State Time: "); Serial.println(millis()-timeout);

                // If there are enough bytes, try to read.
                if (Serial2.available() >= packetSize)
                {
                    char recMode = Serial2.read();
                    char footer = Serial2.read();

                    if (footer == 'E')
                    {
                        switch(recMode)
                        {
                            case '0':
                                mode = AUTONOMOUS;
                                break;
                            case '1':
                                mode = REMOTECONTROL;
                                break;
                            case '2':
                                mode = TUGBOAT;
                            default:
                                break;    
                        }
                    }
                }
            }

            void handleRemoteReading()
            {
                while(Serial2.available() <= 0) {}

                Serial.print("Footer: ");
                char footer = Serial2.read();
                Serial.println(footer);
                if (footer == 'E')
                {
                    takeRemoteReading = 1;
                    // We will set this back to 0 when we actually take a reading. 
                }
            }
        
    }; 

#endif