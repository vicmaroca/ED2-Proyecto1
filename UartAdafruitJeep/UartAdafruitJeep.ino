#include "config.h"
#include <Arduino.h>
/************************ Example Starts Here *******************************/

// Definir pines para los puertos UART
#define TX_PIN_2 17  // Cambiar al pin adecuado en tu placa
#define RX_PIN_2 16  // Cambiar al pin adecuado en tu placa
HardwareSerial miSerial2(2); // Mi instancia personalizada de Serial2

// this int will hold the current count for our sketch
int luz = 0;
int rain = 0;
int objeto = 0;


// set up the 'counter' feed
AdafruitIO_Feed *lucesFeed = io.feed("luces");
AdafruitIO_Feed *lluviaFeed = io.feed("lluvia");
AdafruitIO_Feed *obsFeed = io.feed("obs");

void setup() {
  // start the serial connection
  Serial.begin(115200);
  miSerial2.begin(9600, SERIAL_8N1, RX_PIN_2, TX_PIN_2);
  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  io.run();

   if (miSerial2.available()) {
        String mensaje = miSerial2.readStringUntil('\n');   // Leer el string hasta encontrar un salto de línea
        mensaje.trim();   //// Eliminar espacios en blanco al inicio y final del string

      if (mensaje.startsWith("D1-")) {  // Verificar si el string comienza con "D1-"
          String objetoStr = mensaje.substring(3);  // Obtener el valor numérico restante
          int objeto = valueStr.toInt();  // Convertir el valor a un entero
          
          //Serial.print("Valor recibido: ");
          //Serial.println(objeto);
      }
      if (mensaje.startsWith("D2-")) {  // Verificar si el string comienza con "D1-"
          String luzStr = mensaje.substring(3);  // Obtener el valor numérico restante
          int luz = valueStr2.toInt();  // Convertir el valor a un entero
          
          //Serial.print("Valor recibido: ");
          //Serial.println(luz);
      }
      if (mensaje.startsWith("D3-")) {  // Verificar si el string comienza con "D1-"
          String rainStr = mensaje.substring(3);  // Obtener el valor numérico restante
          int rain = valueStr3.toInt();  // Convertir el valor a un entero
          
          //Serial.print("Valor recibido: ");
          //Serial.println(rain);
      }

        Serial.print("Mensaje en miSerial2: ");
        Serial.println(mensaje);
    }

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
   
  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending Luz -> ");
  Serial.println(luz);
  lucesFeed->save(luz);
  delay(4000);
  
  Serial.print("sending Lluvia -> ");
  Serial.println(rain);
  lluviaFeed->save(rain);
  delay(4000);

  Serial.print("sending Proximidad -> ");
  Serial.println(objeto);
  obsFeed->save(objeto);
  delay(4000);

}

