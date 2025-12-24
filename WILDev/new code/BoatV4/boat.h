#ifndef BOAT_H
#define BOAT_H
    
    #include "config.h"
    #include "types.h"
    #include "gps.h"
    #include "imu.h"
    #include "motor.h"
    #include "radio.h"
    #include "sd_logger.h"
    #include "sensors.h"
    #include "types.h"
    #include "utility.h"
    #include <SD.h>

    class Boat
    {
        public:
            Motor leftMotor;
            Motor rightMotor; 
            GlobalPositioning gps;
            BNO bno;
            Point currentLoc;
            Point targetLoc;
            WaterMonitor waterMonitor; 
            RadioControl remote; 
            SDLog logger;
            HeadingAverage headingAvg; 
            NavigationState navState;
            Path targetPath;

            // Initilize all the classes on the constructor of boat. 
            Boat() : leftMotor(LEFT_MOTOR_PIN, ESC_FULL_FORWARD, ESC_FULL_REVERSE, ESC_STOP_SIGNAL), 
                     rightMotor(RIGHT_MOTOR_PIN, ESC_FULL_FORWARD, ESC_FULL_REVERSE, ESC_STOP_SIGNAL),
                     gps(GPS_I2C_ADD),
                     bno(BNO_ADD),
                     currentLoc(0, 0),
                     targetLoc(0, 0),
                     waterMonitor(ATLAS_DO_ADD,
                                  ATLAS_ORP_ADD,
                                  ATLAS_PH_ADD,
                                  ATLAS_C_ADD,
                                  ONE_WIRE_BUS),
                     remote(MAX_WAIT_MS),
                     logger(SD_CHIP_SELECT),
                     targetPath(MAX_WAYPOINTS)
            {
                navState.waypointIndex   = 0;
                navState.averageGPSError = 0;
                navState.gpsError        = 0;
                navState.headingError    = 0;
                startGPSAvg              = 1;
            }

            void begin()
            {
                leftMotor.begin();
                rightMotor.begin();
                delay(ESC_INIT_TIME_MS);

                bool gpsError = gps.begin();
                bool bnoError = bno.begin();
                bool waterMonitorError = waterMonitor.begin();
                
                remote.begin(SERIAL_RADIO_BAUD);
                logger.setAutoWaterDataFileName(ECO_DATA_FILE);
                logger.setGPSDataFileName(GPS_DATA_FILE); 
                logger.setRemoteWaterDataFileName(ECO_REMOTE_FILE);
                bool sdError        = logger.begin();
                bool pathParseError = targetPath.parsePath();
                targetPath.printPath();
                gpsPollingTimer = millis();
                
                if (DEBUG_PRINT_SERIAL)
                {
                    Serial.print("GPS     : "); Serial.println(gpsError == 1 ? "SUCCESS" : "FAIL");
                    Serial.print("BNO     : "); Serial.println(bnoError == 1 ? "SUCCESS" : "FAIL");
                    Serial.print("SENSORS : "); Serial.println(waterMonitorError == 1 ? "SUCCESS" : "FAIL");
                    Serial.print("SD      : "); Serial.println(sdError == 1 ? "SUCCESS" : "FAIL");
                    Serial.print("PATH    : "); Serial.println(pathParseError == 1 ? "SUCCESS" : "FAIL");
                }
                
                // if (!(gpsError && bnoError && waterMonitorError && sdError && pathParseError))
                // {
                //     // Something failed. Stop.
                //     while(1);
                // }

                // ::::
                // remote.mode = AUTONOMOUS;

                navState.numWaypoints = targetPath.numWayPoints;
            }

            void updateAvgGPSError()
            {
                navState.averageGPSError = headingAvg.get() + navState.averageGPSError;
                headingAvg.reset();
            }

            void finishedPath()
            {
                navState.waypointIndex--; 
                navState.averageGPSError = 0;
                startGPSAvg = 1;
                headingAvg.reset();
            }
        
            void updateDestination()
            {
                targetLoc.latitude = targetPath.dLats[navState.waypointIndex]; 
                targetLoc.longitude = targetPath.dLons[navState.waypointIndex]; 
            }

            void dontTurnArroundCheck()
            {
                Serial.println();
                if (navState.waypointIndex != 0)
                {
                    navState.currentHeading = clampHeading(bno.getHeading() - navState.averageGPSError);
                    navState.targetHeading  = currentLoc.headingTo(targetLoc);
                    navState.headingError   = getHeadingError(navState.currentHeading, navState.targetHeading);  // Update the error in our heading. -1 to 1. 

                    Serial.println();
                    Serial.print("Error: "); Serial.println(navState.headingError);
                    if (abs(navState.headingError) > 0.5 && navState.waypointIndex < navState.numWaypoints-1)
                    {
                        // Assume the next way point is better. 
                        navState.waypointIndex++;
                    }
                }
                
            }
            
            void updateNavState()
            {
                // ::::
                // Serial.print(targetLoc.latitude); Serial.print(" "); Serial.println(targetLoc.longitude);
                navState.distanceToWaypoint = currentLoc.distanceTo(targetLoc);                                  // Distance from current to target waypoint
                //Serial.print("DistancToWayPoint: "); Serial.println(navState.distanceToWaypoint);
                navState.targetHeading      = currentLoc.headingTo(targetLoc);                                   // Target heading to go from current waypoint to target waypoint
                navState.currentHeading     = clampHeading(bno.getHeading() - navState.averageGPSError);         // The heading our IMU reports + an average correction value from the heading of motion that the GPS sees. Bounded by 0 to 359
                navState.headingError       = getHeadingError(navState.currentHeading, navState.targetHeading);  // Update the error in our heading. -1 to 1. 
                navState.gpsError           = navState.currentHeading - (gps.data.headingMotion / 100000.0);     // Update the error from our GPS heading
                
                // Not updating here:
                // navState.averageGPSError; // Based on time
                // navState.numWaypoints;    // Based on path
                // navState.waypointIndex;   // Based on location 
            }   
            void rotateToTargetHeading()
            {
                // While our heading is too far off from the target.
                while(abs(navState.headingError) > ACCEPTABLE_HEADING_ERROR)
                {
                    remote.read(); // Make sure we don't block commands from the remote

                    if (remote.mode != AUTONOMOUS)
                    {
                        // If we changed to any other mode, we need to exit.
                        return; 
                    }
                    if (navState.headingError >= 0) // Rotate Right
                    {
                        // leftMotor.updateSpeed(ESC_ROTATE_FORWARD); // Set the left motor to reverse
                        // rightMotor.updateSpeed(ESC_ROTATE_REVERSE);// Set the right motor to forward
                        updateSpeedSync(ESC_ROTATE_FORWARD, ESC_ROTATE_REVERSE); 
                    }
                    else // Heading error <0 , Rotoate Left
                    {
                        // leftMotor.updateSpeed(ESC_ROTATE_REVERSE);
                        // rightMotor.updateSpeed(ESC_ROTATE_FORWARD);
                        updateSpeedSync(ESC_ROTATE_REVERSE, ESC_ROTATE_FORWARD);
                    }
                    navState.currentHeading = clampHeading(bno.getHeading() - navState.averageGPSError);
                    navState.headingError   = getHeadingError(navState.currentHeading, navState.targetHeading);
                }
                // Set the motors back to straight after heading is corrected
                // leftMotor.updateSpeed(ESC_FORWARD);
                // rightMotor.updateSpeed(ESC_FORWARD);
                updateSpeedSync(ESC_FORWARD, ESC_FORWARD);
            }

            double getHeadingError(double aHeading, double aTargetHeading)
            {
                double error = aTargetHeading - aHeading;
                // The max value of this error should be 180.
                // For example: (Target 359) - (Heading 0) = 359, but this should really only be a -1. 
                // The negative says to rotate left. 
                // 359 - 360 = -1
                // so we subtract 360. 

                // 0 - 359 = -359, but this should really be a +1. 
                // The positive says to rotate right. 
                // -359 + 360 = 1
                // so we add 360. 

                // 180 - 0 = 180. 
                // If we want to center from -180 to 180, we will subtract 360 here. 

                // 0 - 180 = -180.  
            
                if (error >= 180)
                {
                    error -= 360;
                }
                else if (error < -180)
                {
                    error += 360;
                }

                // Set error number between -1 and 1. 
                return error/180.0;
            }

            void updateSpeedSync(int targetLeft, int targetRight)
            {   
                while (leftMotor.currentSpeed != targetLeft || rightMotor.currentSpeed != targetRight)
                {
                    int currentLeftSpeed = leftMotor.currentSpeed; 
                    int currentRightSpeed = rightMotor.currentSpeed;
                    // Handle the left motor
                    if (currentLeftSpeed != targetLeft)
                    {
                        if(currentLeftSpeed < targetLeft)
                        {
                            leftMotor.setSpeed(currentLeftSpeed + 1); 
                        }
                        else
                        {
                            leftMotor.setSpeed(currentLeftSpeed -1); 
                        }
                    }

                    // Handle the right motor 
                    if (currentRightSpeed != targetRight)
                    {
                        if(currentRightSpeed < targetRight)
                        {
                            rightMotor.setSpeed(currentRightSpeed+1); 
                        }
                        else
                        {
                            rightMotor.setSpeed(currentRightSpeed-1); 
                        }
                    }
                }
            }

            double clampHeading(double myHeading)
            {
                // Takes in a heading in degrees and makes sure it is within the range [0,360) 
                while(myHeading < 0 || myHeading >=360)
                {
                    if (myHeading < 0)
                    {
                        myHeading += 360;
                    }
                    else if (myHeading >= 360)
                    {
                        myHeading -= 360;
                    }
                }

                return  myHeading; 
            }

            void handleRemote()
            {
                Serial.print(remote.leftMotorRec); Serial.print(" "); Serial.println(remote.rightMotorRec);

                // leftMotor.updateSpeed(remote.leftMotorRec);
                // rightMotor.updateSpeed(remote.rightMotorRec);
                updateSpeedSync(remote.leftMotorRec, remote.rightMotorRec); 

                if (remote.takeRemoteReading)
                {
                    // Stop the motors to take a reading. 
                    // leftMotor.updateSpeed(ESC_STOP_SIGNAL);
                    // rightMotor.updateSpeed(ESC_STOP_SIGNAL);
                    updateSpeedSync(ESC_STOP_SIGNAL, ESC_STOP_SIGNAL); 
                    
                    // Get location and data.
                    int succ = gps.getData();
                    if (succ)
                    {
                        EcoData data = waterMonitor.getData();

                        logger.logWaterData(gps.data.date, gps.data.position, data, remote.mode);
                        remote.sendEcoData(data);
                    }
                }
            }
            
            void handleAuto()
            {
                // Make sure we have the right designation. This will change if wayPointIndex changes. 
                if (remote.incrementWaypoint == 1)
                {
                    if (navState.waypointIndex+1 < navState.numWaypoints)
                    {
                        navState.waypointIndex++; 
                    }
                    remote.incrementWaypoint = 0;
                } 
                // updateDestination();
                
                // Make sure we don't hammer the GPS too often. 
                if ( millis() - gpsPollingTimer > GPS_POLLING_MS)
                {
                    // Get the data from the GPS
                    int success = gps.getData();
                    gpsPollingTimer = millis(); 
                    
                    // If we actually got data back
                    if (success)
                    {
                        currentLoc = gps.data.position;
                        // updateDestination();
                        // dontTurnArroundCheck();
                        updateDestination();
                        // Update the variables we will use to navigate 
                        updateNavState(); 
                        
                        // Tell the remote and save the data to a file 
                        remote.sendAutoData(navState);
                        logger.logAutonomous(navState, gps.data);

                        // For debugging. May need to comment out rotateToTargetHeading() method if stationary testing. 
                        printVars(gps.data, navState);

                        // Make sure we have a good fix, we are in autonomous mode, and the gps had more than 6 SIV. 
                        if (gps.data.gnssFixOK && remote.mode == AUTONOMOUS && (gps.data.SIV > 6))
                        {
                            // Start the timer to update our GPS heading error only after we got good data once.
                            if (startGPSAvg)
                            {
                                gpsErrorPollingTimer = millis();
                                startGPSAvg     = 0;
                            }

                            headingAvg.update(navState.gpsError); // Keep up with the gps error.

                            // If our heading is too far off from the acceptable error
                            // stop moving and go to the subrouting to fix the heading of the boat. 
                            if (abs(navState.headingError) > ACCEPTABLE_HEADING_ERROR)
                            {
                                // leftMotor.updateSpeed(ESC_STOP_SIGNAL);
                                // rightMotor.updateSpeed(ESC_STOP_SIGNAL);
                                updateSpeedSync(ESC_STOP_SIGNAL, ESC_STOP_SIGNAL);

                                rotateToTargetHeading();
                            }

                            if (navState.distanceToWaypoint <= WAYPOINT_DISTANCE_THRESHOLD)
                            {
                                // leftMotor.updateSpeed(ESC_STOP_SIGNAL);
                                // rightMotor.updateSpeed(ESC_STOP_SIGNAL);
                                updateSpeedSync(ESC_STOP_SIGNAL, ESC_STOP_SIGNAL);
                                
                                EcoData data = waterMonitor.getData();
                                
                                logger.logWaterData(gps.data.date, gps.data.position, data, remote.mode);

                                navState.waypointIndex++;
                                
                                navState.averageGPSError = headingAvg.get() + navState.averageGPSError;
                                headingAvg.reset();
                                gpsErrorPollingTimer = millis();

                                if (navState.waypointIndex >= navState.numWaypoints)
                                {
                                    finishedPath();
                                }
                            }
                            else if ((millis() - gpsErrorPollingTimer)> GPS_ERROR_POLLING_MS)
                            {
                                navState.averageGPSError = headingAvg.get() + navState.averageGPSError;
                                headingAvg.reset();
                                gpsErrorPollingTimer = millis(); 
                            }
                            else
                            {
                                // Our heading is too far off and we are not close enough to the point. 
                                // We should move forward
                                // leftMotor.updateSpeed(ESC_FORWARD);
                                // rightMotor.updateSpeed(ESC_FORWARD);
                                updateSpeedSync(ESC_FORWARD, ESC_FORWARD); 
                            }
                        }
                        
                    }
                    else
                    {
                        // We didn't get good gps data.
                        Serial.print("GPS didn't read properly.");
                        // leftMotor.updateSpeed(ESC_STOP_SIGNAL);
                        // rightMotor.updateSpeed(ESC_STOP_SIGNAL);
                        updateSpeedSync(ESC_STOP_SIGNAL, ESC_STOP_SIGNAL);
                    }

                }
            }

            void handleTugboat()
            {
                // leftMotor.updateSpeed(ESC_STOP_SIGNAL);
                // rightMotor.updateSpeed(ESC_STOP_SIGNAL);
                updateSpeedSync(ESC_STOP_SIGNAL, ESC_STOP_SIGNAL);

                int succ = gps.getData();
                if (succ)
                {
                    EcoData data = waterMonitor.getData();
                    logger.logWaterData(gps.data.date, gps.data.position, data, remote.mode);
                }
            }

            private: 
                unsigned long gpsPollingTimer;
                unsigned long gpsErrorPollingTimer;
                int startGPSAvg;
    }; 

#endif



/* 


                int success = gps.getData();
                if (success)
                {
                    // We got new data!
                    // Update the information we need to control the boat 
                    currentLoc.latitude = gps.data.position.latitude;
                    currentLoc.longitude= gps.data.position.longitude;
*/