#ifndef SCENE_MAIN_MENU_H
#define SCENE_MAIN_MENU_H

#include "../flipper.h"
/** main menu options enum */
typedef enum {
    Pod_Gpio_Display_Selection,
    Pod_Pws_Display_Selection,
    Pod_Pws_Selection_Selection,
    Pod_Display_Selection,
    Pod_Pws_Transmit_Display_Selection,
    Pod_Settings_Selection,
} PodMainMenuSceneIndex;

/** main menu events */
typedef enum {
    Pod_Gpio_Display_Selection_Event,
    Pod_Pws_Display_Selection_Event,
    Pod_Pws_Selection_Selection_Event,
    Pod_Display_Selection_Event,
    Pod_Pws_Transmit_Display_Selection_Event,
    Pod_Settings_Selection_Event,
} PodMainMenuEvent;

void pod_menu_callback(void* context, uint32_t index);
void pod_main_menu_scene_on_enter(void* context);
bool pod_main_menu_scene_on_event(void* context, SceneManagerEvent event);
void pod_main_menu_scene_on_exit(void* context);

#endif