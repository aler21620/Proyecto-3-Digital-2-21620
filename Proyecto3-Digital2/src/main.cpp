#include <Adafruit_NeoPixel.h> 


#define PIN  23
#define NUMPIXELS  24

Adafruit_NeoPixel pixels(NUMPIXELS,PIN, NEO_GRB);

#define PIN2  15
#define NUMPIXELS2  16

Adafruit_NeoPixel pixels2(NUMPIXELS2,PIN2, NEO_GRB);


void setPixel(int Pixel, byte red, byte green, byte blue);
void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause);
void showStrip(void);
void colorWipe(byte red, byte green, byte blue, int SpeedDelay);
void setAll(byte red, byte green, byte blue);
void BouncingColoredBalls(int BallCount, byte colors[][3]);

//***********************************************************************************************
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  pixels.begin(); 
  pixels.clear();
  pixels.setBrightness(255);

  pixels.setPixelColor(0, pixels.Color(255,100,150));
  pixels.show();

  pixels2.begin(); 
  pixels2.clear();
  pixels2.setBrightness(255);

  pixels2.setPixelColor(0, pixels.Color(255,100,150));
  pixels2.show();

}

void loop() {
 byte colors[3][3] = { {0xff, 0,0}, 
                        {0xff, 0xff, 0xff}, 
                        {0   , 0   , 0xff} };

  BouncingColoredBalls(3, colors);
  colorWipe(0x00,0xff,0x00, 50);
  colorWipe(0x00,0x00,0x00, 50);
   // put your main code here, to run repeatedly:
  /*delay(100); // this speeds up the simulation
  
  pixels.setBrightness(255);

  for(int i =0; i < 16; i++){
    pixels.setPixelColor(i, pixels.Color(random(255),random(255),random(255)));
    pixels.show();
    delay(100); // this speeds up the simulation
  
  }
  for(int i =16; i < 32; i++){
    pixels.setPixelColor(i, pixels.Color((255),(255),(255)));
    pixels.show();
    delay(100); // this speeds up the simulation
  
  }
  pixels.clear();
  pixels.show();
*/
}


void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   pixels.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}
void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUMPIXELS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   pixels.setPixelColor(Pixel, pixels.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUMPIXELS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void BouncingColoredBalls(int BallCount, byte colors[][3]) {
  float Gravity = -9.81;
  int StartHeight = 1;
  
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
  
  for (int i = 0 ; i < BallCount ; i++) {   
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0; 
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2); 
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
  
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
  
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUMPIXELS - 1) / StartHeight);
    }
  
    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);
    }
    
    showStrip();
    setAll(0,0,0);
  }
}