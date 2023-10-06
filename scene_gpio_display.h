#ifndef SCENE_GPIO_DISPLAY_H
#define SCENE_GPIO_DISPLAY_H

#include "flipper.h"

typedef enum{
    GPIO_Event_Exit,
    GPIO_Event_Type_Tick,
    GPIO_Event_Type_Key,
}GpioDisplayEventType;

typedef struct {
    GpioDisplayEventType type; //Reason for this event
    InputEvent input; //This data is specific to keypress data
}GpioDisplayEvent;

void pod_gpio_display_view_redraw_widget(App* app);
//static void pod_gpio_display_render_callback(void* context);
//static bool pod_gpio_display_input_callback(InputEvent* input_event, void*context);
//static void pod_gpio_display_tick_callback(void* context); 
void pod_gpio_display_scene_on_enter(void* context);
bool pod_gpio_display_scene_on_event(void* context, SceneManagerEvent event);
void pod_gpio_display_scene_on_exit(void* context);
#endif