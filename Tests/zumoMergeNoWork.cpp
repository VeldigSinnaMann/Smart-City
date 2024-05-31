#include <Wire.h>
#include <Arduino.h>
#include <Zumo32U4.h>

#define I2C_SLAVE_ADDRESS 0x04
#define UPDATE_INTERVAL 60000 // Send data every 60 seconds

Zumo32U4Motors motors;
Zumo32U4IMU imu;

const float conversion_factor = 0.061; // [mg/LSB]
const float g = 9.81; // [m/s^2]
float accelerations_m_s2[3];

// Variables for calculating average acceleration
const int bufferLength = 60;
float accelerationBuffer[bufferLength];
float accelXBuffer[bufferLength];
float accelYBuffer[bufferLength];
int bufferIndex = 0;
unsigned long lastUpdateTime = 0;

void receiveEvent(int howMany);
void requestEvent();
void updateAccelerationBuffer(float newXAcceleration, float newYAcceleration, float newAcceleration);
float calculateAverageAccel();

void setup() {
    Serial.begin(115200); // For debugging purposes
    Wire.begin(I2C_SLAVE_ADDRESS);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    if (!imu.init()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
    imu.enableDefault();

    memset(accelerationBuffer, 0, sizeof(accelerationBuffer));
}

void loop() {
    if (millis() - lastUpdateTime >= UPDATE_INTERVAL) {
        imu.read();
        accelerations_m_s2[0] = imu.a.x * conversion_factor / 1000 * g;
        accelerations_m_s2[1] = imu.a.y * conversion_factor / 1000 * g;
        accelerations_m_s2[2] = sqrtf((accelerations_m_s2[0] * accelerations_m_s2[0]) + (accelerations_m_s2[1] * accelerations_m_s2[1]));

        updateAccelerationBuffer(accelerations_m_s2[0], accelerations_m_s2[1], accelerations_m_s2[2]);
        lastUpdateTime = millis();
    }
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

void requestEvent() {
    float avgA = calculateAverageAccel(); // Calculate average acceleration
    Wire.write((uint8_t*)&avgA, sizeof(avgA)); // Send the average total acceleration
}

void updateAccelerationBuffer(float newXAcceleration, float newYAcceleration, float newAcceleration) {
    accelXBuffer[bufferIndex] = newXAcceleration;
    accelYBuffer[bufferIndex] = newYAcceleration;
    accelerationBuffer[bufferIndex] = newAcceleration;
    bufferIndex = (bufferIndex + 1) % bufferLength; // Increment buffer index, wrap around if necessary
}

float calculateAverageAccel() {
    float sumA = 0;
    for (int i = 0; i < bufferLength; i++) {
        sumA += accelerationBuffer[i];
    }
    return sumA / bufferLength; // Return the average total acceleration
}
