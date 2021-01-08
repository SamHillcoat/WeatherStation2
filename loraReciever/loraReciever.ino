#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <SPI.h>
#include <LoRa.h>

#define SS 15
#define RST 0
#define DIO0 5

const char* ssid = "TelstraE243B9";
const char* password = "ur8aqaxszc";
const char* host = "http://httpbin.org/post";

WiFiClient client;
HTTPClient http;

char url[] = "";

bool newDataFlag = false;
char weatherData[30];

void setup() {
  Serial.begin(115200);
  delay(10);
  while (!Serial);
  Serial.setDebugOutput(true);


  Serial.println("Connecting to ");
  Serial.println(ssid);

  // Setting Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }

  Serial.println("WiFi Connected");
  yield();
  
  Serial.println("Connecting to LoRa Radio");
/*
  LoRa.setPins(SS, RST, DIO0);
  LoRa.onReceive(onReceive);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  //LoRa.receive();
  Serial.println("Ready to receive over LoRa");
  */
}

void loop() {
  //delay(1000);
  // LoRa Stuff
  // try to parse packet

  

  char testData[] = "HelloWorld";
  

  if (newDataFlag) {
    Serial.println("Packet Received: ");
    Serial.print(weatherData);
    //PostRequest(weatherData);
    weatherData[0] = '\0';
    newDataFlag = false;
  }
  
  PostRequest();
  Serial.println("Got back to main loop");
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  yield();
}

void PostRequest() {
  // Wifi POST Request


  const int httpPort = 80;

  char address[] = "";
  strcat(address, host);
  strcat(address, url);
  
  

  http.begin(client, address);
  yield();

  Serial.println(address);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  auto httpCode = http.POST("HelloWorld");
  //auto httpCode = http.GET();
  Serial.println("HTTP POST sent");
  Serial.println(httpCode);
  yield();
  delay(5);

  //String payload = http.getString();
  http.end();
  //yield();
  Serial.println("Http connection closed");
  //Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  //Serial.println("Connection Failed [HTTP]");
    //return;
}


  
  


  
/*

void onReceive(int packetSize) {
  // packet received
  newDataFlag = true;

  for (int i = 0; i < packetSize; i++) {
    //Serial.println("Recieved packe: (ISR)");
    //Serial.print((char)LoRa.read());

    weatherData[i] = (char)LoRa.read();
    
  }
}

/*
int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    //char packetData = (char)LoRa.read();
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  } */