#include <Wire.h>
#define BNOAddress 0x28 // Found using I2C scanner
#define OPR_MODE 0x3D
#define UNIT_SEL 0x3B

void setup() 
{
  Serial.begin(115200);
  Wire.begin(); 

  // I2C Scanner -> 0x28 found
  for(int add=1; add<=127; add++)
  {
    Wire.beginTransmission(add); 
    int reply = Wire.endTransmission(); 
    if (reply == 0)
    {
      Serial.print("0x");
      Serial.print(add, HEX); 
    }
  }

  // Configure the BNO to IMU mode
  // OPR_MODE is 0x3D
  // IMU setting : xxxx1000b
  Wire.beginTransmission(BNOAddress); 
  Wire.write(OPR_MODE);
  Wire.write(1<<3);
  Wire.endTransmission();
  
  // Unit Select Register
  // Set Degrees xxxxx0xxb
  Wire.beginTransmission(BNOAddress);
  Wire.write(UNIT_SEL);
  Wire.write(0<<2); 
  Wire.endTransmission();
}

void loop() 
{  
  // Heading Regisgers 1A LSB , 1B MSB
  Wire.beginTransmission(BNOAddress);
  Wire.write(0x1A);
  Wire.endTransmission();
  Wire.requestFrom(BNOAddress, 2); // Go Get 1A, 1B

  uint8_t LSB = Wire.read();
  uint8_t MSB = Wire.read();

  // Combine the data and divide by 16
  Serial.println((float)(((int)MSB<<8 | LSB))/16.0);  
  delay(100);
}
