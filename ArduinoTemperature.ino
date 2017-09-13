#include <Arduino.h>

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "var.h"
#include "bme280SparkAccess.h"
#include "SI7021Access.h"
#include "DS3231Access.h"
#include "sdAccess.h"
#include "lcdAccess.h"

#pragma region Global var
BME280SparkAccess bme;
DS3231Access rtc;
sdAccess sda;
lcdAccess lcd;
SI7021Access si7021;

String datestr, timestr;
float temp, hum, pres, altitude, dewpoint, dhttemp, dhthum, sitemp, sihum;
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
	SPI.begin();
	Wire.begin();
	rtc.init();
	bme.init();
	si7021.init();
	lcd.begin();
	lcd.displayText();
	sda.init();

	BlinkLed(BLINK_INIT, BLINK_INIT_TIME);
}

//
//  Loop
//
void loop() {
	currentMillis = millis();

	bme.getData(&temp, &hum, &pres);
	sitemp = si7021.getTemperature();
	sihum = si7021.getHumidity();
	//bme.CalculatedData(&altitude, &dewpoint);

	BlinkLed(1, BLINK_TIME);
	lcd.displayData(temp, hum, dhttemp, dhthum);

	/* Réalise une prise de mesure toutes les DELAY_BETWEEN_MEASURES millisecondes */
	if (currentMillis - previousMillis >= LOG_FREQUENCY) {
		previousMillis = currentMillis;

		RtcDateTime now = rtc.getDateTime();
		Serial.println(rtc.getDateTimeStr());
		sda.WriteData(temp, hum, pres, altitude, dewpoint, dhttemp, dhthum, now);
	}
	delay(ACQ_FREQUENCY);
}