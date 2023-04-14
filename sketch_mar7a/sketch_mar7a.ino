#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <math.h>
#define outputpin A0

const char* ssid = "7b05d4";
const char* password = "286148654";
String url = "http://192.168.0.18:5164/api/Room/update_temperature";
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
int counter = 1;
int analogValue;
float celsius;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(9600);

  // Connect to Wi-Fi network
  Serial.print("Connecting to wifi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  if((millis() - lastTime) > timerDelay){
    if(WiFi.status() == WL_CONNECTED){
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      WiFiClient client;
      HTTPClient http;
      http.begin(client, url);
      StaticJsonDocument<64> data;
      String serializedData;
      uint8_t mac[6];
      WiFi.macAddress(mac);
      char macStr[18] = "";
      sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      analogValue = analogRead(outputpin);
      celsius = analogValue * (3.3 / 1024.0) * 100;
      data["Value"] = celsius;
      data["Id"] = macStr;
      serializeJson(data, serializedData);
      http.addHeader("Content-Type", "application/json");
      int resCode = http.POST(serializedData);
      http.end();
    }
    lastTime = millis();
  }
}