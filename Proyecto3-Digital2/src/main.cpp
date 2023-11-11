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
#define RX_2 16       // Para comunicación serial con TIVA
#define TX_2 17       // Para comunicación serial con TIVA
#define CIRCLE_PIN 25 // Para la conexión del Neopíxel
#define NUM_CIRCLE_LEDS 24 //Número de pines del Neopíxel 
#define BRIGHT 100   // Brillo del Neopíxel

Adafruit_NeoPixel circle(NUM_CIRCLE_LEDS, CIRCLE_PIN, NEO_GRB + NEO_KHZ800);

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
void temperatura(void);

//*****************************************************************************
// Variables Globales
//*****************************************************************************
float temp;
int senal;
Generic_LM75 temperature;

//*****************************************************************************
// Configuración
//*****************************************************************************
void setup() {
  // Comunicación UART0 con la computadora Serial (0)
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Se configuró Serial 0");
  Serial2.begin(115200, SERIAL_8N1, RX_2, TX_2); // Establecer comunicación serial con TIVA

  circle.begin();
  circle.clear();
  circle.setBrightness(BRIGHT);
}

//*****************************************************************************
// Loop
//*****************************************************************************
void loop()
{
  uint8_t rojo = 255;
  uint8_t verde = 0;
  uint8_t azul = 0;

  for (int i = 0; i < NUM_CIRCLE_LEDS; i++)
  {
    circle.setPixelColor(i, rojo, verde, azul);
    circle.show();
    delay(40);
    azul = azul + 12;
    rojo = rojo - 12;
  }

  rojo = 0;
  verde = 255;
  azul = 0;
  circle.clear();

  for (int i = NUM_CIRCLE_LEDS - 1; i >= 0; i--)
  {
    circle.setPixelColor(i, rojo, verde, azul);
    circle.show();
    delay(50);
  }

  // And again
  verde = 0;
  azul = 255;
  circle.clear();

  for (int i = 0; i < NUM_CIRCLE_LEDS; i++)
  {
    circle.setPixelColor(i, rojo, verde, azul);
    circle.show();
    delay(30);
    verde += 8;
  }

  // Clear down the circle LEDs and start again
  circle.clear();
  circle.show();

  temperatura();
  // Recibir datos de la TIVA C para colocar en la LCD
  if (Serial2.available())
  {
    senal = Serial2.read();
  }

  if (senal == '1')
  {
    temp = temperature.readTemperatureC(); // Asignar la temperatura leída a 'temp'
    Serial2.println(temp);
    Serial.print("Dato enviado a TIVA C: ");
    Serial.print(temp);
    Serial.print("°C 🌡️ \n");
    senal = 0;
  }

  if (senal == '2')
  {
    // Agregar función de neopíxel
    senal = 0;
  }
}

//*****************************************************************************
// Funciones
//*****************************************************************************
void temperatura(void)
{
  temp = temperature.readTemperatureC();
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" C");
  delay(250);
}
