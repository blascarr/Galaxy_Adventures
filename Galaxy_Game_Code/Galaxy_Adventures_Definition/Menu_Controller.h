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
  Serial.print("event: ");
  Serial.println(e);
  return proceed;
}


/*
result shipData(eventMask e,navNode& nav,prompt& item) {
  Serial.print("event: ");
  Serial.println(e);
  return proceed;
}
*/

MENU( debug,"Debug",showEvent,anyEvent,noStyle
  ,OP("Load",showEvent,anyEvent)
  ,OP("No Fuel",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU( loadPlayers,"Cargar Jugadores",showEvent,anyEvent,noStyle
  ,OP("Book",showEvent,anyEvent)
  ,OP("Data",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU( loadTreasures,"Cargar Tesoros",showEvent,anyEvent,noStyle
  ,OP("Book",showEvent,anyEvent)
  ,OP("Data",showEvent,anyEvent)
  ,EXIT("<Back")
);


MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,SUBMENU( debug )
  ,SUBMENU( loadPlayers )
  ,SUBMENU( loadTreasures )
  ,OP("Save EEPROM",showEvent,anyEvent)
  ,EXIT("<Back")
);


#define MAX_DEPTH 2

MENU_OUTPUTS(out, MAX_DEPTH
  ,LIQUIDCRYSTAL_OUT(lcd,{0,0,20,4})
  ,NONE
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);//the navigation root object
