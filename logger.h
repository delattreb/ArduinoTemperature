//
// Created by Bruno on 30/06/2017.
//


#include <string.h>
#include <time.h>
#include "var.h"

class Logger {

private:

	int loglevel;

public:

	Logger(int level) {
		loglevel = level;
	}

	~Logger() {

	}

	void log(int level, String logMessage) {
		String message = "";
		if (level >= loglevel) {

			switch (level) {
			case DEBUG:
				message = "DEBUG - ";
				break;
			case INFO:
				message = "INFO - ";
				break;
			case WARNING:
				message = "WARNING - ";
				break;
			case ERROR:
				message = "ERROR - ";
				break;
			case CRITICAL:
				message = "CRITICAL - ";
				break;
			}
			if (level != NOLOG) {
				message += logMessage;
				Serial.println(message);
			}
		}
	}
};



