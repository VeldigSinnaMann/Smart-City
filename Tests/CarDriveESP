#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x04
#define BUFFER_SIZE 4

const char *ssid = "NTNU-IOT";  // WiFi SSID
const char *password = "";  // WiFi password
const char *mqtt_server = "10.25.17.1";  // MQTT-server IP
const char *mqtt_topic_input = "zumo/kontrollerInput";  // MQTT topic for input
const char *mqtt_topic_output = "zumo/accelerationData";  // MQTT topic for output

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMillis = 0;

void send_i2c_data(int16_t leftMotor, int16_t rightMotor);
void receive_i2c_data();
void setup_wifi();
void reconnect();
void callback(char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    Wire.begin();
}

void loop() {
    if (!client.connected()) {
        unsigned long currentMillis = millis();
        if (currentMillis - lastMillis >= 5000) {
            lastMillis = currentMillis;
            reconnect();
        }
    }
    client.loop();

    // Receive I2C data every 60 seconds
    if (millis() - lastMillis >= 60000) {
        receive_i2c_data();
        lastMillis = millis();
    }
}

void send_i2c_data(int16_t leftMotor, int16_t rightMotor) {
    Wire.beginTransmission(I2C_SLAVE_ADDRESS);
    Wire.write(lowByte(leftMotor));
    Wire.write(highByte(leftMotor));
    Wire.write(lowByte(rightMotor));
    Wire.write(highByte(rightMotor));
    Wire.endTransmission();

    // Print the data sent via I2C for debugging
    Serial.print("Sent via I2C - Left Motor: ");
    Serial.print(leftMotor);
    Serial.print(", Right Motor: ");
    Serial.println(rightMotor);
}

void receive_i2c_data() {
    Wire.requestFrom(I2C_SLAVE_ADDRESS, BUFFER_SIZE);
    float avgAcceleration;
    Wire.readBytes((char*)&avgAcceleration, sizeof(avgAcceleration));

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.2f", avgAcceleration);
    client.publish(mqtt_topic_output, buffer);

    // Print the data received via I2C for debugging
    Serial.print("Received via I2C - Average Acceleration: ");
    Serial.println(avgAcceleration);
}

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

void reconnect() {
    if (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(mqtt_topic_input);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void callback(char *topic, byte *payload, unsigned int length) {
    char *message = new char[length + 1];
    for (unsigned int i = 0; i < length; i++) {
        message[i] = (char)payload[i];
    }
    message[length] = '\0';

    // Convert message to integer values
    int16_t leftMotor = atoi(strtok(message, ","));
    int16_t rightMotor = atoi(strtok(NULL, ","));

    // Ensure values are within range
    leftMotor = constrain(leftMotor, 0, 800);
    rightMotor = constrain(rightMotor, 0, 800);

    // Now send these values with I2C
    send_i2c_data(leftMotor, rightMotor);

    delete[] message;
}
