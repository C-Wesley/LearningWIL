#include "boat.h"
#include "utility.h"

Boat boat;

unsigned long pollControllerTimer;  

void setup()
{
    setupI2C();
    
    if (DEBUG_PRINT_SERIAL)
    {
      Serial.begin(DEBUG_SERIAL_BAUD); 
      Serial.println("Begain Serial Print for debugging.");
      Serial.println("==================================");
      Serial.println("I2C Scan:");
      i2cDetect();
      Serial.println("=================================="); 
    }
     
    boat.begin();

    pollControllerTimer = millis();
}

void loop()
{
    if( (millis() - pollControllerTimer) > CONTROLLER_POLLING_MS)
    {
        boat.remote.read();
        pollControllerTimer = millis();
    }

    switch (boat.remote.mode)
    {
        case REMOTECONTROL:
            boat.handleRemote();
            break;
        case AUTONOMOUS:
            boat.handleAuto(); 
            break;
        case TUGBOAT:
            boat.handleTugboat();
            break;
        default: 
            break;
    }
}
