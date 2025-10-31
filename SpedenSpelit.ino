#include <Arduino.h>
#include "buttons.h"
#include "display.h"
#include "leds.h"
#include "sounds.h"

volatile bool newTimerInterrupt = false; // Käytetään timerin keskeytysten hallintaan
volatile bool timeToCheckGameStatus = false; // Buttons-moodulin ISR keskeytypalvelin muuttaa tilaksi true, kun nappia painetaan. Void loop tarkistaa joka kierroksella, onko arvo true
volatile int userNumbers[255];  // Käyttäjän painamat numerot, jotka vastaavat painikkeita
volatile int randomNumbers[255]; // Satunnaiset numerot, jotka määrittävät syttyvät ledit

int timerInterruptionCounter = 0; // Indeksimuuttuja, jonka arvo kasvaa joka kerta, kun timer keskeytys tapahtuu. Muuttujan avulla voi tallentaa randomNumbers[] -tauluun peräkkäisiin alkioihin arvoja
int currentScore = 0; // Pelin sen hetkinen tulos, joka näytetään 7-segmenteissä
bool gameStarted = false; // Muuttujan avulla ohjataan, milloin peli on päällä ja milloin valmiustilassa. Void loop() tarkistaa joka kierroksella, onko muuttujan arvo false. gameStarted = false = valmiustila
int buttonInterruptionsCounter = 0; // Muuttuja jolla pidetään kirjaa painallusten lukumäärästä
const int defaultOutCompRegValue = 15624; // Vastaa yhden hertsin keskeytystaajuutta
volatile int timerSpeedCounter = 0; // Tällä seurataan pelin nopeutta
int timerSpeed = defaultOutCompRegValue; // Käytetään nopeuttamaan pelin nopeutta. timerSpeed annetaan initialize() -funktiolla argumentiksi

volatile bool cheatModeActive = false; // Muuttuja, joka kertoo onko huijausmoodi aktivoitu

void setup() {
  Serial.begin(9600);
  setupLeds();     // Ledmoduuli
  initializeDisplay();  // Näyttömoduuli
  initButtonsAndButtonInterrupts(); // Painikkeet ja keskeytykset
  showResults(currentScore); // Kytketään nykyinen tulos 7-segmenteille heti kun virrat kytketään
  show2(5); // Kun ohjelma uploadataan Arduinolle, naytetaan ledeilla lyhyt valoshow
  initializeGame(); // Alustetaan pelissä käytettävät muuttujat ja taulukot
  gameStarted = false; // Aloitetaan valmiustilassa
  cheatModeActive = false;
}

