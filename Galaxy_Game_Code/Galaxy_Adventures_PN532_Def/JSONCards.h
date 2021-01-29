
#ifdef SDLOADER
  #include <SD.h>
#else

#endif

String JSONString = "{\"name\":\"Superherosquad\",\"color\":\"BLUE\",\"colorValue\":34456,\"credits\":1200,\"energy\":100,\"resources\":{\"uranium\":36,\"titanium\":25,\"steel\":57,\"crystal\":12},\"fuel\":{\"gas\":17,\"hydrogen\":29,\"water\":56,\"oxygen\":21},\"life\":{\"flora\":2,\"fauna\":3,\"bacterium\":7,\"oceans\":2},\"life_forms\":{\"martian\":\"Moleculian\",\"venus\":\"SunCloud\",\"saturn\":\"DustSouls\",\"outergalaxy\":\"Crockroach\"},\"achievement\":{\"mechanism\":true,\"containment\":false,\"virus\":false,\"aceesslevel\":false},\"binnacle\":{\"shipment\":1,\"tripulation\":2,\"notes\":3,\"wreck\":4},\"audio\":{\"track\":5,\"track\":7,\"track\":3,\"track\":6},\"accesslevel-I\":{\"msg1\":\"La salida\",\"msg2\":\"se encuentra delante\",\"msg3\":\"del aula\",\"msg4\":\"bajo la silla\"},\"accesslevel-II\":{\"msg1\":\"Si tu eres tu\",\"msg2\":\"y yo soy yo\",\"msg3\":\"quien ses ,as tonto\",\"msg4\":\"de los dos\"},\"accesslevel-III\":{\"msg1\":\"Busca en\",\"msg2\":\"el ojo del destino\",\"msg3\":\"cuando cae la noche\",\"msg4\":\"sobre la luz\"},\"message\":{\"msg\":\"Un foton\",\"enc\":\"puede existir\",\"pls\":\"y desaparecer\",\"flu\":\"3 veces\"}}";


DynamicJsonDocument doc(1500);

class CardJSON{
  public:
    String values[64];
    
    #ifdef SDLOADER
      File root;
      char *filename = "/P1.TXT";
    #endif
    
    CardJSON( ){
      
    }

    void loadPlayer( RFID_Controller &card ){
      //String to byte array
      uint8_t cardstart = 4;
       for( int i = cardstart; i<= 16; i++ ){
        byte buffer[16];
        
        byte cstr [16];
        if( i%4 != 3 ){
          //Serial.println( values[ i-cardstart ] );
          strcpy( cstr, values[ i-cardstart ].c_str() );
          //Serial.println( cstr );
          card.writeBlock( i, cstr );
        } 
      }
    }

    void print( RFID_Controller &card ){
      
    }
    
