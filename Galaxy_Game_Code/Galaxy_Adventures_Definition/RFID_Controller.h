
typedef struct {
    byte    size;     // Number of bytes in the UID. 4, 7 or 10.
    byte    uid[10];
    byte    sak;      // The SAK (Select acknowledge) byte returned from the PICC after successful selection.
} UID;

void CARD_IRQ()
{
  cardPresent = false;
  strip.success( false );
}

class RFID_Controller: public MFRC522{
  
  public:
    MFRC522::MIFARE_Key key; 
    UID lastCard;

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
      PCD_SetAntennaGain( RxGain_max );
      PCD_Init( );
      PCD_WriteRegister(MFRC522::ComIrqReg, 0x80); //Clear interrupts
      PCD_WriteRegister(MFRC522::ComIEnReg, 0x7F); //Enable all interrupts
      PCD_WriteRegister(MFRC522::DivIEnReg, 0x14);
      
      for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;  //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
      }
    }

    bool update(){
        bool cardOn = false;
        cardOn = PICC_IsNewCardPresent();
          if (  cardOn  ) {
            
            if (  PICC_ReadCardSerial()) 
            {
              cardPresent = true;
              checkCurrentCard();
            }
         }
         PICC_HaltA();
        return cardOn;
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

    int writeBlock(int blockNumber, byte arrayAddress[]) {
      if (  PICC_IsNewCardPresent()) {
        // Select one of the cards
        //Serial.print("Card Present ");
        if (  PICC_ReadCardSerial()) {
        fast_reset();
        if(cardPresent){
              MFRC522::MIFARE_Key rekey;
              for (byte i = 0; i < 6; i++) {
                  rekey.keyByte[i] = 0xFF;
              }
              
              //this makes sure that we only write into data blocks. Every 4th block is a trailer block for the access/security info.
              int largestModulo4Number=blockNumber/4*4;
              int trailerBlock=largestModulo4Number+3;//determine trailer block for the sector
              if (blockNumber > 2 && (blockNumber+1)%4 == 0){Serial.print(blockNumber);Serial.println(" is a trailer block:");return 2;}
              Serial.print(blockNumber);
              Serial.println(" is a data block:");
              
              //authentication of the desired block for access
              byte status = PCD_Authenticate( MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &rekey, &( uid ));
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
              PICC_HaltA();
              PCD_StopCrypto1();
          
              Serial.println("block was written");
                }
        }
        }
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
        
          PICC_HaltA();
          PCD_StopCrypto1();
          return data;
     }

     void fast_reset(){
        digitalWrite(RST_PIN, HIGH);
        PCD_Reset();
        PCD_WriteRegister(TModeReg, 0x80);      // TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
        PCD_WriteRegister(TPrescalerReg, 0x43);   // 10μs.
        //  mfrc522.PCD_WriteRegister(TPrescalerReg, 0x20);   // test

        PCD_WriteRegister(TReloadRegH, 0x00);   // Reload timer with 0x064 = 30, ie 0.3ms before timeout.
        PCD_WriteRegister(TReloadRegL, 0x1E);
        //mfrc522.PCD_WriteRegister(TReloadRegL, 0x1E);

        PCD_WriteRegister(TxASKReg, 0x40);    // Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
        PCD_WriteRegister(ModeReg, 0x3D);   // Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)

        PCD_AntennaOn();  
     }
};
