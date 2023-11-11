//*****************************************************************************
// Universidad del Valle de Guatemala
// BE3015 - Electrónica Digital 2
// Proyecto 3 - María Alejandra Rodríguez
// Sensor de temperatura I2C - Comunicación con TIVA C y Pantalla SPI
//*****************************************************************************
//*****************************************************************************
// Librerías
//*****************************************************************************
#include <Arduino.h>
#include <Adafruit_NeoPixel.h> 
#include <Temperature_LM75_Derived.h> 
#include <Wire.h>

//*****************************************************************************
//  Definición de pines
//*****************************************************************************
#define RX_2 16 //Para comunicación serial con TIVA
#define TX_2 17 //Para comunicación serial con TIVA
#define PIN  23 //Para la conexión del Neopíxel
#define NUMPIXELS  24 //Número de píxeles del neopíxel 

Adafruit_NeoPixel pixels(NUMPIXELS,PIN, NEO_GRB); //Funciones de la librería 

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
uint32_t readADC_Cal(int ADC_Raw);
//void temperatura(void);
void setPixel(int Pixel, byte red, byte green, byte blue);
void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause);
void showStrip(void);
void colorWipe(byte red, byte green, byte blue, int SpeedDelay);
void setAll(byte red, byte green, byte blue);
void BouncingColoredBalls(int BallCount, byte colors[][3]);

//*****************************************************************************
// Variables Globales
//*****************************************************************************
/*int Sensor_Raw = 0;
float voltaje =0.0; 
float Sensor1 = 0.0;*/
float temp;
int senal;
Generic_LM75 temperature;

//*****************************************************************************
// Configuración
//*****************************************************************************
void setup() {
  //Comunicación UART0 con la computadora Serial (0)
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Se configuró Serial 0");
  Serial2.begin(115200, SERIAL_8N1, RX_2, TX_2); //Establecer comunicación serial con TIVA

  pixels.begin(); 
  pixels.clear();
  pixels.setBrightness(255);

  pixels.setPixelColor(0, pixels.Color(255,100,150));
  pixels.show();
  
}

//*****************************************************************************
// Loop
//*****************************************************************************
void loop() {
  Serial.print("Temperature = ");
  temp = temperature.readTemperatureC();
  Serial.print(temp);
  Serial.println(" C");
  delay(250);
  // Recibir datos de la TIVA C para colocar en la LCD
  if (Serial2.available()) {
    senal = Serial2.read();
  }

  if(senal == '1') {
    temp = temperature.readTemperatureC();  // Asignar la temperatura leída a 'temp'
    Serial2.println(temp);
    Serial.print("Dato enviado a TIVA C: ");
    Serial.print(temp);
    Serial.print("°C 🌡️ \n");
    senal = 0; 
  }

  if(senal == '2') {
    //Agregar función de neopíxel 
    senal = 0; 
  }
  
  /* //Funciones para el neopíxel 
  byte colors[3][3] = { {0xff, 0,0}, 
                        {0xff, 0xff, 0xff}, 
                        {0   , 0   , 0xff} };

  BouncingColoredBalls(3, colors);
  colorWipe(0x00,0xff,0x00, 50);
  colorWipe(0x00,0x00,0x00, 50);

  /*delay(100); 
  
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

//*****************************************************************************
// Funciones
//*****************************************************************************
/*void temperatura(void) {
// Leer el pin LM35_Sensor1 ADC
  Sensor_Raw = analogRead(SensorTemp);
  // Calibrar ADC y tomar el voltaje en mV
  voltaje = readADC_Cal(Sensor_Raw);
  Sensor1 = ((voltaje/4095)*3.25)/0.01; // De ser necesario se multiplica por un factor para que lea correctamente la temperatura
}*/

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