#include <Wire.h>
#include <Arduino.h>
#include <Zumo32U4.h>

#define I2C_SLAVE_ADDRESS 0x04

Zumo32U4Motors motors;

void receiveEvent(int howMany);

void setup() {
    Serial.begin(115200); // For debugging purposes
    Wire.begin(I2C_SLAVE_ADDRESS);
    Wire.onReceive(receiveEvent);
}

void loop() {
    // Add your main code logic here if necessary
}

void receiveEvent(int howMany) {
    if (howMany < 4) return; // Ensure at least four bytes are received

    int16_t leftMotor, rightMotor;

    // Read left motor speed
    uint8_t leftLowByte = Wire.read();
    uint8_t leftHighByte = Wire.read();
    leftMotor = (leftHighByte << 8) | leftLowByte;

    // Read right motor speed
    uint8_t rightLowByte = Wire.read();
    uint8_t rightHighByte = Wire.read();
    rightMotor = (rightHighByte << 8) | rightLowByte;

    // Convert the range from 0-800 to -400 to 400 by subtracting 400
    leftMotor -= 400;
    rightMotor -= 400;

    Serial.print("Setting left motor speed to: ");
    Serial.println(leftMotor); // Debugging motor speed
    Serial.print("Setting right motor speed to: ");
    Serial.println(rightMotor); // Debugging motor speed

    motors.setSpeeds(leftMotor, rightMotor);
}
