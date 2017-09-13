//
// Created by Bruno on 30/06/2017.
//

#include "var.h"

class sdAccess {

private:

	bool initMonth = false;
	int memMonth;

public:

	sdAccess() {
		initMonth = false;
		memMonth = 0;
	}

	~sdAccess() {
	}
	
	void init()
	{
		if (!SD.begin(SD_PIN)) {
#ifdef INFO
			Serial.println("SD KO!");
#endif 
		}
		else {
#ifdef INFO
			Serial.println("SD OK");
#endif 
		}
	}
	void WriteData(float temp, float  hum, float  pres, float  altitude, float  dewpoint, float dhttemp, float dhthum, RtcDateTime now) {
		if (!initMonth)
		{
			memMonth = now.Month();
			initMonth = true;
		}

		if (now.Month() != memMonth)
			memMonth = now.Month();

		String strFile = String(now.Year()) + String(now.Month()) + ".csv";
#ifdef DEBUG
		Serial.print("SD Writing data: ");
		Serial.println(strFile);
#endif		

		if (!SD.begin(SD_PIN)) {}
		File myFile = SD.open(strFile, FILE_WRITE);
		if (myFile) {
			String datestr = String(now.Day()) + "/" + String(now.Month()) + "/" + String(now.Year());
			String timestr = String(now.Hour()) + ":" + String(now.Minute()) + ":" + String(now.Second());
			myFile.print(datestr + " ");
			myFile.print(timestr);
			myFile.print(";");
			myFile.print(String(temp));
			myFile.print(";");
			myFile.print(String(hum));
			myFile.print(";");
			myFile.print(String(dhttemp));
			myFile.print(";");
			myFile.println(String(dhthum));

			myFile.close();
		}
		else {
#ifdef DEBUG
			Serial.println("Write NOK");
#endif
		}
	}
};



