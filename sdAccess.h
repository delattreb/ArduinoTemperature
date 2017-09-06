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

	void WriteData(float temp, float  hum, float  pres, float  altitude, float  dewpoint, float dhttemp, float dhthum, DateTime now) {
		if (!initMonth)
		{
			memMonth = now.month();
			initMonth = true;
		}

		if (now.month() != memMonth)
			memMonth = now.month();

		String strFile = String(now.year()) + String(now.month()) + ".csv";
#ifdef DEBUG
		Serial.print("SD Writing data: ");
		Serial.println(strFile);
#endif		

		if (!SD.begin(SD_PIN)) {}
		File myFile = SD.open(strFile, FILE_WRITE);
		if (myFile) {
			String datestr = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
			String timestr = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
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



