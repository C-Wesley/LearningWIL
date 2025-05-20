#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "shared.h"

char leftSpeedChars[5];
char rightSpeedChars[5];



void beginController()
{
  Serial2.begin(57600);
  programMode = AUTONOMOUS;
}

void _read_state()
{
  // Read the Program Mode...
  char data = Serial2.read();

  // Make sure we got an "E" for end.
  if (Serial2.read() == 'E')
  {
    if (data == '0')
    {
      Serial.println("Got AUTO mode.");
      programMode = AUTONOMOUS;
    }
    else if (data == '1')
    {
      Serial.println("Got REMOTE mode.");
      programMode = REMOTECONTROL;
      //Serial.println("Switched to remote control mode.");
    }
    delayMicroseconds(100);
  }
}

void _read_speed()
{
  uint8_t numPackets = Serial2.available();

  while (numPackets < 8)
  {
    numPackets = Serial2.available();
    delayMicroseconds(50);
  }

  for (int i = 0; i < 4; i++)
  {
    leftSpeedChars[i] = Serial2.read();
  }
  leftSpeedChars[4] = '\0';

  for (int i = 0; i < 4; i++)
  {
    rightSpeedChars[i] = Serial2.read();
  }
  rightSpeedChars[4] = '\0';

  if (Serial2.read() == 'E')
  {
    leftMotorSpeedRec = atoi(leftSpeedChars);
    rightMotorSpeedRec = atoi(rightSpeedChars);

    if (leftMotorSpeedRec < 1100)
    {
      leftMotorSpeedRec = 1100;
    }
    if (leftMotorSpeedRec > 1900)
    {
      leftMotorSpeedRec = 1900;
    }

    if (rightMotorSpeedRec < 1100)
    {
      rightMotorSpeedRec = 1100;
    }
    if (rightMotorSpeedRec > 1900)
    {
      rightMotorSpeedRec = 1900;
    }
    delayMicroseconds(100);
    Serial.print(leftMotorSpeedRec); Serial.print(" "); Serial.println(rightMotorSpeedRec);
  }
}

void readController()
{
  //Serial.print("trying to read");
  while (Serial2.available() > 0)
  {
    //Serial.print(" "); Serial.print("Inside available"); Serial.print(" ");
    char start = Serial2.read();
    delayMicroseconds(100);
    if (start == 'S')
    {
      //Serial.print("Trying to read speed!");
      _read_speed();
    }
    else if (start == 'M')
    {
      //Serial.print("trying to read state!");
      _read_state();
    }
  }
  //Serial.println();

}

void sendControllerDataAuto()
{
  Serial2.print('C');
  Serial2.print(waypointIndex);    
  Serial2.print(numWayPoints);     
  Serial2.print(distanceBetween);  
  Serial2.print(currentHeading);  
  Serial2.print(targetHeading);    
  Serial2.print(myGPSPair.lat1_L); 
  Serial2.print(myGPSPair.lat2_L); 
  Serial2.print('E');
}

void sendControllerDataRemote()
{
  Serial2.print('C');
  //    Serial1.print(
}

#endif
