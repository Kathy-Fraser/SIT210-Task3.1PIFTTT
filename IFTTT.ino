// ***
// File :: IFTTT
// Author :: Kathleen Fraser
// Student Number :: 96709661
// ***

// Initialise variables.
#include "secrets.h"
#include <WiFiNINA.h>

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Create a client to connect to a specified internet IP address and port.
WiFiClient client;

// Intiailise IFTTT pathway and key.
char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/sunlight/with/key/cOXNSSzO3rIQ21TaJ_l8NC";

// Initialise variables.
const int sensorPin = A0;
int lightThreshold = 400;
bool isSunlight = false;
bool noSunlight = false;

void setup() {
  
  // Initialisse WiFi connection.
  WiFi.begin(ssid, pass);

  // Initialise serial connection.
  Serial.begin(9600);

}

// Function - Read light with sensor.
void LightSensor() {

  // Initialise variables.
  int lightValue = analogRead(sensorPin);
  isSunlight = (lightValue > lightThreshold);

  if (isSunlight) {
    Serial.println("Sunlight detected");
  }
  else {
    Serial.println("No sunlight");
  }
}

void loop() {

  // Call LightSensor() function.
  LightSensor();

  if (isSunlight && !noSunlight) {

    // Update queryString based on sunlight detection.
    String sunlightStatus = "SUNLIGHT";
    String queryString = "?value1=" + sunlightStatus;

    // Make and send HTTP request.
    if (client.connect(HOST_NAME, 80)) {
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: Close");
      client.println();

      Serial.println("Sunlight detected. Triggering IFTTT Webhook");

      // Sets variable accordingly to avoid repetitive triggering of the Webhook.
      noSunlight = true;
    }
    else {
      Serial.println("Failed to connect to IFTTT server.");
    }
  }
  else if (!isSunlight && noSunlight) {

    // Update queryString based on sunlight detection.
    String sunlightStatus = "NO_SUNLIGHT";
    String queryString = "?value1=" + sunlightStatus;

    // Make and send HTTP request.
    if (client.connect(HOST_NAME, 80)) {
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: Close");
      client.println();

      Serial.println("No sunlight detected. Triggering IFTTT Webhook");

      // Sets variable accordingly to avoid repetitive triggering of the Webhook.
      noSunlight = false;
    }
    else {
      Serial.println("Failed to connect to IFTTT server.");
    }

  }

  // 1 minute interval between sensor readings.
  delay(60000);
}
