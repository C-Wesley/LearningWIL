#ifndef UTILITY_H
#define UTILITY_H

    #include <Wire.h>
    #include "config.h"
    void setupI2C()
    {
        // Begin Wire for the GPS and BNO. 
        // The GPS requests that it operate on "FAST MODE I2C" and that standard mode is not supported.
        Wire.begin();
        Wire.setClock(400000);
        // Begin Wire2 for the Atlas Scientific sensors. Be warned, there are problems when putting all the sensors on the same bus.
        Wire2.begin();
    }

    void i2cDetect()
    {
        //I2C SCanner
        Serial.println("Scanning Wire and Wire2:"); 
        for (int addr = 0; addr<=127; addr++)
        {
            Wire.beginTransmission(addr);             // These are separate busses so we can start at the same time.  
            Wire2.beginTransmission(addr);
            int reply = Wire.endTransmission();
            int reply2 = Wire2.endTransmission(addr);

            if (reply == 0)
            {
                Serial.print("ADDR found on Wire at: "); Serial.println(addr, HEX); 
            }
            if (reply2 == 0)
            {
                Serial.print("ADDR found on Wire2 at: "); Serial.println(addr, HEX);
            }
        }
    }

    void setUpRGB_LED() 
    {   
        pinMode(RGB_R, OUTPUT); // Red
        pinMode(RGB_G, OUTPUT); // Green
        pinMode(RGB_B, OUTPUT); // Blue
    }
      
    void i2cErrorBlink()
    {
        digitalWrite(RGB_B, LOW);
        digitalWrite(RGB_R, HIGH);
        digitalWrite(RGB_G, HIGH);
        delay(500);
        digitalWrite(RGB_B, HIGH);
        digitalWrite(RGB_R, HIGH);
        digitalWrite(RGB_G, HIGH);
        delay(500);
    }

    void sdErrorBlink()
    {
        digitalWrite(RGB_B, HIGH);
        digitalWrite(RGB_R, HIGH);
        digitalWrite(RGB_G, LOW);
        delay(500);
        digitalWrite(RGB_B, HIGH);
        digitalWrite(RGB_R, HIGH);
        digitalWrite(RGB_G, HIGH);
        delay(500);
    }

    void printVars(GlobalPositioningData& data, NavigationState& state)
    {
        Serial.print(data.date.month);                    Serial.print("/");
        Serial.print(data.date.day);                      Serial.print("/");
        Serial.print(data.date.year);                     Serial.print(" ");
        Serial.print(data.date.hour);                     Serial.print(":");
        Serial.print(data.date.minute);                   Serial.print(":");
        Serial.print(data.date.second);                   Serial.print(", ");

        // Validity Flags
        Serial.print(data.fixType);                  Serial.print(", ");
        Serial.print(data.gnssFixOK);                Serial.print(", ");
        Serial.print(data.headVehValid);             Serial.print(", ");
        Serial.print(data.invalid_llh);              Serial.print(", ");

        // Navigation Data
        Serial.print(data.position.latitude);         Serial.print(", ");
        Serial.print(data.position.longitude);        Serial.print(", ");
        Serial.print(data.SIV);                       Serial.print(", ");
        Serial.print(data.groundSpeed);               Serial.print(", ");
        Serial.print(data.headingMotion);             Serial.print(", ");
        Serial.print(data.headingVehicle);            Serial.print(", ");
        Serial.print(state.distanceToWaypoint);       Serial.print(", ");
        Serial.print(state.targetHeading);            Serial.print(", ");
        Serial.print(state.currentHeading);           Serial.print(", ");
        Serial.print(state.headingError);             Serial.print(", ");
        Serial.print(state.gpsError);                 Serial.print(", ");
        Serial.print(state.averageGPSError);          Serial.print(", ");
        Serial.print(state.waypointIndex);            Serial.println();
    }
#endif