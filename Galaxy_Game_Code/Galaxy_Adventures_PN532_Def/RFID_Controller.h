typedef struct {
    byte    size = 4;     // Number of bytes in the UID. 4, 7 or 10.
    byte    uid[10];
    byte    keyblock;      // The SAK (Select acknowledge) byte returned from the PICC after successful selection.
} UID;

class RFID_Controller: public PN532 {
  public:

    UID lastCard;
    UID currentCard;
    
    // Prepare the security key for the read and write functions.
    byte keyblock[6] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

    int block  = 4;
    bool cardPresent;
    unsigned long checkstamp = 0;
    unsigned long check_time = 2000;
    
    RFID_Controller( PN532_SPI &SPISelector ):PN532( SPISelector ) {
      
    }

    void init(){
      SPI.begin(); 
      begin();
      setPassiveActivationRetries(0xFF);

      // configure board to read RFID tags
      SAMConfig();
    }

    bool update(){
      if( ( millis() - checkstamp ) > check_time ){
        checkstamp = millis();
      
        cardPresent = this->readPassiveTargetID(PN532_MIFARE_ISO14443A, &currentCard.uid[0], &currentCard.size);
        if ( cardPresent ) {
          if ( checkCurrentCard( ) ){
            //Serial.println("NEW CARD");
          }
        }
      }
      return cardPresent;
    }

    bool checkCurrentCard( ){
      int n = memcmp( &lastCard , &currentCard, sizeof(lastCard) );
      if( n!=0  ){
          //Overwrite currentUID
            memcpy( &lastCard , &currentCard, sizeof( currentCard ) );
            Serial.print("UID Card ");
            printArray( lastCard.uid,  lastCard.size);
            Serial.println();
            return true;
        }else{
          return false;
        }
    }

    void printArray(byte *buffer, byte bufferSize) {
       for (byte i = 0; i < bufferSize; i++) {
          Serial.print(buffer[i] < 0x10 ? " 0" : " ");
          Serial.print(buffer[i], HEX);
       }
    }

    bool writeBlock(int blockNumber , byte arrayAddress[]) {
      bool pass = this->mifareclassic_AuthenticateBlock(lastCard.uid, lastCard.size, blockNumber, 0, keyblock );
        if ( pass ){
          /*Serial.print("Sector Block: ");
          Serial.print(blockNumber);
          Serial.println(" has been authenticated" );*/
          uint8_t data[16];
          bool dataread = this->mifareclassic_WriteDataBlock ( blockNumber, arrayAddress );
          
          if( dataread ){
            Serial.println("Data Written: ");
            //this->PrintHexChar(arrayAddress, 16);
            //Serial.println("");
            return true;
          }else{
            Serial.println("Ooops ... unable to read the requested block.  Try another key?");
          }
        }else{
            Serial.println("Ooops ... authentication failed: Try another key?");
        }
        return false;
    }

    void readBlock(){
      if ( cardPresent ) {
        Serial.println( readStringBlock( block ) );
      }
      
    }
    
    String readStringBlock( int blockNumber ) {
        bool pass = this->mifareclassic_AuthenticateBlock(lastCard.uid, lastCard.size, blockNumber, 0, keyblock );
        uint8_t data[16];
        if ( pass ){
          /*Serial.print("Sector Block: ");
          Serial.print(blockNumber);
          Serial.println(" has been authenticated" );
          */
          
          bool dataread = this->mifareclassic_ReadDataBlock( blockNumber, data);
          if( dataread ){
            /*Serial.println("Data: ");
            this->PrintHexChar(data, 16);
            Serial.println("");*/
            return String((char*)data);
          }else{
            Serial.println("Ooops ... unable to read the requested block.  Try another key?");
          }
        }else{
            Serial.println("Ooops ... authentication failed: Try another key?");
        }
        return "";
    }
};