void loop() {
  while (!gameStarted) { // Tarkistetaan onko peli valmiustilassa
    showReadyState(); // Näytetään että ollaan valmiustilassa, vilkuttamalla ledejä
    
    if (timeToCheckGameStatus) { // Napin painallus havaittu
      timeToCheckGameStatus = false; // Nollataan tila
      if (cheatModeActive) { // Tarkistetaan huijaustila
        if (buttonPressed == 3) { // Jos oikea nappi painettu
          for (int i = 3; i > 0; i--) { // Countdown helpottamaan aloittamista
            showResults(i);
            delay(500);
          }
          startTheCheatGame(); // Käynnistetään peli huijaustilassa
        }
      } else if (buttonPressed == 3) { // Jos huijaustila ei ole aktiivinen
        userNumbers[3] = 4;
        for (int i = 3; i > 0; i--) { // Countdown helpottamaan aloittamista
          showResults(i);
          delay(500);
        }
        startTheGame(); // Käynnistää tavallisen pelin
      }
    }
  }

  if (newTimerInterrupt) { // Timer-keskeytyspalvelija on muuttanut newTimerInterrupt arvoon true
    randomNumbers[timerInterruptionCounter] = random(0,4); // Arvo luku välillä 0-3 (byte) ja tallenna listaan randomNumbers, kohtaan timerInterruptionCounter
    if (randomNumbers[timerInterruptionCounter] == randomNumbers[timerInterruptionCounter - 1]) { // Tässä haarassa sammutetaan ledit jos arvonta osuu samaan lediin useamman kerran
      clearAllLeds(); // Sammutetaan ledit
      delay(200); // Pieni viive ledien välissä
    }
    setLed(randomNumbers[timerInterruptionCounter]); // Sytytetään arvottu ledi
    timerInterruptionCounter++; // Lisätään muuttujaan +1, jotta seuraavalla kierroksella ei kirjoiteta edellisen päälle
    if (timerInterruptionCounter % 10 == 0) { // Nopeutetaan peliä joka 10 painalluksen jälkeen
      //Serial.println("Peli nopeutuu!"); // Debugaus
      timerSpeed = max(timerSpeed / 1.1, 0.2 * defaultOutCompRegValue); // Nopeutetaan peliä 10%, mutta ei alle 200ms
      initializeTimer(timerSpeed); // Alustetaan ajastin nykyisellä pelinopeudella (timerSpeed), joka määrittää ledien vilkkumisen ja keskeytyvälit
      //Serial.print("Uusi timer-nopeus: "); // Debugaus
      //Serial.println(timerSpeed); // Debugaus
    }
    newTimerInterrupt = false; // Muutetaan newTimerInterrupt tilaan false
  }
  if (timeToCheckGameStatus) { // Napin painallus havaittu
    userNumbers[buttonInterruptionsCounter] = buttonPressed; // Tallennetaan painetun napin numero userNumbers listaan kohtaan buttonInterrptionsCounter
    checkGame(buttonPressed); // Kutsutaan checkGame-funktio tarkistamaan painalluksen oikeellisuus
    buttonInterruptionsCounter++; // Lisätään muuttujaan +1, jotta seuraavalla kierroksella ei kirjoiteta edellisen päälle
    timeToCheckGameStatus = false; // Muutetaan timeToCheckGameStatus tilaan false
  }
  if ((timerInterruptionCounter - buttonInterruptionsCounter) > 10) { // Jos käyttäjä jäi 10 lediä jälkeen mennään haaraan jossa häviää pelin
    //Serial.println("Painoit liian myöhässä, hävisit pelin!"); // Debugausta
    showResults(currentScore); // Näytetään 7-segmenttinäytöllä käyttäjän tulos
    playMockingTone(); // Soitetaan playMockingTone häviämisen merkiksi
    clearAllLeds(); // Sammutetaan kaikki ledit
    stopTheGame(); // pysäytetään peli
  }
}

void showReadyState() { // Funktio valmiustilalle
  static unsigned long lastBlinkTime = 0; // Viimeisin aika jolloin ledit vaihtoi tilaa
  static bool ledsOn = false; // Muuttuja joka tallentaa ledien tilan päällä/pois
  unsigned long currentMillis = millis(); // Tallennetaan nykyinen tila millisekunteina
  initializeGame();
  if (currentMillis - lastBlinkTime >= 800) { // Jos edellisestä vilkkumisesta on vähintään 800ms mennään tähän haaraan
    if (ledsOn) { // Jos ledit on päällä mennän tähän
      clearAllLeds(); // Sammuttaa ledit
    } else { // Jos ledit on pois päältä mennään tähän
      setAllLeds(); // Sytytä kaikki ledit
    }
    ledsOn = !ledsOn; // Vaihdetaan tila päällä --> pois päältä tai päinvastoin
    lastBlinkTime = currentMillis; // Päivitetään viimeisin vilkkumisaika nykyhetkeen
  }
}

void startTheCheatGame() {
  cheatModeActive = true;
  startTheGame();
  // Näytetään huijausmoodi on aktivoitu
  Serial.println("Cheat Mode Activated!");
}

