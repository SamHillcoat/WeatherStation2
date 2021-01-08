#include <LoRa.h>
#include <SPI.h>

/* 
Runs on weather station client arduino (Sparkfun Blackboard)
Requires Dragino LoRa Sheild connected as default.

Written by: Sam Hillcoat
Last Modified: 5/1/21
*/


int counter = 0;

char fakeData[60];

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");


  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  fakeDataGen();

  Serial.print("Sending packet: ");
  Serial.println(fakeData);

  // send LoRa packet
  LoRa.beginPacket();
  LoRa.print(fakeData);
  //LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}


void fakeDataGen() {
  fakeData[0] = '\0';
  // Generates fake weather data for testing
  long windspeed = random(0,40);
  long winddir = random(0,359);

  sprintf(fakeData, "{windspeed : %ld, winddir : %ld}", windspeed, winddir);
  delay(500);

}
