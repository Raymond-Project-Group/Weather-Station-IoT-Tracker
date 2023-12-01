#ifndef APP_H
#define APP_H

#include "flipper.h"
#include "./bme280/bme280.h"
#include "./flipperzero-gps/gps_uart.h"
#include "./weather_stations/weather_station.h"
#include "./weather_stations/ws_transmit.h"

typedef enum {
    POD_Neutral_Delta,
    POD_Left_Side_Delta,
    POD_Right_Side_Delta,
} PodDeltaState;

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

typedef enum {
    Decimal,
    DMS,
    Coordinate_Count,
} CoordinateUnits;
extern const char* CoordinateNames[Coordinate_Count]; // = {"mbar","hPa","PSI","inHg","mmHg","Torr"};

typedef enum { UTC, EST, Time_Count } TimeStandards;
extern const char* TimeNames[Time_Count];

typedef enum { pws_sync, manual_only, Log_Mode_Count,} LogModes;
extern const char* LogModeNames[Log_Mode_Count];

typedef enum { br4800, br9600, br19200, br38400, br57600, br115200, Baud_Rates_Count } BaudRates;
extern const char* GPSBaudRateLabels[Baud_Rates_Count];
extern const int GPSBaudRates[6];

typedef enum { fr30000, fr303878, fr30425, fr31000, fr31500, fr31800, fr39000, fr41800, fr43307, fr43342, fr43392, fr43442, fr43477,fr43890,fr86835,fr91500,fr92500, hopping, Frequency_Count } Frequency;
extern const char* FrequencyLables[Frequency_Count];
extern const uint32_t Frequencies[Frequency_Count-1];

typedef struct {
    bool edit;
    uint8_t selection;
    int digit;
    int rawTemp;
    int rawHum;
    int rawPres;
} OffsetState;

typedef struct {
    bool bme_initialized;
    bool gps_initialized;
    bool pws_initialized;
} InitializationStates;

typedef struct {
    TemperatureUnits temperature;
    HumidityUnits humidity;
    PressureUnits pressure;
    CoordinateUnits coordinates;
    TimeStandards time;
    LogModes logMode;
    BaudRates gps_baudrate;
    Frequency freq;
    float temp_offset;
    float humidity_offset;
    float pressure_offset;
} __attribute__((packed)) AppSettings;

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
    InitializationStates* initialization_states;
    Storage* storage;
    Stream* file_stream; // output log stream
    Message* message; //Weather Station Transmission Module
    uint16_t sub_menu_size; //Used to note how large the sub menu is
    bool clear_mem;  //Should we clear PWS Mem
    uint8_t next_pws_scene; //PWS Selection is reused code, it can direct either to PWS Display or POD Display
    uint16_t canvas_y_offset; //Used to note how far off the y-axis we are(scroll up and down functionality)
    PodDeltaState deltaState;
    OffsetState* offsetState;
} App;

App* app_alloc();
void app_quit(App* app);
void app_free(App* app);

#endif