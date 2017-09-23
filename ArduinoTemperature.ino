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

	sda.init(SD_PIN, DATAFILE);
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
#ifdef DEBUG
		Serial.println("Receive data");
#endif 
		String wific = esp8266.readString();
		if (wific.equalsIgnoreCase("WKO") == 0)
			lcd.displayWiFiConnexion("!");
		if (wific.equalsIgnoreCase("WOK") == 0)
			lcd.displayWiFiConnexion("*");
	}

	// Acquisition
	if (currentMillis - previousMillis >= LOG_FREQUENCY) {
		String dataT = "T" + String(sitemp);
		String dataH = "H" + String(sihum);
#ifdef DEBUG
		Serial.println("Send to ESP8266");
		Serial.print("Temperature: ");
		Serial.println(dataT);
		Serial.print("Humidity: ");
		Serial.println(dataH);
#endif
		
		esp8266.println(dataT);
		delay(2000);
		esp8266.println(dataH);

		previousMillis = currentMillis;
		RtcDateTime now = rtc.getDateTime();
		sda.WriteDataTemp(sitemp, sihum, rtc.getDateTimeStr());
	}
	delay(ACQ_FREQUENCY);
}