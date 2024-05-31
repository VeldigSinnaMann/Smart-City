#include "Joystick.h"

uint8_t LeftTrackPin = 36;
uint8_t RightTrackPin = 39;
int LeftTrack = 0;
int RightTrack = 0;
int prevDirLeftTrackVal = 1;
int prevDirRightTrackVal = 1;
int neutralLeftTrack = 0;
int neutralRightTrack = 0;
int deadZone = 0.07 * 4095 / 2;  // 7% of range, but this will be relative to neutral position
int DirLeftTrackVal = 0;
int DirRightTrackVal = 0;
int MappedLeftTrackVal = 0;
int MappedRightTrackVal = 0;

unsigned long previousMillis = 0;
const long interval = 300;  // Interval for updating joystick values

void setupJoystick() {
    Serial.begin(115200);
    pinMode(LeftTrackPin, INPUT);
    pinMode(RightTrackPin, INPUT);

    // Determine neutral position for each axis. Adjust as needed.
    for (int i = 0; i < 100; i++) {
        neutralLeftTrack += analogRead(LeftTrackPin);
        neutralRightTrack += analogRead(RightTrackPin);
        delay(1);
    }

    neutralLeftTrack /= 100;
    neutralRightTrack /= 100;
}

void readAndUpdateJoystick() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        LeftTrack = analogRead(LeftTrackPin);
        RightTrack = analogRead(RightTrackPin);

        DirLeftTrackVal = (LeftTrack > neutralLeftTrack + deadZone) - (LeftTrack < neutralLeftTrack - deadZone);
        DirRightTrackVal = (RightTrack > neutralRightTrack + deadZone) - (RightTrack < neutralRightTrack - deadZone);

        int newMappedLeftTrackVal = (abs(LeftTrack - neutralLeftTrack) < deadZone) ? 0 : map(LeftTrack, 0, 4095, -400, 400);
        int newMappedRightTrackVal = (abs(RightTrack - neutralRightTrack) < deadZone) ? 0 : map(RightTrack, 0, 4095, -400, 400);

        if (DirLeftTrackVal != prevDirLeftTrackVal && newMappedLeftTrackVal != 0) {
            newMappedLeftTrackVal = 0;
            previousMillis = currentMillis; // Reset the delay timer
        }

        if (DirRightTrackVal != prevDirRightTrackVal && newMappedRightTrackVal != 0) {
            newMappedRightTrackVal = 0;
            previousMillis = currentMillis; // Reset the delay timer
        }

        MappedLeftTrackVal = newMappedLeftTrackVal + 400; // Adjusting to range 0-800
        MappedRightTrackVal = newMappedRightTrackVal + 400; // Adjusting to range 0-800

        prevDirLeftTrackVal = DirLeftTrackVal;
        prevDirRightTrackVal = DirRightTrackVal;

        // Debug output
        Serial.print("LeftTrack: "); Serial.print(LeftTrack);
        Serial.print(" MappedLeftTrackVal: "); Serial.print(MappedLeftTrackVal);
        Serial.print(" RightTrack: "); Serial.print(RightTrack);
        Serial.print(" MappedRightTrackVal: "); Serial.println(MappedRightTrackVal);
    }
}
