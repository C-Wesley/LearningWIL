#include <SPI.h>
#include <Wire.h>

const int DOaddress  = 0x61;  // 97
const int ORPaddress = 0x62;  // 98
const int pHaddress  = 0x63;  // 99
const int Caddress   = 0x64;  // 100

// Return code of the sensor
byte code       = 0;

// 
byte charIn     = 0;
char sensorData[20];
int timeDelay   = 900;
float data;

void requestAtlas(int sensorAddress)
{
  Wire.beginTransmission(sensorAddress);
  Wire.write(82);              // (R)ead
  Wire.endTransmission();
} // end of request()

float readResponseAtlas(int sensorAddress)
{
  Wire.requestFrom(sensorAddress, 20, 1);
  code = Wire.read();

  // 1 is success
  // 2 syntax error
  // 254 not ready
  // 255 no data to send

  // Read the data from the sensor
  for (int ii = 0; ii < 20; ii++)
  {
    charIn = Wire.read();
    sensorData[ii] = charIn;
  }
  Wire.endTransmission();
  // Parse the data
  data = atof(sensorData);

  // Reset Variables
  charIn = 0;
  for (int jj = 0; jj < 20; jj++)
  {
    sensorData[jj] = 0;
  }
  return data;
} //end of readResponseAtlas()

void setup()
{
  Serial.begin(9600);
  while (!Serial) {}
  Wire.begin();

  // I2C Scanner
  for (int address = 0; address < 128; address++)
  {
    Wire.beginTransmission(address); // Sends out the initial address with the write command

    if (Wire.endTransmission() == 0) // Theis means we got an ACK back
    {
      Serial.print("Ox"); Serial.println(address, HEX);
    }
    delay(5);
  }
}

void loop()
{
  requestAtlas(DOaddress);
  requestAtlas(ORPaddress);
  requestAtlas(pHaddress); 
  requestAtlas(Caddress);
  
  delay(timeDelay);
   
  float DO  = readResponseAtlas(DOaddress);
  float ORP = readResponseAtlas(ORPaddress);
  float pH  = readResponseAtlas(pHaddress);
  float C   = readResponseAtlas(Caddress);
  
  Serial.print(DO);  Serial.print(" ");
  Serial.print(ORP); Serial.print(" "); 
  Serial.print(pH);  Serial.print(" "); 
  Serial.print(C);   Serial.println();
}
