#include "logger.h"

#include <math.h>
#include "../pod.h"

#define APP_PATH_FOLDER "/ext/pod"
#define APP_FILENAME_DATA "data.csv"

Stream* logger_stream_alloc(Storage* storage) {
    FuriString* filepath = furi_string_alloc();
    furi_string_printf(filepath, "%s/%s", APP_PATH_FOLDER, APP_FILENAME_DATA);

    storage_common_mkdir(storage, APP_PATH_FOLDER);

    Stream* file_stream = file_stream_alloc(storage);

    if(!file_stream_open(
           file_stream, furi_string_get_cstr(filepath), FSAM_WRITE, FSOM_OPEN_APPEND)) {
        FURI_LOG_E(
            TAG,
            "An error occured while opening log file: %d",
            file_stream_get_error(file_stream));

        logger_stream_free(file_stream);
        furi_string_free(filepath);

        return NULL;
    }

    furi_string_free(filepath);
    return file_stream;
}

void logger_stream_free(Stream* file_stream) {
    file_stream_close(file_stream);
    stream_free(file_stream);
}

void logger_stream_append(App* app) {
    furi_mutex_acquire(app->mutex, FuriWaitForever);
    append_bme_log(app->file_stream, app->bme280->data);
    stream_write_char(app->file_stream, ',');
    append_gps_log(app->file_stream, app->gps_uart->status);
    stream_write_char(app->file_stream, ',');
    append_ws_log(app->file_stream, app->pws, !app->weather_station_initialized);
    stream_write_char(app->file_stream, '\n');
    furi_mutex_release(app->mutex);
}

void append_bme_log(Stream* file_stream, Bme280Data* bme_data) {

    stream_write_format(
        file_stream,
        "%f,%f,%f",
        (double)bme_data->temperature,
        (double)bme_data->humidity,
        (double)bme_data->pressure);
}

void append_gps_log(Stream* file_stream, GpsStatus* gps_status) {

    stream_write_format(file_stream, "%f,%f,", (double)gps_status->latitude, (double)gps_status->longitude);

    if(gps_status->date_year >= 0) {
        stream_write_format(
            file_stream,
            "20%02d-%02d-%02d ",
            gps_status->date_year,
            gps_status->date_month,
            gps_status->date_day);
    } else {
        stream_write_cstring(file_stream, "xxxx-xx-xx ");
    }

    if(gps_status->time_hours >= 0) {
        stream_write_format(
            file_stream,
            "%02d:%02d:%02d,",
            gps_status->time_hours,
            gps_status->time_minutes,
            gps_status->time_seconds);
    } else {
        stream_write_cstring(file_stream, "xx:xx:xx,");
    }

    stream_write_format(file_stream, "%d", gps_status->satellites_tracked);
}

void append_ws_log(Stream* file_stream, WeatherStationContext* ws, bool override) {

    // prevents accessing junk data
    if(override) {
        stream_write_cstring(file_stream, ",,,,");
        return;
    }
    WSBlockGeneric* ws_data = ws->data->generic;
    stream_write_cstring(file_stream, "override not set, going to attempt write");

    stream_write_format(file_stream, "%f, %f, %ld, %s, %ld", (double) ws_data->temp, (double) ws_data->humidity, ws_data->timestamp, furi_string_get_cstr(ws->data->protocol_name), ws_data->id);

}
