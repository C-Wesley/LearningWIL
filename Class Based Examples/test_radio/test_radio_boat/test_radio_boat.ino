#include "types.h"
#include "config.h"
#include "radio.h"

// This file simulates what the boat would be expected to handle. 
// It looks for the program mode from the remote controller and responds accordingly

RadioControl myRadio(MAX_WAIT_MS);
void setup()
{
    Serial.begin(9600);
    while(!Serial);
    myRadio.begin(SERIAL_RADIO_BAUD);
    myRadio.mode = REMOTECONTROL;
}

void handleRemote()
{
    Serial.print("LS, RS: "); 
    Serial.print(myRadio.leftMotorRec); 
    Serial.print(" ");
    Serial.println(myRadio.rightMotorRec);

    if (myRadio.takeRemoteReading)
    {
        EcoData fakeEco; 
        fakeEco.conductivity = 200; 
        fakeEco.DO = 16;
        fakeEco.pH = 8.21; 
        fakeEco.ORP = 1200;
        fakeEco.waterTemp = 30.25;

        myRadio.sendEcoData(fakeEco);
        Serial.println("Sent EcoData...");
    }
}

void handleAutonomous()
{
    NavigationState fakeState;
    fakeState.distanceToWaypoint = 10;
    fakeState.targetHeading      = 20;
    fakeState.currentHeading     = 30;
    fakeState.headingError       = 0.01;
    fakeState.gpsError           = 0.05;
    fakeState.averageGPSError    = -0.25;
    fakeState.waypointIndex      = 10; 
    fakeState.numWaypoints       = 24;
    myRadio.sendAutoData(fakeState);
    Serial.println("Sent Navigation State...");
}
void handleTugboat()
{
    Serial.println("TugBoat Mode!");
}

void loop()
{
    while (Serial2.available() > 0) // If the radio is trying to speak to us
    {
        myRadio.read();          // Read it 

        switch (myRadio.mode)    // Do the appropriate response 
        {
            case REMOTECONTROL:
                Serial.println("Handling Remote Mode");
                handleRemote();
                break;
            case AUTONOMOUS: 
                Serial.println("Handling Autonomous Mode");
                handleAutonomous();
                break;
            case TUGBOAT:
                Serial.println("Handling Tugboat Mode");
                handleTugboat();
                break;
            default:
                break; 
        }
    }

}