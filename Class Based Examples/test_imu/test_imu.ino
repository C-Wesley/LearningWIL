#include "config.h"
#include "types.h"
#include "imu.h"

BNO mybno(BNO_ADD);

void setup()
{
    Serial.begin(9600);
    while(!Serial) {}
    Wire.begin();
    
    if (!mybno.begin())
    {
        Serial.println("BNO Failed to init");
        while(1){}
    }
    Serial.println("BNO success!");
}
void loop()
{
    float heading = mybno.getHeading();
    Serial.println(heading);
    delay(500);
}