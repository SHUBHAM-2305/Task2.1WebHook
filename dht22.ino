#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;  // network SSID (name)
char pass[] = SECRET_PASS;  // network password
                            //Both of these are stored in secrets.h file

WiFiClient client;

unsigned long ChannelID = SECRET_CH_ID;//channel ID or number
const char* APIKey = SECRET_WRITE_APIKEY;//API key for writing data to ThingSpeak channel

#define DHTPIN 2       // Pin where the DHT sensor is connected
#define DHTTYPE DHT22  // DHT 22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); //initializes serial communication at a baud rate of 9600bps
  dht.begin();  // Start the DHT sensor

  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi...");
  // Tries to connect to the WiFi network every 5 seconds until it is successful
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }
//Success message after being connected to WiFi
  Serial.println("Connected!");

  ThingSpeak.begin(client);  // Initialize communication with ThingSpeak
}

void loop() {
  // Reading temperature data comming from sensor
  float temperature = dht.readTemperature();

  // Check if the temperature we get is valid or not, and prints the error message according to that isnan = Not a Number
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the temperature to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Update ThingSpeak field 1 with the temperature data
  ThingSpeak.setField(1, temperature);

  // Write the data to ThingSpeak
  int x = ThingSpeak.writeFields(ChannelID, APIKey);

  if (x == 200) {
    Serial.println("Temperature successfully sent to ThingSpeak.");
  } else {
    Serial.println("Error sending temperature data to ThingSpeak. HTTP error code: " + String(x));
  }

  // Wait 60 seconds before sending data again
  delay(6000);
}
