#include <ESP8266WiFi.h>
#include <MQTTClient.h>

const char* ssid = "ModDevLab_2.4GHz";
const char* password = "3313033130A*";

WiFiClient WiFiclient;
MQTTClient client;

unsigned long lastMillis = 0;

uint16_t analogData = 0;

void setup() {
  
  Serial.begin(115200);
  pinMode(A0, INPUT);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);

  Serial.print("connecting to MQTT broker...");
  client.begin("m16.cloudmqtt.com", 16629, WiFiclient);
  connect();
  
}

void connect() {
  while (!client.connect("ESP8266Client", "Node1", "aabbccdd")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");
  client.subscribe("/sensor/level");
}

void loop() {
  
  client.loop();
  if (!client.connected()) {
    connect();
  }

  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    analogData = analogRead(A0);
    analogData = map(analogData, 0, 1023, 0, 100);
    Serial.println(analogData);
    client.publish("/sensor/level", (String)analogData);
  }
  
}
