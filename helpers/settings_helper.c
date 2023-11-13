#include "settings_helper.h"

#include "file_locations.h"
#include "../pod.h"


void app_settings_from_byte_array(AppSettings* settings, const uint8_t* data) {
    memcpy(settings, data, sizeof(AppSettings)); // There may be an attack here I'll need to patch in regards to manually modifying the settings file
}

void byte_array_from_app_settings(const AppSettings* settings, uint8_t* data) {
    memcpy(data, settings, sizeof(AppSettings));
}

AppSettings* app_settings_setup(Storage* storage) {

    AppSettings* settings = malloc(sizeof(AppSettings));


    FuriString* filepath = furi_string_alloc();
    furi_string_printf(filepath, "%s/%s", APP_PATH_FOLDER, APP_FILENAME_DATA);

    storage_common_mkdir(storage, APP_PATH_FOLDER);

    Stream* file_stream = file_stream_alloc(storage);

    if(!file_stream_open(
           file_stream, furi_string_get_cstr(filepath), FSAM_READ, FSOM_OPEN_EXISTING)) {

        // in event no settings file is created

    	settings->temperature = 1;
    	settings->humidity = 0;
    	settings->pressure = 0;
    	settings->time = 0;
    	settings->logMode = 0;
        settings->gps_baudrate = GPSBaudRates[0];
        settings->temp_offset = 0.0f;
        settings->humidity_offset = 0.0f;
        settings->pressure_offset = 0.0f;


    } else {

        uint8_t* file_data = malloc(sizeof(AppSettings));
        stream_read(file_stream, file_data, sizeof(AppSettings));
        app_settings_from_byte_array(settings, file_data);
        free(file_data);

    }

    file_stream_close(file_stream);
    stream_free(file_stream);
    furi_string_free(filepath);


    return settings;
}

void app_settings_close(AppSettings* settings, Storage* storage) {

    FuriString* filepath = furi_string_alloc();
    furi_string_printf(filepath, "%s/%s", APP_PATH_FOLDER, APP_FILENAME_DATA);

    storage_common_mkdir(storage, APP_PATH_FOLDER);

    Stream* file_stream = file_stream_alloc(storage);

    if(!file_stream_open(
           file_stream, furi_string_get_cstr(filepath), FSAM_WRITE, FSOM_CREATE_ALWAYS)) {

        FURI_LOG_E(
            TAG,
            "An error occured while creating settings file: %d",
            file_stream_get_error(file_stream));

    } else {

        uint8_t* file_data = malloc(sizeof(AppSettings));
        byte_array_from_app_settings(settings, file_data);
        stream_write(file_stream, file_data, sizeof(AppSettings));
        free(file_data);

    }

    file_stream_close(file_stream);
    stream_free(file_stream);
    furi_string_free(filepath);
    
    free(settings);
}