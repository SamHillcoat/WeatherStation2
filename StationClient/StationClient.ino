#include <LoRa.h>
#include <SPI.h>

#include <SparkFunHTU21D.h>
#include <SparkFunMPL3115A2.h>

#include <Wire.h> //I2C needed for sensors
/* 
Runs on weather station client arduino (Sparkfun Blackboard)
Requires Dragino LoRa Sheild connected as default.

Written by: Sam Hillcoat
Last Modified: 5/1/21
*/


MPL3115A2 myPressure; //Create an instance of the pressure sensor
HTU21D myHumidity; //Create an instance of the humidity sensor


//Global Variables 
long lastSecond; //The millis counter to see when a second rolls by
#define SENDING_INTERVAL 5000 // Amount of millis between sending data

//Hardware Pin Definitions
const byte STAT_BLUE = 7;
const byte STAT_GREEN = 8;

const byte REFERENCE_3V3 = A3;
const byte BATT = A2;
const byte WSPEED = 3;

//Windspeed Variables
//Windspeed is measured by the hardware interupt on D3 (wspeedIRQ)
// One click per second in 2.4km/h
long lastWindCheck = 0;
volatile long lastWindIRQ = 0;
volatile byte windClicks = 0;

//Global Variables - LoRa Shield

int counter = 0;
char fakeData[60];
char weatherData[60];

void setup() {
  Serial.begin(9600);
  while (!Serial);
  

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Sender Online");


  // Weather Sheild Setup
  pinMode(STAT_BLUE, OUTPUT); //Status LED Blue
  pinMode(STAT_GREEN, OUTPUT); //Status LED Green
  pinMode(WSPEED, INPUT_PULLUP);
  pinMode(REFERENCE_3V3, INPUT);


  //Configure the pressure sensor
  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags

  //Configure the humidity sensor
  myHumidity.begin();

  lastSecond = millis();

  

  attachInterrupt(digitalPinToInterrupt(3), wspeedIRQ, FALLING);
  Serial.println("Weather Shield online!");

	// turn on interrupts
	interrupts();
}

void loop() {

  //Print readings every  5 seconds (change in global variables at top)
  if (millis() - lastSecond >= SENDING_INTERVAL)
  {
    digitalWrite(STAT_BLUE, HIGH); //Blink stat LED

    lastSecond += SENDING_INTERVAL;

    //Check Humidity Sensor
    float humidity = myHumidity.readHumidity();

    if (humidity == 998) //Humidty sensor failed to respond
    {
      Serial.println("I2C communication to sensors is not working. Check solder connections.");

      //Try re-initializing the I2C comm and the sensors
      myPressure.begin(); 
      myPressure.setModeBarometer();
      myPressure.setOversampleRate(7);
      myPressure.enableEventFlags();
      myHumidity.begin();
    }
    else
    {
      Serial.print("Humidity = ");
      Serial.print(humidity);
      Serial.print("%,");
      float temp_h = myHumidity.readTemperature();
      Serial.print(" temp_h = ");
      Serial.print(temp_h, 2);
      Serial.print("C,");

      //Check Pressure Sensor
      float pressure = myPressure.readPressure();
      Serial.print(" Pressure = ");
      Serial.print(pressure);
      Serial.print("Pa,");

      //Check tempf from pressure sensor
      float tempf = myPressure.readTemp();
      Serial.print(" temp_p = ");
      Serial.print(tempf, 2);
      Serial.print("C,");

      /*
      //Check batt level
      float batt_lvl = get_battery_level();
      Serial.print(" VinPin = ");
      Serial.print(batt_lvl);
      Serial.print("V");
      */

      //Windspeed
      float windspeed = get_wind_speed();
      Serial.print(" Windspeed = ");
      Serial.print(windspeed);
      Serial.print("km/h");

      Serial.println();

      send_data_over_lora();
    }

    digitalWrite(STAT_BLUE, LOW); //Turn off stat LED
  }

  delay(100);

}


void fakeDataGen() {
  fakeData[0] = '\0';
  // Generates fake weather data for testing
  long windspeed = random(0,40);
  long winddir = random(0,359);

  sprintf(fakeData, "{\"windspeed\" : %ld, \"winddir\" : %ld}", windspeed, winddir);
  delay(3000);

}


void generate_weather_data() {
  weatherData[0] = '\0';
  // Generates fake weather data for testing
  long windspeed = get_wind_speed;
  long winddir = random(0,359);

  // TODO: Add other sensors to data array to be sent

  sprintf(weatherData, "{\"windspeed\" : %ld, \"winddir\" : %ld}", windspeed, winddir);
  //delay(100);

}

void wspeedIRQ() {
// Activated by the magnet in the anemometer (2 ticks per rotation), attached to input D3

	if (millis() - lastWindIRQ > 10) // Ignore switch-bounce glitches less than 10ms (142MPH max reading) after the reed switch closes
	{
		lastWindIRQ = millis(); //Grab the current time
		windClicks++; //There is 2.4km/h for each click per second.
	}
}

float get_wind_speed() {
	float deltaTime = millis() - lastWindCheck; //750ms

	deltaTime /= 1000.0; //Covert to seconds

	float windSpeed = (float)windClicks / deltaTime; //3 / 0.750s = 4

	windClicks = 0; //Reset and start watching for new wind
	lastWindCheck = millis();

	windSpeed *= 2.4;

	/* Serial.println();
	 Serial.print("Windspeed:");
	 Serial.println(windSpeed);*/

	return(windSpeed);
}

void send_data_over_lora() {
  Serial.print("Sending packet: ");
  Serial.println(weatherData);
  

  // send LoRa packet
  LoRa.beginPacket();
  LoRa.print(weatherData);
  //LoRa.print(counter);
  LoRa.endPacket();

  // Counter for error checking
  counter++;
  Serial.print("Count: ");
  Serial.print(counter);
  Serial.println();
}