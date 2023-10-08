#include "flipper.h"
#include "app.h"
#include "scenes.h"
#include "scene_gpio_display.h"
#include "bme280.h"
#include "pod.h"
#include "logger.h"
#include "unit_conversion.h"

#include <applications/services/gui/modules/widget.h>
#include <applications/services/gui/modules/widget_elements/widget_element.h>
#include <applications/services/gui/view.h>


void pod_gpio_display_view_redraw_temperature(App* app,uint8_t tX, uint8_t tY) //Draw Temperature
{
    int length;
    Bme280Data* data = app->bme280->data;

    widget_add_frame_element(app->widget,tX,tY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* temperatureUnitWidget;//Temperature's units is the only one included in the icon symbol
    float temp = data->temperature;
    switch(app->settings->temperature)
    {
        case F:
            temperatureUnitWidget = (Icon*)&I_temp_F_11x14;
            temp = temperature_conversion(C,F,temp);
            break;
        case C:
            temperatureUnitWidget = (Icon*)&I_temp_C_11x14;
            break;
        case K:
            temperatureUnitWidget = (Icon*)&I_temp_K_11x14;
            temp = temperature_conversion(C,K,temp);
            break;
        default:
            FURI_LOG_E(TAG, "Unrecognised temperature type in pod_gpio_display_view_redraw_widget");
            app_quit(app);
            return;
    }
    widget_add_icon_element(app->widget, tX+2, tY+3, temperatureUnitWidget);
    length = snprintf(NULL,0,"%6.2f",(double)temp)+1;//finds num of digits in temperature
    char t[length];//creates string for temp
    snprintf(t,length,"%6.2f",(double)temp);//stores temp in string
    widget_add_string_element(app->widget,tX+14,tY+13,AlignLeft,AlignBottom,FontPrimary,t);
}

void pod_gpio_display_view_redraw_humidity(App* app,uint8_t hX, uint8_t hY) //Draw Humidity
{
    int length;
    Bme280Data* data = app->bme280->data;

    widget_add_frame_element(app->widget,hX,hY,61,19,0);  
    //Humidity Icon and Pressure Icon are static, units then get put to the right
    Icon* humidityWidget = (Icon*)&I_hum_9x15;
    widget_add_icon_element(app->widget,hX+2,hY+2,humidityWidget);
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* humidityUnitWidget;
    float humid = data->humidity;
    switch(app->settings->humidity)
    {
        case relative:
            humidityUnitWidget = (Icon*)&I_percent_11x15;
            break;
        case absolute:
            humidityUnitWidget = (Icon*)&I_g_m3_11x15;
            humid = humidity_conversion(relative,C,absolute,humid,data->temperature);
            break;
        default:
            FURI_LOG_E(TAG, "Unrecognised humidity type in pod_gpio_display_view_redraw_widget");
            app_quit(app);
            return;
    }
    widget_add_icon_element(app->widget,hX+49,hY+2,humidityUnitWidget);
    length = snprintf(NULL,0,"%6.2f",(double)humid)+1;//finds num of digits in humidity
    char h[length];//creates string for humidity
    snprintf(h,length,"%6.2f",(double)humid);//stores humidity in string
    widget_add_string_element(app->widget,hX+11,hY+13,AlignLeft,AlignBottom,FontPrimary,h);
}

void pod_gpio_display_view_redraw_pressure(App* app, uint8_t pX, uint8_t pY) //Draw Pressure
{
    int length;
    Bme280Data* data = app->bme280->data;
    widget_add_frame_element(app->widget,pX,pY,61,19,0);  
    
    //Humidity Icon and Pressure Icon are static, units then get put to the right
    Icon* pressureWidget = (Icon*)&I_pressure_7x13;
    widget_add_icon_element(app->widget,pX+2,pY+4,pressureWidget);
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* pressureUnitWidget;
    float press = data->pressure;
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
            press = pressure_conversion(mbar,Torr,press);
            break;
        case PSI:
            pressureUnitWidget = (Icon*)&I_PSI_15x15;
            press = pressure_conversion(mbar,PSI,press);
            break;
        case mmHg:
            pressureUnitWidget = (Icon*)&I_mm_hg_15x15;
            press = pressure_conversion(mbar,mmHg,press);
            break;
        case inHg:
            pressureUnitWidget = (Icon*)&I_in_hg_15x15;
            press = pressure_conversion(mbar,inHg,press);
            break;
        default:
            FURI_LOG_E(TAG, "Unrecognised pressure type in pod_gpio_display_view_redraw_widget");
            app_quit(app);
            return;
    }
    widget_add_icon_element(app->widget,pX+45,pY+2,pressureUnitWidget);
    length = snprintf(NULL,0,"%6.2f",(double)press)+1;//finds num of digits in pressure
    char p[length];//creates pressure for temp
    snprintf(p,length,"%6.2f",(double)press);//stores temp in pressure
    widget_add_string_element(app->widget,pX+9,pY+13,AlignLeft,AlignBottom,FontPrimary,p);
}

