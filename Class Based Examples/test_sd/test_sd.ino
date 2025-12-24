#include "config.h"
#include "types.h"
#include "sd_logger.h"

SDLog myLog(SD_CHIP_SELECT); 
Path myPath(MAX_WAYPOINTS);

void setup()
{
    Serial.begin(9600);
    while(!Serial);
    myLog.setAutoWaterDataFileName(ECO_DATA_FILE);
    myLog.setGPSDataFileName(GPS_DATA_FILE); 
    myLog.setRemoteWaterDataFileName(ECO_REMOTE_FILE);
    bool succ = myLog.begin();
    
    if (!succ)
    {
        Serial.println("SD Card failed to init");
        while(1) {}
    }

    NavigationState fakeState;
    fakeState.distanceToWaypoint = 10;
    fakeState.targetHeading      = 20;
    fakeState.currentHeading     = 30;
    fakeState.headingError       = 0.01;
    fakeState.gpsError           = 0.05;
    fakeState.averageGPSError    = -0.25;
    fakeState.waypointIndex      = 10; 

    EcoData fakeEco; 
    fakeEco.conductivity = 200; 
    fakeEco.DO = 16;
    fakeEco.pH = 8.21; 
    fakeEco.ORP = 1200;
    fakeEco.waterTemp = 30.25;

    GlobalPositioningData fakeData;
    fakeData.date.day           = 10;
    fakeData.date.year          = 2025;
    fakeData.date.month         = 7;
    fakeData.date.hour          = 1; 
    fakeData.date.minute        = 35;
    fakeData.date.second        = 45;
    fakeData.position.latitude  = 330000000;
    fakeData.position.longitude = -830000000;
    fakeData.fixType            = 1;
    fakeData.gnssFixOK          = 2;
    fakeData.headVehValid       = 3;
    fakeData.invalid_llh        = 4;
    fakeData.SIV                = 5;
    fakeData.groundSpeed        = 6;
    fakeData.headingMotion      = 7;
    fakeData.headingVehicle     = 8;

    ProgramMode mode = REMOTECONTROL;
    myLog.logWaterData(fakeData.date, fakeData.position, fakeEco, mode);
    mode = AUTONOMOUS;
    myLog.logWaterData(fakeData.date, fakeData.position, fakeEco, mode);
    mode = TUGBOAT; 
    myLog.logWaterData(fakeData.date, fakeData.position, fakeEco, mode);
    myLog.logAutonomous(fakeState, fakeData);

    int pathSucc = myPath.parsePath();

    if (pathSucc)
    {
        Serial.println("Parsed successfully!");
        myPath.printPath();
    }
    else
    {
        Serial.println("Parse Path Unsuccessful.");
    }
}

void loop()
{

}