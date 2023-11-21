#include "flipper.h"
#include "app.h"
#include "scenes/scenes.h"
#include "logger/logger.h"

const char* TemperatureNames[Temp_Count] = {"F", "C", "K"};
const char* HumidityNames[Humid_Count] = {"Relative", "Absolute"};
const char* PressureNames[Pressure_Count] = {"mbar", "hPa", "PSI", "inHg", "mmHg", "Torr"};
const char* TimeNames[Time_Count] = {"UTC", "EST"};
const char* LogModeNames[Log_Mode_Count] = {"Auto", "Manual"};

App* app_alloc() { //allocate and initialize app.  add required views and scenes
    App* app = malloc(sizeof(App));


    app->scene_manager = scene_manager_alloc(&pod_scene_manager_handlers, app);
    app->view_dispatcher = view_dispatcher_alloc();

    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, pod_custom_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, pod_back_event_callback);

    app->submenu = submenu_alloc();
    app->variable_item_list = variable_item_list_alloc();
    app->widget = widget_alloc();

    view_dispatcher_add_view(
        app->view_dispatcher, Pod_Submenu_View, submenu_get_view(app->submenu)); //Main Menu Page
    view_dispatcher_add_view(
        app->view_dispatcher, Pod_Widget_View, widget_get_view(app->widget)); //GPIO Display Page
    view_dispatcher_add_view(
        app->view_dispatcher,
        Pod_Variable_Item_List_View,
        variable_item_list_get_view(app->variable_item_list)); //Settings Page

    app->storage = furi_record_open(RECORD_STORAGE);
    app->file_stream = logger_stream_alloc(app->storage);

    app->settings = app_settings_alloc();
    FURI_LOG_I("POD", "accessable at 7");
    app->bme_initialized = false;
    app->gps_initialized = false;
    app->weather_station_initialized = false;

    app->deltaState = POD_Neutral_Delta;

    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    return app;
}

AppSettings* app_settings_alloc() { //allocate space for appsettings
    AppSettings* settings = malloc(sizeof(AppSettings));
    return settings;
}

void app_init_settings(App* app) { //initialize appsettings
    app->settings->temperature = 1;
    app->settings->humidity = 0;
    app->settings->pressure = 0;
    app->settings->time = 0;
    app->settings->logMode = 0;
}

void app_quit(App* app) { //close app
    scene_manager_stop(app->scene_manager);
}

void app_free(App* app) { //free created spaces and close views and settings
    furi_assert(app);
    free(app->settings);
    //bme_free(app->bme280);

    furi_mutex_free(app->mutex);

    view_dispatcher_remove_view(app->view_dispatcher, Pod_Submenu_View);
    view_dispatcher_remove_view(app->view_dispatcher, Pod_Variable_Item_List_View);
    view_dispatcher_remove_view(app->view_dispatcher, Pod_Widget_View);

    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);

    submenu_free(app->submenu);
    variable_item_list_free(app->variable_item_list);
    widget_free(app->widget);

    logger_stream_free(app->file_stream);
    furi_record_close(RECORD_STORAGE);

    free(app);
}
