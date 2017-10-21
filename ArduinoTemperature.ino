#include <Arduino.h>

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "var.h"
#include "libSI7021.h"
#include "libDS3231.h"
#include "libSD.h"
#include "libLCD.h"

#pragma region Global var
libDS3231 rtc;
libSD sda;
libLCD lcd;
libSI7021 si7021;
SoftwareSerial esp8266(RX_PIN, TX_PIN);

String datestr, timestr;
static unsigned long previousMillis = 0;
unsigned long currentMillis;
float sitemp, sihum; 
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
	esp8266.begin(SERIAL_BAUD);
	delay(10);

#ifdef DEBUG
	Serial.begin(SERIAL_BAUD);
	delay(10);
	while (!Serial) {
		; //Needed for native USB port only
	}
#endif
	SPI.begin();
	Wire.begin();

	sda.init();
	rtc.init();
	si7021.init();
	lcd.begin();
	lcd.displayText();

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
	// Check WiFi connexion
	if (esp8266.available()) {
		String wific = esp8266.readString();
		if (wific.equalsIgnoreCase("WKO") == 0)
			lcd.displayWiFiConnexion("!");
		if (wific.equalsIgnoreCase("WOK") == 0)
			lcd.displayWiFiConnexion("*");
	}

	// Acquisition
	if (currentMillis - previousMillis >= LOG_FREQUENCY) {
		String dataT = "T" + String(DEVICE_NUMBER) + ':' + String(sitemp);
		String dataH = "H" + String(DEVICE_NUMBER) + ':' + String(sihum);
#ifdef DEBUG
		Serial.println(dataT);
		Serial.println(dataH);
#endif

		esp8266.println(dataT);
		delay(2000);
		esp8266.println(dataH);

		previousMillis = currentMillis;
		RtcDateTime now = rtc.getDateTime();
		String fileName;
		fileName = now.Month() + now.Year() + ".csv";

		sda.WriteDataTemp(sitemp, sihum, rtc.getDateTimeStr(), fileName);
	}
	delay(ACQ_FREQUENCY);
}