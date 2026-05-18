#ifndef TYPES_H 
#define TYPES_H
    
    // Structs are like arrays, but 
    // instead of requiring the same type
    // we can use different types.
    // Each element in the struct is called 
    // a "member".
    // To access memebers, we use 
    // StructName.MemberName  <- notice the '.' 

    /* =======================================================
        System State

         The navigation state contains information used
         to make decisions about how to control WIL's heading

         MotorState contians the speed of the motors
       =======================================================*/
    struct NavigationState 
    {
        double distanceToWaypoint;  // Meters
        double targetHeading;       // degrees 0-360
        double currentHeading;      // degrees 0-360
        double headingError;        // Normalized Error [-1, 1]
        double gpsError;            // Degrees, difference GPS vs heading sensor
        double averageGPSError;     // degrees, running average
        int waypointIndex;          // current waypoin index
        int numWaypoints;           // total number of waypoints 
    };

    /*======================================
        Sensor Data 
         EcoData contains the readings from 
         the atlas scientific sensors. 
      ======================================*/

    struct EcoData 
    {
        float DO;            // Dissolved Oxygen
        float ORP;           // Oxidation-Reduction Potential
        float pH;            
        float conductivity;  // Electrical Conductivity 
        float waterTemp;     // Temperature in Celcius
    };

    /* ===========================================================================
        Program Modes of Operation
    
        An enumeration (or enum) is a user defined data type that contains 
        a set of named integer constants. It is used to assign meaningful 
        names to integer values, which makes a program easy to read and maintain.
        https://www.geeksforgeeks.org/c/enumeration-enum-c/
       ===========================================================================*/  

    enum ProgramMode : uint8_t
    {
        REMOTECONTROL = 0,
        AUTONOMOUS    = 1,    
        TUGBOAT       = 2    
    }; 
#endif