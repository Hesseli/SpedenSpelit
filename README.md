# 🎮 Speden Spelit - Arduino Muistipeli

Klassinen toteutus suomalaisesta "Speden Spelit" -muistipelistä (tunnetaan myös nimellä Simon Says) Arduino-alustalle. Peli sisältää neljä LED-valoa, vastaavat painikkeet, 7-segmenttinäytön pisteenlaskua varten sekä äänipalautteen pietsokaiuttimen kautta.

[![Arduino](https://img.shields.io/badge/Alusta-Arduino-blue.svg)](https://www.arduino.cc/)
[![Lisenssi](https://img.shields.io/badge/Lisenssi-MIT-green.svg)](LICENSE)
[![Kieli](https://img.shields.io/badge/Kieli-C++-orange.svg)](https://www.cplusplus.com/)

## 📋 Sisällysluettelo

- [✨ Ominaisuudet](#-ominaisuudet)
- [🛠️ Laitteistovaatimukset](#️-laitteistovaatimukset)
- [📁 Koodin rakenne](#-koodin-rakenne)
- [📌 Pinien määritykset](#-pinien-määritykset)
- [🎯 Pelin kulku](#-pelin-kulku)
- [🔑 Huijauskoodi](#-huijauskoodi)
- [🚀 Käyttöönotto](#-käyttöönotto)

## ✨ Ominaisuudet

- 🧠 **Muistipeli**: Pelaajan täytyy toistaa satunnaisesti luotu LED-valosarja painamalla vastaavia painikkeita
- 📊 **7-segmenttinäyttö**: Näyttää nykyisen pistemäärän (0-255)
- 🔊 **Äänipalautteet**: Pietsokaiutin antaa äänipalautteen oikeista/vääristä painalluksista, pelin päättymisestä ja voitosta
- ⏱️ **Aikarajoitettu pelaaminen**: Pelaajalla on rajoitettu aika vastaamiseen, ohjataan Arduino Timer1-keskeytyksellä
- 🎵 **"Tokyo Drift" voittomelodia**: Soitetaan kun maksimipistemäärä (255) saavutetaan
- 😏 **"Mocking" häviömelodia**: Soitetaan kun peli hävitään
- 🎮 **Painikkeiden ohjaus**: Käyttää PinChangeInterrupts (PCINT) -keskeytyksiä nopeaan painalluksen tunnistukseen
- 🤫 **Piilotettu huijauskoodi**: Aktivoi erikoistilan kokeneille pelaajille

## 🛠️ Laitteistovaatimukset

Tämä projekti on suunniteltu käytettäväksi tyypillisen Arduino-mikrokontrollerin (esim. Arduino Uno) kanssa.

### Tarvittavat komponentit:
- 🎛️ **Arduino-mikrokontrolleri** (Uno/Nano/jne.)
- 💡 **4x LED-valoa** (pelaajan palautteeseen)
- 🔘 **4x Painokytkintä/painiketta**
- 📺 **7-segmenttinäyttömoduuli** (ohjataan 74HC595 shift-rekisterillä)
- 🔊 **1x Pietsokaiutin** (äänipalautteisiin)
- ⚡ **Vastuksia ja kytkentäjohtoja**

### Kytkentäkaavio
```
    Arduino Uno
    ┌─────────────┐
    │      D2-D5  ├──► Painikkeet
    │      A2-A5  ├──► LED-valot
    │         D6  ├──► Pietsokaiutin
    │     D8-D12  ├──► 7-segmenttinäyttö
    └─────────────┘
```


## 📁 Koodin rakenne

| 📄 Tiedosto | 📝 Kuvaus |
|-------------|-----------|
| `display.cpp` | Sisältää toiminnot 7-segmenttinäytön alustamiseen ja lukujen (0-255) näyttämiseen shift-rekisterin avulla |
| `display.h` | Määrittelee 7-segmenttinäytön toiminnot |
| `sounds.cpp` | Sisältää toiminnot pietsokaiuttimen käyttöön. Määrittelee melodian nuotit ja keston. Sisältää `playTokyoDriftTone()` ja `playMockingTone()` -funktiot |
| `sounds.h` | Määrittelee äänitoiminnot ja sisältää julkiset ilmoitukset äänifunktioille |
| `pitches.h` | Määrittelee nuottien taajuudet (NOTE_C4, NOTE_DS5 jne.) pietsokaiuttimen käyttöön |
| `buttons.cpp` | Hoitaa kytkinten alustuksen, tilanseurannan ja PCINT-keskeytyksen käsittelyn. Sisältää huijauskoodin logiikan |
| `buttons.h` | Määrittelee kytkimiin ja keskeytyksiin liittyvät globaalit muuttujat ja funktiot |
| `leds.cpp` | Sisältää toiminnot LEDien ohjaamiseen. Sisältää `setupLeds()`, `setLed()`, `clearAllLeds()` ja visuaaliset esittelytoiminnot (`show1()`, `show2()`) |
| `leds.h` | Määrittelee LED-toiminnot |
| `SpedenSpelit.h` | Sisältää pelin logiikkaan liittyvät toiminnot: ajastimen alustuksen, pelin tilan tarkistuksen (`checkGame()`) ja pelin käynnistyksen/pysäytyksen |
| `SpedenSpelit.ino` | Sisältää `setup()` ja `loop()` -funktiot sekä pelin ydinkulun |


## 📌 Pinien määritykset

| 🔧 Laitteen toiminto | 📄 Tiedosto | 📍 Arduino Pin | 📝 Kuvaus |
|---------------------|-------------|----------------|-----------|
| **7-Segmenttinäyttö** | | | |
| Data (DS) | `display.cpp` | D8 | Syöttää dataa shift-rekisteriin bitti kerrallaan |
| Output Enable (OE) | `display.cpp` | D9 | Pidetään LOW:ssa ulostulojen läpivientiä varten |
| Latch (ST_CP) | `display.cpp` | D10 | Lataa datan ulostulopinneihin (HIGH aktivoi) |
| Clock (SH_CP) | `display.cpp` | D11 | Shift Clock, ohjaa datan siirtoa rekisterissä |
| Reset (MR) | `display.cpp` | D12 | Master Reset, pidetään HIGH:ssa normaalisti |
| **Ääni** | | | |
| Pietsokaiutin | `sounds.cpp` | D6 | Äänipalautteiden soittamiseen |
| **Syötteet** | | | |
| Kytkin 1 | `buttons.cpp` | D2 | Kytkin PinChangeInterruptille |
| Kytkin 2 | `buttons.cpp` | D3 | Kytkin PinChangeInterruptille |
| Kytkin 3 | `buttons.cpp` | D4 | Kytkin PinChangeInterruptille |
| Kytkin 4 | `buttons.cpp` | D5 | Kytkin PinChangeInterruptille |
| **Ulostulot** | | | |
| LED 1 | `leds.cpp` | A2 | LED-valon ulostulo |
| LED 2 | `leds.cpp` | A3 | LED-valon ulostulo |
| LED 3 | `leds.cpp` | A4 | LED-valon ulostulo |
| LED 4 | `leds.cpp` | A5 | LED-valon ulostulo |


## 🎯 Pelin kulku

### 1. 🚀 Käynnistys
Peli aloitetaan `startTheGame()` -funktiolla, joka nollaa pisteet ja alustaa satunnaisten numeroiden sarjan.

### 2. 💡 LED-sarja
Peli näyttää satunnaisen LED-sarjan, jonka pituus kasvaa jokaisen tason jälkeen. LEDien välkkyminen voidaan keskeyttää, jos pelaaja painaa nappia (`timeToCheckGameStatus`).

### 3. 🎮 Pelaajan syöte
Pelaaja painaa kytkimiä toistaakseen näytetyn sarjan. Kytkimen painallukset käsitellään keskeytyksenä (`ISR(PCINT2_vect)`).

### 4. ✅ Tarkistus
Jokaisen kytkimen painalluksen jälkeen `checkGame()` -funktio tarkistaa, vastaako painallus odotettua sarjan kohtaa.

#### Oikein ✅
- Peli jatkuu, ja seuraavaa painallusta odotetaan
- Jos sarja on suoritettu, pisteet nousevat ja uusi, pidempi sarja aloitetaan

#### Väärin/Aika loppui ❌
- Peli pysäytetään (`stopTheGame()`)
- Soitetaan `playMockingTone()`

#### Voitto 🏆
- Jos pelaaja saavuttaa maksimipisteet 255, peli päättyy
- Soitetaan `playTokyoDriftTone()`


## 🔑 Huijauskoodi

Koodiin on rakennettu piilotettu huijauskoodi, joka aktivoi erillisen tilan (`cheatModeActive`).

| 🎮 Toiminto | 📋 Koodi | 📄 Tiedosto | 🔧 Mekanismi |
|-------------|----------|-------------|--------------|
| **Huijauskoodi** | `1, 0, 2, 0, 3` | `buttons.cpp` | Koodi seurataan keskeytyksen sisällä |

> **Huom:** Painikkeiden indeksit viittaavat numeroihin 0-3. Jos käyttäjä painaa väärän painikkeen koodin syöttämisen aikana, syöttö nollataan.

**Tilan muuttuja:** `volatile bool cheatModeActive`

## 🚀 Käyttöönotto

### 1. 📥 Lataa koodi
```bash
git clone https://github.com/yourusername/SpedenSpelit.git
cd SpedenSpelit
```

### 2. 🔌 Kytke laitteisto
Seuraa [pinien määrityksiä](#-pinien-määritykset) ja kytke komponentit Arduino-levylle.

### 3. 📤 Lataa Arduino-levylle
1. Avaa `SpedenSpelit.ino` Arduino IDE:ssä
2. Valitse oikea levy ja portti
3. Klikkaa "Upload"

### 4. 🎮 Pelaa!
Paina mitä tahansa painiketta aloittaaksesi pelin.

---

**🎯 Hauskaa pelaamista!** 🚀