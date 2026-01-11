typedef struct {
  float pi;
  int budget;
  uint8_t mode;
} SensorData;

SensorData myRecData;
byte b[sizeof(SensorData)];

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(57600); 
}

void loop() 
{
  if (Serial1.available() >= sizeof(myRecData))
  {
    char header = Serial1.read();
    if (header == (0x65))
    {
      for(int i=0; i<sizeof(myRecData); i++)
      {
        b[i] = Serial1.read(); 
      }
      memcpy(&myRecData, b, sizeof(myRecData));
      
      Serial.print("Pi:     ");     Serial.println(myRecData.pi);
      Serial.print("Budget: "); Serial.println(myRecData.budget);
      Serial.print("Mode:   ");   Serial.println(myRecData.mode);
    }

  }
}

