#ifndef SCENE_POD_DISPLAY_H
#define SCENE_POD_DISPLAY_H

#include "../flipper.h"

typedef enum{
    POD_Display_Exit_Event,
    POD_Display_Tick_Event,
    POD_Display_Scroll_Event,
    POD_Display_Key_Event,
    POD_Display_Log_Event,
}PodDisplayEventType;

typedef struct {
    PodDisplayEventType type; //Reason for this event
    InputEvent input; //This data is specific to keypress data
}PodDisplayEvent;

void pod_display_view_redraw_widget(App* app);
//static void pod_pod_display_render_callback(void* context);
// bool pod_pod_display_input_callback(InputEvent* input_event, void*context);
void pod_display_tick_callback(void* context); 
void pod_display_scene_on_enter(void* context);
bool pod_display_scene_on_event(void* context, SceneManagerEvent event);
void pod_display_scene_on_exit(void* context);
#endif