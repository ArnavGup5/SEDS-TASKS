#include <Wire.h> // Includes the Wire library, which is used for I2C communication. This library handles the communication between the Arduino and I2C devices like the MS4525DO sensor.


#define MS4525DO_ADDRESS 0x28 // This address is used to communicate with the sensor over the I2C bus.


#define AIR_DENSITY 1.225 //This value is used in the formula to calculate airspeed from the pressure difference

void setup() {
  Serial.begin(9600);// This allows the Arduino to send data to the Serial Monitor
  Wire.begin();//This sets up the communication protocol so the Arduino can communicate with I2C devices like the MS4525DO sensor.

  // Initialize the sensor
  if (!initializeSensor()) { //checks if the sensor is properly set up as if it isnt then it might enter an infinte loop
    Serial.println("Sensor initialization failed!");
    while (1); // Stop the program if the sensor fails to initialize
  }
}

void loop() {
  float pressureDifference = readPressureDifference(); // Read the pressure difference
  float airspeed = calculateAirspeed(pressureDifference); // Calculate airspeed

  Serial.print("Pressure Difference (Pa): "); //to print exact text
  Serial.println(pressureDifference); //to print the text recieved
  Serial.print("Airspeed (m/s): ");
  Serial.println(airspeed);

  delay(1000); // Wait for 1 second before the next reading
}

bool initializeSensor() {
  // In most cases, no additional initialization is needed for MS4525DO
  return true; //This function is intended for initializing the sensor. For the MS4525DO, no special initialization is required beyond setting up I2C communication, so it always returns true
}

float readPressureDifference() {
  Wire.beginTransmission(MS4525DO_ADDRESS); //Begins an I2C transmission to the sensor at the specified address.
  Wire.write(0x00); // Start reading pressure data
  Wire.endTransmission();
  
  Wire.requestFrom(MS4525DO_ADDRESS, 2); //Requests 2 bytes of data from the sensor.
  
  if (Wire.available() == 2) { //Checks if 2 bytes of data are available to read.
    byte msb = Wire.read();
    byte lsb = Wire.read(); //Reads the Most Significant Byte (MSB) and Least Significant Byte (LSB) of the pressure data
    
    int16_t rawPressure = (msb << 8) | lsb; //Combines MSB and LSB into a 16-bit integer representing the raw pressure data.
    
    
    
    return rawPressure * 0.0625; // Convert raw pressure to Pascal
  } else {
    Serial.println("Failed to read pressure data!");
    return 0;
  }
}

float calculateAirspeed(float pressureDifference) {
  // Calculate airspeed using the pressure difference
  return sqrt(2 * pressureDifference / AIR_DENSITY);
}
