#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"

//#include <EEPROM.h>
#include <ArduinoJson.h>

#define RFID_PIN 48  //slave select pin
#define RST_PIN 47  //reset pin

// ----- JSON Controller ----- //
#define JSONBUFFER 300
StaticJsonDocument< JSONBUFFER > JSONbuffer;

//#define SDLOADER

#ifdef SDLOADER
  #define SD_PIN 23
#endif

// ----- LCD Controller ----- //
  #define RS 2
  #define EN 3
  #define LCDPIN_4 4
  #define LCDPIN_5 5
  #define LCDPIN_6 6
  #define LCDPIN_7 7

// ----- Encoder ----- //
  #define encA 11
  #define encB 10
  //this encoder has a button here
  #define encBtn 12

//----- Neopixel Configuration -----//
#define NUM_LEDS_PER_STRIP 12
#define PIXEL_PIN 14 //Defines the Datapin for NeoPixel
#define BRIGHTNESS 50

#include "Neopixel_Controller.h"
Neopixel_Controller strip = Neopixel_Controller(NUM_LEDS_PER_STRIP, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

#include "RFID_Controller.h"
#include "JSONCards.h"
#include "Galaxy_Game.h"

//----- GALAXY GAME Configuration -----//

#define SPACECHIPS 3
#define TREASURES 3

PN532_SPI pn532spi(SPI, RFID_PIN);
RFID_Controller nfc(pn532spi);

Galaxy_Game game( nfc, strip );

// -----Players Declaration ----//
CardJSON player;

//----- Menu Declaration -----//
#include "Menu_Controller.h"

Menu_Controller galaxy_menu;

unsigned long bps = 9600;

void setup() {
  Serial.begin( bps ); 
  Serial.println("Galaxy Adventures");
  
  game.init();
  galaxy_menu.init();
}

void loop() {
  game.update();
  galaxy_menu.update();
}
