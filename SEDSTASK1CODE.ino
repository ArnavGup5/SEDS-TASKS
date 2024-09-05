const int windowSize = 5; // Size of the moving average window 
//IT IS A TECHNIQUE USED TO SMOOTH OUT SHORT TERM FLUCTUATIONS
//IT REFERS TO THE NUMBER OF DATA POINTS USED TO CALCULATE THE AVERAGE
const float smoothingFactor = 0.6; // Exponential smoothing factor
//LOWER VALUE PLACES HIGHER IMPORTANCE TO OBSERVATONS IN PAST MEANING LONGER DURATIONS AND VALUE CLOSER TO 1 PLACES HIGHER IMPORTANCE TO RECENT CHANGES
//I PUT 0.8 CLOSER TO 1 BECAUSE IN CASE OF  SATELITE ANY RECENT CHANGES MAY IMPACT A LOT AND WE NEED TO SEE THOSE CHANGES AS QUICKLY AS POSSIBLE
const float threshold = 0.2; // Threshold for determining movement
//THRESHOLD HELPS IN FIGGURING OUT WHICH CHANGES ARE NORMAL FLUCTUATIONS IN THE DATA AND WHICH ARE MEANINGFUL CHANGES 
//WE ARE USING CONST BECAUSE THESE ARE VALUES INPUTTED INTO THE VARIABLE SPECIFIED THAT CANT CHANGE
// Variables
float altitudeReadings[windowSize]; // Array to hold recent altitude readings
int index = 0; // Current index for the moving average window
float movingAverage = 0; // Current moving average value
float lastAltitude = 0; // Last altitude reading for comparison
float currentAltitude = 0; // Current altitude reading from sensor
float smoothedAltitude = 0; // Smoothed altitude value
float altitudeChange = 0; // Change in altitude

void setup() {
  Serial.begin(9600); // RATE AT WHICH DATA IS SENT BY ARDUINO TO SERIAL MONITOR
  // Initialize altitude readings
  for (int i = 0; i < windowSize; i++) { //Initializes the altitudeReadings array to 0. This ensures that the array starts with a clean state before collecting real sensor data.
    altitudeReadings[i] = 0;
  }
}

void loop() {
  // Read the current altitude from the sensor (simulated here)
  currentAltitude = readAltitudeSensor(); // Replace with actual sensor reading function
  //(VARAIBLENAME , PINNUMBER)
  // Update the moving average filter
  updateMovingAverage(currentAltitude);
  
  // Calculate altitude change
  altitudeChange = currentAltitude - lastAltitude;
  
  // DetermineS the movement status
  if (abs(altitudeChange) < threshold) { //WILL SHOW UP OR DOWN ONLY WHEN BIG ENOUGH CHANGES
    Serial.println("Stationary");
  } else if (altitudeChange > 0) {
    Serial.println("Moving Up");
  } else {
    Serial.println("Moving Down");
  }

  // Update lastAltitude for the next loop iteration SO THAT THEN THE NEXT SET OF DATA 'S MOVING AVERAGE CAN BE CALCULATED
  lastAltitude = currentAltitude;
  
  // Delay to simulate time between readings
  delay(100); // Adjust the delay based on your sensor update rate
}

void updateMovingAverage(float newAltitude) {
  // Add the new altitude to the window
  altitudeReadings[index] = newAltitude; //Adds the new altitude reading to the altitudeReadings array at the current index position
  
  // Update the moving average
  float sum = 0;
  for (int i = 0; i < windowSize; i++) { //Calculates the moving average of the altitude readings by summing all values in the array and dividing by windowSize
    sum += altitudeReadings[i];
  }
  movingAverage = sum / windowSize;
  
  // Update the index for the next reading
  index = (index + 1) % windowSize; //Updates the index to point to the next position in the array. It wraps around to 0 when it reaches the end of the array
  
  // Apply exponential smoothing
  smoothedAltitude = (smoothingFactor * newAltitude) + ((1 - smoothingFactor) * smoothedAltitude);
} //This smooths the altitude data, giving more weight to recent readings based on the smoothingFactor.

float readAltitudeSensor() {
  // Simulated sensor reading (replace with actual sensor code)
  // For testing, you might use random values or a fixed altitude.
  return 0;
}
