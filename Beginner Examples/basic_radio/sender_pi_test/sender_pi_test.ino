float pi = 3.14;

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(57600);
}

void loop() 
{

  
  byte * b = (byte *) &pi; 
  Serial1.write(0x65);
  Serial1.write(b, sizeof(pi));
  delay(10000);
}