#include <Adafruit_NeoPixel.h>

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS_PER_STRIP, PIN, NEO_GRB + NEO_KHZ800);

 
struct rangeColor{
  uint32_t min_HUE;
  uint32_t max_HUE;
  rangeColor(uint32_t mincolor, uint32_t maxcolor): min_HUE( mincolor ),max_HUE( maxcolor ){}
};

rangeColor NEOPIXEL_BLUE( 65536/2, 65536/3*2 );
rangeColor NEOPIXEL_GREEN( 65536/2, 65536/3*2 );
rangeColor NEOPIXEL_RED( 65536/2, 65536/3*2 );

class Neopixel_Controller : public Adafruit_NeoPixel{
  public:
    uint32_t timestamp;
    uint16_t duration = 50;
    uint8_t currentLED = 0;
    uint32_t currentColor = 65536/3;
    rangeColor currentRangeColor = NEOPIXEL_BLUE;
    
    long firstPixelHue;
    uint8_t speed = 64;
    
    typedef void (Neopixel_Controller::*pixelFunction)();
    //pixelFunction update_fn;
    pixelFunction update_fn = &Neopixel_Controller::rainbow;

  Neopixel_Controller ( uint16_t n, uint16_t pin , neoPixelType type=NEO_GRB+NEO_KHZ800) : Adafruit_NeoPixel( n, pin, type ){
    
  }

  void update(){
    if ( millis()- timestamp > duration ) {
        timestamp = millis();
        
        ( this->*update_fn)() ;
    }
  }

  void pixelRun(){
        currentLED ++;
        setPixelColor( currentLED% ( NUM_LEDS_PER_STRIP ), currentColor  );
        show();
        
        setPixelColor( (currentLED -1)%( NUM_LEDS_PER_STRIP ),  Color( 0, 0 ,0 )    );
        show();
  }

  void colorRun(){
        currentLED ++;
        if( currentLED % ( 2*NUM_LEDS_PER_STRIP ) <= 7){
            setPixelColor(currentLED % ( NUM_LEDS_PER_STRIP ), currentColor );
        }else if ( currentLED % ( 2*NUM_LEDS_PER_STRIP ) > 7 ){
            setPixelColor(currentLED % ( NUM_LEDS_PER_STRIP ),  Color( 0,0,0 ) );
        }
        show();
  }

  // Duration 5 and speed 64
  void rainbow(){
        currentLED ++;
        firstPixelHue = firstPixelHue + speed;
        firstPixelHue = (firstPixelHue % (5*65536)); //5*65536
        int pixelHue = firstPixelHue + ( currentLED % ( NUM_LEDS_PER_STRIP ) * 10923L / (numPixels())); //65536L/6
        setPixelColor(currentLED % ( NUM_LEDS_PER_STRIP ), gamma32( ColorHSV(pixelHue)));
        show();
  }

  // Solo de un color concreto
  void glow(){
        currentLED ++;
        firstPixelHue = 65536/2 + firstPixelHue + speed;
        firstPixelHue = (firstPixelHue % (5*65536/6));
        int pixelHue = firstPixelHue + ( currentLED % ( NUM_LEDS_PER_STRIP ) * 10923L / ( 6*numPixels()));

        setPixelColor(currentLED % ( NUM_LEDS_PER_STRIP ), gamma32( ColorHSV(pixelHue)));
        show();
  }
  
  void success( bool ON ){
    if ( ON ){
      changeEffect( &colorRun );
    }else{
      changeEffect( &glow );
    }
  }

  void changeEffect( pixelFunction _f ){
      currentLED = 0;
      update_fn = _f;
  }

  void changeSpeed( uint8_t v){
    speed = v;
  }

  void changeRefresh( uint16_t dt ){
    duration = dt;
  }

  void setColor( uint32_t c){
    currentColor = c;
  }
};
