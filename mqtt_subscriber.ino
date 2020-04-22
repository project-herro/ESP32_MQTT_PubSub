
#include <WiFi.h>
#include <MQTTClient.h>

// SSID, PASSWORD
const char ssid[] = "XXX";
const char pass[] = "YYY";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  // shiftr "key", "Secret"
  Serial.print("\nconnecting...");
  while (!client.connect("ESP32_sub", "KEY", "SECRET")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  client.subscribe("/hello");
}


void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  delay(10);
}


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  
  // broker
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}


void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }
  client.onMessage(messageReceived);
}
