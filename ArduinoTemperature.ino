#include <Arduino.h>
#include <SoftwareSerial.h>
#include "var.h"
#include "libSI7021.h"
#include "libLCD.h"

#pragma region Global var
libLCD lcd;
libSI7021 si7021;
SoftwareSerial esp8266(RX_PIN, TX_PIN);

String datestr, timestr, cmd;
static unsigned long previousMillisAcq = 0, previousMillisLog = 0;
unsigned long currentMillis;
float sitemp, sihum, offsetTemp[MAXDEVICE] = { -3.5,-2.9,-3.0,-0.0,-0.0 }, offsetHum[MAXDEVICE] = { -18.0,-17.0,-13.0,-27.0,-0.0 }; // Offet for devices
String inputString = "";
bool stringComplete = false;
#pragma endregion

//
// serialEvent
//
void serialEvent() {
	if (!stringComplete) {
		if (esp8266.available()) {
			inputString = esp8266.readStringUntil(char(10));
#ifdef DEBUG
			Serial.print("Read: ");
			Serial.println(inputString);
#endif 
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
	esp8266.flush();
#ifdef DEBUG
	Serial.begin(SERIAL_BAUD);
	while (!Serial) continue;
	Serial.flush();
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
	serialEvent();
	if (currentMillis - previousMillisAcq >= ACQ_FREQUENCY) {
		sitemp = si7021.getTemperature() + offsetTemp[DEVICE_NUMBER - 1];
		sihum = si7021.getHumidity() + offsetHum[DEVICE_NUMBER - 1];
		lcd.displayData(sitemp, sihum);
		previousMillisAcq = currentMillis;
	}
	if (stringComplete) {
		cmd = inputString;
#ifdef DEBUG
		Serial.print("Cmd: ");
		Serial.println(cmd);
#endif
		if (cmd.startsWith("WK") == 1)
			lcd.displayWiFiKo();
		if (cmd.startsWith("WO") == 1)
			lcd.displayWiFiOk();
		if (cmd.startsWith("MK") == 1)
			lcd.displayMQTTKo();
		if (cmd.startsWith("MO") == 1)
			lcd.displayMQTTOk();
		if (cmd.startsWith("S") == 1)
			lcd.displayWiFiDbSignal(cmd.substring(1, cmd.length() - 1));
		inputString = "";
		stringComplete = false;
	}

	// Acquisition
	if (currentMillis - previousMillisLog >= LOG_FREQUENCY) {
		String dataT = "T" + String(DEVICE_NUMBER) + ':' + String(sitemp);
		String dataH = "H" + String(DEVICE_NUMBER) + ':' + String(sihum);
#ifdef DEBUG
		Serial.println("Acquisition");
		Serial.println(dataT);
		Serial.println(dataH);
#endif
		esp8266.println(dataT);
		delay(SERIAL_DELAY);
		esp8266.println(dataH);
		previousMillisLog = currentMillis;
	}
}