#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

const byte firstPin = 2; // First PinChangeInterrupt on D-bus
const byte lastPin =  5; // Last PinChangeInterrupt on D-bus

extern volatile int buttonPressed;

extern volatile bool timeToCheckGameStatus;
extern volatile bool cheatModeActive;

// Alustaa kytkimet ja keskeytykset
void initButtonsAndButtonInterrupts(void);
ISR(PCINT0_vect);
#endif