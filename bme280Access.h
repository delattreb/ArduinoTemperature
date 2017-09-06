/*
Vin (Voltage In)    ->  3.3V
Gnd (Ground)        ->  Gnd
SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro
*/

#include <BME280I2C.h>
#include "var.h"


class BME280Access {

private:

	BME280I2C bme;
	bool metric = true;

public:

	BME280Access() {
	}

	~BME280Access() {
	}

	void init() {
		while (!bme.begin()) {
#ifdef DEBUG
			Serial.println("Could not find BME280 sensor");
#endif
			delay(1000);
		}
#ifdef DEBUG
		Serial.println("Find BME280 sensor");
#endif
	}

	void getData(float* ttemp, float* thum, float* tpres) {
		float temp, hum, pres;
		uint8_t pressureUnit(0x4);  // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi

		bme.read(pres, temp, hum, metric, pressureUnit);
		*ttemp = temp;
		*thum = hum;
		*tpres = pres;
#ifdef DEBUG
		Serial.print("T ");
		Serial.print(temp);
		Serial.print(" H: ");
		Serial.print(hum);
		Serial.print(" P: ");
		Serial.println(pres);
#endif
	}

	void CalculatedData(float* taltitude, float* tdewpoint) {
		float altitude = bme.alt(metric);
		float dewPoint = bme.dew(metric);
		*taltitude = altitude;
		*tdewpoint = dewPoint;
#ifdef DEBUG
		//Serial.print("A: ");
		//Serial.print(altitude);
		//Serial.print(" D: ");
		//Serial.println(dewPoint);
#endif
	}
};