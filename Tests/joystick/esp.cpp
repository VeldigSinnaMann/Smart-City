//ESP32 on joystick
#include <Arduino.h>
#include "../lib/Joystick/joystick.h"
#include <PubSubClient.h>
#include <WiFi.h>


// WiFi and MQTT configuration
const char* ssid = "NTNU-IOT";  // WiFi SSID
const char* password = "";  // WiFi passord
const char* mqtt_server = "10.25.17.1";  // MQTT-server IP
const char* mqttClientName = "ESP8266Client"; //Client name for the esp that sends the joystick values
const char* mqttSubscribeTopic = "esp32/output"; // the topic to connect to
const char* mqttPublishTopic = "esp32/kontrollerOutput"; //
long messageInterval = 100;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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

void publishJoystickValues() {
    long now = millis();
    if (now - lastMsg > messageInterval) {
        String payload = String(MappedLeftTrackVal) + "," + String(MappedRightTrackVal);
        client.publish(mqttPublishTopic,payload.c_str() , true);
    }
}

void setup() {
    Serial.begin(115200);
    setupJoystick();
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    readAndUpdateJoystick();
    publishJoystickValues();
}
