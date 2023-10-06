#include "flipper.h"
#include "app.h"
#include "scenes.h"
#include "scene_gpio_display.h"
#include "bme280.h"
#include "pod.h"

#include <applications/services/gui/modules/widget.h>
#include <applications/services/gui/modules/widget_elements/widget_element.h>
#include <applications/services/gui/view.h>

void pod_gpio_display_view_redraw_widget(App* app)
{
    FURI_LOG_I(TAG, "Redrawing GPIO View Widgets");
    widget_reset(app->widget);
    int length;
    Bme280Data* data = app->bme280->data;

    //Build the canvas    
    uint8_t tX = 2;
    uint8_t  tY = 3;
    uint8_t  hX = 2;
    uint8_t  hY = 23;
    uint8_t  pX = 2;
    uint8_t  pY = 43;
    widget_add_frame_element(app->widget,0,0,128,64,0); //Flipper screen size is 128x64, this draws a border around it
    widget_add_frame_element(app->widget,tX,tY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    widget_add_frame_element(app->widget,hX,hY,61,19,0);  
    widget_add_frame_element(app->widget,pX,pY,61,19,0);  
    
    //Humidity Icon and Pressure Icon are static, units then get put to the right
    Icon* humidityWidget = (Icon*)&I_hum_9x15;
    Icon* pressureWidget = (Icon*)&I_pressure_7x13;
    widget_add_icon_element(app->widget,hX+2,hY+2,humidityWidget);
    widget_add_icon_element(app->widget,pX+2,pY+4,pressureWidget);


    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* temperatureUnitWidget;//Temperature's units is the only one included in the icon symbol
    Icon* humidityUnitWidget;
    Icon* pressureUnitWidget;
    switch(app->settings->temperature)
    {
        case F:
            temperatureUnitWidget = (Icon*)&I_temp_F_11x14;
            break;
        case C:
            temperatureUnitWidget = (Icon*)&I_temp_C_11x14;
            break;
        case K:
            temperatureUnitWidget = (Icon*)&I_temp_K_11x14;
            break;
        default:
            FURI_LOG_E(TAG, "Unrecognised temperature type in pod_gpio_display_view_redraw_widget");
            app_quit(app);
            return;
    }
    widget_add_icon_element(app->widget, tX+2, tY+3, temperatureUnitWidget);
    length = snprintf(NULL,0,"%6.2f",(double)data->temperature)+1;//finds num of digits in temperature
    char t[length];//creates string for temp
    snprintf(t,length,"%6.2f",(double)data->temperature);//stores temp in string
    widget_add_string_element(app->widget,tX+15,tY+2,AlignLeft,AlignTop,FontPrimary,t);
    
    switch(app->settings->humidity)
    {
        case relative:
            humidityUnitWidget = (Icon*)&I_percent_11x15;
            break;
        case absolute:
            humidityUnitWidget = (Icon*)&I_kg_m3_11x15;
            break;
        default:
            FURI_LOG_E(TAG, "Unrecognised humidity type in pod_gpio_display_view_redraw_widget");
            app_quit(app);
            return;
    }
    widget_add_icon_element(app->widget,hX+49,hY+2,humidityUnitWidget);
    length = snprintf(NULL,0,"%6.2f",(double)data->humidity)+1;//finds num of digits in humidity
    char h[length];//creates string for humidity
    snprintf(h,length,"%6.2f",(double)data->humidity);//stores humidity in string
    widget_add_string_element(app->widget,hX+17,hY+2,AlignLeft,AlignTop,FontPrimary,h);

    switch(app->settings->pressure)
    {
        case mbar:
            pressureUnitWidget = (Icon*)&I_mbar_15x15;
            break;
        case hPa:
            pressureUnitWidget = (Icon*)&I_hpa_15x15;
            break;
        case Torr:
            pressureUnitWidget = (Icon*)&I_torr_15x15;
            break;
        case PSI:
            pressureUnitWidget = (Icon*)&I_PSI_15x15;
            break;
        case mmHg:
            pressureUnitWidget = (Icon*)&I_mm_hg_15x15;
            break;
        case inHg:
            pressureUnitWidget = (Icon*)&I_in_hg_15x15;
            break;
        default:
            FURI_LOG_E(TAG, "Unrecognised pressure type in pod_gpio_display_view_redraw_widget");
            app_quit(app);
            return;
    }
    widget_add_icon_element(app->widget,pX+45,pY+2,pressureUnitWidget);
    length = snprintf(NULL,0,"%6.2f",(double)data->pressure)+1;//finds num of digits in pressure
    char p[length];//creates pressure for temp
    snprintf(p,length,"%6.2f",(double)data->pressure);//stores temp in pressure
    widget_add_string_element(app->widget,pX+19,pY+2,AlignLeft,AlignTop,FontPrimary,p);

}
static bool pod_gpio_display_input_callback(InputEvent* input_event, void*context)//called when button is pressed
{
    App* app = context;
    bool consumed = false;
    furi_assert(app->queue);
    GpioDisplayEvent event = {.type = GPIO_Event_Type_Key, .input = *input_event};
    if(event.input.type == InputTypeShort && event.input.key == InputKeyBack) {
        view_dispatcher_send_custom_event(app->view_dispatcher,GPIO_Event_Exit);
        consumed = true;
    }
    //furi_message_queue_put(app->queue, &event, FuriWaitForever);
    return consumed;
    //return false;
}
/*
void pod_gpio_display_render_callback(Canvas* canvas, void* context)//Updates screen
{
    UNUSED(canvas);
    App* app = context;
    if(furi_mutex_acquire(app->mutex, 200) != FuriStatusOk) //aquires mutex permission 
    { 
        return;
    }
    pod_gpio_display_view_redraw_widget(app); //redraws widgets
    furi_mutex_release(app->mutex); //releases mutex
}*/

void pod_gpio_display_tick_callback(void* context) 
{
    App* app = context;
    furi_assert(app->queue);
    FuriMessageQueue* queue = app->queue;
    GpioDisplayEvent event = {.type = GPIO_Event_Type_Tick};
    // It's OK to lose this event if system overloaded (so we don't pass a wait value for 3rd parameter.)
    FURI_LOG_I(TAG,"Updating Message Queue");
    furi_message_queue_put(queue, &event, 0);
    //view_dispatcher_send_custom_event(app->view_dispatcher,GPIO_Event_Type_Tick);
    scene_manager_handle_tick_event(app->scene_manager);
}
/*static void pod_gpio_tick_event_callback_test(void* context)
{
    App* app = context;
    FURI_LOG_I(TAG,"TICK EVENT CALLBACK WORKS");
    scene_manager_handle_tick_event(app->scene_manager);
}*/
void pod_gpio_display_scene_on_enter(void* context)
{
    FURI_LOG_I(TAG, "GPIO Display Scene entered");
    App* app = context;
    widget_reset(app->widget);

    app->bme280 = bme_init();
    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    //Queue for events(Ticks or input)
    app->queue = furi_message_queue_alloc(8,sizeof(GpioDisplayEvent));

    pod_gpio_display_view_redraw_widget(app);
    view_set_context(widget_get_view(app->widget), app);
    //view_set_draw_callback(widget_get_view(app->widget),pod_gpio_display_render_callback);
    view_set_input_callback(widget_get_view(app->widget),pod_gpio_display_input_callback);
    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Widget_View);

    // Update the screen fairly frequently (every 1000 milliseconds = 1 second.)
    app->timer = furi_timer_alloc(pod_gpio_display_tick_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 1000);
    //view_dispatcher_set_tick_event_callback(app->view_dispatcher,pod_gpio_tick_event_callback_test,100);//Every .1 seconds, send out the tick event callback
    //if(bme_ready(app->bme280)){
    //    FURI_LOG_I(TAG,"BME280 Read Successfully");
    //}
    /*FURI_LOG_I(TAG,"Entering Loop");
    
    // Main loop
    GpioDisplayEvent event;
    bool processing = true;
    do {
        FURI_LOG_I(TAG,"Entering Loop1");
        if(furi_message_queue_get(app->queue, &event, FuriWaitForever) == FuriStatusOk) {
            FURI_LOG_I(TAG,"Entering Loop2");
            switch(event.type) {
                case GPIO_Event_Type_Key:
                    FURI_LOG_I(TAG,"Entering Loop3");
                    // Short press of back button exits the program.
                    if(event.input.type == InputTypeShort && event.input.key == InputKeyBack) {
                        processing = false;
                    }
                    break;
                case GPIO_Event_Type_Tick:
                    FURI_LOG_I(TAG,"Entering Loop4");
                    // Every timer tick we update the i2c status.
                    furi_mutex_acquire(app->mutex, FuriWaitForever);
                    FURI_LOG_I(TAG,"BME280 Reading");
                    if(bme_read_sensors(app->bme280)){
                        FURI_LOG_I(TAG,"BME280 Read Successfully");
                    }
                    furi_mutex_release(app->mutex);
                    break;
                default:
                    break;
            }
            // Send signal to update the screen (callback will get invoked at some point later.)
            //view_port_update(NULL);
        } else {
            // We had an issue getting message from the queue, so exit application.
            processing = false;
        }
    } while(processing);*/


    //pod_gpio_display_scene_on_exit(app);
}

