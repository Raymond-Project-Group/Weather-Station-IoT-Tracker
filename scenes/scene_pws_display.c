#include "../flipper.h"
#include "../app.h"
#include "scenes.h"
#include "scene_pws_display.h"
#include "../pod.h"
#include "../logger/logger.h"
#include "../unit_conversion/unit_conversion.h"
#include "../helpers/settings_helper.h"

#include <applications/services/gui/modules/widget.h>
#include <applications/services/gui/modules/widget_elements/widget_element.h>
#include <applications/services/gui/view.h>
#include "scene_widgets.h"

void pod_pws_display_view_redraw_widget(App* app) {
    FURI_LOG_I(TAG, "Redrawing PWS View Widgets");
    widget_reset(app->widget);
    widget_add_frame_element(
        app->widget,
        0,
        0,
        128,
        64,
        0); //Flipper screen size is 128x64, this draws a border around it
    widget_add_frame_element(app->widget, 126, app->canvas_y_offset + 2, 2, 20, 0); //Scroll Bar
    if(app->canvas_y_offset == 0) //Banner
    {
        Icon* pwsBanner = (Icon*)&I_pws_display_banner_90x15;
        widget_add_icon_element(app->widget, 18, 5, pwsBanner);
    }

    if(app->pws->txrx->history_size > 0) //If init is working
    {
        FURI_LOG_I(TAG, "Weather Stations active");
        FURI_LOG_I(TAG, furi_string_get_cstr(app->pws->data->protocol_name));

        //Build the canvas
        uint8_t tempX = 2;
        uint8_t tempY = 23;
        uint8_t humX = 64;
        uint8_t humY = 23;
        uint8_t timerX = 2;
        uint8_t timerY = 43;
        //uint8_t rssiX = 64;
        //uint8_t rssiY = 43;
        uint8_t timeX = 64;
        uint8_t timeY = 43;
        uint8_t latX = 2;
        uint8_t latY = 63;
        uint8_t longX = 64;
        uint8_t longY = 63;
        uint8_t altX = 2;
        uint8_t altY = 83;
        uint8_t satX = 64;
        uint8_t satY = 83;//*/

        if(tempY > app->canvas_y_offset) //Should you draw temp?
        {
            pod_widgets_redraw_temperature(
                app, tempX, tempY - app->canvas_y_offset, Pod_Pws_Display_Scene, app->deltaState);
        }
        if(humY > app->canvas_y_offset) //Should you draw humidity?
        {
            pod_widgets_redraw_humidity(
                app, humX, humY - app->canvas_y_offset, Pod_Pws_Display_Scene, app->deltaState);
        }
        if(timerY > app->canvas_y_offset) //Should you draw timer?
        {
            pod_widgets_redraw_timer(app, timerX, timerY - app->canvas_y_offset);
        }
        if(timeY > app->canvas_y_offset) //Should you draw time?
        {
            pod_widgets_redraw_time(app, timeX, timeY - app->canvas_y_offset);
        }
        if(latY > app->canvas_y_offset) //Should you draw latitude?
        {
            pod_widgets_redraw_latitude(app, latX, latY - app->canvas_y_offset);
        }
        if(longY > app->canvas_y_offset) //Should you draw longitude?
        {
            pod_widgets_redraw_longitude(app, longX, longY - app->canvas_y_offset);
        }
        if(altY > app->canvas_y_offset) //Should you draw altitude?
        {
            pod_widgets_redraw_altitude(app, altX, altY - app->canvas_y_offset);
        }
        if(satY > app->canvas_y_offset) //Should you draw satellites?
        {
            pod_widgets_redraw_satellites(app, satX, satY - app->canvas_y_offset);
        }
        /*if(rssiY > app->canvas_y_offset)
        {
            pod_widgets_redraw_rssi(app,rssiX,rssiY - app->canvas_y_offset);
        }*/
        //FURI_LOG_I(TAG, "Weather Stations 4");
    }
    else
    {
        widget_add_string_element(app->widget,3,24,AlignLeft,AlignTop,FontPrimary,"No PWS Connected");
    }
}
static bool pod_pws_display_input_callback(
    InputEvent* input_event,
    void* context) //called when button is pressed
{
    App* app = context;
    bool consumed = false;
    furi_assert(app->queue);
    PwsDisplayEvent event = {.type = PWS_Display_Key_Event, .input = *input_event};
    if(event.input.type == InputTypeShort && event.input.key == InputKeyBack) {
        view_dispatcher_send_custom_event(app->view_dispatcher, PWS_Display_Exit_Event);
        consumed = true;
    } else if(
        event.input.type == InputTypeShort && event.input.key == InputKeyUp &&
        app->canvas_y_offset > 0) {
        app->canvas_y_offset -= 20;
        view_dispatcher_send_custom_event(app->view_dispatcher, PWS_Display_Scroll_Event);
        consumed = true;
    } else if(
        event.input.type == InputTypeShort && event.input.key == InputKeyDown &&
        app->canvas_y_offset < 40) {
        app->canvas_y_offset += 20;
        view_dispatcher_send_custom_event(app->view_dispatcher, PWS_Display_Scroll_Event);
        consumed = true;
    } else if(event.input.type == InputTypeShort && event.input.key == InputKeyOk) {
        view_dispatcher_send_custom_event(app->view_dispatcher, PWS_Display_Log_Event);
        consumed = false;
    }
    //furi_message_queue_put(app->queue, &event, FuriWaitForever);
    return consumed;
    //return false;
}

