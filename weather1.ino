#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
//-------- Customise these values -----------
const char* ssid = "sdf";
const char* password = "123456789";
//String command;
#include "DHT.h"
#define DHTPIN D2    // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);
 
#define ORG "nbwv9o"
#define DEVICE_TYPE "IOT"
#define DEVICE_ID "shabeer123"
#define TOKEN "7386183786aA"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

void setup() {
 Serial.begin(115200);
 Serial.println();
 dht.begin();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}
 
void loop() {
  int light=analogRead(A0);
  Serial.println(light);
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t) || isnan(light))
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}
PublishData(t,h,light);
 if (!client.loop()) {
    mqttConnect();
  }
delay(100);
}
void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void PublishData(float temp, float humid, float light){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += temp;
  payload+="," "\"humidity\":";
  payload += humid;
  payload += "," "\"light\":";
 
  payload +=light;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
