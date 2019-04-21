//
// Read BME280 to get temperature and humidity and pressure
//
// Returns: String with temperature and humidity
//
#include "common.h"
#include "getBME280.h"
#include "DebugMacros.h"

float * getBME280() {
    static float sensors[3];
    bme.read(pres, temp, hum, tempUnit, presUnit);
    // Convert readings to string for Nextion display
    dtostrf(temp,5, 2, sensors[0]);
    dtostrf(hum,5, 2, sensors[1]);
    dtostrf(pres,6, 4, sensors[2]);
    return sensors;
}
