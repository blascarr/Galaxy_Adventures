
class RFID_Controller: public MFRC522{
  public:
    MFRC522::MIFARE_Key key; 
    // ---- Time Controller ---- //
    long timestamp;
    uint16_t duration = 200 ;
    int block = 4; 
    
    RFID_Controller( uint8_t SSPIN, uint8_t RSTPIN ):MFRC522( SSPIN, RSTPIN ) {
      
    }
    
    void init(){
      SPI.begin(); 
      PCD_Init( );
    }

    void update(){
      if( ( millis() - timestamp ) > duration ){
         
         timestamp = millis();
         
          if (  PICC_IsNewCardPresent()) {
            // Select one of the cards
            if (  PICC_ReadCardSerial()) 
            {
                 Serial.println("card selected Data:");
                 printArray( uid.uidByte,  uid.size);
                 Serial.println();
                 //Serial.println( readBlock( reader, block , key  ));
                 
                 //Serial.println("read block: ");
            }
          
         } 
      }
       
    }

    void printArray(byte *buffer, byte bufferSize) {
       for (byte i = 0; i < bufferSize; i++) {
          Serial.print(buffer[i] < 0x10 ? " 0" : " ");
          Serial.print(buffer[i], HEX);
       }
    }


    String readBlock(MFRC522 & reader, int blockNumber, MFRC522::MIFARE_Key key_block ) {
        int trailerBlock=blockNumber/4*4+3;//determine trailer block for the sector
        byte readbackblock[18];
        
        byte status = reader.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key_block, &(reader.uid));
      
        if (status != MFRC522::STATUS_OK) {
               Serial.print("PCD_Authenticate() failed (read): ");
               Serial.println(reader.GetStatusCodeName(status));
               reader.PICC_HaltA();
               reader.PCD_StopCrypto1();
               return "ERROR AUTHENTICATE";
        }
      
        //reading a block
        byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
        status = reader.MIFARE_Read(blockNumber, readbackblock, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
          if (status != MFRC522::STATUS_OK) {
                  Serial.print("MIFARE_read() failed: ");
                  Serial.println(reader.GetStatusCodeName(status));
                   reader.PICC_HaltA();
                  reader.PCD_StopCrypto1();
                  return "ERROR READ";
          }
          //Serial.println("block was read");
          String data = String((char*)readbackblock);
          //Serial.println(data);
        
          reader.PICC_HaltA();
          reader.PCD_StopCrypto1();
          return data;
     }
};
