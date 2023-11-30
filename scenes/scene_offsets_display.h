#ifndef SCENE_OFFSETS_DISPLAY_H
#define SCENE_OFFSETS_DISPLAY_H

#include "../flipper.h"
#include "../app.h"

typedef enum {
	Offsets_Display_Exit_Event,
    Offsets_Display_Tick_Event,
    Offsets_Display_Scroll_Event,
    Offsets_Display_Input_Event,
    Offsets_Display_Key_Event,
    Offsets_Display_Log_Event,
} OffsetsDisplayEventType;

typedef struct {
    OffsetsDisplayEventType type;
    InputEvent input;
} OffsetsDisplayEvent;

void offsets_display_view_redraw_widget(App* app);
void offsets_display_tick_callback(void* context); 
void offsets_display_scene_on_enter(void* context);
bool offsets_display_scene_on_event(void* context, SceneManagerEvent event);
void offsets_display_scene_on_exit(void* context);


#endif