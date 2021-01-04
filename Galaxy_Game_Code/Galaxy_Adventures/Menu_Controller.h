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

MENU( refuel,"Repostar",showEvent,anyEvent,noStyle
  ,OP("Load",showEvent,anyEvent)
  ,OP("No Fuel",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU( binnacle,"Bitacora",showEvent,anyEvent,noStyle
  ,OP("Book",showEvent,anyEvent)
  ,OP("Data",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU( alien_life,"Formas de vida",showEvent,anyEvent,noStyle
  ,OP("Alien",showEvent,anyEvent)
  ,OP("Animales",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU( treasures,"Buscar tesoros",showEvent,anyEvent,noStyle
  ,OP("Dinero",showEvent,anyEvent)
  ,OP("Cofres",showEvent,anyEvent)
  ,EXIT("<Back")
);


MENU( voicenotes,"Notas de voz",showEvent,anyEvent,noStyle
  ,OP("Capitan",showEvent,anyEvent)
  ,OP("Grabaciones",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU( mining,"Mineria",showEvent,anyEvent,noStyle
  ,OP("Hierro",showEvent,anyEvent)
  ,OP("Acero",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU( bomb,"Activar bomba",showEvent,anyEvent,noStyle
  ,OP("Nuclear",showEvent,anyEvent)
  ,OP("Planetaria",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU( debug,"Leer nave",showEvent,anyEvent,noStyle
  ,OP("Read data",showEvent,anyEvent)
  ,EXIT("<Back")
);

MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,SUBMENU( debug )
  ,SUBMENU( refuel )
  ,SUBMENU( binnacle )
  ,SUBMENU( alien_life )
  ,SUBMENU( treasures )
  ,SUBMENU( voicenotes )
  ,SUBMENU( mining )
  ,SUBMENU( bomb )
  ,EXIT("<Back")
);


#define MAX_DEPTH 2

MENU_OUTPUTS(out, MAX_DEPTH
  ,LIQUIDCRYSTAL_OUT(lcd,{0,0,20,4})
  ,NONE
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);//the navigation root object
