## Atlas Scientific Sensors (I²C)

**What the Program Does**
- Scans the I²C bus to find connected devices
- Sends read commands (`'R'`) to:
  - Dissolved Oxygen (DO)
  - ORP
  - pH
  - Conductivity sensors
- Reads and parses floating-point measurements
- Prints all sensor values to the Serial Monitor