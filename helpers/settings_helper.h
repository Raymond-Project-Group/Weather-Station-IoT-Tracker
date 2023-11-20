#ifndef SETTINGS_HELPER_H
#define SETTINGS_HELPER_H

#include "../app.h"

#include "../flipperzero-gps/gps_uart.h"

#define OFFSET_STEP 0.1f
#define TEMP_OFFSET_MIN -5.0f
#define TEMP_OFFSET_MAX 5.0f
#define PRESSURE_OFFSET_MIN -5.0f
#define PRESSURE_OFFSET_MAX 5.0f
#define HUMIDITY_OFFSET_MIN -5.0f
#define HUMIDITY_OFFSET_MAX 5.0f

void app_settings_from_byte_array(AppSettings* settings, const uint8_t* data);
void byte_array_from_app_settings(const AppSettings* settings, uint8_t* data);

AppSettings* app_settings_setup(Storage* storage);
void app_settings_close(AppSettings* settings, Storage* storage);

GpsUart* simple_gps_uart_enable(App* app);


#endif