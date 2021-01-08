/*
Written for Wemos D1 R2 ESP8266 Board
Draguino LoRa Shield


Pin Mapping:

WeMos D1 R2 ---> LoRa Shield
D1 --> 2
D2 --> 6
D3 --> 9
D5 --> 13
D6 --> 12
D7 --> 11
D8 --> 10
GND --> GND
3.3V --> 3.3V

This pin mapping is set in the code with LoRa.setpins()
*/
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <SPI.h>
#include <LoRa.h>


// Pin mapping for LoRa Shield
#define SS 15 // Slave Select
#define RST 0 //Reset
#define DIO0 5 // DIO0 (interupt pin)

// WiFi and Http data
const char* ssid = "TelstraE243B9";
const char* password = "ur8aqaxszc";
#define HOST "deviotweather.com/senddata" // The website to send request


const char * headerKeys[] = {"Location", "server"} ;
const size_t numberOfHeaders = 2;

bool newDataFlag = false;
char weatherData[30];

// Set up objects for wifi and http clients
WiFiClientSecure client;
HTTPClient http;


void setup() {

    Serial.begin(115200);

    Serial.println();

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());

    // LoRa Setup
    Serial.println("Connecting to LoRa...");

    LoRa.setPins(SS, RST, DIO0);
    LoRa.onReceive(onReceive);

    if (!LoRa.begin(915E6)) {
        delay(500);
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    LoRa.receive(); // Set LoRa to constantly receives
    Serial.println("Ready to receive over LoRa");


}

void loop() {
   // postRequest();
    delay(5000);

    if (newDataFlag) {
        Serial.println("Packet Received: ");
        Serial.print(weatherData);
        postRequest(weatherData);
        weatherData[0] = '\0';
        newDataFlag = false;
    }
}


void postRequest(const char *dataString) {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    
    client.setInsecure();
    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    //client.verify("38:ED:73:74:0C:52:E6:63:1E:65:5E:F3:C8:BA:7F:1C:80:FA:39:5C", "https://deviotweather.com");
    http.begin(client, "https://deviotweather.com/senddata"); //HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    int httpCode = http.POST(dataString);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == 301) {
        const String& payload = http.getString();

        http.collectHeaders(headerKeys, numberOfHeaders);
        Serial.println("received payload:\n<<");
        Serial.println(http.header("server"));
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    //yield();
  }
}

void onReceive(int packetSize) {
  // packet received
  newDataFlag = true;

  for (int i = 0; i < packetSize; i++) {
    //Serial.println("Recieved packe: (ISR)");
    //Serial.print((char)LoRa.read());

    weatherData[i] = (char)LoRa.read();
  }
}