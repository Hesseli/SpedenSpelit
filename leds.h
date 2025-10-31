#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>

extern volatile bool timeToCheckGameStatus;

// Funktioiden esittelyt
void setupLeds(void);
void setLed(byte ledNumber);
void clearAllLeds(void);
void setAllLeds(void);
void show1(void);
void show2(int);
#endif