void pod_pws_display_tick_callback(
    void* context) //every second this is called to read the bme and update the display
{
    App* app = context;
    furi_assert(app->queue);
    FuriMessageQueue* queue = app->queue;
    PwsDisplayEvent event = {.type = PWS_Display_Tick_Event};
    // It's OK to lose this event if system overloaded (so we don't pass a wait value for 3rd parameter.)
    FURI_LOG_I(TAG, "Updating Message Queue");
    furi_message_queue_put(queue, &event, 0);
    scene_manager_handle_tick_event(app->scene_manager);
}

void pod_pws_display_scene_on_enter(void* context) {
    FURI_LOG_I(TAG, "PWS Display Scene entered");
    App* app = context;

    widget_reset(app->widget);
    app->canvas_y_offset = 0;
    app->gps_uart = simple_gps_uart_enable(app);
    app->initialization_states->gps_initialized = true;
    
    if(!app->initialization_states->pws_initialized)
    {
        FURI_LOG_I(TAG, "PWS Not Running");
        app->pws = ws_init(app,app->settings->freq);
        app->initialization_states->pws_initialized = true;
        if(app->pws->txrx->rx_key_state == WSRxKeyStateIDLE) {
            ws_preset_init(app->pws, "AM650", subghz_setting_get_default_frequency(app->pws->setting), NULL, 0);
            ws_history_reset(app->pws->txrx->history);
            app->pws->txrx->rx_key_state = WSRxKeyStateStart;
        }
        if(app->pws->txrx->txrx_state == WSTxRxStateRx) {
            ws_rx_end(app->pws);
        }

        if((app->pws->txrx->txrx_state == WSTxRxStateIDLE) || (app->pws->txrx->txrx_state == WSTxRxStateSleep)) {
            ws_begin(app->pws,subghz_setting_get_preset_data_by_name(app->pws->setting, furi_string_get_cstr(app->pws->txrx->preset->name)));
            ws_rx(app->pws, app->pws->txrx->preset->frequency);
        }
    }
    else
    {
        FURI_LOG_I(TAG, "PWS Still Running");
        //ws_init_data(app->pws);
        FlipperFormat* fff = ws_history_get_raw_data(app->pws->txrx->history,app->pws->txrx->idx_menu_chosen);//Gets Flipper Format and Raw Data
        flipper_format_rewind(fff);
        flipper_format_read_string(fff, "Protocol", app->pws->data->protocol_name);//gets protocol name
        ws_block_generic_deserialize(app->pws->data->generic ,fff);
    }

    //Queue for events(Ticks or input)
    app->queue = furi_message_queue_alloc(8,sizeof(PwsDisplayEvent));
    //FURI_LOG_I(TAG, "Queue Running");

    pod_pws_display_view_redraw_widget(app);
    //FURI_LOG_I(TAG, "Widgets Drawn");
    view_set_context(widget_get_view(app->widget), app);
    //FURI_LOG_I(TAG, "View Running");
    //view_set_draw_callback(widget_get_view(app->widget),pod_pws_display_render_callback);
    view_set_input_callback(widget_get_view(app->widget), pod_pws_display_input_callback);
    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Widget_View);
    //FURI_LOG_I(TAG, "Widget Running");

    // Update the screen fairly frequently (every 1000 milliseconds = 1 second.)
    app->timer = furi_timer_alloc(pod_pws_display_tick_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 1000);
    //FURI_LOG_I(TAG, "Timer Running");
}

bool pod_pws_display_scene_on_event(void* context, SceneManagerEvent event) {
    App* app = context;
    bool consumed = false;
    //UNUSED(app);
    //UNUSED(event);
    switch(event.type){
        case SceneManagerEventTypeTick:
            FURI_LOG_I(TAG,"PWS Scene Manager Event Type Tick");
            PwsDisplayEvent pws_event;
            if(furi_message_queue_get(app->queue, &pws_event, FuriWaitForever) == FuriStatusOk) {
                //FURI_LOG_I(TAG,"PWS Scene Manager Event Type Tick2");
                switch(pws_event.type) {
                    case PWS_Display_Tick_Event:
                        //FURI_LOG_I(TAG,"PWS Tick Event");
                        FURI_LOG_I(TAG,"Updating Screen");
                        furi_mutex_acquire(app->mutex, FuriWaitForever);
                        pod_pws_display_view_redraw_widget(app);//redraw widgets
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
            FURI_LOG_I(TAG, "Custom Event");
            switch(event.event) {
            case PWS_Display_Exit_Event:
                //scene_manager_next_scene(app->scene_manager,Pod_Main_Menu_Scene);
                FURI_LOG_I(TAG, "PWS Exit Event");
                consumed = true;
                scene_manager_previous_scene(app->scene_manager);
                break;
            case PWS_Display_Scroll_Event:
                FURI_LOG_I(TAG, "Scroll Event");
                furi_mutex_acquire(app->mutex, FuriWaitForever);
                pod_pws_display_view_redraw_widget(app); //redraw widgets
                furi_mutex_release(app->mutex);
                consumed = true;
                break;
            case PWS_Display_Log_Event:
                FURI_LOG_I(TAG, "Log Event");
                logger_stream_append(app, app->pws->txrx->idx_menu_chosen, app->pws->data->rssi); // Add new log line using current data
                consumed = false;
                break;
            }
            break;
        default:
            break;
    }
    return consumed;
}

void pod_pws_display_scene_on_exit(void* context) {
    FURI_LOG_I(TAG, "Exiting PWS Display Scene");
    App* app = context;
    gps_uart_disable(app->gps_uart);
    app->initialization_states->gps_initialized = false;
    furi_message_queue_free(app->queue);
    furi_timer_free(app->timer);
    widget_reset(app->widget);
}