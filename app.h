#ifndef APP_H
#define APP_H

#include "flipper.h"


typedef enum {
    F,
    C,
    K,
    Temp_Count,
} TemperatureUnits;
extern char* TemperatureNames[Temp_Count];// = {"F","C","K"};
typedef enum {
    relative,
    absolute,
    Humid_Count,
} HumidityUnits;
extern char* HumidityNames[Humid_Count];// = {"Relative","Absolute"};
typedef enum {
    mBar,
    hPa,
    psi,
    inHg,
    mmHg,
    torr,
    Pressure_Count,
} PressureUnits;
extern char* PressureNames[Pressure_Count];// = {"mBar","hPa","psi","inHg","mmHg","torr"};


typedef struct{
    TemperatureUnits temperature;
    HumidityUnits humidity;
    PressureUnits pressure;
} AppSettings;

typedef struct {//To view examples of modules: https://brodan.biz/blog/a-visual-guide-to-flipper-zero-gui-components/ 
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    TextBox* text_box;
    VariableItemList* variable_item_list;
    AppSettings* settings;
} App;

App* app_alloc();
AppSettings* app_settings_alloc();
void app_quit(App* app);
void app_free(App* app);
void app_init_settings(App* app);

#endif