    void SDdeserialize(){
      #ifdef SDLOADER
        if (!SD.begin( SD_PIN)) {
          Serial.println("Card failed, or not present");
        }else{
          Serial.println("SD Card success");     
        }
        root = SD.open( "/" );
        File dataFile = SD.open( filename );
        
        DeserializationError error = deserializeJson(doc, dataFile);
        //Deserialization error is kaputt
        /*if (error){
            Serial.println(F("Failed to read file, using default configuration"));
        }else{
            Serial.println(F("Deserialization succeeded"));
        }*/
        SDPrintFiles(root, 0);
        
        /*File dataFile = SD.open( filename );
        if (dataFile) {
          while (dataFile.available()) {
            Serial.write(dataFile.read());
          }
          dataFile.close();
        }else {
          Serial.println("error opening p1.json");
        }
        */
        JsonObject obj = doc.as<JsonObject>();
  
        values[0] = obj["name"].as<String>();
        values[1] = obj["color"].as<String>();
        values[2] = obj["colorValue"].as<String>();
        values[3] = obj["credits"].as<String>();
        values[4] = obj["energy"].as<String>();
  
        
        values[5] = obj["resources"]["uranium"].as<String>();
        values[6] = obj["resources"]["titanium"].as<String>();
        // 7 is trailer Block
        values[7] = obj["resources"]["steel"].as<String>();
        values[8] = obj["resources"]["crystal"].as<String>();
        values[9] = obj["fuel"]["gas"].as<String>();
        values[10] = obj["fuel"]["hydrogen"].as<String>();
        values[11] = obj["fuel"]["water"].as<String>();
        values[12] = obj["fuel"]["oxygen"].as<String>();
        values[13] = obj["life"]["flora"].as<String>();
        values[14] = obj["life"]["fauna"].as<String>();
        values[15] = obj["life"]["bacterium"].as<String>();
        values[16] = obj["life"]["oceans"].as<String>();
      
        for( int i = 0; i< 4; i++ ){
          Serial.println( values[ i ] );
        }
        
        /*values[17] = obj["martian"].as<String>();
        values[18] = obj["venus"].as<String>();
        values[19] = obj["saturn"].as<String>();
        values[20] = obj["outer galaxy"].as<String>();
  
        values[21] = obj["mechanism"].as<String>();
        values[22] = obj["containment"].as<String>();
        values[23] = obj["virus"].as<String>();
        values[24] = obj["accesslevel"].as<String>();
        values[25] = obj["shipment"].as<String>();
        values[26] = obj["tripulation"].as<String>();
        values[27] = obj["notes"].as<String>();
        values[28] = obj["wreck"].as<String>();
        values[29] = obj["trackI"].as<String>();
        values[30] = obj["trackII"].as<String>();
        values[31] = obj["trackIII"].as<String>();
        values[32] = obj["trackIV"].as<String>();
        values[33] = obj["msg1"].as<String>();
        values[34] = obj["msg2"].as<String>();
        values[35]= obj["msg3"].as<String>();
        values[36] = obj["msg4"].as<String>();*/
        root.close();
      #endif
    }

    
    void deserialize(){
      #ifndef SDLOADER
        deserializeJson( doc, JSONString);
        JsonObject obj = doc.as<JsonObject>();
  
        values[0] = obj["name"].as<String>();
        values[1] = obj["color"].as<String>();
        values[2] = obj["colorValue"].as<String>();
        values[3] = obj["credits"].as<String>();
        values[4] = obj["energy"].as<String>();
  
        
        values[5] = obj["resources"]["uranium"].as<String>();
        values[6] = obj["resources"]["titanium"].as<String>();
        // 7 is trailer Block
        values[7] = obj["resources"]["steel"].as<String>();
        values[8] = obj["resources"]["crystal"].as<String>();
        values[9] = obj["fuel"]["gas"].as<String>();
        values[10] = obj["fuel"]["hydrogen"].as<String>();
        values[11] = obj["fuel"]["water"].as<String>();
        values[12] = obj["fuel"]["oxygen"].as<String>();
        values[13] = obj["life"]["flora"].as<String>();
        values[14] = obj["life"]["fauna"].as<String>();
        values[15] = obj["life"]["bacterium"].as<String>();
        values[16] = obj["life"]["oceans"].as<String>();
        
        for( int i = 0; i< 4; i++ ){
          Serial.println( values[ i ] );
        }
        
        /*values[17] = obj["martian"].as<String>();
        values[18] = obj["venus"].as<String>();
        values[19] = obj["saturn"].as<String>();
        values[20] = obj["outer galaxy"].as<String>();
  
        values[21] = obj["mechanism"].as<String>();
        values[22] = obj["containment"].as<String>();
        values[23] = obj["virus"].as<String>();
        values[24] = obj["accesslevel"].as<String>();
        values[25] = obj["shipment"].as<String>();
        values[26] = obj["tripulation"].as<String>();
        values[27] = obj["notes"].as<String>();
        values[28] = obj["wreck"].as<String>();
        values[29] = obj["trackI"].as<String>();
        values[30] = obj["trackII"].as<String>();
        values[31] = obj["trackIII"].as<String>();
        values[32] = obj["trackIV"].as<String>();
        values[33] = obj["msg1"].as<String>();
        values[34] = obj["msg2"].as<String>();
        values[35]= obj["msg3"].as<String>();
        values[36] = obj["msg4"].as<String>();*/
      #endif
      
      
    }
    
    #ifdef SDLOADER
        void SDPrintFiles(File dir, int numTabs){
            while (true) {
              File entry =  dir.openNextFile();
              if (! entry) {
                // no more files
                break;
              }
              for (uint8_t i = 0; i < numTabs; i++) {
                Serial.print('\t');
              }
              Serial.print(entry.name());
              if (entry.isDirectory()) {
                Serial.println("/");
                SDPrintFiles(entry, numTabs + 1);
              } else {
                // files have sizes, directories do not
                Serial.print("\t\t");
                Serial.println(entry.size(), DEC);
              }
              entry.close();
            }
        }
    #endif
};
