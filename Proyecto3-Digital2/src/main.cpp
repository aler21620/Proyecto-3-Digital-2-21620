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
#include <Adafruit_NeoPixel.h> //Librería que permitirá codificar el neopíxel 
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Temperature_LM75_Derived.h> //Librería que implementa la comunicación I2C con el sensor
#include <Wire.h> //Para la comunicación I2C

//*****************************************************************************
//  Definición de pines
//*****************************************************************************
#define RX_2 16       // Para comunicación serial con TIVA
#define TX_2 17       // Para comunicación serial con TIVA
#define CIRCLE_PIN 25 // Para la conexión del Neopíxel
#define NUM_CIRCLE_LEDS 24 //Número de pines del Neopíxel 
#define BRIGHT 50   // Brillo del Neopíxel
#define LM75_ADDRESS 0x48 // Dirección I2C del sensor LM75

//Creación del objeto del Neopíxel para poder hacer diferentes diseños con colores
Adafruit_NeoPixel circle = Adafruit_NeoPixel(NUM_CIRCLE_LEDS, CIRCLE_PIN, NEO_GRB + NEO_KHZ800); 

//*****************************************************************************
// Prototipos de función
//*****************************************************************************
float readTemperature(); //Para leer temperatura con sensor I2C
void encenderTodos(void); //Para encender todos los leds en estado de espera
void apagarTodos(void); //Función para apagar el neopíxel 
void enviando (void); //Para indicar que está enviando el dato leído de temperatura
void color_TEMP(void); //Para indicar en que estado está la temperatura 
void guardando(void); //Para indicar que está guardando los datos en la SD

//*****************************************************************************
// Variables Globales
//*****************************************************************************
float temp; //Para guardar la lectura de temperatura
int senal; //Para leer y almacenar la señal que envía TIVA C
Generic_LM75 temperature; //Crear el sensor con la librería
const float TEMP_LOW = 25.0; //Valor mínimo de temperatura para considerarlo en estado bajo 
const float TEMP_MEDIUM = 27.0; //Valor medio de temperatura para considerarlo en estado medio
const float TEMP_HIGH = 30.0; //Valor medio de temperatura para considerarlo en estado alto 

//*****************************************************************************
// Configuración
//*****************************************************************************
void setup() {
  // Comunicación UART0 con la computadora Serial (0)
  Serial.begin(115200);
  Wire.begin(21, 22); //Para el funcionamiento de I2C
  Serial.println("Se configuró Serial 0");
  Serial2.begin(115200, SERIAL_8N1, RX_2, TX_2); // Establecer comunicación serial con TIVA

  //Inicialización del Neopíxel 
  circle.begin();
  circle.clear();
  circle.setBrightness(BRIGHT);
}

//*****************************************************************************
// Loop
//*****************************************************************************
void loop() {
  //Colocar el Neopíxel en estado de espera
  encenderTodos(); //Titila en color amarillo
  delay(500);
  apagarTodos(); 
  delay(500);
  // Recibir datos de la TIVA C
  if (Serial2.available()) {
    senal = Serial2.read();
  }

  //Verificar si la señal es para leer temperatura 
  if (senal == '1') {
    float temperature = readTemperature(); //Leer temperatura del sensor
    enviando(); //Estado de envío de datos en el neopíxel 
    delay(3000);
    Serial2.println(temperature); //Enviar dato a TIVA
    Serial.print("Dato enviado a TIVA C: "); 
    Serial.print(temperature);
    Serial.print("°C 🌡️ \n");
    apagarTodos(); //Apagar el Neopíxel 
    delay(500);
    color_TEMP(); //Encender Neopíxel según estado de la temperatura 
    delay(3000);
    senal = 0; //Regresar la señal a 0, para esperar intrucciones de TIVA
  }

  //Verificar si la señal es para guardar datos 
  if (senal == '2') {
    Serial.print("Señal recibida de TIVA C: ");
    Serial.print("Datos guardados en SD \n"); //Indicar que está guardando los datos 
    guardando(); //Encender el Neopíxel en el color que indica que los guardó 
    delay(3000);
    senal = 0; //Regresar la señal a 0, para esperar intrucciones de TIVA
  }
}

//*****************************************************************************
// Funciones
//*****************************************************************************
//Función para leer la temperatura con el sensor I2C
float readTemperature() {
  Wire.beginTransmission(LM75_ADDRESS); //Leer la temperatura a través de I2C y Wire
  Wire.write(0x00); // Registro de lectura de temperatura (0x00 para lectura)
  Wire.endTransmission();

  Wire.requestFrom(LM75_ADDRESS, 2); // Se solicitan 2 bytes de datos de temperatura
  int16_t tempData = (Wire.read() << 8) | Wire.read(); // Combinar los bytes recibidos

  float temperature = tempData / 256.0; // Convertir datos a grados Celsius
  return temperature;
}

//Función para encender todos los leds del Neopíxel para el estado de espera
void encenderTodos() {
  for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(130, 130, 0)); // Establecer color amarillo en el LED actual
  }
  circle.show(); // Mostrar los cambios en los LEDs
}

//Función para apagar todos los leds
void apagarTodos() {
  for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(0, 0, 0)); // Apagar el LED actual
  }
  circle.show(); // Mostrar los cambios en los LEDs
}

//Función para encender todos los leds e indicar que está enviando el dato 
void enviando () {
  for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(random(255), random(255), random(255))); // Establecer color aleatorio a cada led 
  }
  circle.show(); // Mostrar los cambios en los LEDs
}

//Función para verificar los límites de temperatura y encender los leds de acuerdo al valor 
void color_TEMP () {
  float temperature = readTemperature();
  if(temperature < TEMP_LOW) {
    for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
      circle.setPixelColor(i, circle.Color(0, 0, 255)); // Color AZUL, temperatura baja
    }
    circle.show(); // Mostrar los cambios en los LEDs
  } else if (temperature >= TEMP_LOW && temperature < TEMP_MEDIUM) {
    for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
      circle.setPixelColor(i, circle.Color(0, 255, 0)); // Color VERDE, temperatura ambiente 
    }
    circle.show(); // Mostrar los cambios en los LEDs
  } else if (temperature >= TEMP_MEDIUM && temperature <= TEMP_HIGH) {
    for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
      circle.setPixelColor(i, circle.Color(255, 0, 0)); // Color ROJO, temperatura alta 
    }
    circle.show(); // Mostrar los cambios en los LEDs
  }
}

//Función para encender los leds e indicar que está en estado de guardar datos 
void guardando () { 
  for (int i = 0; i < NUM_CIRCLE_LEDS; i++) {
    circle.setPixelColor(i, circle.Color(200, 120, 120)); // Color para indicar que está guardando 
  }
  circle.show(); // Mostrar los cambios en los LEDs
}
