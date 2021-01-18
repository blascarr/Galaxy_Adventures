
class RFID_Card{
  public:
    MFRC522::MIFARE_Key key;
    
    
};

class Resource{

};

class Fuel{

};

class SpaceShip: public RFID_Card{
  public:
    int score = 0;
    String name;
    char* colorLabel = "";
    uint32_t color;
    byte currentUID[4];
  
};



class Treasure : public RFID_Card{
  public:
};

class SecretKey : public RFID_Card{
  public:
  
};

class Galaxy_Game{
  public:

  RFID_Controller& RFID_reader;
  Neopixel_Controller& strip;

  JsonObject& JSONencoder; 

  // ---- Time Controller ---- //
  long timestamp;
  uint16_t duration = 500;  //Under 500 ms, update() execute several times ( 3 times at least ) 


  bool old_flag = 0;
  bool card_flag = 0;
  
  SpaceShip current_ship;
  
  Galaxy_Game( RFID_Controller &RFID ): RFID_reader( RFID ){
    
  }

  Galaxy_Game( RFID_Controller &RFID, Neopixel_Controller& pixelstrip ): RFID_reader( RFID ), strip( pixelstrip ) {
    
  }
  
  void init(){
    SPI.begin(); 
    RFID_reader.init();
    strip.begin();
    
  }
  
  void update(){
    if( ( millis() - timestamp ) > duration ){
         timestamp = millis();
         //Serial.println( cardPresent );
         
         if ( RFID_reader.update() ){
            strip.success( true );
            
            
            //clearInfo Last Card
            JSONbuffer.clear();
            card_flag = 1;
            // Serialize Info in JSON
            //mapCard();
         }
        
    }
    
    strip.update();
  }

  void debugCard(){
    Serial.println("Raw Info");
    RFID_reader.PICC_DumpToSerial(&( RFID_reader.uid ));
  }

  void mapCard(){

    JsonObject JSONencoder = JSONbuffer.to<JsonObject>();
    JSONencoder["name"] = RFID_reader.readStringBlock( 4 );
    JSONencoder["color"] = RFID_reader.readStringBlock( 5 );
    JSONencoder["colorValue"] = RFID_reader.readBlock( 6 );

    JSONencoder["energy"] = 100;
    JSONencoder["credits"] = 200;
    
      JsonObject resources = JSONencoder.createNestedObject("resources");
      resources["uranium"] = "";
      resources["titanium"] = "";
      resources["steel"] = "";
      resources["crystal"] = "";

      JsonObject fuel = JSONencoder.createNestedObject("fuel");
      fuel["gas"] = "";
      fuel["hydrogen"] = "";
      fuel["water"] = "";
      fuel["oxygen"] = "";
      
      JsonObject production = JSONencoder.createNestedObject("production");
      production["fauna"] = "";
      production["flora"] = "";
      production["bacterium"] = "";
      production["oceans"] = "";

      /*JsonObject alien_life = JSONencoder.createNestedObject("alien_life");
      JsonObject activations = JSONencoder.createNestedObject("activations");

      JsonObject binnacle = JSONencoder.createNestedObject("binnacle");
      JsonObject voicenotes = JSONencoder.createNestedObject("voicenotes");
      JsonObject dates = JSONencoder.createNestedObject("dates");
      JsonObject level_access = JSONencoder.createNestedObject("level_access");

      JsonObject secret_encryption = JSONencoder.createNestedObject("secret_encryption");*/
      
      //Close Readings
    RFID_reader.PICC_HaltA();
    RFID_reader.PCD_StopCrypto1();
    
    parseCard();
  }

  void JSONmap(){
    
  }
  
  void parseCard(){
    
    current_ship.name = JSONbuffer["name"].as<String>();
    current_ship.colorLabel = JSONbuffer["color"].as<const char*>();
    current_ship.color = JSONbuffer["colorValue"];
    Serial.println("Card Parsed ");
  }

  void infoCard(){
    Serial.print( current_ship.name );
    Serial.print( " , " );
    Serial.print( current_ship.colorLabel );
    Serial.print( " , " );
    Serial.println( current_ship.color );
  }

  
};
