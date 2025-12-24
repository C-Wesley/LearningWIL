#include "config.h"
#include "types.h"
#include "sensors.h"

WaterMonitor myMonitor(ATLAS_DO_ADD, 
                      ATLAS_ORP_ADD, 
                       ATLAS_PH_ADD, 
                        ATLAS_C_ADD, 
                        ONE_WIRE_BUS);

EcoData newData;

void setup()
{
    Wire2.begin();
    Serial.begin(9600);
    while(!Serial) {}
    
    int succ = myMonitor.begin();
    if (succ)
    {
        Serial.println("Water Monitor began!");
    }
    else
    {
        Serial.println("Water Monitor failed!");
        while(1) {}
    }
}
void loop()
{
    newData = myMonitor.getData();

    Serial.print("Conductivity :"); Serial.println(newData.conductivity);
    Serial.print("DO           :"); Serial.println(newData.DO);
    Serial.print("pH           :"); Serial.println(newData.pH);
    Serial.print("ORP          :"); Serial.println(newData.ORP);
    Serial.print("Water Temp   :"); Serial.println(newData.waterTemp);
}