bool pod_gpio_display_scene_on_event(void* context, SceneManagerEvent event)
{
    App* app = context;
    bool consumed = false;
    UNUSED(app);
    //UNUSED(event);
    switch(event.type){
        case SceneManagerEventTypeTick:
            FURI_LOG_I(TAG,"Scene Manager Event Type Tick");
            GpioDisplayEvent gpio_event;
            if(furi_message_queue_get(app->queue, &gpio_event, FuriWaitForever) == FuriStatusOk) {
                switch(gpio_event.type) {
                    case GPIO_Event_Type_Tick:
                        FURI_LOG_I(TAG,"GPIO Tick Event");
                        // Every timer tick we update the i2c status.
                        furi_mutex_acquire(app->mutex, FuriWaitForever);
                        FURI_LOG_I(TAG,"Reading BME280");
                        if(bme_read_sensors(app->bme280)){
                            FURI_LOG_I(TAG,"BME280 Read Successfully");
                            consumed = true;// IF bme read successfully, the screen will be updated
                        }
                        furi_mutex_release(app->mutex);
                        break;
                    default:
                        break;
                }
                if(consumed){//the screen needs to be updated
                    FURI_LOG_I(TAG,"Updating Screen");
                    furi_mutex_acquire(app->mutex, FuriWaitForever);
                    pod_gpio_display_view_redraw_widget(app);//redraw widgets
                    furi_mutex_release(app->mutex);
                }
            } 
            else {
                // We had an issue getting message from the queue, so exit application.
                FURI_LOG_E(TAG,"Scene Manger Event Type Tick Error");
            }
            break;
        case SceneManagerEventTypeCustom:
            FURI_LOG_I(TAG,"Custom Event");
            switch(event.event){
                case GPIO_Event_Exit:
                    //scene_manager_next_scene(app->scene_manager,Pod_Main_Menu_Scene);
                    FURI_LOG_I(TAG,"GPIO Exit Event");
                    consumed = true;
                    scene_manager_previous_scene(app->scene_manager);
                    break;
            }
            break;
        default:
            break;
    }
    return consumed;
}

void pod_gpio_display_scene_on_exit(void* context)
{
    FURI_LOG_I(TAG, "Exiting GPIO Display Scene");
    App* app = context;
    bme_free(app->bme280);
    furi_message_queue_free(app->queue);
    furi_mutex_free(app->mutex);
    furi_timer_free(app->timer);
    widget_reset(app->widget);
}