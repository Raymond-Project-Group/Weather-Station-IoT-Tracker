#include "../flipper.h"
#include "../app.h"
#include "scenes.h"
#include "scene_main_menu.h"
/* main menu scene */

/** main menu callback - sends custom events to the scene manager based on the selection */
void pod_menu_callback(void* context, uint32_t index)
{
    App* app = context;
    switch(index) {
    case Pod_Gpio_Display_Selection:
        scene_manager_handle_custom_event(
            app->scene_manager, Pod_Gpio_Display_Selection_Event);
        break;
    case Pod_Pws_Display_Selection:
        scene_manager_handle_custom_event(
            app->scene_manager, Pod_Pws_Display_Selection_Event);
        break;
    case Pod_Display_Selection:
        scene_manager_handle_custom_event(
            app->scene_manager, Pod_Display_Selection_Event);
        break;
    case Pod_Settings_Selection:
        scene_manager_handle_custom_event(
            app->scene_manager, Pod_Settings_Selection_Event);
        break;
    }
}

/** main menu scene - resets the submenu, and gives it content, callbacks and selection enums */
void pod_main_menu_scene_on_enter(void* context)
{
    App* app = context; 
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Main Menu");
    submenu_add_item(app->submenu, "GPIO Display", Pod_Gpio_Display_Selection, pod_menu_callback, app);
    submenu_add_item(app->submenu, "PWS Display", Pod_Pws_Display_Selection, pod_menu_callback, app);
    submenu_add_item(app->submenu, "Pod Display", Pod_Display_Selection, pod_menu_callback, app);
    submenu_add_item(app->submenu, "Settings", Pod_Settings_Selection, pod_menu_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Submenu_View);
}

/** main menu event handler - switches scene based on the event */
bool pod_main_menu_scene_on_event(void* context, SceneManagerEvent event)
{
    App* app = context;
    bool consumed = false;
    switch(event.type) {
    case SceneManagerEventTypeCustom:
        switch(event.event) {
        case Pod_Gpio_Display_Selection_Event:
            FURI_LOG_I("PODS", "Entering GPIO Display Scene");
            scene_manager_next_scene(app->scene_manager, Pod_Gpio_Display_Scene);
            consumed = true;
            break;
        case Pod_Pws_Display_Selection_Event:
            scene_manager_next_scene(app->scene_manager, Pod_Pws_Display_Scene);
            consumed = true;
            break;
        case Pod_Display_Selection_Event:
            scene_manager_next_scene(app->scene_manager, Pod_Display_Scene);
            consumed = true;
            break;
        case Pod_Settings_Selection_Event:
            scene_manager_next_scene(app->scene_manager, Pod_Settings_Hub_Scene);
            consumed = true;
            break;
        }
        break;
    default: // eg. SceneManagerEventTypeBack, SceneManagerEventTypeTick
        consumed = false;
        break;
    }
    return consumed;
}

/** main menu exit*/
void pod_main_menu_scene_on_exit(void* context)
{
    App* app = context;
    submenu_reset(app->submenu);
}