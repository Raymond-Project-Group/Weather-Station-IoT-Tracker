#ifndef LOGGER_H
#define LOGGER_H

#include <toolbox/stream/file_stream.h>

#include "../bme280/bme280.h"
#include "../flipperzero-gps/gps_uart.h"
#include "../weather_stations/protocols/ws_generic.h"
#include "../app.h"

Stream* logger_stream_alloc(Storage* storage);
void logger_stream_free(Stream* stream);

void logger_stream_append(App* app);
void logger_auto_append(void* context);

void append_bme_log(Stream* file_stream, Bme280Data* bme_data, bool override);

void append_gps_log(Stream* file_stream, GpsStatus* gps_status, bool override);

void append_ws_log(Stream* file_stream, WeatherStationContext* ws, bool override);


#endif
