/**
 *  QRCode
 *
 *  A quick example of generating a QR code.
 *
 *  This prints the QR code to the serial monitor as solid blocks. Each module
 *  is two characters wide, since the monospace font used in the serial monitor
 *  is approximately twice as tall as wide.
 *
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <qrcode.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// OLED display object
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

int pinLed1 = 4;
int pinLed2 = 5;
int pinLed3 = 6;
int pinLed4 = 7;
// Pin analogico de entrada para el LDR
int pinLDR = 0;
 
// Variable donde se almacena el valor del LDR
int valorLDR = 0;  

void setup(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  displayQRCode("kDkvYHedDoPjuYp6AiMx");

    // Configuramos como salidas los pines donde se conectan los led
  pinMode(pinLed1, OUTPUT);
  pinMode(pinLed2, OUTPUT);
  pinMode(pinLed3, OUTPUT);
  pinMode(pinLed4, OUTPUT);
  
  //  Configurar el puerto serial
  Serial.begin(9600);
}

void displayQRCode(const char* text) {
  
    // Create the QR code
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(3)];
    qrcode_initText(&qrcode, qrcodeData, 3, 0, text);

    int scale = min(OLED_WIDTH / qrcode.size, OLED_HEIGHT / qrcode.size);

    int shiftX = (OLED_WIDTH - qrcode.size*scale)/2;
    int shiftY = (OLED_HEIGHT - qrcode.size*scale)/2;

    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
          if (qrcode_getModule(&qrcode, x, y)){
            display.fillRect(shiftX + x*scale, shiftY + y*scale, scale, scale, WHITE);
          }

        }
    }
    display.display();
}

void loop() {
  // Apagar todos los leds siempre que se inicia el ciclo
  digitalWrite(pinLed1, LOW);
  digitalWrite(pinLed2, LOW);
  digitalWrite(pinLed3, LOW);
  digitalWrite(pinLed4, LOW);
 
  // Guardamos el valor leido del ADC en una variable
  // El valor leido por el ADC (voltaje) aumenta de manera directamente proporcional
  // con respecto a la luz percibida por el LDR
  valorLDR= analogRead(pinLDR);
  
  // Devolver el valor leido a nuestro monitor serial en el IDE de Arduino
  Serial.println(valorLDR);
 
  // Encender los leds apropiados de acuerdo al valor de ADC
  if(valorLDR < 300)
  {
    digitalWrite(pinLed1, HIGH);
    digitalWrite(pinLed2, HIGH);
  }
  if(valorLDR < 700)
  {
    digitalWrite(pinLed3, HIGH);
    digitalWrite(pinLed4, HIGH);
  }
  if(valorLDR > 1000)
  {
    digitalWrite(pinLed1, LOW);
    digitalWrite(pinLed2, LOW);
    digitalWrite(pinLed3, LOW);
    digitalWrite(pinLed4, LOW);
  }
  // Esperar unos milisegundos antes de actualizar
  delay(100);
}
