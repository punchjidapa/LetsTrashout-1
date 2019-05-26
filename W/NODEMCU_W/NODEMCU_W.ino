#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <MQTTClient.h>

SoftwareSerial UNOSerial(D6, D5);

//const char* ssid = "iPhone";
//const char* password = "punchpunch5";

const char* ssid = "PUNCH";
const char* password = "punchjida";

WiFiClient WiFiclient;
MQTTClient client;

unsigned long lastMillis = 0;

uint16_t analogData = 0;
String positionData = "";

void setup() {

  pinMode(A0, INPUT);
  Serial.begin(9600);
  UNOSerial.begin(9600);
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
  client.begin("postman.cloudmqtt.com", 17323, WiFiclient);
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
    analogData = map(analogData, 232, 1023, 0, 100); //////measure bin///////try to reduce*
    //positionData = random(0, 3);
    Serial.print("IR : ");
    Serial.println(analogData);
    client.publish("/sensor/level", "L" + (String)analogData);
  }
  
  UNOSerial.write("s");
  if (UNOSerial.available() > 0) {
    positionData = UNOSerial.read();
    Serial.print("Position : ");
    Serial.println(positionData);
    Serial.print("Type : ");
    if (positionData == "1") {
      Serial.println("Plastic bottle");
      }
     if (positionData == "2") {
      Serial.println("Glass");
      }
     if (positionData == "3") {
      Serial.println("Carton");
      }
     if (positionData == "4") {
      Serial.println("Can");
      }
    client.publish("/sensor/position", "P" + positionData);
  }
 // delay(10);
 delay(500);
}
