// Here are all the files we need to look at to write this sketch.
// Each file defines functions that are related to their own name.
#include "average.h"
#include "gps.h    "
#include "bno.h"
#include "motor.h  "
#include "path.h   "
#include "sd.h     "
#include "shared.h "
#include "headingControl.h" 
#include "controller.h"
// #include "eco.h"

long ecoPoll;

void printVars()
{

  // Time Stamp
  Serial.print(myGPSData.month);          Serial.print("/"); 
  Serial.print(myGPSData.day);            Serial.print("/"); 
  Serial.print(myGPSData.year);           Serial.print(" "); 
  Serial.print(myGPSData.hour);           Serial.print(":"); 
  Serial.print(myGPSData.minute);         Serial.print(":"); 
  Serial.print(myGPSData.second);         Serial.print(", ");

  // Validity Flags
  Serial.print(myGPSData.fixType);        Serial.print(", ");
  Serial.print(myGPSData.gnssFixOK);      Serial.print(", ");
  Serial.print(myGPSData.headVehValid);   Serial.print(", ");
  Serial.print(myGPSData.invalid_llh);    Serial.print(", ");

  // Data for Navigation 
  Serial.print(myGPSData.lat);            Serial.print(", ");
  Serial.print(myGPSData.lon);            Serial.print(", ");
  Serial.print(myGPSData.SIV);            Serial.print(", ");
  Serial.print(myGPSData.groundSpeed);    Serial.print(", ");
  Serial.print(myGPSData.headingMotion);  Serial.print(", ");
  Serial.print(myGPSData.headingVehicle); Serial.print(", ");
  Serial.print(distanceBetween);          Serial.print(", ");
  Serial.print(targetHeading);            Serial.print(", ");
  Serial.print(currentHeading);           Serial.print(", ");
  Serial.print(headingError);             Serial.print(", ");
  Serial.print(gpsError);                 Serial.print(", ");
  Serial.print(averageGPSError);          Serial.println();  
}

void setup() 
{
  ecoPoll = millis(); 
  // Any Serial.print() statments were used to debug the program at one point in time.
  // I've left them in the program incase they are useful later on. 

  Serial.begin(9600);
  delay(1000);
  Serial.println("#GetObsessed!"); 

  // Begin Wire for the I2C sensors. 
  Wire.begin();

  // The following methods begin their respective sensors.

  // Sparkfun GNSS SMA Module over SMA. 
  setupGPS(); 
  Serial.println("GPS Done");

  // Adafruit BNO08x Inertial Measurement Unit. 
  setupBNO2();
  Serial.println("BNO Done.");

  // BlueRobotics Electronic Speed Controllers. 
  // When in the water, you should hear a music tone, and then a final longer beep.
  // The final longer beep indicates that the motors are synced up and ready to go.
  setupESC(); 
  Serial.println("ESC's Done"); 

  // SD Card module. The one we are using requires 5v vin, but communicates over 3.3v SPI.
  setupSD(); 
  Serial.println("SD Done"); 

  parsePath();
  for (int ii=0; ii<numWayPoints; ii++)
  {
    Serial.print(dLats[ii]);
    Serial.print(" "); 
    Serial.println(dLons[ii]);
  }
  Serial.println("Path parse finished!");

  for (int ii=0; ii<numWayPoints; ii++)
  {
    Serial.print(dLons[ii]); Serial.print(" "); Serial.println(dLats[ii]);
  }
  
  // Turn off onboard the LED
  // The LED is on when LOW. 
  digitalWrite(LED_BUILTIN, HIGH); 

  // This begins the UART to the 1310 that is onboard. 
  // The 1310 is there to merely provide LoRa Communications with the remote control.
  // beginController();
  // Serial.println("Began Controller!");

  // This begins the UART to the Serial Radio
  Serial2.begin(57600);
  Serial.println("Began Controller!");
  
  // Timing Variables
  startTime = millis();        // Timing for GPS heading error update
  pollingStartTime = millis(); // Timing for Polling the GPS module 
}

