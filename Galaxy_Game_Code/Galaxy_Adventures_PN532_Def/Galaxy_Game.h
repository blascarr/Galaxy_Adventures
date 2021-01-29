

class RFID_Card{
  public:
    
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

    int colorSelector = 0;
    
    // ---- Time Controller ---- //
    long timestamp;
    uint16_t duration = 2000;  //Under 500 ms, update() execute several times ( 3 times at least ) 
    
    SpaceShip current_ship;
    
    Galaxy_Game( RFID_Controller &RFID ): RFID_reader( RFID ){
      
    }
  
    Galaxy_Game( RFID_Controller &RFID, Neopixel_Controller& pixelstrip ): RFID_reader( RFID ), strip( pixelstrip ) {
      
    }
  
    void init(){
      RFID_reader.init();
      strip.init();
    }

    void update(){
      if( ( millis() - timestamp ) > duration ){
           timestamp = millis();

           if ( RFID_reader.update() ){
              if( RFID_reader.cardPresent ){
                strip.success( true );
              }
              
              
              //clearInfo Last Card
              JSONbuffer.clear();
              // Serialize Info in JSON
              mapCard();
           }else{
              strip.success( false );
           }          
      }
      
      strip.update();
    }
    

    void infoCard(){
      Serial.print( current_ship.name );
      Serial.print( " , " );
      Serial.print( current_ship.colorLabel );
      Serial.print( " , " );
      Serial.println( current_ship.color );
    }

    void mapCard(){
      
    }
};
