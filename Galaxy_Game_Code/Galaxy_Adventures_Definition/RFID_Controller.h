
typedef struct {
    byte    size;     // Number of bytes in the UID. 4, 7 or 10.
    byte    uid[10];
    byte    sak;      // The SAK (Select acknowledge) byte returned from the PICC after successful selection.
} UID;

class RFID_Controller: public MFRC522{
  
  public:
    MFRC522::MIFARE_Key key; 
    UID lastCard;
    bool noCard = true;
    bool cardON = false;

    // Prepare the security key for the read and write functions.
    byte keyblock[6] ={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  
    // ---- Time Controller ---- //
    long timestamp;
    uint16_t duration = 500;
    int block = 4; 

    long checkstamp;
    uint16_t check_time = 1000 ;
    
    RFID_Controller( uint8_t SSPIN, uint8_t RSTPIN ):MFRC522( SSPIN, RSTPIN ) {
      
    }
    
    void init(){
      SPI.begin(); 
      PCD_Init( );

      for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;  //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
      }
    }

    bool update(){
          if (  PICC_IsNewCardPresent()  ) {

            // Select one of the cards
            //Serial.println("Is Card");
            
            cardON = true;
            if (  PICC_ReadCardSerial()) 
            {
              return checkCurrentCard();
            }
         }else{
            cardON = false;
            return false;
         }
      
    }

    bool checkCurrentCard(){
      if( ( millis() - checkstamp ) > check_time ){
        checkstamp = millis();

        int n = memcmp( &lastCard , &uid, sizeof(lastCard) );

        //Detecta si la tarjeta es la misma de antes
        if( n!=0  ){
          //Overwrite currentUID
            memcpy( &lastCard , &uid, sizeof( uid ) );
            Serial.print("UID Card ");
            printArray( uid.uidByte,  uid.size);
            
            //PICC_DumpToSerial(&( uid ));
            Serial.println();
            return true;
        }else{
          return false;
        }
      }
    }

    void printArray(byte *buffer, byte bufferSize) {
       for (byte i = 0; i < bufferSize; i++) {
          Serial.print(buffer[i] < 0x10 ? " 0" : " ");
          Serial.print(buffer[i], HEX);
       }
    }

    /*bool keepcard( UID &currentUID , byte &masterCard ){
      int n= memcmp ( currentUID, masterCard, sizeof(4*sizeof(byte)) );
      return (( n == 0 ) ? true : false );
    }*/

    int writeBlock(int blockNumber, byte arrayAddress[]) {
      
      //this makes sure that we only write into data blocks. Every 4th block is a trailer block for the access/security info.
      int largestModulo4Number=blockNumber/4*4;
      int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector
      if (blockNumber > 2 && (blockNumber+1)%4 == 0){Serial.print(blockNumber);Serial.println(" is a trailer block:");return 2;}
      Serial.print(blockNumber);
      Serial.println(" is a data block:");
      
      //authentication of the desired block for access
      byte status = PCD_Authenticate( MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &( uid ));
      if (status != MFRC522::STATUS_OK) {
             Serial.print("PCD_Authenticate() failed: ");
             Serial.println( GetStatusCodeName(status) );
             return 3;//return "3" as error message
      }
      
      //writing the block 
      status =  MIFARE_Write( blockNumber, arrayAddress, 16 );
      //status =  MIFARE_Write(9, value1Block, 16);
      if (status != MFRC522::STATUS_OK) {
               Serial.print("MIFARE_Write() failed: ");
               Serial.println( GetStatusCodeName(status) );
               return 4;//return "4" as error message
      }
      Serial.println("block was written");
    }

    
    String readStringBlock( int blockNumber ) {
        int trailerBlock=blockNumber/4*4+3;//determine trailer block for the sector
        byte readbackblock[18];
        
        int timeauth = millis();
        int duration = 2000;

        byte status = PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(uid));
        
        while( (millis() - timeauth) < duration && (status != MFRC522::STATUS_OK) ){
          status = PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(uid));
          PICC_HaltA();
          PCD_StopCrypto1();
        }
        
        if (status != MFRC522::STATUS_OK) {
               Serial.print("PCD_Authenticate() failed (read): ");
               Serial.println( GetStatusCodeName(status));
               PICC_HaltA();
               PCD_StopCrypto1();
               return "ERROR AUTHENTICATE";
        }
        
        /*if ( (millis() - timeauth) > duration+500 ){
              PICC_HaltA();
              PCD_StopCrypto1();
              return "TIMEOUT";
        }*/
      
        //reading a block
        byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
        status = MIFARE_Read(blockNumber, readbackblock, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
          if (status != MFRC522::STATUS_OK) {
                  Serial.print("MIFARE_read() failed: ");
                  Serial.println(GetStatusCodeName(status));
                  PICC_HaltA();
                  PCD_StopCrypto1();
                  return "ERROR READ";
          }
          String data = String((char*)readbackblock);
        
          //PICC_HaltA();
          //PCD_StopCrypto1();
          return data;
     }

     long readBlock( int blockNumber ) {
        int trailerBlock=blockNumber/4*4+3;//determine trailer block for the sector
        byte readbackblock[18];
        
        int timeauth = millis();
        int duration = 2000;

        byte status = PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(uid));
        
        while( (millis() - timeauth) < duration && (status != MFRC522::STATUS_OK) ){
          status = PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(uid));
          PICC_HaltA();
          PCD_StopCrypto1();
        }
        
        if (status != MFRC522::STATUS_OK) {
               Serial.print("PCD_Authenticate() failed (read): ");
               Serial.println( GetStatusCodeName(status));
               PICC_HaltA();
               PCD_StopCrypto1();
               return "ERROR AUTHENTICATE";
        }
        
        /*if ( (millis() - timeauth) > duration+500 ){
              PICC_HaltA();
              PCD_StopCrypto1();
              return "TIMEOUT";
        }*/
      
        //reading a block
        byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
        status = MIFARE_Read(blockNumber, readbackblock, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
          if (status != MFRC522::STATUS_OK) {
                  Serial.print("MIFARE_read() failed: ");
                  Serial.println(GetStatusCodeName(status));
                  PICC_HaltA();
                  PCD_StopCrypto1();
                  return "ERROR READ";
          }
          long data = readbackblock;
        
          //PICC_HaltA();
          //PCD_StopCrypto1();
          return data;
     }
};
