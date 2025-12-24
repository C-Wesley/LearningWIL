#ifndef SHARED_H
#define SHARED_H 
  
  // leftSpeed and rightSpeed will be transmitted 
  int leftSpeed; 
  int rightSpeed;
  int leftSpeedPre = 0;
  int rightSpeedPre = 0;
  
  // Readings from the two pots
  float forwardBackPercent; 
  float leftRightPercent;
  
  int motorValue;
  int turnBias;
  
  // flag to update program from interrupt
  bool updateProgram;
  bool updateSpeed;
  
  bool ledState = 1;
  
  // Timing Variables
  long lastStateSent;
  long lastSpeedSent;
  long currentTime; 

  byte REMOTECONTROL = 0x01;
  byte AUTONOMOUS    = 0x02; 
  int programMode = AUTONOMOUS; 

  // Variables we expect to get back from the boat
  int waypointIndex;
  int numWayPoints;
  double distanceBetween;
  double currentHeading;
  double targetHeading;
  long lat;
  long lon;

#endif 
