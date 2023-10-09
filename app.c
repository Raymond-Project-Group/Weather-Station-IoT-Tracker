#include "flipper.h"
#include "app.h"
#include "scenes.h"

const char* TemperatureNames[Temp_Count] = {"F","C","K"};
const char* HumidityNames[Humid_Count] = {"Relative","Absolute"};
const char* PressureNames[Pressure_Count] = {"mbar","hPa","PSI","inHg","mmHg","Torr"};
const char* TimeNames[Time_Count] = {"UTC","EST"};
App* app_alloc() {//allocate and initialize app.  add required views and scenes
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

    view_dispatcher_add_view(app->view_dispatcher, Pod_Submenu_View, submenu_get_view(app->submenu));//Main Menu Page
    view_dispatcher_add_view(app->view_dispatcher, Pod_Widget_View, widget_get_view(app->widget));//GPIO Display Page
    view_dispatcher_add_view(app->view_dispatcher, Pod_Variable_Item_List_View, variable_item_list_get_view(app->variable_item_list));//Settings Page


    
    app->settings = app_settings_alloc();
    app->gps_uart = gps_uart_enable();
    app->bme280 = bme_init();

    return app;
}

AppSettings* app_settings_alloc(){//allocate space for appsettings
    AppSettings* settings = malloc(sizeof(AppSettings));
    return settings;
}

void app_init_settings(App* app){//initialize appsettings
    app->settings->temperature = 1;
    app->settings->humidity = 0;
    app->settings->pressure = 0;
    app->settings->time = 0;
}

void app_quit(App* app) {//close app
    scene_manager_stop(app->scene_manager);
}

void app_free(App* app) {//free created spaces and close views and settings
    furi_assert(app);
    bme_free(app->bme280);
    gps_uart_disable(app->gps_uart);
    free(app->settings);
    //bme_free(app->bme280);

    view_dispatcher_remove_view(app->view_dispatcher, Pod_Submenu_View);
    view_dispatcher_remove_view(app->view_dispatcher, Pod_Variable_Item_List_View);
    view_dispatcher_remove_view(app->view_dispatcher, Pod_Widget_View);

    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);

    submenu_free(app->submenu);
    variable_item_list_free(app->variable_item_list);
    widget_free(app->widget);

    free(app);
}
