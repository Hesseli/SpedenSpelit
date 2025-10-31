#include "SpedenSpelit.h"
#include "buttons.h"
#include "sounds.h"

volatile const int _btns[4] = { 2, 3, 4, 5 };
volatile bool _btnIsPressed[4] = {false, false, false, false};
volatile int buttonPressed = -1; // -1 toimii merkkausarvona, että mikään nappi ei ole aktiivinen aluksi
volatile const long interruptTreshold = 300;
unsigned long interruptTime = 0;

volatile bool cheatCodeInProgress = false; // Huijauskoodin tila
volatile const int cheatCode[5] = {1, 0, 2, 0, 3}; // Huijauskoodi. Näillä viitataan napin indexiin, ei pinneihin
volatile int cheatCodeIndex = 0;  // Seurataan, mihin kohtaan huijauskoodia ollaan menossa

void initButtonsAndButtonInterrupts() {
  for (int i = 0; i < 4; i++) {
    pinMode(_btns[i], INPUT_PULLUP);
  }
  PCICR = B00000100;
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21);

  interrupts();
}

ISR(PCINT2_vect) {
  unsigned long debounceTimer = millis();

  if (debounceTimer - interruptTime > interruptTreshold) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(_btns[i]) == LOW && !_btnIsPressed[i]) {
        _btnIsPressed[i] = true;
        interruptTime = debounceTimer;
        buttonPressed = i;

        Serial.print("Button pressed: ");
        Serial.println(i);

        // Huijauskoodin tarkistus
        if (!cheatModeActive) { 
          if (cheatCode[cheatCodeIndex] == i) {
            // Oikea nappi huijauskoodissa
            cheatCodeIndex++;
            Serial.print("Cheat Code Index: ");
            Serial.println(cheatCodeIndex);

            if (cheatCodeIndex == 5) { // Huijauskoodi valmis
              cheatModeActive = true;
              cheatCodeIndex = 0;
              Serial.println("Cheat mode activated!");
            }
          } 
          // Väärä nappi huijauskoodissa
          else if (cheatCodeIndex > 0) {
            Serial.println("Wrong button! Cheat code reset.");
            cheatCodeIndex = 0; 
          }
        }
          timeToCheckGameStatus = true; // Tämä asetetaan vain normaalitilassa
      } 
      // Vapautetaan nappi
      else if (digitalRead(_btns[i]) == HIGH) {
        _btnIsPressed[i] = false;
      }
    }
  }
}


