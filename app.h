#ifndef APP_H
#define APP_H

#include "flipper.h"
#include "./bme280/bme280.h"
#include "./flipperzero-gps/gps_uart.h"
#include "./weather_stations/weather_station.h"

typedef enum {
    POD_Neutral_Delta,
    POD_Left_Side_Delta,
    POD_Right_Side_Delta,
} PodDeltaState;
#include "./weather_stations/ws_transmit.h"
typedef enum {
    F,
    C,
    K,
    Temp_Count,
} TemperatureUnits;
extern const char* TemperatureNames[Temp_Count]; // = {"F","C","K"};
typedef enum {
    relative,
    absolute,
    Humid_Count,
} HumidityUnits;
extern const char* HumidityNames[Humid_Count]; // = {"Relative","Absolute"};
typedef enum {
    mbar,
    hPa,
    PSI,
    inHg,
    mmHg,
    Torr,
    Pressure_Count,
} PressureUnits;
extern const char* PressureNames[Pressure_Count]; // = {"mbar","hPa","PSI","inHg","mmHg","Torr"};
typedef enum { UTC, EST, Time_Count } TimeStandards;
extern const char* TimeNames[Time_Count];

typedef enum { pws_sync, manual_only, Log_Mode_Count,} LogModes;
extern const char* LogModeNames[Log_Mode_Count];

typedef struct {
    TemperatureUnits temperature;
    HumidityUnits humidity;
    PressureUnits pressure;
    TimeStandards time;
    LogModes logMode;
} AppSettings;

typedef struct { //To view examples of modules: https://brodan.biz/blog/a-visual-guide-to-flipper-zero-gui-components/
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    TextBox* text_box;
    VariableItemList* variable_item_list;
    Widget* widget;
    AppSettings* settings;
    FuriMessageQueue* queue; //Message queue (items to process).
    FuriMutex* mutex; //Used to provide thread safe access to data.
    FuriTimer* timer; //Used for tick callbacks
    Bme280Context* bme280; //BME Module
    GpsUart* gps_uart; //GPS Module
    WeatherStationContext* pws; //Weather Station Module
    Message* message; //Weather Station Transmission Module
    bool weather_station_initialized; // ensure Weather Station Context points to actual station
    bool bme_initialized; // ensure gps Context points to actual station
    bool gps_initialized; // ensure gps Context points to actual station
    Storage* storage;
    Stream* file_stream; // output log stream
    uint16_t canvas_y_offset; //Used to note how far off the y-axis we are(scroll up and down functionality)
    PodDeltaState deltaState;
} App;

App* app_alloc();
AppSettings* app_settings_alloc();
void app_quit(App* app);
void app_free(App* app);
void app_init_settings(App* app);

#endif