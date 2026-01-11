#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 13       //whatever pin data is connected to
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() 
{
  Serial.begin(9600);
  Serial.println("Temp Demo");

  sensors.begin();
}

void loop() 
{ 
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0); 

  if (tempC!=DEVICE_DISCONNECTED_C)
  {
    Serial.print("Temp for device 1 (index 0) is : ");
    Serial.println(tempC); 
  }
  else
  {
    Serial.println("Error: Could not Read tempData"); 
  }
}
