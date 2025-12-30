# Sensors 

We are using four different I²C Atlas scientific sensors and a one-wire DS18B20 temperature sensor.

The Atlas sensors are the Dissolved Oxygen, pH, conductivity, oxidation reduction potential. 

sensors.h contains three main classes 

- `AtlasSensor` - The class we will use for each atlas sensor. The only difference will be the I²C address we use. 
- `TempSensor` - Class that controls the DS18B20 temp sensor
- `WaterMonitor` - A class that coordinates all the Atlas Scientific sensors and the DS18B20 sensor. 

WIL should only have to communicate with the  `WaterMonitor` class. 

To add a new sensor, you should update the `EcoData` struct and the `WaterMonitor` class. As well as adding your own class for the new sensor.

## AtlasSensor Class

The `AtlasSensor` class provides a minimal interface to Atlas Scientific sensors over I²C.

### Key Characteristics
- Uses `Wire2` (separate from the main I²C bus)
- Reads sensor values as ASCII and converts them to `float`

### Typical Usage Pattern
1. Set the sensor address
2. Call `begin()` to verify presence on the I²C bus
3. Call `request()` to trigger a measurement
4. Call `read()` to retrieve the result

## TempSensor Class 

The `TempSensor` class interfaces with a DS18B20 temperature sensor using the OneWire protocol.

### Responsibilities
- Initialize the OneWire bus
- Trigger temperature conversions
- Read and convert raw sensor data to degrees Celsius

### Notes
- The sensor is configured for 10-bit resolution

## Water Monitor Class 

The `WaterMonitor` class is the main abstraction WIL should interact with. It collects data from all sensors and packages it into an `EcoData` struct.

### Responsibilities
- Own and initialize all sensor objects
- Coordinate sensor read timing
- Populate and return a complete `EcoData` struct

### Currently Managed Sensors
- Dissolved Oxygen (DO)
- Oxidation Reduction Potential (ORP)
- pH
- Conductivity
- Water Temperature

## EcoData Struct

The `EcoData` struct (defined in `types.h`) represents a snapshot of all environmental measurements.

Fields include:
- `float conductivity`
- `float DO`
- `float pH`
- `float ORP`
- `float waterTemp`

### Important
If a new sensor is added:
- A new field must be added to `EcoData`
- The `WaterMonitor` class must be updated to populate it