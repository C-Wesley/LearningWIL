#ifndef RADIO_H
#define RADIO_H

    #include "types.h"

    class Radio
    {
        public:

            NavigationState navState; 
            EcoData         ecoData;

            Radio()
            {

            }

            void begin(int baud)
            {
                Serial1.begin(baud);
            }
            void sendSpeed(int leftSpeed, int rightSpeed)
            {
                Serial1.print('S');
                Serial1.write((byte*) &leftSpeed, sizeof(int));
                Serial1.write((byte*) &rightSpeed, sizeof(int));
                Serial1.print('E');
            }

            void requestEcoData()
            {
                Serial1.print('R');
                Serial1.print('E');
            }

            void requestIncrementWaypoint()
            {
                Serial1.print('W');
                Serial1.print('E');
            }

            void sendMode(ProgramMode mode)
            {
                Serial1.print('M');
                switch(mode)
                {
                    case REMOTECONTROL:
                        Serial1.print('1');
                        break;
                    case AUTONOMOUS:
                        Serial1.print('0');
                        break;
                    case TUGBOAT:
                        Serial1.print('2');
                        break;
                    default:
                        break;
                }
                Serial1.print('E');
            }

            void readNavState()
            {
                int packetSize = sizeof(NavigationState) + 1;
                byte buffer[packetSize]; 
                
                for (int ii=0; ii<packetSize; ii++)
                {
                    buffer[ii] = Serial1.read();
                }

                char footer = buffer[packetSize-1];
                
                if (footer == 'E')
                {
                    memcpy(&navState, buffer, sizeof(NavigationState));
                }
            }

            void readEco()
            {
                // packetSize is the size of data + the footer. We already read the header. 
                int packetSize = sizeof(EcoData) + 1;
                byte buffer[packetSize];
                
                for (int ii=0; ii<packetSize; ii++)
                {
                    buffer[ii] = Serial1.read();
                }
                
                // packetSize represents the total number 
                // of elements. We subtract one to get the index.
                char footer = buffer[packetSize-1];
                if (footer == 'E')
                {
                    memcpy(&ecoData, buffer, sizeof(EcoData));
                } 
            }

            void read()
            {
                while(Serial1.available() > 0)
                {
                    char header = Serial1.read(); 
                    
                    switch(header)
                    {
                        case 'C':
                            Serial.println("Reading Nav...");
                            readNavState();
                            break;
                        case 'R':
                            Serial.println("Reading Eco...");
                            readEco();
                            break;
                        default:
                            break;
                    }
                }
            }
    }; 

#endif