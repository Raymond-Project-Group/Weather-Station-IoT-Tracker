#include "flipper.h"
#include "app.h"
#include "scenes.h"

char* TemperatureNames[Temp_Count] = {"F","C","K"};
char* HumidityNames[Humid_Count] = {"Relative","Absolute"};
char* PressureNames[Pressure_Count] = {"mBar","hPa","psi","inHg","mmHg","torr"};
App* app_alloc() {
    App* app = malloc(sizeof(App));
    app->scene_manager = scene_manager_alloc(&pod_scene_manager_handlers, app);
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, pod_custom_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, pod_back_event_callback);

    app->submenu = submenu_alloc();
    app->text_box = text_box_alloc();
    app->variable_item_list = variable_item_list_alloc();

    view_dispatcher_add_view(app->view_dispatcher, Pod_Submenu_View, submenu_get_view(app->submenu));//Main Menu Page
    view_dispatcher_add_view(app->view_dispatcher, Pod_Text_Box_View, text_box_get_view(app->text_box));//GPIO Display Page
    view_dispatcher_add_view(app->view_dispatcher, Pod_Variable_Item_List_View, variable_item_list_get_view(app->variable_item_list));//Settings Page

    return app;
}

AppSettings* app_settings_alloc(){
    AppSettings* settings = malloc(sizeof(AppSettings));
    return settings;
}

void app_init_settings(App* app){
    app->settings->temperature = 0;
    app->settings->humidity = 0;
    app->settings->pressure = 0;
}

void app_quit(App* app) {
    scene_manager_stop(app->scene_manager);
}

void app_free(App* app) {
    furi_assert(app);

    free(app->settings);

    view_dispatcher_remove_view(app->view_dispatcher, Pod_Submenu_View);
    view_dispatcher_remove_view(app->view_dispatcher, Pod_Variable_Item_List_View);
    view_dispatcher_remove_view(app->view_dispatcher, Pod_Text_Box_View);

    scene_manager_free(app->scene_manager);
    view_dispatcher_free(app->view_dispatcher);

    submenu_free(app->submenu);
    variable_item_list_free(app->variable_item_list);
    text_box_free(app->text_box);

    free(app);
}
