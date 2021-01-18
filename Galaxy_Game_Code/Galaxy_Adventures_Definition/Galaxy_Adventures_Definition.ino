

#include <SPI.h>      //include the SPI bus library
#include <MFRC522.h>  //include the RFID reader library
//#include <EEPROM.h>
#include <ArduinoJson.h>

#define SS_PIN 48  //slave select pin
#define RST_PIN 47  //reset pin
#define IRQ_PIN 18
volatile bool cardPresent;

// ----- JSON Controller ----- //
#define JSONBUFFER 300
StaticJsonDocument< JSONBUFFER > JSONbuffer;

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
  
//----- AddOns Extensions -----//
/*  
 *   - Neopixel LED  
 *   - MP3 DFPlayer
 *   - Servo Opener
 *   - Stepper Mechanism 
*/

//----- Neopixel Configuration -----//
#define NUM_LEDS_PER_STRIP 8
#define PIXEL_PIN 14 //Defines the Datapin for NeoPixel
#define BRIGHTNESS 150

#include "Neopixel_Controller.h"
Neopixel_Controller strip = Neopixel_Controller(NUM_LEDS_PER_STRIP, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

#include "RFID_Controller.h"
#include "playership.h"
#include "Galaxy_Game.h"

//----- GALAXY GAME Configuration -----//

#define SPACECHIPS 3
#define TREASURES 3

RFID_Controller reader( SS_PIN, RST_PIN );
Galaxy_Game game( reader, strip );

// -----Players Declaration ----//
CardJSON player( JSONString );


#include "Menu_Controller.h"

Menu_Controller galaxy_menu;

void setup() {
  Serial.begin( 9600 );
  SPI.begin();   
  Serial.println("Galaxy Adventures");
  game.init();
  galaxy_menu.init();
  player.deserialize();
  attachInterrupt(digitalPinToInterrupt( IRQ_PIN ), CARD_IRQ, RISING);
}

void loop() {
  game.update();
  galaxy_menu.update();
}
