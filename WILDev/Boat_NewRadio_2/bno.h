#ifndef HEADING_H
#define HEADING_H

  // For the BNO
  #define BNOAddress 0x28 // Found using I2C scanner or datasheet
  #define OPR_MODE 0x3D
  #define UNIT_SEL 0x3B 
  
  #include "shared.h "
  #include <Wire.h>

  void setupBNO2()
  {
    Wire.beginTransmission(BNOAddress);
    byte reply = Wire.endTransmission(); 
    
    if (reply == 0)
    {
      // Configure the BNO to IMU mode
      // OPR_MODE is 0x3D
      // IMU setting : xxxx1000b
      Wire.beginTransmission(BNOAddress); 
      Wire.write(OPR_MODE);
      Wire.write(1<<3);
      byte reply1 = Wire.endTransmission();
    
      // Unit Select Register
      // Set Degrees xxxxx0xxb
      Wire.beginTransmission(BNOAddress);
      Wire.write(UNIT_SEL);
      Wire.write(0<<2); 
      byte reply2 = Wire.endTransmission();

      if (!(reply1 == 0 && reply2 == 0))
      {
        Serial.println("Error on the BNO.");
        digitalWrite(LED_BUILTIN, LOW);
        while(1){}
      }
    }
    else
    {
      Serial.println("Error on the BNO.");
      digitalWrite(LED_BUILTIN, LOW);
      while(1){}
    }
  }

  float getHeading2()
  {
    // Heading Regisgers 1A LSB , 1B MSB
    Wire.beginTransmission(BNOAddress);
    Wire.write(0x1A);
    Wire.endTransmission();
    Wire.requestFrom(BNOAddress, 2); // Go Get 1A, 1B
  
    uint8_t LSB = Wire.read();
    uint8_t MSB = Wire.read();
  
    // Combine the data and divide by 16
    return (float)(((int)MSB<<8 | LSB))/16.0; 
  }

#endif 
