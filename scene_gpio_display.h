#ifndef SCENE_GPIO_DISPLAY_H
#define SCENE_GPIO_DISPLAY_H

#include "flipper.h"

typedef enum{
    GPIO_Display_Exit_Event,
    GPIO_Display_Tick_Event,
    GPIO_Display_Scroll_Event,
    GPIO_Display_Key_Event,
}GpioDisplayEventType;

typedef struct {
    GpioDisplayEventType type; //Reason for this event
    InputEvent input; //This data is specific to keypress data
}GpioDisplayEvent;

void pod_gpio_display_view_redraw_widget(App* app);
//static void pod_gpio_display_render_callback(void* context);
// bool pod_gpio_display_input_callback(InputEvent* input_event, void*context);
void pod_gpio_display_tick_callback(void* context); 
void pod_gpio_display_scene_on_enter(void* context);
bool pod_gpio_display_scene_on_event(void* context, SceneManagerEvent event);
void pod_gpio_display_scene_on_exit(void* context);
#endif