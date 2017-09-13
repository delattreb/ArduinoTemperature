//
// Created by Bruno on 13/09/2017.
//

#include <SI7021.h>

class SI7021Access {

private:
	SI7021 sensor;
public:

	SI7021Access() {
	}

	~SI7021Access() {
	}

	void init() {
		sensor.begin();
	}

	float getTemperature() {
		return sensor.getCelsiusHundredths() / 100;
	}

	float getHumidity() {
		return sensor.getHumidityPercent();
	}
};



