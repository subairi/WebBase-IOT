#include <Arduino.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTTYPE    DHT12 
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);
// Replace with your network credentials
const char* ssid = "Elektro";
const char* password = "tanyamashari";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
String readDHT11Temperature() {
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  t = random(-20, 100);
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT11 sensor!");
    return "";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}
String readDHT11Humidity() {
  float h = dht.readHumidity();
  h= random(0, 10);
  if (isnan(h)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return "";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}
void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);
  dht.begin();
  
  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
// Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
// Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
// Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHT11Temperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHT11Humidity().c_str());
  });
// Start server
  server.begin();
}
 
void loop(){
  
}