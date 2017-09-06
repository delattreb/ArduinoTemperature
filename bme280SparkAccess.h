#include <avr/wdt.h>
#include "BlueDot_BME280.h"


class BME280SparkAccess {

private:
	BlueDot_BME280 bme;                                     //Object for Sensor 1
	int bmeDetected = 0;                                    //Checks if Sensor 1 is available

public:

	BME280SparkAccess() {
	}

	~BME280SparkAccess() {
	}

	void init() {

		bme.parameter.communication = 0;                    //I2C communication for Sensor 1 (bme1)
		bme.parameter.I2CAddress = 0x76;                    //I2C Address for Sensor 1 (bme1)

															 //0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
															 //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
															 //0b11:     In normal mode the sensor measures continually (default value)
		bme.parameter.sensorMode = 0b11;                    //Setup Sensor mode for Sensor 1

															 //0b000:      factor 0 (filter off)
															 //0b001:      factor 2
															 //0b010:      factor 4
															 //0b011:      factor 8
															 //0b100:      factor 16 (default value)
		bme.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 1

															//0b000:      factor 0 (Disable humidity measurement)
															//0b001:      factor 1
															//0b010:      factor 2
															//0b011:      factor 4
															//0b100:      factor 8
															//0b101:      factor 16 (default value)
		bme.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 1

															 //0b000:      factor 0 (Disable temperature measurement)
															 //0b001:      factor 1
															 //0b010:      factor 2
															 //0b011:      factor 4
															 //0b100:      factor 8
															 //0b101:      factor 16 (default value)
		bme.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 1

															  //0b000:      factor 0 (Disable pressure measurement)
															  //0b001:      factor 1
															  //0b010:      factor 2
															  //0b011:      factor 4
															  //0b100:      factor 8
															  //0b101:      factor 16 (default value)  
		bme.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 1

															  //For precise altitude measurements please put in the current pressure corrected for the sea level
															  //On doubt, just leave the standard pressure as default (1013.25 hPa);
		bme.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 1)

															  //Also put in the current average temperature outside (yes, really outside!)
															  //For slightly less precise altitude measurements, just leave the standard temperature as default (15°C and 59°F);
		bme.parameter.tempOutsideCelsius = 15;               //default value of 15°C
		bme.parameter.tempOutsideFahrenheit = 59;            //default value of 59°F


															  //The Watchdog Timer forces the Arduino to restart whenever the program hangs for longer than 8 seconds.
															  //This is useful when the program enters an infinite loop and stops giving any feedback on the serial monitor.
															  //However the Watchdog Timer may also be triggered whenever a single program loop takes longer than 8 seconds.
															  //Per default the Watchdog Timer is turned off (commented out).
															  //Do you need to run the Arduino for a long time without supervision and your program loop takes less than 8 seconds? Then remove the comments below!

															  //wdt_enable(WDTO_8S);                                 //Watchdog Timer counts for 8 seconds before starting the reset sequence
		if (bme.init() == 0x60)
		{
#ifdef  INFO
			Serial.println(F("BME280 OK"));
#endif 
			bmeDetected = 1;
		}
		else
		{
#ifdef  INFO
			Serial.println(F("BME280 not detected"));
#endif
			bmeDetected = 0;
		}
	}


	void getData(float  *ttemp, float *thum, float *tpres) {

		wdt_reset();  //This function resets the counter of the Watchdog Timer. Always use this function if the Watchdog Timer is enabled.

		if (bmeDetected)
		{
			*ttemp = bme.readTempC();
			*thum = bme.readHumidity();
			*tpres = bme.readPressure();
			//*talt = bme.readAltitudeMeter();

#ifdef  INFO
			Serial.print(F("Temp "));
			Serial.print(bme.readTempC());
			Serial.print(F(" Hum "));
			Serial.print(bme.readHumidity());
			Serial.print(F(" Pres "));
			Serial.print(bme.readPressure());
			Serial.print(F(" Alt "));
			Serial.println(bme.readAltitudeMeter());
#endif
		}

	}
};