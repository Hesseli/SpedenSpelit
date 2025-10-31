#include "sounds.h"
#include "pitches.h"
#define BUZZER_PIN 6

/*int septemberMelody[] = { // September melodia
  NOTE_FS4, NOTE_A4, NOTE_B4, NOTE_CS5, NOTE_CS5, 0, 0, 0,
  NOTE_CS5, NOTE_CS5, NOTE_E5, NOTE_FS5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_CS5, NOTE_CS5
};

int septemberNoteDurations[] = { // September nuottien pituudet
  8, 8, 8, 8, 8, 4, 4, 8, 
  8, 8, 8, 8, 8, 8, 8, 8, 8
};*/

int tokyoDriftMelody[] = { // TokyoDriftin melodia
  NOTE_AS4, REST, NOTE_AS4, REST, NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_F5, REST, NOTE_F5, REST,
  NOTE_GS5, NOTE_FS5, NOTE_F5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_GS5, NOTE_FS5, NOTE_F5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  NOTE_AS4, NOTE_B4, NOTE_DS5,
  NOTE_AS4, REST, NOTE_AS4, REST,
  REST
};

const int tokyoDriftDurations[] = { // TokyoDriftin nuottien pituudet
  4, 4, 4, 4, 4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  3, 3, 4,
  4, 4, 4, 4,
  1
};

const int tokyoDriftDurationsSize = sizeof(tokyoDriftDurations) / sizeof(int); // Lasketaan nuottien määrä taulukon koon perusteella. Tätä käytetään leds.cpp tiedostossa löytyvässä show2, jossa välkytellään ledejä TokyoDriftin tahtiin 

/*void playSeptemberTone() {
  for (int septemberNote = 0; septemberNote < 17; septemberNote++) { // Toistaa Septemberin kappaleen nuotit
    int septemberNoteDuration = 1000 / septemberNoteDurations[septemberNote]; // Laskee nuotin keston

    tone(BUZZER_PIN, septemberMelody[septemberNote], septemberNoteDuration); // Soittaa nuotin
    int pauseBetweenSeptemberNotes = septemberNoteDuration * 1.70; // Laskee tauon keston nuottien välillä
    delay(pauseBetweenSeptemberNotes); // Odottaa ennen seuraavaa nuottia
    noTone(BUZZER_PIN); // Lopettaa nykyisen nuotin soittamisen
  }
}*/

void playTokyoDriftTone() { // Tätä funktiota käytetään voitto ääniefektin soittamiseen, kun käyttäjä saavuttaa maksimi pisteet pelissä (255)
  for (int tokyoDriftNote = 0; tokyoDriftNote < tokyoDriftDurationsSize; tokyoDriftNote++) { // Toistaa TokyoDrift kappaleen nuotit
    int tokyoDriftDuration = 1000 / tokyoDriftDurations[tokyoDriftNote]; // Laskee nuotin keston

    tone(BUZZER_PIN, tokyoDriftMelody[tokyoDriftNote], tokyoDriftDuration); // Soittaa nuotin
    int pauseBetweenTokyoDriftNotes = tokyoDriftDuration * 1.30; // Laskee tauon keston nuottien välillä
    delay(pauseBetweenTokyoDriftNotes); // Odottaa ennen seuraavaa nuottia
    noTone(BUZZER_PIN); // Lopettaa nykyisen nuotin soittamisen
  }
}

void playMockingTone() { // Tätä funktiota käytetään häviämis ääniefektin soittamiseen, kun käyttäjä painaa väärää painiketta tai on jälkeenjäänyt
  tone(BUZZER_PIN, 190, 440); // Soittaa 190 Hz nuotin 440 ms ajan
  delay(340); // Odottaa 340ms

  tone(BUZZER_PIN, 160, 440); // Soittaa 160 Hz nuotin 440 ms ajan
  delay(340); // Odottaa 340ms

  tone(BUZZER_PIN, 110, 800); // Soittaa 110 Hz nuotin 800 ms ajan
  delay(700); // Odottaa 700ms

  noTone(BUZZER_PIN); // Lopettaa nykyisen nuotin soittamisen
}



