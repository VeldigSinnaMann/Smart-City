#include <Wire.h>
#include <Arduino.h>
#include <Zumo32U4.h>
#include <ArduinoJson.h>

#define I2C_SLAVE_ADDRESS 0x04
#define I2C_BUFFER_SIZE 512
#define JSON_BUF_SIZE 1024

Zumo32U4Motors motors;
void receiveEvent(int howMany);

void setup() {
    Wire.begin(I2C_SLAVE_ADDRESS);
    Wire.onReceive(receiveEvent);
}

void loop() {
    // Add your main code logic here if necessary
}

void receiveEvent(int howMany) {
    char i2c_buffer[I2C_BUFFER_SIZE];
    int i = 0;
    while (Wire.available() && i < I2C_BUFFER_SIZE - 1) {
        i2c_buffer[i] = Wire.read();
        i++;
    }
    i2c_buffer[i] = '\0';

    DynamicJsonDocument doc(JSON_BUF_SIZE);
    DeserializationError error = deserializeJson(doc, i2c_buffer);
    if (error) {
        // handle the error according to the application
        return;
    }
    if (!(doc.containsKey("leftRight") && doc.containsKey("upDown"))) {
        // handle the error according to the application
        return;
    }

    int leftMotor = doc["leftRight"];
    int rightMotor = doc["upDown"];

    motors.setSpeeds(leftMotor, rightMotor);
}
