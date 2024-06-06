//Legger in bibliotek
#include "../lib/Joystick/joystick.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>

// Wifi og MQTT
const char* ssid = "Phone 2";
const char* password = "AgnesErHot21";
const char* mqtt_server = "192.168.38.41";
const char* mqttClientName = "ESP8266Client";
const char* mqttSubscribeTopic = "esp32/output";
const char* mqttPublishTopic = "esp32/kontrollerOutput";
long messageInterval = 100;
// setteropp MQTT
WiFiClient espClient;
PubSubClient client(espClient);
// starte wifi 
void setup_wifi() {
    delay(10);
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
}
// tar imot data som den subscriber på fra MQTT
void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        messageTemp += (char)message[i];
    }
    Serial.println(messageTemp);
}
// kobler seg på MQTT igjen
void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect(mqttClientName)) {
            Serial.println("connected");
            client.subscribe(mqttSubscribeTopic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}
//sender joystick verdier 
void publishJoystickValues() {
    long now = millis();
    if (now - lastMsg > messageInterval) {
        lastMsg = now;
        StaticJsonDocument<256> doc; //PUbSubClient tar maks 256
        doc["leftTrack"] = MappedLeftTrackVal;
        doc["rightTrack"] = MappedRightTrackVal;
        String output;
        serializeJson(doc, output); // setter opp som streng
        client.publish(mqttPublishTopic, output.c_str(), true); // sender
    }
}
// starter opp MQTT og wifi 
void setup() {
    setupJoystick();
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}
// leser
void loop() {
    if (!client.connected()) { // sjekker om vi er koblet til
        reconnect();
    }
    client.loop();
    readAndUpdateJoystick(); //oppdaterer joystick verider
    publishJoystickValues();//sender joystick verider 
}
