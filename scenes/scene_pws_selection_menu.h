#ifndef SCENE_PWS_SELECTION_MENU_H
#define SCENE_PWS_SELECTION_MENU_H
#include "../flipper.h"
#include "../pod.h"

typedef enum{
    PWS_Selection_Exit_Event,
    PWS_Selection_Tick_Event,
    PWS_Selection_Scroll_Event,
    PWS_Selection_Key_Event,
    PWS_Selection_Log_Event,
}PwsSelectionEventType;

typedef struct {
    PwsSelectionEventType type; //Reason for this event
    InputEvent input; //This data is specific to keypress data
}PwsSelectionEvent;
/*
typedef enum {
    Pod_Gpio_Display_Selection,
    Pod_Pws_Display_Selection,
    Pod_Display_Selection,
    Pod_Pws_Transmit_Display_Selection,
    Pod_Settings_Selection,
} PodMainMenuSceneIndex;

// main menu events 
typedef enum {
    Pod_Gpio_Display_Selection_Event,
    Pod_Pws_Display_Selection_Event,
    Pod_Display_Selection_Event,
    Pod_Pws_Transmit_Display_Selection_Event,
    Pod_Settings_Selection_Event,
} PodMainMenuEvent;
*/
void pod_pws_selection_callback(void* context, uint32_t index);
void pod_pws_selection_scene_on_enter(void* context);
bool pod_pws_selection_scene_on_event(void* context, SceneManagerEvent event);
void pod_pws_selection_scene_on_exit(void* context);

#endif