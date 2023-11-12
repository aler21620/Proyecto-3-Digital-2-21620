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
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Temperature_LM75_Derived.h>
#include <Wire.h>

//*****************************************************************************
//  Definición de pines
//*****************************************************************************
#define RX_2 16       // Para comunicación serial con TIVA
#define TX_2 17       // Para comunicación serial con TIVA
#define CIRCLE_PIN 25 // Para la conexión del Neopíxel
#define NUM_CIRCLE_LEDS 24 //Número de pines del Neopíxel 
#define BRIGHT 50   // Brillo del Neopíxel

Adafruit_NeoPixel circle = Adafruit_NeoPixel(NUM_CIRCLE_LEDS, CIRCLE_PIN, NEO_GRB + NEO_KHZ800);

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
void temperatura(void);
void encenderTodos(void);
void apagarTodos(void);
void enviando (void);
void color_TEMP(void); 
void guardando(void);

//*****************************************************************************
// Variables Globales
//*****************************************************************************
float temp;
int senal;
Generic_LM75 temperature;
const float TEMP_LOW = 25.0; //Valor mínimo de temperatura para considerarlo en estado bajo 
const float TEMP_MEDIUM = 27.0; //Valor medio de temperatura para considerarlo en estado medio
const float TEMP_HIGH = 30.0; //Valor medio de temperatura para considerarlo en estado alto 

//*****************************************************************************
// Configuración
//*****************************************************************************
void setup() {
  // Comunicación UART0 con la computadora Serial (0)
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Se configuró Serial 0");
  Serial2.begin(115200, SERIAL_8N1, RX_2, TX_2); // Establecer comunicación serial con TIVA

  //#if defined (__AVR_ATtiny85__)
    //if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  //#endif

  circle.begin();
  circle.clear();
  circle.setBrightness(BRIGHT);
  
}

//*****************************************************************************
// Loop
//*****************************************************************************
void loop() {
  encenderTodos(); 
  delay(500);
  apagarTodos(); 
  delay(500);
  // Recibir datos de la TIVA C para colocar en la LCD
  if (Serial2.available()) {
    senal = Serial2.read();
  }

  if (senal == '1') {
    temperatura();
    enviando();
    delay(3000);
    Serial2.println(temp);
    Serial.print("Dato enviado a TIVA C: ");
    Serial.print(temp);
    Serial.print("°C 🌡️ \n");
    apagarTodos(); 
    delay(500);
    color_TEMP();
    delay(3000);
    senal = 0;
  }

  if (senal == '2') {
    Serial.print("Señal recibida de TIVA C: ");
    Serial.print("Datos guardados en SD \n");
    guardando(); 
    delay(3000);
    senal = 0;
  }
}

//*****************************************************************************
// Funciones
//*****************************************************************************
void temperatura(void) {
  temp = temperature.readTemperatureC();
  delay(250);
}

void encenderTodos() {
  for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(130, 130, 0)); // Establecer color amarillo en el LED actual
  }
  circle.show(); // Mostrar los cambios en los LEDs
}

void apagarTodos() {
  for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(0, 0, 0)); // Apagar el LED actual
  }
  circle.show(); // Mostrar los cambios en los LEDs
}

void enviando () {
  for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(random(255), random(255), random(255))); // Establecer color verde en el LED actual
  }
  circle.show(); // Mostrar los cambios en los LEDs
}

void color_TEMP () {
  temperatura();
  if(temp < TEMP_LOW) {
    for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(10, 10, 179)); // Apagar el LED actual
    }
    circle.show(); // Mostrar los cambios en los LEDs
  } else if (temp >= TEMP_LOW && temp < TEMP_MEDIUM) {
    for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(0, 255, 0)); // Apagar el LED actual
    }
    circle.show(); // Mostrar los cambios en los LEDs
  } else if (temp >= TEMP_MEDIUM && temp <= TEMP_HIGH) {
    for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(255, 0, 0)); // Apagar el LED actual
    }
    circle.show(); // Mostrar los cambios en los LEDs
  }
}

void guardando () { 
  for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(200, 120, 120)); // Establecer color verde en el LED actual
  }
  circle.show(); // Mostrar los cambios en los LEDs
}
