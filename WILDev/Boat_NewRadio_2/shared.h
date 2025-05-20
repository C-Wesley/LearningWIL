#ifndef SHARED_H
#define SHARED_H

// Important Variables
double distanceBetween;       // Distance between current lat/lon and destination lat/lon
double targetHeading;         // The heading we need to travel at to reach destination
double currentHeading;        // The heading we are traveling at 
double headingError;          // Difference in target and current heading
double gpsError;              // difference in myGPSData.Heading and currentHeading 
double averageGPSError = 0;   // Average difference in myGPSData.Heading and currentHeading

int waypointIndex   = 0;
int numWayPoints;
bool updatedAverage = 0;
long startTime; 
long pollingStartTime; 
unsigned long controllerPolling;
bool GotFirstStartTime = 0;

const float ACC_HEAD_ERR = 0.02; 


byte REMOTECONTROL = 0x01;
byte AUTONOMOUS    = 0x02; 

int programMode = AUTONOMOUS; 

int leftMotorSpeedRec;
int rightMotorSpeedRec;

long dLons[50];
long dLats[50];

float measuredTemp;

float currDO;             
float currORP;              
float currpH;             
float currC;
float cTemp;

float averageDO;
float averageORP;
float averagepH;
float averageC;
float averageTemp; 


#endif 
