#include "flipper.h"
#include "app.h"
#include "scenes.h"
#include "scene_main_menu.h"
#include "scene_settings.h"
#include "scene_gpio_display.h"
/** collection of all scene on_enter handlers */
void (*const pod_scene_on_enter_handlers[])(void*) = {
    pod_main_menu_scene_on_enter,
    pod_settings_scene_on_enter,
    pod_gpio_display_scene_on_enter};

/** collection of all scene on event handlers */
bool (*const pod_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    pod_main_menu_scene_on_event,
    pod_settings_scene_on_event,
    pod_gpio_display_scene_on_event};

/** collection of all scene on exit handlers */
void (*const pod_scene_on_exit_handlers[])(void*) = {
    pod_main_menu_scene_on_exit,
    pod_settings_scene_on_exit,
    pod_gpio_display_scene_on_exit};

/** collection of all on_enter, on_event, on_exit handlers */
const SceneManagerHandlers pod_scene_manager_handlers = {
    .on_enter_handlers = pod_scene_on_enter_handlers,
    .on_event_handlers = pod_scene_on_event_handlers,
    .on_exit_handlers = pod_scene_on_exit_handlers,
    .scene_num = Pod_Scene_Count};

/* callbacks */

/** custom event handler */
bool pod_custom_callback(void* context, uint32_t custom_event) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

bool pod_back_event_callback(void* context) {
    furi_assert(context);
    App* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}