#include <Wire.h>
#include <SD.h>

unsigned long startTime;
File outputfile;  

typedef struct 
{
  uint16_t year; 
  uint8_t month; 
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t fixType;
  bool gnssFixOK; 
  bool headVehValid; 
  long lat;
  long lon;
  uint8_t SIV;
  long groundSpeed; 
  long headingMotion;
  long headingVehicle;
  bool invalid_llh;
} NavData; 

NavData readNavPVT()
{
  while(Wire.available() > 0)
  {
    Wire.read();
  }
  // This method will ask the GPS for Position, Velocity, Time data..

  // This array is all the data before the "payload" data
  // u, B, class, id, length (LSB), length (MSB);
  uint8_t header[]= 
  {
    0xB5, 0x62, 0x01, 0x07, 0x00, 0x00  
  };

  // Start the checksum at 0
  uint8_t CK_A = 0;
  uint8_t CK_B = 0; 
  // Array for the payload to go into
  uint8_t payload[92]; 

  // Part of the 8-bit fletcher algorithm. 
  // The class and ID must be counted as with the payload
  for (int i=2; i<6; i++)
  {
    CK_A = CK_A + header[i];
    CK_B = CK_B + CK_A; 
  }

  // This is an empty PVT packet
  // Per the data sheet: 
  // "messages can be polled by sending the message 
  // required to the receiver but without a payload"
  Wire.beginTransmission(0x42);
  Wire.write(0xB5); // Header 
  Wire.write(0x62);
  Wire.write(0x01); // Class
  Wire.write(0x07); // ID
  Wire.write(0x00); // Length
  Wire.write(0x00);
  Wire.write(CK_A); // Checksum
  Wire.write(CK_B);
  Wire.endTransmission();

  // Now we expect the GPS to send out a similar
  // packet with it's own header. Everything should be the same 
  // except for the length. The length should be 92 -> 0x5C.

  // Let's request the data. 
  // 6 bytes for the header
  // 92 bytes for the payload 
  // 2 bytes for the checksums
  Wire.requestFrom(0x42, 100);
  for (int i=0; i<6; i++)
  {
    header[i] = Wire.read(); 
  }
  for (int i=0; i<92; i++)
  {
    payload[i] = Wire.read();
  }
  uint8_t checksumA = Wire.read(); 
  uint8_t checksumB = Wire.read(); 

  // Checking for -> 0xB562, 0x0107, and 0x00C5
  uint16_t head_ = header[0]<<8 | header[1];         //Header
  uint16_t class_id = header[2]<<8 | header[3];      // Class/ID
  uint16_t payLoadLength = header[5] <<8 | header[4];// Length

  // Here, we check if the data is what we expect:
  // head_         -> 0xB562
  // class_id      -> 0x0107
  // payLoadLength -> 0x005C
  // If so, we can proceed to read the payload 
  if (head_ == 0xB562 && class_id == 0x0107 && payLoadLength == 0x005C)
  {
    CK_A = 0;
    CK_B = 0;
    // Get the first part of the checksum using the header we read in. 
    for (int i=2; i<6; i++)
    {
      CK_A = CK_A + header[i];
      CK_B = CK_A + CK_B;
    }
    for (int i=0; i<92; i++)
    {
      CK_A = CK_A + payload[i];
      CK_B = CK_A + CK_B;
    }
    if (checksumA == CK_A && checksumB == CK_B)
    {
      NavData data; 
      data.year   = payload[5]<<8 | payload[4];
      data.month  = payload[6]; 
      data.day    = payload[7]; 
      data.hour   = payload[8];
      data.minute = payload[9];
      data.second = payload[10];

      data.fixType      = payload[20];
      data.gnssFixOK    = payload[21] & 0x01; // Grab  the first bit from the "flags" byte. 
      data.headVehValid = (payload[21] & (1<<5))>>5; // Grab the bit in the 2^5 position from the "flags" byte.
      
      data.lat = payload[31] <<24 | payload[30] << 16 | payload[29]<<8 | payload[28]; 
      data.lon = payload[27] <<24 | payload[26] << 16 | payload[25]<<8 | payload[24];
      data.SIV = payload[23];
      
      data.groundSpeed    = payload[63] << 24 | payload[62] << 16 | payload[61] << 8 | payload[60]; 
      data.headingMotion  = payload[67] << 24 | payload[66] << 16 | payload[65] << 8 | payload[64];
      data.headingVehicle = payload[87] << 24 | payload[86] << 16 | payload[85] << 8 | payload[84];
      data.invalid_llh    = payload[78] & 0x01; 
      return data; 
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  while(!Serial)
  {
    
  }
  Wire.begin();

  // Set the reading register to the data register. 
  Wire.beginTransmission(0x42);
  Wire.write(0xFF);
  Wire.endTransmission();
  delay(100); 

  pinMode(48, OUTPUT); 
  if(!SD.begin(48))
  {
    Serial.println("Failed!");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
  }

  startTime = millis();
  outputfile = SD.open("data.txt", FILE_WRITE); 
  outputfile.print("Lat, Lon, SIV, groundSpeed, Heading");
  outputfile.close();
}

void loop() 
{
  if ((millis() - startTime) > 1000)
  {
    NavData data = readNavPVT();
    startTime = millis();

    Serial.print(data.month);  Serial.print("/"); 
    Serial.print(data.day);    Serial.print("/"); 
    Serial.print(data.year);   Serial.print(" "); 
    Serial.print(data.hour);   Serial.print(":"); 
    Serial.print(data.minute); Serial.print(":"); 
    Serial.print(data.second); Serial.print(" ");

    // Validity Flags
    Serial.print(data.fixType);      Serial.print(", ");
    Serial.print(data.gnssFixOK);    Serial.print(", ");
    Serial.print(data.headVehValid); Serial.print(", ");
    Serial.print(data.invalid_llh);  Serial.print(", ");

    // Data for Navigation 
    Serial.print(data.lat);            Serial.print(", ");
    Serial.print(data.lon);            Serial.print(", ");
    Serial.print(data.SIV);            Serial.print(", ");
    Serial.print(data.groundSpeed);    Serial.print(", ");
    Serial.print(data.headingMotion);  Serial.print(", ");
    Serial.println(data.headingVehicle);
    
    // If you have the sd card connected: 
    //    
    //    outputfile = SD.open("data.txt", FILE_WRITE);
    //
    //    // Time Stamp
    //    outputfile.print(data.month);  outputfile.print("/"); 
    //    outputfile.print(data.day);    outputfile.print("/"); 
    //    outputfile.print(data.year);   outputfile.print(" "); 
    //    outputfile.print(data.hour);   outputfile.print(":"); 
    //    outputfile.print(data.minute); outputfile.print(":"); 
    //    outputfile.print(data.second); outputfile.print(" ");
    //
    //    // Validity Flags
    //    outputfile.print(data.fixType);      outputfile.print(", ");
    //    outputfile.print(data.gnssFixOK);    outputfile.print(", ");
    //    outputfile.print(data.headVehValid); outputfile.print(", ");
    //    outputfile.print(data.invalid_llh);  outputfile.print(", ");
    //
    //    // Data for Navigation 
    //    outputfile.print(data.lat);            outputfile.print(", ");
    //    outputfile.print(data.lon);            outputfile.print(", ");
    //    outputfile.print(data.SIV);            outputfile.print(", ");
    //    outputfile.print(data.groundSpeed);    outputfile.print(", ");
    //    outputfile.print(data.headingMotion);  outputfile.print(", ");
    //    outputfile.println(data.headingVehicle); 
    //    
    //    outputfile.close();
  }
}
