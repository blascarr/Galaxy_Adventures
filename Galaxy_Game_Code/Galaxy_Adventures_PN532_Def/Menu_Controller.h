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

// ---- COLOR FIELDS ---- //

result writeColor(eventMask e,navNode& nav,prompt& item) {
  byte colorLabel[16] = { "BLUE" };
  byte colorValue[16] = { "65532" };
  Serial.println( game.colorSelector );
  String color = "BLUE";
  String colorval = "65532";
  
  switch ( game.colorSelector ){
    case 0:
      color = "BLUE";
      colorval = "43690";
    break;
    case 1:
      color = "GREEN";
      colorval = "21845";
    break;
    case 2:
      color = "YELLOW";
      colorval = "10923";
    break;
    case 3:
      color = "RED";
      colorval =  "0";
    break;
    case 4:
      color = "CYAN";
      colorval = "32768";
    break;
    case 5:
      color = "MAGENTA";
      colorval = "54613";
    break;
  }
  color.getBytes(colorLabel, 16 );
  colorval.getBytes(colorValue, 16 );
  
  if ( game.RFID_reader.writeBlock( 5 , colorLabel ) ){
    Serial.print("Write in Block ");
    Serial.print( 5 );
    Serial.print(" : ");
    Serial.println( color );
  }
  
  if( game.RFID_reader.writeBlock( 6 , colorValue ) ){
    Serial.print("Write in Block ");
    Serial.print( 6 );
    Serial.print(" : ");
    Serial.println( colorval );
  }
  
  return proceed;
}

// Detectar que hay tarjeta y mostrar informacion de la ultima tarjeta clasificada
result serialCardInfo(eventMask e,navNode& nav,prompt& item) {
  game.infoCard();
  return proceed;
}

result loadCodePlayer(eventMask e,navNode& nav,prompt& item) {
  Serial.println("Loading Player... ");
  player.deserialize();
  player.loadPlayer( game.RFID_reader);
  return proceed;
}

result loadSDPlayer(eventMask e,navNode& nav,prompt& item) {
  Serial.println("Loading Player... ");
  player.SDdeserialize();
  player.loadPlayer( game.RFID_reader);
  return proceed;
}

result viewPlayer(eventMask e,navNode& nav,prompt& item) {
  Serial.println("Player Data... ");
  player.print( game.RFID_reader);
  return proceed;
}

result readBlock(eventMask e,navNode& nav,prompt& item) {
  Serial.print("Read Block ");
  Serial.print( nfc.block );
  Serial.print (" From Card: ");
  game.RFID_reader.readBlock();
  Serial.println();
  return proceed;
}


SELECT( game.colorSelector ,ColorSelector,"Select",doNothing,noEvent,noStyle
  ,VALUE("BLUE",0,writeColor,enterEvent)
  ,VALUE("GREEN",1,writeColor,enterEvent)
  ,VALUE("YELLOW",2,writeColor,enterEvent)
  ,VALUE("RED",3,writeColor,enterEvent)
  ,VALUE("CYAN",4,writeColor,enterEvent)
  ,VALUE("MAGENTA",5,writeColor,enterEvent)
);

MENU( loadPlayers,"Cargar Jugadores",showEvent,anyEvent,noStyle
  //,FIELD( nplayer,"Write Name","",0,5,1,1,writeName,exitEvent,wrapStyle )
  //,SUBMENU( ColorSelector )
  ,OP("LOAD FROM CODE",loadCodePlayer,enterEvent)
  ,OP("LOAD FROM SD",loadSDPlayer,enterEvent)
  ,OP("LOAD FROM EEPROM",showEvent,enterEvent)
  ,EXIT("<Back")
);

MENU( writePlayers,"Write Data",showEvent,anyEvent,noStyle
  //,FIELD( nplayer,"Write Name","",0,5,1,1,writeName,exitEvent,wrapStyle )
  ,SUBMENU( ColorSelector )
  ,EXIT("<Back")
);

MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,OP("Read Card",serialCardInfo,enterEvent)
  ,OP("Read Player",doNothing,enterEvent)
  ,FIELD( nfc.block,"Read Block","",4,63,1,1,readBlock ,exitEvent,wrapStyle )
  //,OP("Raw Card",doNothing,enterEvent )
  ,SUBMENU( loadPlayers )
  ,SUBMENU( writePlayers )
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
