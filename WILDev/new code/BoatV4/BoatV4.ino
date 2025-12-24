#include "boat.h"
#include "utility.h"

Boat boat;

unsigned long pollControllerTimer; 

void setup()
{
    Serial.begin(DEBUG_SERIAL_BAUD); 
    setupI2C();
    // i2cDetect();

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
