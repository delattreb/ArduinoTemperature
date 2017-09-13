#include <Arduino.h>

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "var.h"
#include "SI7021Access.h"
#include "DS3231Access.h"
#include "sdAccess.h"
#include "lcdAccess.h"

#pragma region Global var
DS3231Access rtc;
sdAccess sda;
lcdAccess lcd;
SI7021Access si7021;

String datestr, timestr;
float sitemp, sihum;
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

	sitemp = si7021.getTemperature();
	sihum = si7021.getHumidity();
	lcd.displayData(sitemp, sihum);
	BlinkLed(1, BLINK_TIME);
	
	/* Réalise une prise de mesure toutes les DELAY_BETWEEN_MEASURES millisecondes */
	if (currentMillis - previousMillis >= LOG_FREQUENCY) {
		previousMillis = currentMillis;

		RtcDateTime now = rtc.getDateTime();
		Serial.println(rtc.getDateTimeStr());
		sda.WriteData(sitemp, sihum, now);
	}
	delay(ACQ_FREQUENCY);
}