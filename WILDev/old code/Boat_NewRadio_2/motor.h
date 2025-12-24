#ifndef MOTOR_H
#define MOTOR_H

#include "shared.h"
#include "headingControl.h" // Needed for "fixHeading" method. 
#include "controller.h"
#include <Servo.h>

  int leftControlPin  = 6;
  int rightControlPin = 7; 
  int currentLeftSpeed; 
  int currentRightSpeed; 
  Servo leftServo;
  Servo rightServo;
  
  void setMotorSpeed(int aLeftSpeed, int aRightSpeed)
  {
    // For our ESC, this has a range of 1100 - 1900. 
    // 1500 is off, 1900 is fully forwards, and 1100 is fully reversed.
    rightServo.writeMicroseconds(aRightSpeed);
    leftServo.writeMicroseconds(aLeftSpeed);
  }

  void setupESC()
  {
      pinMode(leftControlPin, OUTPUT);
      pinMode(rightControlPin, OUTPUT); 
  
      // Begin servo objects. 
      leftServo.attach(leftControlPin);
      rightServo.attach(rightControlPin);
  
      // This should initalize the ESC. 
      // Make sure you hear the startup sequence complete.
      // Otherwise you need to try again. 
      leftServo.writeMicroseconds(1500);
      rightServo.writeMicroseconds(1500); 
      delay(10000); 
  }

  void updateMotors(int targetLeft, int targetRight)
  {
      while (currentLeftSpeed != targetLeft || currentRightSpeed != targetRight)
      {
          if (currentLeftSpeed != targetLeft)
          {
              if(currentLeftSpeed < targetLeft)
              {
                  currentLeftSpeed++; 
              }
              else
              {
                  currentLeftSpeed--;
              }
          }
  
          if (currentRightSpeed != targetRight)
          {
              if(currentRightSpeed < targetRight)
              {
                  currentRightSpeed++; 
              }
              else
              {
                  currentRightSpeed--;
              }
          }
          setMotorSpeed(currentLeftSpeed, currentRightSpeed);
      }
  }

  void rotateInPlaceBoat()
  {
    // While our heading is too far off from the target
    while(abs(headingError) > ACC_HEAD_ERR)
    {
      // check the controller for updates
      readController();
      
      if (programMode == REMOTECONTROL)
      {
        // if we should be in remote control mode, leave this method. 
        return; 
      } 
      if (headingError > 0)
      {
        // Rotate Right 
        // Set the left motor Forward and the right motor to reverse.
        updateMotors(1700, 1280);  
      }
      else 
      {
        // Rotate Left 
        // Set the left motor Reverse and the right motor Forward.
        updateMotors(1280, 1700);
      }
  
      currentHeading  = getHeading2() - averageGPSError; 
      currentHeading = fixHeading(currentHeading); 
      headingError    = getHeadingError(currentHeading, targetHeading);
    }
    // Set the motors back to straight
    updateMotors(1650, 1650);
  }
  
#endif
