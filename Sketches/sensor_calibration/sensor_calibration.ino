// Analog input port
#define SENSOR_PIN A0

int sensor_value;

void setup() {
  // open serial monitor
  Serial.begin(9600);
  
  // using function to convert analog voltage into digital value
  // changing resolution does not change voltage value
  // 12 bits sets to values from 0 to 4095
  analogReadResolution(12);


}

void loop() {
  // read in sensor value
  sensor_value = analogRead(SENSOR_PIN);

  // print to serial monitor
  Serial.println(sensor_value);

  delay(200); 
}

// max dry_value = 2600
// max wet_value = 1090