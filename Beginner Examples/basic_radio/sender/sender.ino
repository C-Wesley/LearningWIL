typedef struct {
  float pi;
  int budget;
  uint8_t mode;
} SensorData;

SensorData myExampleData; 

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(57600);

  myExampleData.pi     = 3.14;
  myExampleData.budget = 42069;
  myExampleData.mode   = 0x05;
}

void loop() 
{
  byte * b = (byte *) &myExampleData; 
  Serial1.write(0x65);
  Serial1.write(b, sizeof(myExampleData));
  delay(10000);
}

