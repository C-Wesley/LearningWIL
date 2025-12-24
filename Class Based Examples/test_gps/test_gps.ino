#include "config.h"
#include "types.h"
#include "gps.h"

GlobalPositioning mygps(GPS_I2C_ADD); 


void setup()
{
    Serial.begin(9600);
    while(!Serial) {}  // Wait for serial to connect
    Wire.begin();
    
    if (!mygps.begin())
    {
        Serial.println("GPS Failed to init");
        while(1){}
    }
    Serial.println("GPS success!");
}

void loop()
{
    int success = mygps.getData();

    if (success)
    {
        Serial.print(mygps.data.date.month); Serial.print('/'); Serial.print(mygps.data.date.day); Serial.print('/'); Serial.println(mygps.data.date.year); 
        Serial.print(mygps.data.position.longitude); Serial.print(", "); Serial.println(mygps.data.position.latitude);
        Serial.print("fixType        :"); Serial.println(mygps.data.fixType);
        Serial.print("gnssFixOkay    :"); Serial.println(mygps.data.gnssFixOK);
        Serial.print("headVehValid   :"); Serial.println(mygps.data.headVehValid);
        Serial.print("invalid_llh    :"); Serial.println(mygps.data.invalid_llh);
        Serial.print("GroundSpeed    :"); Serial.println(mygps.data.groundSpeed);
        Serial.print("headingMotion  :"); Serial.println(mygps.data.headingMotion);
        Serial.print("headingVehicle :"); Serial.println(mygps.data.headingVehicle); 
        Serial.print("SIV            :"); Serial.println(mygps.data.SIV);
        Serial.println("==================="); 
    }
}