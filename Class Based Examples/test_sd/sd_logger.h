#ifndef SD_LOGGER_H
#define SD_LOGGER_H

    // This file contains the SDLog class and the Path class  
    // SDLog must be setup before parsing the path. 
    //    There's probally a less coupled way to do this.

    #include "types.h"
    #include <SD.h>

    class SDLog
    {
        public:
            SDLog(uint8_t cs)
            {
                // We need to know what pin the SD card is on. 
                chipSelect = cs;
                pinMode(chipSelect, OUTPUT);
            }

            // We don't want to hardcode the file names. Instead I define them in config.
            // and provide a way to change them here.
            void setRemoteWaterDataFileName(const char* rwd)
            {
                remote_water_data = rwd;
            }
            void setAutoWaterDataFileName(const char* awd)
            {
                auto_water_data = awd;
            }
            void setGPSDataFileName(const char* gpsd)
            {
                gps_data = gpsd;
            }

            bool begin()
            {
                if (!SD.begin(chipSelect))
                {
                    return 0;
                }
                else
                {
                    File header = SD.open(gps_data, FILE_WRITE);
                    header.println("Time, fixType, gnssFixOk, headVehValid, "
                                "invalid_llh, latitude, longitude, SIV, groundSpeed, headingMotion, "
                                "headingVehicle, distanceBetween, targetHeading, "
                                "currentHeading, headingError, gpsError, averageGPSError, index");
                    header.close();

                    header = SD.open(auto_water_data, FILE_WRITE); 
                    header.println("Time, Date, Lon, Lat, DO, ORP, pH, Conductivity, Temp");
                    header.close();

                    header = SD.open(remote_water_data, FILE_WRITE);
                    header.println("Time, Date, Lon, Lat, DO, ORP, pH, Conductivity, Temp");
                    header.close();

                    return 1;
                }
            }

            void logWaterData(const Date &date, const Point &location, const EcoData &data, ProgramMode mode)
            {
                File ecoFile;
                if (mode == REMOTECONTROL)
                    ecoFile = SD.open(remote_water_data, FILE_WRITE);
                else // Both autonomous and Tugboat for now. 
                    ecoFile = SD.open(auto_water_data, FILE_WRITE);

                // Timestamp
                ecoFile.print(date.month);   ecoFile.print("/");
                ecoFile.print(date.day);     ecoFile.print("/");
                ecoFile.print(date.year);    ecoFile.print(", ");
                ecoFile.print(date.hour);    ecoFile.print(":");
                ecoFile.print(date.minute);  ecoFile.print(":");
                ecoFile.print(date.second);  ecoFile.print(", ");

                // Position + Sensor Data
                ecoFile.print(location.longitude); ecoFile.print(", ");
                ecoFile.print(location.latitude);  ecoFile.print(", ");
                ecoFile.print(data.DO);            ecoFile.print(", ");
                ecoFile.print(data.ORP);           ecoFile.print(", ");
                ecoFile.print(data.pH);            ecoFile.print(", ");
                ecoFile.print(data.conductivity);  ecoFile.print(", ");
                ecoFile.print(data.waterTemp);     ecoFile.println();

                ecoFile.close();
            }

            void logAutonomous(const NavigationState &state, const GlobalPositioningData &data)
            {
                // NavData comes from the GPS
                // navState contains the calculations that may have used NavData
                File navFile = SD.open(gps_data, FILE_WRITE);

                // Date and Time
                navFile.print(data.date.month);                    navFile.print("/");
                navFile.print(data.date.day);                      navFile.print("/");
                navFile.print(data.date.year);                     navFile.print(" ");
                navFile.print(data.date.hour);                     navFile.print(":");
                navFile.print(data.date.minute);                   navFile.print(":");
                navFile.print(data.date.second);                   navFile.print(", ");

                // Validity Flags
                navFile.print(data.fixType);                  navFile.print(", ");
                navFile.print(data.gnssFixOK);                navFile.print(", ");
                navFile.print(data.headVehValid);             navFile.print(", ");
                navFile.print(data.invalid_llh);              navFile.print(", ");

                // Navigation Data
                navFile.print(data.position.latitude);         navFile.print(", ");
                navFile.print(data.position.longitude);        navFile.print(", ");
                navFile.print(data.SIV);                       navFile.print(", ");
                navFile.print(data.groundSpeed);               navFile.print(", ");
                navFile.print(data.headingMotion);             navFile.print(", ");
                navFile.print(data.headingVehicle);            navFile.print(", ");
                navFile.print(state.distanceToWaypoint);       navFile.print(", ");
                navFile.print(state.targetHeading);            navFile.print(", ");
                navFile.print(state.currentHeading);           navFile.print(", ");
                navFile.print(state.headingError);             navFile.print(", ");
                navFile.print(state.gpsError);                 navFile.print(", ");
                navFile.print(state.averageGPSError);          navFile.print(", ");
                navFile.print(state.waypointIndex);            navFile.println();
                navFile.close();  
            }
        private:
            uint8_t chipSelect;
            const char* remote_water_data;
            const char* auto_water_data;
            const char* gps_data;
    };

    class Path
    {   
        // The path is stored on the SD card in "path.csv"
        // :TODO: don't hardcode the path filename. 
        //   -> Long Term, don't store path on the boat itself, but have the remote control transmit the path.
        //   -> Have several paths on remote and change dynamically. Don't need to get in water after deploying boat.  
        
        public:
            Path(int maxLength)
            {
                dLons = new long[maxLength]();
                dLats = new long[maxLength]();
                MAX_LEN = maxLength;
                numWayPoints = 0;
            }

            // Make sure the arrays are deleted since we started them with "new"
            // Also known as a desructor
            ~Path()
            {
                delete[] dLons;
                delete[] dLats;
            }

            bool parsePath() 
            {
                // Returns if the parse was successful
                
                File pathFile = SD.open("path.csv");

                if (!pathFile)
                {
                    return 0;
                }

                int ln = 0;
                while (pathFile.available() && (ln-1 < MAX_LEN)) 
                {
                    ln++;
                    char aline[50] = {};
                    char data     = ' ';
                    int index     = 0;

                    // Read one line from the file
                    while(data != '\n')
                    {
                        data = pathFile.read();
                        aline[index] = data;
                        index++;
                    }

                    if (ln != 1)
                    {
                        char* tok = strtok(aline, ",");      // Read the first token
                        int id    = atoi(tok);               // Save it as an int

                        tok            = strtok(NULL, ",");  // Read the next token
                        long longitude = atoi(tok);          // This is our longitude

                        tok            = strtok(NULL, ",");  // Read the next token
                        long latitude  = atol(tok);          // Save it as a long

                        tok            = strtok(NULL, ",");  // We should have a null value now

                        dLons[id-1] = longitude;
                        dLats[id-1] = latitude;
                    }
                    else
                    {
                        Serial.println("Header of Path: ");
                        Serial.print(aline);
                    }
                }
                numWayPoints = ln-1;
                pathFile.close();
                return 1;
            }

            void printPath()
            {
                Serial.print("Number of Waypoints: "); Serial.println(numWayPoints);
                for (int ii = 0; ii < numWayPoints; ii++)
                {
                    Serial.print("Waypoint "); Serial.print(ii + 1); Serial.print(": ");
                    Serial.print(dLons[ii]); Serial.print(", "); Serial.println(dLats[ii]);
                }
            }
        
        private: 
            long* dLons;
            long* dLats;
            int numWayPoints;
            int MAX_LEN;
        
    };

#endif