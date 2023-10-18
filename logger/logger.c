#include "logger.h"

#include "../pod.h"

#define APP_PATH_FOLDER "/ext/pod"
#define APP_FILENAME_DATA "data.csv"


Stream* logger_stream_alloc(Storage* storage)
{

    FuriString* filepath = furi_string_alloc();
    furi_string_printf(filepath, "%s/%s", APP_PATH_FOLDER, APP_FILENAME_DATA);

    storage_common_mkdir(storage, APP_PATH_FOLDER);

    Stream* file_stream = file_stream_alloc(storage);

    if (!file_stream_open(file_stream, furi_string_get_cstr(filepath), FSAM_WRITE, FSOM_OPEN_APPEND)) {
        FURI_LOG_E(TAG, "An error occured while opening log file: %d", file_stream_get_error(file_stream));

        logger_stream_free(file_stream);
        furi_string_free(filepath);

        return NULL;
    }

    furi_string_free(filepath);
    return file_stream;
}

void logger_stream_free(Stream* file_stream)
{
    file_stream_close(file_stream);
    stream_free(file_stream);
}

void logger_stream_append(Stream* file_stream, Bme280Data* bme_data, GpsStatus* gps_status)
{
    stream_write_format(file_stream, "%f,%f,%f,%f,%f,20%02d-%02d-%02d %02d:%02d:%02d\n",
        (double)bme_data->temperature,
        (double)bme_data->humidity,
        (double)bme_data->pressure,
        (double)gps_status->longitude,
        (double)gps_status->latitude,
        gps_status->date_year,
        gps_status->date_month,
        gps_status->date_day,
        gps_status->time_hours,
        gps_status->time_minutes,
        gps_status->time_seconds);
}