void pod_gpio_display_view_redraw_widget(App* app)
{
    FURI_LOG_I(TAG, "Redrawing GPIO View Widgets");
    widget_reset(app->widget);
    widget_add_frame_element(app->widget,0,0,128,64,0); //Flipper screen size is 128x64, this draws a border around it
    
    if(app->bme280->state == BME_Disabled)//If initialization failed
    {
        FURI_LOG_I(TAG, "BME disabled");
        bme_free(app->bme280);//free
        app->bme280 = bme_init();//reattempt initialization
    }
    
    if(app->bme280->state == BME_Active)//If init is working
    {

        FURI_LOG_I(TAG, "BME active");

        //Build the canvas
        uint8_t tX = 2;
        uint8_t  tY = 3;    
        uint8_t  hX = 2;
        uint8_t  hY = 23;
        uint8_t  pX = 2;
        uint8_t  pY = 43;

        if(tY > app->canvas_y_offset)//Should you draw temp?
        {
            pod_gpio_display_view_redraw_temperature(app,tX,tY - app->canvas_y_offset);
        }
        if(hY > app->canvas_y_offset)//Should you draw humidity?
        {
            pod_gpio_display_view_redraw_humidity(app,hX,hY - app->canvas_y_offset);
        }
        if(pY > app->canvas_y_offset)//Should you draw pressure?
        {
            pod_gpio_display_view_redraw_pressure(app,pX,pY - app->canvas_y_offset);
        }
    }
    else
    {
        widget_add_string_element(app->widget,3,3,AlignLeft,AlignBottom,FontPrimary,"Flipper Failed to Connect to BME280");
    }
}
static bool pod_gpio_display_input_callback(InputEvent* input_event, void*context)//called when button is pressed
{
    App* app = context;
    bool consumed = false;
    furi_assert(app->queue);
    GpioDisplayEvent event = {.type = GPIO_Display_Key_Event, .input = *input_event};
    if(event.input.type == InputTypeShort && event.input.key == InputKeyBack) {
        view_dispatcher_send_custom_event(app->view_dispatcher,GPIO_Display_Exit_Event);
        consumed = true;
    }
    else if(event.input.type == InputTypeShort && event.input.key == InputKeyUp && app->canvas_y_offset>0){
        app->canvas_y_offset-=20;
        view_dispatcher_send_custom_event(app->view_dispatcher,GPIO_Display_Scroll_Event);
        consumed = true;
    }
    else if(event.input.type == InputTypeShort && event.input.key == InputKeyDown && app->canvas_y_offset<128){
        app->canvas_y_offset+=20;
        view_dispatcher_send_custom_event(app->view_dispatcher,GPIO_Display_Scroll_Event);
        consumed = true;
    }
    else if (event.input.type == InputTypeShort && event.input.key == InputKeyOk) {
        view_dispatcher_send_custom_event(app->view_dispatcher, GPIO_Display_Log_Event);
        consumed = false;
    }
    //furi_message_queue_put(app->queue, &event, FuriWaitForever);
    return consumed;
    //return false;
}

void pod_gpio_display_tick_callback(void* context)//every second this is called to read the bme and update the display
{
    App* app = context;
    furi_assert(app->queue);
    FuriMessageQueue* queue = app->queue;
    GpioDisplayEvent event = {.type = GPIO_Display_Tick_Event};
    // It's OK to lose this event if system overloaded (so we don't pass a wait value for 3rd parameter.)
    FURI_LOG_I(TAG,"Updating Message Queue");
    furi_message_queue_put(queue, &event, 0);
    scene_manager_handle_tick_event(app->scene_manager);
}
void pod_gpio_display_scene_on_enter(void* context)
{
    FURI_LOG_I(TAG, "GPIO Display Scene entered");
    App* app = context;

    widget_reset(app->widget);
    app->canvas_y_offset = 0;
    //app->bme280 = bme_init();
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
}

bool pod_gpio_display_scene_on_event(void* context, SceneManagerEvent event)
{
    App* app = context;
    bool consumed = false;
    //UNUSED(app);
    //UNUSED(event);
    switch(event.type){
        case SceneManagerEventTypeTick:
            FURI_LOG_I(TAG,"Scene Manager Event Type Tick");
            GpioDisplayEvent gpio_event;
            if(furi_message_queue_get(app->queue, &gpio_event, FuriWaitForever) == FuriStatusOk) {
                switch(gpio_event.type) {
                    case GPIO_Display_Tick_Event:
                        FURI_LOG_I(TAG,"GPIO Tick Event");
                        FURI_LOG_I(TAG,"Updating Screen");
                        furi_mutex_acquire(app->mutex, FuriWaitForever);
                        pod_gpio_display_view_redraw_widget(app);//redraw widgets
                        furi_mutex_release(app->mutex);
                        consumed = true;
                        //furi_mutex_release(app->mutex);
                        break;
                    default:
                        break;
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
                case GPIO_Display_Exit_Event:
                    //scene_manager_next_scene(app->scene_manager,Pod_Main_Menu_Scene);
                    FURI_LOG_I(TAG,"GPIO Exit Event");
                    consumed = true;
                    scene_manager_previous_scene(app->scene_manager);
                    break;
                case GPIO_Display_Scroll_Event:
                    FURI_LOG_I(TAG,"Scroll Event");
                    furi_mutex_acquire(app->mutex, FuriWaitForever);
                    pod_gpio_display_view_redraw_widget(app);//redraw widgets
                    furi_mutex_release(app->mutex);
                    consumed = true;
                    break;
                case GPIO_Display_Log_Event:
                    FURI_LOG_I(TAG, "Log Event");
                    furi_mutex_acquire(app->mutex, FuriWaitForever);
                    logger_stream_append(app->file_stream, app->bme280->data); // Add new log line using current data
                    furi_mutex_release(app->mutex);
                    consumed = false;
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
    furi_message_queue_free(app->queue);
    furi_mutex_free(app->mutex);
    furi_timer_free(app->timer);
    widget_reset(app->widget);
}