#ifndef SCENE_PWS_TRANSMIT_DISPLAY_H
#define SCENE_PWS_TRANSMIT_DISPLAY_H

#include "../flipper.h"
#include "../weather_stations/weather_station.h"
typedef enum{
    PWS_Transmit_Display_Exit_Event,
    PWS_Transmit_Display_Tick_Event,
    PWS_Transmit_Display_Scroll_Event,
    PWS_Transmit_Display_Input_Event,
    PWS_Transmit_Display_Key_Event,
    PWS_Transmit_Display_Log_Event,
}PwsTransmitDisplayEventType;

typedef struct {
    PwsTransmitDisplayEventType type; //Reason for this event
    InputEvent input; //This data is specific to keypress data
}PwsTransmitDisplayEvent;

void pod_pws_transmit_display_view_redraw_widget(App* app);
//static void pod_pws_transmit_display_render_callback(void* context);
// bool pod_pws_transmit_display_input_callback(InputEvent* input_event, void*context);
void pod_pws_transmit_display_tick_callback(void* context); 
void pod_pws_transmit_display_scene_on_enter(void* context);
bool pod_pws_transmit_display_scene_on_event(void* context, SceneManagerEvent event);
void pod_pws_transmit_display_scene_on_exit(void* context);
#endif