#include "flipper.h"
#include "app.h"
#include "scenes/scenes.h"
#include "logger/logger.h"
#include "helpers/settings_helper.h"

const char* TemperatureNames[Temp_Count] = {"F", "C", "K"};
const char* HumidityNames[Humid_Count] = {"Relative", "Absolute"};
const char* PressureNames[Pressure_Count] = {"mbar", "hPa", "PSI", "inHg", "mmHg", "Torr"};
const char* CoordinateNames[Coordinate_Count] = {"Degrees", "DMS"};
const char* TimeNames[Time_Count] = {"UTC", "EST"};
const char* LogModeNames[Log_Mode_Count] = {"Auto", "Manual"};
const char* GPSBaudRateLabels[Baud_Rates_Count] = { "4800", "9600", "19200", "38400", "57600", "115200"};
const int GPSBaudRates[6] = { 4800, 9600, 19200, 38400, 57600, 115200 };
const char* FrequencyLables[Frequency_Count] = { "300.00", "303.78", "304.25", "310.00", "315.00", "318.00", "390.00", "418.00", "433.07", "433.42", "433.92", "434.42", "434.77","438.90","868.35","915.00","925.00", "Hop"};
const uint32_t Frequencies[Frequency_Count-1] = { 300000000, 303780000, 304250000, 310000000, 315000000, 318000000, 390000000, 418000000, 433070000, 433420000, 433920000, 434420000, 434770000,438900000,868350000,915000000,925000000 };

App* app_alloc() { //allocate and initialize app.  add required views and scenes
    App* app = malloc(sizeof(App));

    FURI_LOG_I("POD", "app alloc begin");

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

    app->settings = app_settings_setup(app->storage);

    app->offsetState = malloc(sizeof(OffsetState));
    app->offsetState->edit = false;
    app->offsetState->selection = 0;
    app->offsetState->digit = 0;

    app->initialization_states = (InitializationStates*) malloc(sizeof(InitializationStates));
    app->initialization_states->pws_initialized = false;
    app->initialization_states->bme_initialized = false;
    app->initialization_states->gps_initialized = false;

    app->deltaState = POD_Neutral_Delta;

    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    return app;
}


void app_quit(App* app) { //close app
    scene_manager_stop(app->scene_manager);
}

void app_free(App* app) { //free created spaces and close views and settings
    furi_assert(app);
    app_settings_close(app->settings, app->storage);
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

    free(app->offsetState);

    logger_stream_free(app->file_stream);
    furi_record_close(RECORD_STORAGE);

    free(app);
}