void loop()
{
  // If we haven't read the controller for 100 ms.
  // Read the controller and remember that we read it. 
  if (millis() - controllerPolling > 100)
  {
    readController(); 
    controllerPolling = millis();
  }
  
  // If we are in the autonomous program mode, do the autonomous operation
  if (programMode == AUTONOMOUS)
  {
    
    // Set the "destination" to point 2 in the GPS pair.
    myGPSPair.lat2_L = dLats[waypointIndex]; 
    myGPSPair.lon2_L = dLons[waypointIndex];
//    Serial.println("Here");
//    Serial.println(dLats[waypointIndex]);
//    Serial.println(dLons[waypointIndex]);
    
    // Update Our GPS data every second. (1Hz rate of GPS).
    if (millis() - pollingStartTime > 1000)
    {

      // Go get the data from the GPS. 
      myGPSData = readNavPVT(); 
      pollingStartTime = millis(); 
      
      // Set the "current" position to point 1 in the GPS pair. 
      myGPSPair.lat1_L = myGPSData.lat;
      myGPSPair.lon1_L = myGPSData.lon; 
    
      // Get the information we need to control the robot
      distanceBetween = getDistanceBetween(myGPSPair);                          // Distance from current to target waypoint
      targetHeading   = getTargetHeading(myGPSPair);                            // Target heading to go from current waypoint to target waypoint
      currentHeading  = getHeading2() - averageGPSError;                        // The heading our IMU reports + an average correction value from the heading of motion that the GPS sees.
      currentHeading  = fixHeading(currentHeading);                             // Make sure the current heading stays inside 0 to 360.
      headingError    = getHeadingError(currentHeading, targetHeading);         // Update the error in our heading. 
      gpsError        = currentHeading - (myGPSData.headingMotion / 100000.0);  // Update the error from our GPS heading
    
      // Log the data to the SD card.
      logToSd();

      // Used for debugging. Must have Serial.begin() in the setup if you uncomment this. 
      printVars();
      
      // If the GPS has a good fix and we are still in autonomous mode.
      if (myGPSData.gnssFixOK and programMode != REMOTECONTROL)
      {
        
        // Start the gps heading error timer
        if(GotFirstStartTime == 0)
        {
          startTime         = millis();
          GotFirstStartTime = 1;
        }
        
        // Keep track of the gps heading error
        updateAverage(gpsError);
    
        // If our heading is too far off from the acceptable error
        //     Stop moving and go to the subroutine to fix the heading of the boat.
        if (abs(headingError) > ACC_HEAD_ERR)
        {
          updateMotors(1500, 1500); // Stop to think
          rotateInPlaceBoat();      // Fix Heading
        }
    
        // If we are within 2.0 m of our destination point. 
        if(distanceBetween <= 2.0) 
        {
          // Turn Motors Off and go to the next waypoint
          updateMotors(1500, 1500); 
          waypointIndex++; 
    
          // Get the average for this leg and use it for the next one. Reset the average after. 
          averageGPSError = getAverage() + averageGPSError;
          resetAverage();
          startTime = millis();
    
          if (waypointIndex >= numWayPoints)
          {
            // Done with path. Freeze. 
            waypointIndex     = 0;
            programMode       = REMOTECONTROL;
            GotFirstStartTime = 0;
            averageGPSError   = 0;
            resetAverage();
          }
        }
        else if ((millis() - startTime) > 10000) // If we haven't updated GPS average for 10 seconds. 
        {
          averageGPSError = getAverage() + averageGPSError;  
          resetAverage(); 
          startTime = millis();
        }
        else
        {
          // Our heading is not too far off and we are not close enough to the point 
          // Go Forwards
          updateMotors(1650, 1650);
        }
      }
      else
      {
        // 6 Sats are not in view. Standby...
        updateMotors(1500, 1500);
      }
    }
    else
    {
      // Handle other autonomous stuff... ;
    }
  }
  else if (programMode == REMOTECONTROL)
  {
    // We already read the controller eariler
    updateMotors(leftMotorSpeedRec, rightMotorSpeedRec); 
  }
}
