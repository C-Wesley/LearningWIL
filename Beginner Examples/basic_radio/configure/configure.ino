void setup() 
{
  Serial.begin(9600);
  Serial1.begin(57600);
}

void loop() 
{

  if (Serial.available() > 0)
  {
    char data = Serial.read();
    Serial1.print(data); 
  }

  if (Serial1.available() > 0)
  {
    char data = Serial1.read();
    Serial.print(data);
  }
}
