//
// Created by Bruno on 30/06/2017.
//

#include "RTClib.h"

class rtcAccess {

private:
	DS1307 rtc;

public:

	bool timeset = false;

	rtcAccess() {
	}

	~rtcAccess() {
	}

	void init() {
		rtc.begin();
		if (!rtc.isrunning()) {
#ifdef DEBUG
			Serial.println("RTC is NOT running");
#endif 
			rtc.adjust(DateTime(__DATE__, __TIME__));
#ifdef DEBUG
			Serial.println("Date and time adjusted");
			Serial.println(getDateStr());
			Serial.println(getTimeStr());
#endif 
		}
		else {
#ifdef DEBUG
			Serial.println("RTC is running");
			Serial.println(getDateStr());
			Serial.println(getTimeStr());
#endif 
		}
	}

	DateTime getDateTime() {
		DateTime now = rtc.now();
		return now;
	}

	String getDateTimeStr()
	{
		DateTime now = rtc.now();
		return String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
	}

	String getDateTimeStrEn()
	{
		DateTime now = rtc.now();
		return String(String(now.year()) + "-" + String(now.month()) + "-" + now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
	}

	String getDateStr()
	{
		DateTime now = rtc.now();
		return String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
	}

	String getTimeStr() {
		DateTime now = rtc.now();
		return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
	}
};



