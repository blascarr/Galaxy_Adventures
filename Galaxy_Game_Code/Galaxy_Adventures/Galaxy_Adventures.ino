

#include <SPI.h>      //include the SPI bus library
#include <MFRC522.h>  //include the RFID reader library


#define SS_PIN 48  //slave select pin
#define RST_PIN 47  //reset pin

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
#define NUM_LEDS_PER_STRIP 10
#define PIN A5  //Defines the Datapin for NeoPixel

  
#include "Neopixel_Controller.h"
#include "RFID_Controller.h"
#include "Menu_Controller.h"
#include "Galaxy_Game.h"

//----- GALAXY GAME Configuration -----//

#define SPACECHIPS 3
#define TREASURES 3

RFID_Controller reader( SS_PIN, RST_PIN );

Galaxy_Game game( reader );

void setup() {
  Serial.begin( 9600 );
  SPI.begin();   
  Serial.println("Galaxy Adventures");
  game.init();
}

void loop() {
  game.update();

}
