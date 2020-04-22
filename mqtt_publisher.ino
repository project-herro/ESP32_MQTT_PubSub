
#include <WiFi.h>
#include <MQTTClient.h>

// SSID, PASSWORD
const char ssid[] = "XXX";
const char pass[] = "YYY";

int lpcnt=0 ;

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    lpcnt +=1 ;
    if (lpcnt>10) { ESP.restart(); }
  }

  // shiftr "key", "Secret"
  Serial.print("\nconnecting...");
  while (!client.connect("ESP32_pub", "KEY", "SECRET")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
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
  connect();
}


void loop() {
  client.loop();
  delay(10);
  if (!client.connected()) {
    connect();
  }
  if (millis() - lastMillis > 3000) {
    lastMillis = millis();
    client.publish("/hello", "world");
  }
  client.onMessage(messageReceived);
}
