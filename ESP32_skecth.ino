#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADS1X15.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "prjiot";
const char* password = "88888888";
const char* endpoint = "https://proxy.nptel.pp.ua/update";

// ADC
Adafruit_ADS1115 ads;  // Create an ADS1115 instance

// Define sensor pins
#define SOIL_MOISTURE_PIN 32

void setup() {
  Serial.begin(115200);

  // Initialize ADS1115
  ads.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");
}

void loop() {
  // Read soil moisture
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  
  // Convert ADS1115 reading (if using) to voltage
  //float soilMoistureVoltage = ads.readADC_SingleEnded(0) * 0.1875 / 1000; // Conversion for ADS1115
  // Uncomment the above line if you're using ADS1115
  
  // Print soil moisture
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture);

  // Create an HTTPClient object
  HTTPClient http;
  
  // Create a JSON object
  StaticJsonDocument<200> jsonPayload;
  jsonPayload["moisture"] = soilMoisture;
  
  // Serialize the JSON object to a string
  String payload;
  serializeJson(jsonPayload, payload);
  
  // Send the POST request to the endpoint
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(payload);
  
  if (httpResponseCode == 200) {
    Serial.println("Moisture level updated successfully");
  } else {
    Serial.print("HTTP POST request failed with error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
  
  delay(5000);  // Delay for 10 seconds before the next update
}