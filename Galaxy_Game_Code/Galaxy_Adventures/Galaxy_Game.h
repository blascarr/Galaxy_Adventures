
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
    uint32_t color;
  
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

  Galaxy_Game( RFID_Controller &RFID ): RFID_reader( RFID ){
    
  }
  
  void init(){
    SPI.begin(); 
    RFID_reader.init();
    pinMode(encBtn,INPUT_PULLUP);
    encoder.begin();
    lcd.begin(20,4);
    lcd.setCursor(7, 0);
    lcd.print("Galaxy");
    lcd.setCursor(5, 1);
    lcd.print("Adventures");
    delay(2000);
  }
  
  void update(){
    RFID_reader.update();
    nav.poll();
  }

  
};
