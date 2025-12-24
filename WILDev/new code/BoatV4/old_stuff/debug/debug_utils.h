#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

    #include "../shared/config.h"
    #include "../shared/types.h"

    void i2cErrorBlink()
    {
        // Blink Blue light for I2C Error 
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
    // Blink the Green Light for SD Error
    digitalWrite(RGB_B, HIGH);
    digitalWrite(RGB_R, HIGH);
    digitalWrite(RGB_G, LOW);
    delay(500);
    digitalWrite(RGB_B, HIGH);
    digitalWrite(RGB_R, HIGH);
    digitalWrite(RGB_G, HIGH);
    delay(500);
  }

  void printVars(const NavData& navData, const NavigationState& navState)
  {
    // This method prints common variables that we care about. 
    // It is useful for debugging. 
    
    // Time Stamp
    Serial.print(navData.month);          Serial.print("/"); 
    Serial.print(navData.day);            Serial.print("/"); 
    Serial.print(navData.year);           Serial.print(" "); 
    Serial.print(navData.hour);           Serial.print(":"); 
    Serial.print(navData.minute);         Serial.print(":"); 
    Serial.print(navData.second);         Serial.print(", ");

    // Validity Flags
    Serial.print(navData.fixType);        Serial.print(", ");
    Serial.print(navData.gnssFixOK);      Serial.print(", ");
    Serial.print(navData.headVehValid);   Serial.print(", ");
    Serial.print(navData.invalid_llh);    Serial.print(", ");

    // Data for Navigation 
    Serial.print(navData.lat);            Serial.print(", ");
    Serial.print(navData.lon);            Serial.print(", ");
    Serial.print(navData.SIV);            Serial.print(", ");
    Serial.print(navData.groundSpeed);    Serial.print(", ");
    Serial.print(navData.headingMotion);  Serial.print(", ");
    Serial.print(navData.headingVehicle); Serial.print(", ");
    Serial.print(navState.distanceBetween);          Serial.print(", ");
    Serial.print(navState.targetHeading);            Serial.print(", ");
    Serial.print(navState.currentHeading);           Serial.print(", ");
    Serial.print(navState.headingError);             Serial.print(", ");
    Serial.print(navState.gpsError);                 Serial.print(", ");
    Serial.print(navState.averageGPSError);          Serial.println();  
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
    // This method turns on the pins for the on board RGB Pins on the arduino GIGA
    pinMode(RGB_R, OUTPUT);
    pinMode(RGB_G, OUTPUT);
    pinMode(RGB_B, OUTPUT); 
  }

  void rgb_off()
  {
    digitalWrite(RGB_R, HIGH); 
    digitalWrite(RGB_G, HIGH);
    digitalWrite(RGB_B, HIGH);
  }




#endif