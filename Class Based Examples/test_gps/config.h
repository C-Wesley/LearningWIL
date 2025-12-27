#ifndef CONFIG_H
#define CONFIG_H 
    /* ===================================================================================
        These variables are ones that may be useful to change at some point in time.
        I've grouped them together in this file. As a design principle, I didn't want these hard 
        coded into the classes. Instead, the class should own it's own copy. This makes the 
        classes reusable in other code independent of this config file. 

        The keyword "const" is useful to ensure a value is not changed at run time. 
        The keyword "constexpr" is useful to ensure a value is not changed at compile time.
       ====================================================================================*/

    /* =============
        Debug Flags
       ============= */
    constexpr bool DEBUG_PRINT_GPS_DATA = false;
    constexpr bool DEBUG_PRINT_SD_LOG   = true;
    constexpr bool DEBUG_PRINT_SERIAL   = true;

    /* ================
        Path Constants
       ================ */
    constexpr int MAX_WAYPOINTS = 200;

    /* ===========================
        Heading Control Constants
       =========================== */
    constexpr double ACCEPTABLE_HEADING_ERROR = 0.02;
    constexpr int HEADING_AVERAGE_LIMIT    = 100;
    
    /* =================
        Serial Settings
       ================= */
    constexpr unsigned long SERIAL_RADIO_BAUD = 57600;
    constexpr unsigned long DEBUG_SERIAL_BAUD = 9600;

    /* ===============
        I2C Addresses
       =============== */
    constexpr uint8_t   GPS_I2C_ADD = 0x42; 
    constexpr uint8_t  ATLAS_DO_ADD = 0x61;
    constexpr uint8_t ATLAS_ORP_ADD = 0x62;
    constexpr uint8_t  ATLAS_PH_ADD = 0x63;
    constexpr uint8_t   ATLAS_C_ADD = 0x64;
    constexpr uint8_t       BNO_ADD = 0x28;

    constexpr int I2C_TIMEOUT_MS = 100;

    /* =====================
        Sensor Configs 
       =====================*/
    constexpr int ATLAS_SENSOR_DELAY_MS = 1000; // How long each atlas sensor has to wait before getting data.
    constexpr int ONE_WIRE_BUS = 13;

    /* =========
        SD Card 
       ========= */
    constexpr int SD_CHIP_SELECT = 48;
    constexpr const char* GPS_DATA_FILE   = "GPSData.csv";
    constexpr const char* ECO_DATA_FILE   = "ECO.csv"; 
    constexpr const char* ECO_REMOTE_FILE = "ECOR.csv";
    constexpr const char* PATH_FILE       = "path.csv";

    /* =======================
        ESC Control Variables
       ======================= */
    constexpr int LEFT_MOTOR_PIN  = 6;         // Must be a PWM 
    constexpr int RIGHT_MOTOR_PIN = 7;         // Must be a PWM
    constexpr int MOTOR_SETUP_DELAY_MS = 5000; // Delay to allow ESC startup 
    constexpr int ESC_STOP_SIGNAL  = 1500;     // Neutral Speed 
    constexpr int ESC_FULL_FORWARD = 1900;     // Full Throttle Forward
    constexpr int ESC_FULL_REVERSE = 1100;     // Full throttle reverse
    constexpr int ESC_UPDATE_DELAY_US = 20;    // Delay between ESC updates to make smooth changes 

    constexpr int ESC_ROTATE_FORWARD  = 1700;
    constexpr int ESC_ROTATE_REVERSE  = 1280;

    /* =========================
        RGB LED Pin Assignments
       ========================= */
    constexpr int RGB_R = 86;
    constexpr int RGB_G = 87;
    constexpr int RGB_B = 88;

    /* =========================
    Remote Control Constants
    ========================= */
    constexpr unsigned long MAX_WAIT_MS = 150; // Ammount of time to wait for full packet to be recieved before moving on. 

#endif