void checkGame(byte nbrOfButtonPush ) { // checkGame funktio tarkistaa vastasiko painettu nappi odotettua
  if (randomNumbers[currentScore] == nbrOfButtonPush) { // Jos painallus on oikea mennään tähän haaraan
    //Serial.println("Oikea nappi!");  // Debugaus
    if (randomNumbers[currentScore + 1] == 255) { // Sammuttaa ledit nappia painettaessa, jos ei olla yhtään jäljessä
      clearAllLeds(); // Sammutetaan kaikki ledit
    }
    // Tarkistetaan onko huijaustila aktiivinen
    if (cheatModeActive) {
      showResults(currentScore + 5); // Päivitetään pisteet huijaustilassa lisäämällä 5
      currentScore += 5; // Kasvatetaan pistemäärää huijaustilassa
    } else {
      showResults(currentScore + 1); // Päivitetään pisteet normaalisti
      currentScore++; // Kasvatetaan pistemäärää normaalisti
    }
    if (currentScore >= 255) { // Peli päättyy, jos pääsee 255 asti
      //Serial.print("Onnea, voitit pelin! Pisteesi: "); // Debugaus
      //Serial.println(currentScore); // Debugaus
      playTokyoDriftTone(); // Soitetaa tokyodrift voiton merkiksi
      clearAllLeds(); // Sammutetaan kaikki ledit
      stopTheGame(); // Pysäytetään peli
    }
  } else {
    //Serial.println("Väärä nappi, hävisit pelin!"); // Debugaus
    playMockingTone(); // Soitetaan häviämisen merkiksi
    clearAllLeds(); // Sammutetaan kaikki ledit
    stopTheGame(); // Pysäytetään peli
  }
}

void stopTheGame()  { // stopTheGame funktiota käytetään pelin pysäyttämiseen ja valmiustilaan palaamiseen
  initializeGame();
  gameStarted = false; // Palautetaan peli valmiustilaan
  cheatModeActive = false; // Palautetaan peli normaaliin tilaan
  //Serial.print("Paina sinistä nappia aloittaaksesi pelin"); // Debugaus
  showReadyState(); // Käynnistetään valmiustilan led-efekti
}

void startTheGame() { // startTheGame funktiota käytetään pelin käynnistämiseen
  clearAllLeds(); // Sammutetaan kaikki ledit
  initializeGame(); // Alustetaan pelissä käytettävät muuttujat ja asetukset
  delay(1000); // Pieni viive ennen pelin alkua
  showResults(currentScore); // Näytetään nykyiset pisteet
  //Serial.println("Peli alkaa!"); // Debugaus
  gameStarted = true; // Asetetaan pelin tila käynnistetyksi
}

void initializeTimer(int gameSpeed){ // Alustetaan timer pelinopeutta varten
  noInterrupts();  // Estetään keskeytykset, jotta voidaan muokata rekisterejä turvallisesti
  TCCR1A = 0;  // Tyhjennetään TCCR1A-rekisteri
  TCCR1B = 0;  // Tyhjennetään TCCR1B-rekisteri
  TCNT1 = 0;   // Nollataan laskuri
  OCR1A = gameSpeed;  // Asetetaan vertailuarvo, joka määrittää keskeytysaikavälin
  TCCR1B |= (1 << WGM12);  // Valitaan CTC-moodi (Clear Timer on Compare Match)
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Määritellään kellotaajuus
  TIMSK1 |= (1 << OCIE1A);  // Otetaan käyttöön keskeytys OCR1A:sta
  interrupts();  // Käynnistetään keskeytykset
}

ISR(TIMER1_COMPA_vect) { // Timerin keskeytyspalvelija
  newTimerInterrupt = true; // Muutetaan newTimerInterrupt arvoon true, jotta main loop havaitsee keskeytyksen tapahtuneen
  //Serial.print("."); // Debugaus
}

void initializeGame() { // Alustetaan pelissä tarvittavat muuttujat sekä timer
  noInterrupts();
  buttonPressed = -1; // Nollataan painetun painikkeen arvo
  timeToCheckGameStatus = false; // Nollataan pelin tilan tarkistuksen lippu
  timerSpeedCounter = 0; // Nollataan pelinopeuden laskuri
  for(int i = 0; i < 255; i++) { // For looppia käytetään taulukoiden yhdenmukaiseen nollaamiseen, jotta tauluihin ei jää mitään dataa
    randomNumbers[i] = -1; // Nollataan satunnaislukutaulukko
    userNumbers[i] = -1; // Nollataan käyttäjän syötteet
  }
  timerInterruptionCounter = 0; // Nollataan keskeytyslaskuri
  buttonInterruptionsCounter = 0;
  currentScore = 0; // Nollataan pisteet
  gameStarted = false; // Asetetaan peli valmiustilaan
  timerSpeed = defaultOutCompRegValue; // Alustetaan timer oletusarvolla
  initializeTimer(defaultOutCompRegValue); // Käynnistetään timer oletusarvolla
  interrupts();
}