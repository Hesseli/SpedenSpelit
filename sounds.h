// sounds.h
#include <Arduino.h>
#ifndef SOUNDS_H
#define SOUNDS_H

extern const int tokyoDriftDurations[];
extern const int tokyoDriftDurationsSize; 

// Funktioiden julkiset ilmoitukset
void playTokyoDriftTone(); // Funktion TokyoDriftin soittamiseen voiton merkiksi
void playSeptemberTone(); // Funktion Septemberin soittamiseen
void playMockingTone(); // Funktion Mockingin soittamiseen

#endif
