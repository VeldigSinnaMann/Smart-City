//Imorterer nødvendige bibliotek
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
//setter slave adresse
#define I2C_SLAVE_ADDRESS 0x04
//Erkærer wifi og mqtt konstanter
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "192.168.95.41";
const char* mqtt_topic = "zumo/kontrollerInput";

//starter pubsub og wifi
WiFiClient espClient;
PubSubClient client(espClient);

// millis for delay i loop
unsigned long lastMillis = 0;
// kode for å sende til zumo
void send_i2c_data(const char* data) {
    Wire.beginTransmission(I2C_SLAVE_ADDRESS);
    Wire.write(data);
    Wire.endTransmission();
}
// oppstart av wifi
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
//for å koble seg tilbake på Nodre-RED
void reconnect() {
    if (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(mqtt_topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
        }
    }
}

// tar imot data fra Node-RED
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String mqtt_data;
    for (int i = 0; i < length; i++) {
        mqtt_data += (char)payload[i];
    }
    Serial.println(mqtt_data);
    send_i2c_data(mqtt_data.c_str());
}
// setup der vi setter Serial wifi og starter kobling til Node-RED
void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    Wire.begin();
}
// sjekker om vi er koblet til Node-RED
void loop() {
    if (!client.connected()) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastMillis >= 5000) {
            lastMillis = currentMillis;
            reconnect();
        }
    }
    client.loop();
}
