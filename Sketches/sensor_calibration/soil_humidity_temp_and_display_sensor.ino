/*
  Author: Dawan Savage Bell
  Date: Dec 06, 2023
  Description: Soil Moisture Cpacitive Sensor with DHT22 Temp and Humidity displayed on OLED Display Monitor.
  Uses Arudino Nano Esp32.
  Resource used for sensor logic: https://www.youtube.com/watch?v=pgGpuws7f9o&list=PLWNDWPAClRVr7Y4cv1H0WAcB9Zqwgpi9p
*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DEFINING OLED DISPLAY VALUES 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // olded Address

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT22 SENSOR SET UP
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2 // Digital pin connected to the DHT sensor 
#define DHTTYPE DHT22
// creating dht obj
DHT_Unified dht(DHTPIN, DHTTYPE);

float temp;
float humid;

// capcitive Analog input port
#define SENSOR_PIN A7

// constants taken from sensor calibrations
const int max_wet_value = 3600;
const int max_dry_value = 4095;

int soil_moisture_value;
int soil_moisture_percent;
sensors_event_t event;

void display_initilization() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("voltage allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

 // display.setRotation(2); // Rotate 180 degrees
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.clearDisplay();
}

void read_and_display_temp() {
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));

    display.println(F("Error reading temperature!"));
  }
  else {
    Serial.println(F("Temp: "));
    temp = event.temperature;
    Serial.print(temp);
    Serial.println(F("°C"));

    display.println(F("Temp: "));
    display.print(temp);
    display.print(F("°C"));

    display.display();
  }
}

void read_and_display_humidity() {
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    display.clearDisplay();
    Serial.println(F("Error reading humidity!"));
    display.println(F("Error reading humidity!"));
    display.display();
  }
  else {
    display.clearDisplay();
    //display.setRotation(2); // Rotate 180 degrees
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 3);     // Start at top-left corner

    Serial.print(F("Humidity: "));
    humid = event.relative_humidity;
    Serial.print(humid);
    Serial.println(F("%"));

    display.println(F("Humidity: "));
    display.print(humid);
    display.print(F("%"));

    display.display();
  }
}

void display_soil_moisture(int soil_moisture_percent) {
    // position and print to display
  display.setCursor(20, 0);
  display.setTextSize(1); 
  display.setTextColor(WHITE);
  display.println("Moisture");

  display.setCursor(40, 40);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(soil_moisture_percent);
  display.println("%");
  display.display();
}

void setup() {
  // open serial monitor
  Serial.begin(9600);
  display_initilization();
  analogSetAttenuation(ADC_0db);
  // using function to convert analog voltage into digital value
  analogReadResolution(12);
  dht.begin();
  // Set delay between sensor readings based on sensor details.
  display.clearDisplay();
}

void loop() {
  // read sensor value
  soil_moisture_value = analogRead(SENSOR_PIN);
  Serial.print(soil_moisture_value);
  Serial.print(" - ");

  // determine soil moisture percentage value
  // mapping current value between the max wet and dry values
  soil_moisture_percent = map(soil_moisture_value, max_dry_value, max_wet_value, 0, 100);

  // keep values between 0 and 100;
  // if the soil mosisture_value goes below or over the constants for dry and wet value
  // it will keep the values at either 0 or 100;
  soil_moisture_percent = constrain(soil_moisture_percent, 0, 100);

  // print to serial monitor
  Serial.println(soil_moisture_percent);
  display.clearDisplay();

  display_soil_moisture(soil_moisture_percent);

  delay(3000); 

  // clear display for next reading
  display.clearDisplay();

  read_and_display_temp();
  delay(4000);

  display.clearDisplay();
  
  read_and_display_humidity();
  delay(4000);

  display.clearDisplay();

}
