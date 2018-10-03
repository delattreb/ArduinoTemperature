#include <Arduino.h>

#include <SPI.h>
//#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "var.h"
#include "libSI7021.h"
//#include "libDS3231.h"
//#include "libSD.h"
#include "libLCD.h"

#pragma region Global var
//libDS3231 rtc;
//libSD sda;
libLCD lcd;
libSI7021 si7021;
SoftwareSerial esp8266(RX_PIN, TX_PIN);

String datestr, timestr;
static unsigned long previousMillis = 0;
unsigned long currentMillis;
float sitemp, sihum, offsetTemp[MAXDEVICE] = { -3.5,-2.9,-3.0,-0.0,-0.0 }, offsetHum[MAXDEVICE] = { -18.0,-17.0,-13.0,-0.0,-0.0 }; // Offet for devices
#pragma endregion

//
//* Setup
// param: 
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
	//	sda.init();
	//	rtc.init(false);
	si7021.init();
	lcd.begin();
	lcd.displayText();
	lcd.displayWiFiConnexion("!");
}

//
//  Loop
//
void loop() {
	currentMillis = millis();

	sitemp = si7021.getTemperature() + offsetTemp[DEVICE_NUMBER - 1];
	sihum = si7021.getHumidity() + offsetHum[DEVICE_NUMBER - 1];
	lcd.displayData(sitemp, sihum);

	// Check WiFi connexion
	if (esp8266.available()) {
		String wific = esp8266.readString();
		if (wific.startsWith("K", 0))
			lcd.displayWiFiConnexion("!");
		if (wific.startsWith("O", 0))
			lcd.displayWiFiConnexion("*");
		if (wific.startsWith("S", 0))
			lcd.displayWiFiDbSignal(wific.substring(1, 4));
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
		//RtcDateTime now = rtc.getDateTime();
		//String fileName;
		//fileName = now.Month() + now.Year() + ".csv";
		//sda.WriteDataTemp(sitemp, sihum, rtc.getDateTimeStr(), fileName);
	}
	delay(ACQ_FREQUENCY);
}