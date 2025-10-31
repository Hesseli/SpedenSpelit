#include "leds.h"
#include "sounds.h"

const byte ledPins[] = {A2, A3, A4, A5};
const byte numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

void setupLeds(void) {
    for (byte i = 0; i < numLeds; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }
}

void setLed(byte ledNumber) {
    if (ledNumber < numLeds) {
        clearAllLeds();
        digitalWrite(ledPins[ledNumber], HIGH);
    }
}

void clearAllLeds(void) {
    for (byte i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], LOW);
    }
}

void setAllLeds() {
    for (byte i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], HIGH);
    }
}

void show1()
{
  unsigned long currentMillis = millis();
  for(int i = 0;i<=15;i++) // Käy läpi luvut 0-15
  {
    for(int j =0;j<4;j++) // Käy läpi jokaisen ledin
    {
      if(i &(1<<j)) // Kasvatetaan binäärilukua
      {
        
        digitalWrite(ledPins[j], HIGH);
      }
      else
      {
        digitalWrite(ledPins[j], LOW);
      }
      if(timeToCheckGameStatus) // Jos nappia painetaan show1:n ollessa kaynnissa, sammutetaan kaikki ledit ja lopetetaan show1 keskenkaiken return-komennolla
      {
        timeToCheckGameStatus = false;
        clearAllLeds();
        return; 
      }
    }
    delay(500);
  }
  clearAllLeds();
}


void show2(int repeats) {
  
  int ledOrder[] = {4, 4, 4, 4, 3, 2, 1}; 
  int ledOrderSize = sizeof(ledOrder) / sizeof(int);

  for (int repeat = 0; repeat < tokyoDriftDurationsSize; repeat++) {
    int ledToLight = ledOrder[repeat % ledOrderSize] - 1; // Muutetaan LEDin numero indeksiksi

    // Sytytetään ja sammutetaan LED melodian tahtiin
    int tokyoDriftDuration = 1000 / tokyoDriftDurations[repeat];
    setLed(ledToLight);            // Sytytä LED
    delay(tokyoDriftDuration / 2); // Viive, puolikas nuotti
    clearAllLeds();                // Sammuta LED
    delay(tokyoDriftDuration / 2); // Viive, puolikas nuotti

    // Tarkistetaan, jos peliin liittyvä nappi on painettu
    if (timeToCheckGameStatus) {
      timeToCheckGameStatus = false;
      clearAllLeds();
      return;
    }
  }
}