#include "display.h"
#include "radio.h"

Display display;

const int leftButton  = 1;
const int rightButton = 0;

const int rightJoystick = A0; 
const int leftJoystick  = A1;

const int sdCS = 5;

volatile bool takeRemoteMeasurement;
volatile ProgramMode mode;
Radio remote;

unsigned long speedTimer;
unsigned long modeTimer;
volatile unsigned long extraFunctionTimer; 
volatile unsigned long changeModeTimer; 

int leftSpeed;
int rightSpeed;

int prevLeftSpeed;
int prevRightSpeed;

int SERIAL_RADIO_BAUD = 57600;

void changeProgramMode() // Right button on the controller
{
  if (millis() - changeModeTimer > 500)
  {
    switch(mode)
    {
        case AUTONOMOUS:
            mode = TUGBOAT;
            break;
        case REMOTECONTROL:
            mode = AUTONOMOUS;
            break;
        case TUGBOAT:
            mode = REMOTECONTROL;
            break;
        default:
            break;
    }
    changeModeTimer = millis(); 
  }
}

void extraFunctionButton()
{
    // If we request a eco reading in remote control mode and we haven't requested it for 5 seconds.
  if (takeRemoteMeasurement != 1 && mode == REMOTECONTROL && millis() - extraFunctionTimer > 5000)
  {
    takeRemoteMeasurement = 1;
    extraFunctionTimer = millis();
  }

  if (mode == AUTONOMOUS && millis() - extraFunctionTimer > 2000)
  {
    remote.requestIncrementWaypoint();
    extraFunctionTimer = millis();
  }
}

void setup()
{
    Serial.begin(9600);
    Serial1.begin(SERIAL_RADIO_BAUD);

    analogReadResolution(10);       // 10 bits on analogRead.. max of 1023 
    pinMode(rightJoystick, INPUT);
    pinMode(leftJoystick,  INPUT);
    pinMode(rightButton,   INPUT);
    pinMode(leftButton,    INPUT);

    mode = REMOTECONTROL;
    display.begin();

      // Interrupt for the buttons 
    attachInterrupt(digitalPinToInterrupt(rightButton), changeProgramMode, RISING); 
    attachInterrupt(digitalPinToInterrupt(leftButton), extraFunctionButton, RISING);

    takeRemoteMeasurement = 0;
    speedTimer         = millis();
    modeTimer          = millis();
    extraFunctionTimer = millis();
}

void loop()
{
    remote.read();
    
    // Send the program mode every 1.5 seconds
    if ( (millis() - modeTimer) >= 1500)
    {
        remote.sendMode(mode);
        modeTimer = millis();
    }

    // Send a new speed every 250 miliseconds when in Remote control mode (4 times/second)
    if ( (millis() - speedTimer) >= 250 && mode == REMOTECONTROL)
    {
        readSpeed(); // Updates leftSpeed, rightSpeed; 

        //if (leftSpeed != prevLeftSpeed || rightSpeed!= prevRightSpeed)
            remote.sendSpeed(leftSpeed, rightSpeed);
        
        // prevLeftSpeed  = leftSpeed;
        // prevRightSpeed = rightSpeed
        speedTimer = millis();
    }

    // If we asked for a remote reading
    if (takeRemoteMeasurement)
    {
        remote.requestEcoData();
        takeRemoteMeasurement = 0;
    }

    switch (mode)
    {
      case REMOTECONTROL:
        display.displayRemoteInfo(leftSpeed, rightSpeed, remote.ecoData);
        break;
      case AUTONOMOUS:
        display.displayAutonomous(remote.navState);
        break;
      case TUGBOAT:
        display.displayTugboat();
        break;
      default:
        break;
    }
}

int readSpeed()
{
    // Read the pots
    float forwardBackPercent = analogRead(rightJoystick) / 1023.0;

    // the left joystick is backwards. We can flip it arround
    // by subtracting the raw reading from 1023. 
    // Pushing it to the left causes an increase in the analog read.
    // We need a push to the left to cause a decrease in the analog read. 
    float leftRightPercent   = (1023 - analogRead(leftJoystick)) / 1023.0; 
  
    // Rescale reading. Motor Range is 1100 - 1900. 
    int motorValue = (forwardBackPercent) * 800 + 1100; 
    // Rescale reading. Bias is between -400 and 400. 
    int turnBias   = (leftRightPercent) * 800 - 400;
    
    leftSpeed = motorValue + turnBias; 
    rightSpeed = motorValue - turnBias;

    // Bounds checking... 
    if (leftSpeed > 1900)
      leftSpeed = 1900;
    else if (leftSpeed < 1100)
      leftSpeed = 1100;

    if (rightSpeed > 1900)
      rightSpeed = 1900;
    else if (rightSpeed < 1100)
      rightSpeed = 1100; 
}