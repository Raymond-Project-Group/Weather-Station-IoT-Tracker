#ifndef SETTINGS_HELPER_H
#define SETTINGS_HELPER_H

#include "../app.h"

#include "../flipperzero-gps/gps_uart.h"

#define OFFSET_MAX 999
#define OFFSET_MIN -999

void app_settings_from_byte_array(AppSettings* settings, const uint8_t* data);
void byte_array_from_app_settings(const AppSettings* settings, uint8_t* data);

AppSettings* app_settings_setup(Storage* storage);
void app_settings_close(AppSettings* settings, Storage* storage);

GpsUart* simple_gps_uart_enable(App* app);


float get_calibrated_temperature(App* app);
float get_calibrated_humidity(App* app);
float get_calibrated_pressure(App* app);

#endif