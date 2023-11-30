#include "logger.h"

#include <math.h>
#include "../pod.h"
#include "../helpers/file_locations.h"
#include "../helpers/settings_helper.h"


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

void logger_stream_append(App* app, uint16_t idx, float rssi) {
    furi_mutex_acquire(app->mutex, FuriWaitForever);
    append_bme_log(app->file_stream, app, !app->initialization_states->bme_initialized);
    stream_write_char(app->file_stream, ',');
    append_gps_log(app->file_stream, app, !app->initialization_states->gps_initialized);
    stream_write_char(app->file_stream, ',');
    append_ws_log(app->file_stream, app, idx, rssi, !app->initialization_states->pws_initialized);
    stream_write_char(app->file_stream, '\n');
    furi_mutex_release(app->mutex);
}

void logger_auto_append(void* context, uint16_t idx, float rssi) {
    furi_assert(context);
    App* app = (App*) context;
    if (app->settings->logMode == manual_only)
        return;

    logger_stream_append(app, idx, rssi);
}

void append_bme_log(Stream* file_stream, App* app, bool override) {

    // prevents accessing junk data
    if(override) {
        stream_write_cstring(file_stream, ",,");
        return;
    }

    stream_write_format(
        file_stream,
        "%f,%f,%f",
        (double)get_calibrated_temperature(app),
        (double)get_calibrated_humidity(app),
        (double)get_calibrated_pressure(app));
}

void append_gps_log(Stream* file_stream, App* app, bool override) {

    GpsStatus* gps_status = app->gps_uart->status;

    // prevents accessing junk data
    if(override) {
        stream_write_cstring(file_stream, ",,xxxx-xx-xx xx:xx:xx,0");
        return;
    }

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

void append_ws_log(Stream* file_stream, App* app, uint16_t idx, float rssi, bool override) {


    // prevents accessing junk data
    if(override) {
        stream_write_cstring(file_stream, ",,,,,");
        return;
    }

    WSBlockGeneric* ws_data = malloc(sizeof(WSBlockGeneric));
    FuriString* protocol = furi_string_alloc();

    FlipperFormat* fff = ws_history_get_raw_data(app->pws->txrx->history,idx);//Gets Flipper Format and Raw Data

    flipper_format_rewind(fff);

    flipper_format_read_string(fff, "Protocol", protocol);//gets protocol name

    ws_block_generic_deserialize(ws_data,fff);

    stream_write_format(file_stream, "%f, %f, %ld, %s, %ld, %f", (double) ws_data->temp, (double) ws_data->humidity, ws_data->timestamp, furi_string_get_cstr(protocol), ws_data->id, (double) rssi);
    
    furi_string_free(protocol);
    free(ws_data);

    FURI_LOG_I(TAG, "ws log complete");
}
