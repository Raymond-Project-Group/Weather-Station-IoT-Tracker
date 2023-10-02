#ifndef UNIT_CONVERSION_H
#define UNIT_CONVERSION_H
#include "app.h"

float temperature_conversion(int start, int end, float temp);
float humidity_conversion(int startHumid, int startTemp, int end, float humid, float temp);
float pressure_conversion(int start, int end, float press);
#endif