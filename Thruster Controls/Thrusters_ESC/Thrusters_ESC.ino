#include <Servo.h>

int LeftMotorPin = 10; 
Servo LeftMotor; 

void setup() 
{
  Serial.begin(9600);
  
  LeftMotor.attach(LeftMotorPin); // Connect to the Servo. 
  
  // Delay to Wait for ESC to initialize 
  LeftMotor.writeMicroseconds(1500); 
  delay(10000); 
}

void loop() 
{
  // Read a speed in from the serial monitor
  while(Serial.available() > 3)
  {
    int motorSpeed = Serial.parseInt();
    Serial.print("Setting the motor speed to: "); Serial.println(motorSpeed);
    LeftMotor.writeMicroseconds(motorSpeed);
  }
}
