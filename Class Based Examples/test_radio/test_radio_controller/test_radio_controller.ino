#include "types.h"
#include "config.h"

void testRemoteMode()
{
    Serial.println("----------------------");
    Serial.println("Testing Remote mode...");
    Serial1.print('M');
    Serial1.print('1');
    Serial1.print('E');
    
    Serial.println("Sent Remote Control Packet...");
    delay(2000); 

    int lSpeed = 1650;
    int rSpeed = 1760;
    Serial1.print('S');
    Serial1.write((byte*) &lSpeed, sizeof(int));
    Serial1.write((byte*) &rSpeed, sizeof(int));
    Serial1.print('E');
    Serial.println("Sent Speed...");
    delay(2000);

    lSpeed = 1420;
    rSpeed = 1333;
    Serial1.print('S');
    Serial1.write((byte*) &lSpeed, sizeof(int));
    Serial1.write((byte*) &rSpeed, sizeof(int));
    Serial1.print('E');
    Serial.println("Sent Different Speed...");
    delay(2000); 

    Serial1.print('R');
    Serial1.print('E'); 
    Serial.println("Sent request for ECO data...");
    
    // Time how long it takes to get all bytes of the response.
    // This will fail if the boat doesn't respond with
    // the right about of data.
    unsigned long start = millis(); 
    while(Serial1.available() < sizeof(EcoData)+2) {}
    unsigned long elapsedTime = millis() - start;

    Serial.println("Attempting to parse EcoData...");
    if (Serial1.available() > 0)
    {
        char start = Serial1.read();
        if (start == 'R')
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
                Serial.println("EcoData received properly!");
                Serial.print("The time took to get all bytes was: "); Serial.println(elapsedTime);
                EcoData recData;
                memcpy(&recData, buffer, sizeof(EcoData));

                Serial.println();
                Serial.print("Conductivity :"); Serial.println(recData.conductivity);
                Serial.print("DO           :"); Serial.println(recData.DO);
                Serial.print("pH           :"); Serial.println(recData.pH);
                Serial.print("ORP          :"); Serial.println(recData.ORP);
                Serial.print("Water Temp   :"); Serial.println(recData.waterTemp);
            } 
        }
    }
    Serial.println("----------------------");
}

void testAutonomousMode()
{
    Serial.println("--------------------------");
    Serial.println("Testing Autonomous mode...");

    Serial1.print('M');
    Serial1.print('0');
    Serial1.print('E');
    Serial.println("Sent Autonomous Packet");
    // The autonomous mode will send back a NavigationState message. 
    
    // Time how long it takes assuming the boat handles it instantly. 
    unsigned long start = millis(); 
    while(Serial1.available() < sizeof(NavigationState)+2) {}
    unsigned long elapsedTime = millis() - start;

    Serial.println("Attempting to parse NavigationState...");
    if (Serial1.available() > 0)
    {
        char start = Serial1.read();

        if (start == 'C')
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
                Serial.println("NavigationState received properly!");
                Serial.print("The time took to get all bytes was: "); Serial.println(elapsedTime);
                NavigationState recState;
                memcpy(&recState, buffer, sizeof(NavigationState));
                Serial.print("Distance to Waypoint :"); Serial.println(recState.distanceToWaypoint);
                Serial.print("Target Heading       :"); Serial.println(recState.targetHeading);
                Serial.print("Current Heading      :"); Serial.println(recState.currentHeading);
                Serial.print("Heading Error        :"); Serial.println(recState.headingError);
                Serial.print("GPS Error            :"); Serial.println(recState.gpsError);
                Serial.print("Average GPS Error    :"); Serial.println(recState.averageGPSError);
                Serial.print("Waypoint Index       :"); Serial.println(recState.waypointIndex);
                Serial.print("Number of Waypoints  :"); Serial.println(recState.numWaypoints);
            }
        }
    }
    Serial.println("--------------------------");
}

void testTugboatMode()
{
    Serial.println("-----------------");
    Serial.println("Tesitng Tug Boat!");
    Serial1.print('M');
    Serial1.print('2');
    Serial1.print('E');
    delay(1000);
    Serial.println("-----------------");
}

void setup()
{
    Serial.begin(9600);
    while(!Serial);
    Serial1.begin(SERIAL_RADIO_BAUD);

    testRemoteMode();
    Serial.println();
    testAutonomousMode();
    Serial.println();
    testTugboatMode();
    Serial.println();

    // Looking at any unread data is useful when the comms are failing. 
    // We can look at if there is a misalignment 
    // issue or forgetting to read characters etc.
    Serial.println("Junk left over...");
    while(Serial1.available() > 0)
    {
        Serial.print((char) Serial1.read());
    }
    Serial.println();
}

void loop()
{
}