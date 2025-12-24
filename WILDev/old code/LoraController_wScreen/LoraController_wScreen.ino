#include <SPI.h>
#include <LoRa.h>
#include "Display.h"
#include "shared.h"

void setup() 
{
  Serial.begin(9600);
  
  analogReadResolution(10); // 10 bits on analogRead.. max of 1023 
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  // Interrupt for the button
  attachInterrupt(digitalPinToInterrupt(4), changeProgramMode, CHANGE); 

  // Start the display
  setupDisplay();
  
  // LED.. start in "remote control mode" 
  programMode = REMOTECONTROL;
  
  // Start Radio comms
  Serial1.begin(57600); 
}

void loop() 
{

  Serial.println(programMode);
  // If it's been more than 1.5 seconds since the last state sent
  if (millis() - lastStateSent >= 1500)
  {
    // send the state again
    updateProgram = 1;
  }
  
  // If it's been more than 400ms since the last speed sent
  if (millis() - lastSpeedSent >= 100)
  {
    updateSpeed = 1; 
  }

  // If in remote control mode
  if (programMode == REMOTECONTROL and updateSpeed)
  {
    // Read the pots
    forwardBackPercent = analogRead(A0) / 1023.0;
    leftRightPercent = analogRead(A1)/ 1023.0; 
  
    // Rescale reading. Motor Range is 1100 - 1900. 
    motorValue = (forwardBackPercent) * 800 + 1100; 
    // Rescale reading. Bias is between -400 and 400. 
    turnBias   = (leftRightPercent) * 800 - 400;
    
    leftSpeed = motorValue + turnBias; 
    rightSpeed = motorValue - turnBias;

    // Bounds checking... 
    if (leftSpeed > 1900)
      leftSpeed = 1900;
    if (leftSpeed < 1100)
      leftSpeed = 1100;

    if (rightSpeed > 1900)
      rightSpeed = 1900;
    if (rightSpeed < 1100)
      rightSpeed = 1100; 

    // Don't send if we already sent this value
    for (int i=0; i <2; i++)
    {
      Serial1.print('S');
      Serial1.print(leftSpeed);
      Serial1.print(rightSpeed);
      Serial1.print('E');
    }

    // Keep track of the previous values
    leftSpeedPre = leftSpeed;
    rightSpeedPre = rightSpeed; 
    lastSpeedSent = millis(); 
    updateSpeed = 0;
    Serial.println("Displaying Remote Info:");
    displayRemoteInfo(); 
  }
  else if (programMode == AUTONOMOUS)
  {
//    int packetSize = LoRa.parsePacket();
//    
//    if (packetSize)
//    {
//      char start = LoRa.read();
//
//      if (start == 'C')
//      {
//        int recWaypointIndex      = LoRa.parseInt();
//        int recNumWayPoints       = LoRa.parseInt();
//        double recDistanceBetween = LoRa.parseFloat();
//        double recCurrentHeading  = LoRa.parseFloat();
//        double recTargetHeading   = LoRa.parseFloat();
//        long   recLat             = LoRa.parseInt(); 
//        long   recLon             = LoRa.parseInt();
//        char stopByte             = LoRa.read();
//
//        if (stopByte == 'E')
//        {
//          waypointIndex = recWaypointIndex;
//          numWayPoints  = recNumWayPoints; 
//          distanceBetween = recDistanceBetween;
//          currentHeading  = recCurrentHeading;
//          targetHeading   = recTargetHeading;
//          lat             = recLat; 
//          lon             = recLon; 
//        }
//      }
//    }
    displayAutonomousInfo();
  }
  
  // We need to change the mode of the program
  // and send it to the receiver. 
  if (updateProgram)
  { 
    Serial.println("Updaing Program..");
    char charToSend; 
    if (programMode == AUTONOMOUS)
    {
      charToSend = '0';
    }
    else if (programMode == REMOTECONTROL)
    {
      charToSend = '1';
    }
    // Send the state to the receiver 2 times 
    for (int i = 0; i<2; i++)
    {
      Serial1.print('M');
      Serial1.print(charToSend);
      Serial1.print('E');
      delayMicroseconds(50);
    }

    // Update the last time sent and clear the flag. 
    lastStateSent = millis(); 
    updateProgram = 0;
  }
}

void changeProgramMode()
{
  if (programMode == AUTONOMOUS && digitalRead(4) == LOW)
  {
    programMode = REMOTECONTROL;
  }
  else if (programMode == REMOTECONTROL  && digitalRead(4) == HIGH)
  {
    programMode = AUTONOMOUS;
  }
}

//void changeProgramMode()
//{
//  // When the button is pressed,
//  // set the updateProgram flag.
//      // Update the LED with the current state. 
//  if (programMode == REMOTECONTROL)
//  {
//    programMode = AUTONOMOUS;
//    ledState = 0; 
//  }
//  else if (programMode == AUTONOMOUS)
//  {
//    programMode = REMOTECONTROL;
//    ledState = 1; 
//  } 
//  updateProgram = 1; 
//}
