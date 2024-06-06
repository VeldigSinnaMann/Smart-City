#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <Arduino.h>

// Deklarer variabler
extern uint8_t LeftTrackPin;
extern uint8_t RightTrackPin;
extern int LeftTrack;
extern int RightTrack;
extern int neutralLeftTrack;
extern int neutralRightTrack;
extern int deadZone;
extern int delayAtZero;
extern int MappedLeftTrackVal;
extern int MappedRightTrackVal;

// Funksjonsdeklarering
void setupJoystick();
void readAndUpdateJoystick();

#endif // JOYSTICK_H
