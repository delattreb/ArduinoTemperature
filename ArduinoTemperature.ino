#include <Arduino.h>
#include <SoftwareSerial.h>
#include "var.h"
#include "libSI7021.h"
#include "libLCD.h"

#pragma region Global var
libLCD lcd;
libSI7021 si7021;
SoftwareSerial esp8266(RX_PIN, TX_PIN);

String datestr, timestr, wific, cmd;
static unsigned long previousMillisAcq = 0, previousMillisLog = 0;
unsigned long currentMillis;
float sitemp, sihum, offsetTemp[MAXDEVICE] = { -3.5,-2.9,-3.0,-0.0,-0.0 }, offsetHum[MAXDEVICE] = { -18.0,-17.0,-13.0,-0.0,-0.0 }; // Offet for devices
String inputString = "";
bool stringComplete = false;
#pragma endregion

//
// serialEvent
//
void serialEvent() {
	while (esp8266.available()) {
		char inChar = (char)esp8266.read();
		inputString += inChar;
		if (inChar == '\n') {
			stringComplete = true;
		}
	}
}

//
// Setup
//
void setup() {
	esp8266.begin(SERIAL_BAUD);
	while (!esp8266) continue;
	delay(10);

#ifdef DEBUG
	Serial.begin(SERIAL_BAUD);
	delay(10);
	while (!Serial) continue;
#endif
	si7021.init();
	lcd.begin();
	lcd.displayText();
	lcd.displayWiFiKo();
	lcd.displayMQTTKo();
}

//
//  Loop
//
void loop() {
	currentMillis = millis();
	if (currentMillis - previousMillisAcq >= ACQ_FREQUENCY) {
		sitemp = si7021.getTemperature() + offsetTemp[DEVICE_NUMBER - 1];
		sihum = si7021.getHumidity() + offsetHum[DEVICE_NUMBER - 1];
		lcd.displayData(sitemp, sihum);
		previousMillisAcq = currentMillis;
	}
	if (stringComplete) {
		wific = inputString;
		inputString = "";
		stringComplete = false;

		cmd = "";
		for (int i = 0; i < wific.length(); i++) {
			cmd += (char)wific[i];
			if ((char)wific[i] == '\n') {
				if (cmd.startsWith("WK"))
					lcd.displayWiFiKo();
				if (cmd.startsWith("WO"))
					lcd.displayWiFiOk();
				if (cmd.startsWith("MK"))
					lcd.displayMQTTKo();
				if (cmd.startsWith("MO"))
					lcd.displayMQTTOk();
				if (cmd.startsWith("S"))
					lcd.displayWiFiDbSignal(wific.substring(1, 4));
				cmd = "";
			}
		}
	}

	// Acquisition
	if (currentMillis - previousMillisLog >= LOG_FREQUENCY) {
		String dataT = "T" + String(DEVICE_NUMBER) + ':' + String(sitemp);
		String dataH = "H" + String(DEVICE_NUMBER) + ':' + String(sihum);
#ifdef DEBUG
		Serial.println(dataT);
		Serial.println(dataH);
#endif
		esp8266.println(dataT);
		delay(SERIAL_DELAY);
		esp8266.println(dataH);
		previousMillisLog = currentMillis;
	}
	serialEvent();
}