#include <SPI.h>
#include <LoRa.h>

int counter = 0;
#define LEDPIN 13

void setup() {


  if (!LoRa.begin(915E6)) {
    //Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  
  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
