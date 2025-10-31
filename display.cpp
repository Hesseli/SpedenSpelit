#include <Arduino.h>
#include "display.h"


int dataPin = 8;      // Shift rekisterin pinni 14. DS. tänne syötetään data bitti kerrallaan
int outPutEnable = 9; // Shift rekisterin pinni 13. OE. active low, antaa ulostuloja läpi, kun 0. pidetään nollassa
int latchPin = 10;  // Shift rekisterin pinni 12. ST_CP. latch. lähettää datan ulostulopinneihin. pidetään nollassa, kunnes kaikki bitit on luettu
int clockPin = 11;  // Shift rekisterin pinni 11. SH_CP. shift clock ohjaa , milloin data siirtyy siirtorekisterissä seuraavalle paikalle
int reset = 12;    // Shift rekisterin pinni 10. master reset. active low, eli pidetään high tilassa kunnes halutaan resetoida


byte numerot[10] = {  // 7-segmentille numeroiden tulostukseen tarvittavat tavut MSB- muodossa. Vasemmalta oikealle pinnit A-G

B11111100, // 0 
B01100000, // 1
B11011010, // 2
B11110010, // 3
B01100110, // 4
B10110110, // 5
B00111110, // 6
B11100000, // 7
B11111110, // 8
B11100110  // 9
};


void initializeDisplay(){    // funktiossa laitetaan pinnit output tilaan

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(outPutEnable, OUTPUT);
  pinMode(reset, OUTPUT);
}


void showResults(uint8_t numero){    // funktiolle annetaan luku, josta erotellaan ykköset, kymmenet ja sadat. Koska muuttuja ona muotoa uint8_t (8 bittinen), suurin luku joka voidaan esittää on 255
 uint8_t ones = numero % 10;        // jakojäännös kymmenestä antaa ykköset
 uint8_t tens = (numero % 100) / 10;  // jakojäännös sadasta jaettuna kymmenellä antaa kymmenet
 uint8_t hundreds = numero / 100;     // jako sadalla antaa sadat

 displayNumber(hundreds, tens, ones);  // kutsutaan tulostusfunktio, jolle annetaan parametreinä erotellut luvut
}


void displayNumber(uint8_t hundreds, uint8_t tens, uint8_t ones) { 
  
  uint8_t onesDigit = numerot[ones];  // otetaan taulukosta tulostettavaa lukua vastaava tavu ja määritetään se muuttujaan
  uint8_t tensDigit = numerot[tens];
  uint8_t hundredsDigit = numerot[hundreds];
  digitalWrite(outPutEnable, LOW);            // outputit päälle
  digitalWrite(reset, HIGH);                 // reset pois
  digitalWrite(latchPin, LOW);              // latch pinni alas tiedonsiirron ajaksi


  for(uint8_t i = 0; i<8;i++){         // for loopissa verrataan tavun bittejä bittimaskiin, jotta saadaan tavut syötettyä datapinniin bitti kerrallaan
    uint8_t maski = B00000001;
    if((hundredsDigit & (maski << i)) > 0){    // maskivertailu & operaatiolla. 1 & 1 = 1. 1 & 0 = 0. Shiftataan maskia vasemmalle, kunnes tavun kaikki bitit on luettu
       digitalWrite(dataPin, 1);
    }
    
    else{
      digitalWrite(dataPin, 0);
    }
    digitalWrite(clockPin, HIGH); // nostetaan ja lasketaan kellopinni. Data siirtyy output pinneissä eteenpäin kellopulssin laskevalla reunalla
    digitalWrite(clockPin, LOW);
    }
  
  ////////////////////////////////////////////   Toistetaan jokaiselle luvulle
   
  for(uint8_t i = 0; i<8;i++){
    uint8_t maski = B00000001;
    if((tensDigit & (maski << i)) > 0){
  
       digitalWrite(dataPin, 1);
    
    }
    else{
      digitalWrite(dataPin, 0);
    }
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
   }
  ///////////////////////////////////////
    
  for(uint8_t i = 0; i<8;i++){
    uint8_t maski = B00000001;
    if((onesDigit & (maski << i)) > 0){
  
     	digitalWrite(dataPin, 1);
    
    }
    else{
      digitalWrite(dataPin, 0);
    }
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
   }
   
  digitalWrite(latchPin, HIGH); // latch pinni ylös ja data lähtee output pinneistä ulos
}
