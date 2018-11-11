#include <Arduino.h>
#include <SoftwareSerial.h>
#include "var.h"
#include "libSI7021.h"
#include "libLCD.h"

#pragma region Global var
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

	sitemp = si7021.getTemperature() + offsetTemp[DEVICE_NUMBER - 1];
	sihum = si7021.getHumidity() + offsetHum[DEVICE_NUMBER - 1];
	lcd.displayData(sitemp, sihum);

	// Check WiFi connexion
	if (esp8266.available()) {
		String wific = esp8266.readString();
		if (wific.startsWith("WK", 0))
			lcd.displayWiFiKo();
		if (wific.startsWith("WO", 0))
			lcd.displayWiFiOk();
		if (wific.startsWith("MK", 0))
			lcd.displayMQTTKo();
		if (wific.startsWith("MO", 0))
			lcd.displayMQTTOk();
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
		delay(SERIAL_DELAY);
		esp8266.println(dataH);
		previousMillis = currentMillis;
	}
	delay(ACQ_FREQUENCY);
}