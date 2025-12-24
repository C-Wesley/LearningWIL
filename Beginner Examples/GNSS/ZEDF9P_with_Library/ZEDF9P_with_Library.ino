#include <Wire.h>                       // Needed for I2C comms
#include <SparkFun_u-blox_GNSS_v3.h>    // Inlcude the ZED-F9P library

SFE_UBLOX_GNSS myGNSS;                  // Create the GNSS object 

void setup() 
{
  Serial.begin(9600);                   // For the Serial Monitor 
  Serial.println("Stand by to getSome!"); 

  Wire.begin();                         // Make sure I2C is started

  while(myGNSS.begin() == false)
  {
    Serial.print("Something went wrong... Retrying");
    delay(1000);                        // Retry in a second 
  }
}

void loop() 
{
  if(myGNSS.getPVT() == true) // If there is new data for us to read
  {

    // Read the data we care about 
    long lat = myGNSS.getLatitude();   // Coordinates in Decimal Degrees * 10^7. (so it stays as a long data type instead of float).
    long lon = myGNSS.getLongitude(); 

    long hour   = myGNSS.getHour();    // Time Data 
    long minute = myGNSS.getMinute();  
    long second = myGNSS.getSecond();

    long day    = myGNSS.getDay();     // Date Data
    long month  = myGNSS.getMonth();
    long year   = myGNSS.getYear(); 

    long siv    = myGNSS.getSIV(); 

    // Print the data 
    Serial.print("Latitude: ");  Serial.println(lat); 
    Serial.print("Longitude: "); Serial.println(lon);
    Serial.print("SIV: ");       Serial.println(siv);

    // This doesn't handle adding leading zeros so a time might show up as 3:3:3 instead of 03:03:03
    Serial.print("Time: ");      Serial.print(hour); Serial.print(":"); 
    Serial.print(minute);        Serial.print(":");  Serial.println(second);

    // The same issue applies here. The leading zeros are not handled
    Serial.print("Date: "); Serial.print(month); Serial.print("/"); Serial.print(day); Serial.println(year);
    

    delay(1000);
  }
}
