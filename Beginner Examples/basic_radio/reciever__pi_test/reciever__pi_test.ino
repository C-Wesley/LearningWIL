void setup() 
{
  Serial.begin(9600);
  Serial1.begin(57600); 
}
float pi;
byte b[4];  
void loop() 
{
  if (Serial1.available() >= 5)
  {
    char data = Serial1.read();
    if (data == (0x65))
    {
      for(int i=0; i<4; i++)
      {
        b[i] = Serial1.read(); 
      }
      memcpy(&pi, b, sizeof(float));
      Serial.print("Pi is: "); Serial.println(pi);
    }

  }
}