#include <OneWire.h>
OneWire ds(10);       // Create an object using pin 10

byte data[9];         // Array for 9 bytes of data we will read
byte addr[8];         // Array to hold the 64 bits of the address
float celsius;        // Our final answer in degrees C

void setup() 
{
  Serial.begin(9600); // So we can see the answer! 
  ds.search(addr);    // Search for our device // Returns a 1  
  ds.reset();         // Resets the bus??
}

void loop() 
{
  // Configure
  ds.select(addr);    // Select our devices using its addr 
  ds.write(0x4E);     // Write our configuration to the scratch pad 
  ds.write(0x00);     // This is the TH alarm - We ain't got one
  ds.write(0x00);     // This is the TL alarm - We ain't got that neither 
  ds.write(0x3F);     // Sets to 10-bit resolution; see page 9 of the data sheet!
  ds.write(0x48);     // Copies the contents of the "scratchpad" over to the EEPROM
  ds.reset();         // Reset!

  // Measure and read
  ds.select(addr);    // Select our device (again!) 
  ds.write(0x44);     // This initiates a temperature conversion!
  delay(200);         // Let it finish conversion (187.5 ms)
  ds.reset();
  ds.select(addr);    // Select it (yes again!);
  ds.write(0xBE);     // Read the scratchpad 

  for(int ii=0; ii<9; ii++)
  {
    data[ii] = ds.read(); // Read the bytes 
  }

  // Making Humpty Dumpty. data[1] is most significant byte so shift it over 8
  int16_t raw = (data[1]<<8) | (data[0]); 
  // Clear bits 1 and 0 since we are using 10-bit resolution 
  raw &= ~0x03;    
  // Convert to temperature in Celsius                       
  celsius = raw/16.0;                     

  Serial.print("Temperature is: ");
  Serial.print(celsius); 
  Serial.println(" C");
}
