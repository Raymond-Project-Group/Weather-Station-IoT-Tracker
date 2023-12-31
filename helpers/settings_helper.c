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

    FURI_LOG_I(TAG, "Beginning settings setup");

    AppSettings* settings = malloc(sizeof(AppSettings));


    FuriString* filepath = furi_string_alloc();
    furi_string_printf(filepath, "%s/%s", APP_PATH_FOLDER, APP_FILENAME_SETTINGS);

    storage_common_mkdir(storage, APP_PATH_FOLDER);

    Stream* file_stream = file_stream_alloc(storage);
    
    if(!file_stream_open(
           file_stream, furi_string_get_cstr(filepath), FSAM_READ, FSOM_OPEN_EXISTING)) {

        // in event no settings file is created
        FURI_LOG_I(TAG, "No settings file exists, loading defaults");
        default_settings(settings);


    } else {

        FURI_LOG_I(TAG, "Loading settings from file");
        stream_read(file_stream, (uint8_t*) settings, sizeof(AppSettings));

        if (!validate_settings(settings))
            default_settings(settings);

    }

    file_stream_close(file_stream);
    stream_free(file_stream);
    furi_string_free(filepath);


    return settings;
}

void app_settings_close(AppSettings* settings, Storage* storage) {

    FuriString* filepath = furi_string_alloc();
    furi_string_printf(filepath, "%s/%s", APP_PATH_FOLDER, APP_FILENAME_SETTINGS);

    storage_common_mkdir(storage, APP_PATH_FOLDER);

    Stream* file_stream = file_stream_alloc(storage);

    if(!file_stream_open(
           file_stream, furi_string_get_cstr(filepath), FSAM_WRITE, FSOM_CREATE_ALWAYS)) {

        FURI_LOG_E(
            TAG,
            "An error occured while creating settings file: %d",
            file_stream_get_error(file_stream));

    } else {

        stream_write(file_stream, (uint8_t*) settings, sizeof(AppSettings));

    }

    file_stream_close(file_stream);
    stream_free(file_stream);
    furi_string_free(filepath);
    
    free(settings);
}

GpsUart* simple_gps_uart_enable(App* app) {
    return gps_uart_enable(GPSBaudRates[app->settings->gps_baudrate]);
}

float get_calibrated_temperature(App* app) {
    return app->bme280->data->temperature + app->settings->temp_offset;
}

float get_calibrated_humidity(App* app) {
    return app->bme280->data->humidity + app->settings->humidity_offset;
}

float get_calibrated_pressure(App* app) {
    return app->bme280->data->pressure + app->settings->pressure_offset;
}

bool validate_settings(AppSettings* settings) {

    if (settings->temperature >= Temp_Count)
        return false;

    if (settings->humidity >= Humid_Count)
        return false;

    if (settings->pressure >= Pressure_Count)
        return false;

    if (settings->time >= Time_Count)
        return false;

    if (settings->logMode >= Log_Mode_Count)
        return false;

    if (settings->gps_baudrate >= Baud_Rates_Count)
        return false;

    if (settings->coordinates >= Coordinate_Count)
        return false;

    if (settings->freq >= Frequency_Count)
        return false;

    // Check if any of the floats are NaN
    if (settings->temp_offset == settings->temp_offset || settings->humidity_offset == settings->humidity_offset || settings->pressure_offset == settings->pressure_offset)
        return false;

    if (settings->temp_offset > 10 || settings->temp_offset < -10)
        return false;

    if (settings->humidity_offset > 10 || settings->humidity_offset < -10)
        return false;

    if (settings->pressure_offset > 10 || settings->pressure_offset < -10)
        return false;

    return true;
}

void default_settings(AppSettings* settings) {

    	settings->temperature = 1;
    	settings->humidity = 0;
    	settings->pressure = 0;
    	settings->time = 0;
        settings->coordinates = 0;
    	settings->logMode = 0;
        settings->gps_baudrate = 0;
        settings->freq = 10;
        settings->temp_offset = 0.0f;
        settings->humidity_offset = 0.0f;
        settings->pressure_offset = 0.0f;

}
