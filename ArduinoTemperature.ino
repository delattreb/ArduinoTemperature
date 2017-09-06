#include <Arduino.h>

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "bme280SparkAccess.h"
#include "rtcAccess.h"
#include "sdAccess.h"
#include "lcdAccess.h"
//#include "dth22Access.h"
#include "var.h"


#pragma region Global var
BME280SparkAccess bme;
rtcAccess rtc;
sdAccess sda;
lcdAccess lcd;
//dht22Access dht;

String datestr, timestr;
float temp, hum, pres, altitude, dewpoint, dhttemp, dhthum;
DateTime now;
static unsigned long previousMillis = 0;
unsigned long currentMillis;
#pragma endregion 

//
// BlinkLed
//
void BlinkLed(int cpt, int time) {
	for (int i = 1; i <= cpt; i++)
	{
		digitalWrite(LED_PIN, HIGH);
		delay(time);
		digitalWrite(LED_PIN, LOW);
		delay(time);
	}
}

//
// Setup
//
void setup() {
#ifdef DEBUG
	Serial.begin(SERIAL_BAUD);
	while (!Serial) {
		; //Needed for native USB port only
	}
#endif
	for (int i = 0; i < NB_IO_OUTPUT; i++) // For LowPower
		pinMode(LED_PIN, OUTPUT);

	SPI.begin();
	Wire.begin();
	rtc.init();
	bme.init();
	lcd.begin();
	lcd.displayText();

#ifdef DEBUG
	Serial.print(APPNAME);
	Serial.print(" v");
	Serial.println(APPVERSION);
#endif

	BlinkLed(BLINK_INIT, BLINK_INIT_TIME);
}

//
//  Loop
//
void loop() {
	currentMillis = millis();

	bme.getData(&temp, &hum, &pres);
	//bme.CalculatedData(&altitude, &dewpoint);
	//dht.getData(&dhttemp, &dhthum);

	BlinkLed(1, BLINK_TIME);
	lcd.displayData(temp, hum, dhttemp, dhthum);

	/* Réalise une prise de mesure toutes les DELAY_BETWEEN_MEASURES millisecondes */
	if (currentMillis - previousMillis >= LOG_FREQUENCY) {
		previousMillis = currentMillis;

		now = rtc.getDateTime();
		sda.WriteData(temp, hum, pres, altitude, dewpoint, dhttemp, dhthum, now);
	}
	delay(ACQ_FREQUENCY);
}