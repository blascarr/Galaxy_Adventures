#include <Wire.h>
#include <menu.h>
#include <menuIO/liquidCrystalOut.h>

#include <menuIO/serialIn.h>
#include <menuIO/encoderIn.h>
#include <menuIO/keyIn.h>
#include <menuIO/chainStream.h>

using namespace Menu;

  // LCD INSTANCE /////////////////////////////////////////
LiquidCrystal lcd(RS, EN, LCDPIN_4, LCDPIN_5, LCDPIN_6, LCDPIN_7);

  // ENCODER INSTANCE /////////////////////////////////////////
encoderIn<encA,encB> encoder;//simple quad encoder driver
encoderInStream<encA,encB> encStream(encoder,4);// simple quad encoder fake Stream

//a keyboard with only one key as the encoder button
keyMap encBtn_map[]={{-encBtn,defaultNavCodes[enterCmd].ch}};//negative pin numbers use internal pull-up, this is on when low
keyIn<1> encButton(encBtn_map);//1 is the number of keys

//input from the encoder + encoder button + serial

menuIn* inputsList[]={&encStream,&encButton,};
chainStream<2> in(inputsList);//3 is the number of inputs


// Events Declaration /////////////////////////////////////////
result showEvent(eventMask e,navNode& nav,prompt& item) {
  //Serial.print("event: ");
  //Serial.println(e);
  return proceed;
}

// Detectar que hay tarjeta y mostrar informacion de la ultima tarjeta clasificada
result serialCardInfo(eventMask e,navNode& nav,prompt& item) {
  game.debugCard();
  return proceed;
}

result readCard(eventMask e,navNode& nav,prompt& item) {
  game.infoCard();
  return proceed;
}

uint8_t nplayer = 0;
result writeName(eventMask e,navNode& nav,prompt& item) {
 game.RFID_reader.fast_reset();
  byte blockcontent[16] = {"Player 1"};
  game.RFID_reader.writeBlock( 4 , blockcontent );
}

// ---- COLOR FIELDS ---- //

int mode=0;

result writeColor(eventMask e,navNode& nav,prompt& item) {
  byte colorLabel[16] = { "BLUE" };
  byte colorValue[16] = { 65532 };
  
  switch (mode){
    case 0:
      colorLabel[16] = { "BLUE" };
      colorValue[16] = { 43690 };
    break;
    case 1:
      colorLabel[16] = { "GREEN" };
      colorValue[16] = { 21845 };
    break;
    case 2:
      colorLabel[16] = { "YELLOW" };
      colorValue[16] = { 10923 };
    break;
    case 3:
      colorLabel[16] = { "RED" };
      colorValue[16] = { 0 };
    break;
    case 4:
      colorLabel[16] = { "CYAN" };
      colorValue[16] = { 32768 };
    break;
    case 5:
      colorLabel[16] = { "MAGENTA" };
      colorValue[16] = { 54613 };
    break;
  }

  game.RFID_reader.writeBlock( 5 , colorLabel );
  game.RFID_reader.writeBlock( 6 , colorValue );
  
  return proceed;
}

result loadPlayer(eventMask e,navNode& nav,prompt& item) {
  Serial.println("Loading Player... ");
  player.loadPlayer( game.RFID_reader);
  return proceed;
}

SELECT(mode,ColorSelector,"Select",doNothing,noEvent,noStyle
  ,VALUE("BLUE",0,writeColor,enterEvent)
  ,VALUE("GREEN",1,writeColor,enterEvent)
  ,VALUE("YELLOW",2,writeColor,enterEvent)
  ,VALUE("RED",3,writeColor,enterEvent)
  ,VALUE("CYAN",4,writeColor,enterEvent)
  ,VALUE("MAGENTA",5,writeColor,enterEvent)
);


MENU( loadPlayers,"Cargar Jugadores",showEvent,anyEvent,noStyle
  ,FIELD( nplayer,"Write Name","",0,5,1,1,writeName,exitEvent,wrapStyle )
  ,SUBMENU( ColorSelector )
  ,OP("LOAD FROM CODE",loadPlayer,enterEvent)
  ,OP("LOAD FROM EEPROM",showEvent,anyEvent)
  ,OP("LOAD FROM SD",showEvent,anyEvent)
  ,EXIT("<Back")
);


MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,OP("Read Card",readCard,enterEvent)
  ,OP("Raw Card",serialCardInfo,enterEvent )
  ,SUBMENU( loadPlayers )
  ,OP("Save EEPROM",showEvent,anyEvent)
  ,EXIT("<Back")
);

#define MAX_DEPTH 3

MENU_OUTPUTS(out, MAX_DEPTH
  ,LIQUIDCRYSTAL_OUT(lcd,{0,0,20,4})
  ,NONE
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);//the navigation root object

class Menu_Controller{
  public:
  
    Menu_Controller(){};

    void init(){
      pinMode(encBtn,INPUT_PULLUP);
      encoder.begin();
      lcd.begin(20,4);
      lcd.setCursor(7, 0);
      lcd.print("Galaxy");
      lcd.setCursor(5, 1);
      lcd.print("Adventures");
      delay(1000);
    }

    void update(){
      nav.poll();
      if(nav.sleepTask) {
        //Serial.println("Sleeping");
      }
    }
};
