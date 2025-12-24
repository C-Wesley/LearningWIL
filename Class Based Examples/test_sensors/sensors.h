#ifndef ATLAS_H
#define ATLAS_H

    // This file defines the classes that work with the sensors we are using to monitor the water. 

    #include "types.h"
    #include <Wire.h>
    #include <OneWire.h>
    #include <DallasTemperature.h>

    class AtlasSensor
    {
        public: 
            AtlasSensor()
            {
                // Not passing in the address to start because of the WaterMonitor class. 
                // Forcing us to use the setAddress method
                address = 0;
            }

            void setAddress(uint8_t add)
            {
                address = add;
            }

            bool begin()
            {
                Wire2.beginTransmission(address); 
                uint8_t reply = Wire2.endTransmission();
                
                // The I2C bus sits high. If a device pulls it low, reply will be 0. This indicates a device is at this address.
                // If reply is 0 it indicates success, so we return !0 = 1. !1 = 0, for fail.
                return !reply; 
            }

            void request()
            {
                Wire2.beginTransmission(address);
                Wire2.write('R');         // (R)ead
                Wire2.endTransmission();
            }

            float read()
            {
                Wire2.requestFrom(address, 20);
                uint8_t code = Wire2.read();
                char sensorData[19];
                
                // Code values
                // 1 is success
                // 2 syntax error
                // 254 not ready
                // 255 no data to send
                
                if (code == 1)
                {   
                    // Read the data from the sensor
                    for (int ii = 0; ii < 19; ii++)
                    {
                        char charIn = Wire2.read();
                        sensorData[ii] = charIn;
                    }
                    // Parse the data
                    float sdata = atof(sensorData);

                    return sdata;
                }
                for (int ii =0; ii<19; ii++)
                {
                    Wire2.read(); // Clear out our junk that we requested. 
                }
                return code;
            }
        private: 
            uint8_t address; 
    };

    class TempSensor
    {
        // Class for the DS18B20 
        public: 
            TempSensor()
            {}
            
            void begin(uint8_t pin)
            {
                ds.begin(pin); 
                ds.search(addr);
                ds.reset();
            }

            float get()
            {
                ds.select(addr); // Select our device using its address
                ds.write(0x4E);  // Wirte our configuration to the scratch pad
                ds.write(0x00);  // This is the TH alarm, not using
                ds.write(0x00);  // This is the TL alarm, not using
                ds.write(0x3F);  // Sets to 10 bit resoltuion; see page 9 of the datasheet
                ds.write(0x48);  // Copies the contents of the scratch pad 
                ds.reset(); 
                
                // Measure and read
                ds.select(addr); // Select our device (again!)
                ds.write(0x44);  // This initiates a temperature conversion! 
                delay(200);      // Let it finish conversion (187.5 ms)
                ds.reset();      
                ds.select(addr); // Select it (yes again!)
                ds.write(0xBE);  // Read the scratch pad

                for(int ii=0; ii<9; ii++)
                {
                    data[ii] = ds.read(); // Read the bytes
                }

                uint16_t raw = (data[1]<<8) | data[0]; 
                // Clear bits 1 and 0 since we are using 10-bit resolution
                raw &= ~0x03;
                // Convert to temperature in Celsius 
                float celsius = raw/16.0;

                return celsius;
            }
        private: 
            OneWire ds;
            byte addr[8];
            byte data[9]; 
    }; 

    class WaterMonitor
    {
        public: 
            AtlasSensor DO;
            AtlasSensor ORP;
            AtlasSensor pH;
            AtlasSensor conductivity;
            TempSensor temp;
            
            WaterMonitor(uint8_t DO_add, uint8_t ORP_add, uint8_t pH_add, uint8_t c_add, uint8_t wire_bus)
            {
                DO.setAddress(DO_add);
                ORP.setAddress(ORP_add);
                pH.setAddress(pH_add); 
                conductivity.setAddress(c_add);
                temp.begin(wire_bus);
            }

            // Since we manually allocated memory, we must deallocated it in the destructor. 
            ~WaterMonitor()
            {
                delete &DO;
                delete &ORP;
                delete &pH;
                delete &conductivity;
                delete &temp;
            }

            bool begin()
            {
                bool do_ret = DO.begin();
                bool orp_ret = ORP.begin();
                bool pH_ret = pH.begin();
                bool conductivity_ret = conductivity.begin(); 

                uint8_t reply = do_ret && orp_ret && pH_ret && conductivity_ret;  // If any are false, we failed. 

                return reply;
            }

            const EcoData& getData()
            {
                DO.request();
                ORP.request();
                pH.request();
                conductivity.request();

                delay(ATLAS_SENSOR_DELAY_MS); // Give sensors time to respond
                
                data.waterTemp = temp.get();
                data.DO  = DO.read();
                data.ORP = ORP.read();
                data.pH  = pH.read();
                data.conductivity = conductivity.read();

                return data;
            }

            private:
                EcoData data;
    }; 

#endif