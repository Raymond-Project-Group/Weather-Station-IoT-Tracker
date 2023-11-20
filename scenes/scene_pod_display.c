#include "../flipper.h"
#include "../app.h"
#include "scenes.h"
#include "scene_pod_display.h"
#include "../pod.h"
#include "../logger/logger.h"
#include "../unit_conversion/unit_conversion.h"
#include "../helpers/settings_helper.h"
#include "scene_widgets.h"

#include <applications/services/gui/modules/widget.h>
#include <applications/services/gui/modules/widget_elements/widget_element.h>
#include <applications/services/gui/view.h>

void pod_display_view_redraw_time(App* app, uint8_t tX, uint8_t tY) //Draw Time
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(
        app->widget,
        tX,
        tY,
        61,
        19,
        0); //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* timeWidgetUnits; //Time's units is the included in the icon symbol
    int8_t hours = data->time_hours;
    switch(app->settings->time) {
    case UTC:
        timeWidgetUnits = (Icon*)&I_time_UTC_11x13;
        break;
    case EST:
        timeWidgetUnits = (Icon*)&I_time_EST_11x13;
        hours -=
            4; //This will might be added to unit conversions, but right now it doesnt need to be
        if(hours < 0) {
            hours += 24;
        }
        break;
    default:
        FURI_LOG_E(TAG, "Unrecognised time type in pod_display_view_redraw_widget");
        app_quit(app);
        return;
    }
    widget_add_icon_element(app->widget, tX + 2, tY + 3, timeWidgetUnits);
    length = snprintf(NULL, 0, "%02d:%02d:%02d", hours, data->time_minutes, data->time_seconds) +
             1; //finds num of digits in time
    char t[length]; //creates string for time
    if(data->time_seconds < 0) { // checks for illegal time
        strcpy(t, "xx:xx:xx");
    } else {
        snprintf(
            t,
            length,
            "%02d:%02d:%02d",
            hours,
            data->time_minutes,
            data->time_seconds); //stores time in string
    }
    widget_add_string_element(
        app->widget, tX + 14, tY + 13, AlignLeft, AlignBottom, FontPrimary, t);
}

void pod_display_view_redraw_latitude(App* app, uint8_t lX, uint8_t lY) //Draw Latitude
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(
        app->widget,
        lX,
        lY,
        61,
        19,
        0); //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* latitudeUnitWidget = (Icon*)&I_latitude_N_10x15;
    widget_add_icon_element(app->widget, lX + 48, lY + 2, latitudeUnitWidget);
    length =
        snprintf(NULL, 0, "%7.4f", (double)data->latitude) + 1; //finds num of digits in latitude
    char t[length]; //creates string for latitude
    snprintf(t, length, "%7.4f", (double)data->latitude); //stores latitude in string
    widget_add_string_element(
        app->widget, lX + 2, lY + 13, AlignLeft, AlignBottom, FontPrimary, t);
}

void pod_display_view_redraw_longitude(App* app, uint8_t lX, uint8_t lY) //Draw Longitude
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(
        app->widget,
        lX,
        lY,
        61,
        19,
        0); //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* longitudeUnitWidget = (Icon*)&I_longitude_W_10x15;
    widget_add_icon_element(app->widget, lX + 48, lY + 2, longitudeUnitWidget);
    length =
        snprintf(NULL, 0, "%7.4f", (double)data->longitude) + 1; //finds num of digits in longitude
    char t[length]; //creates string for longitude
    snprintf(t, length, "%7.4f", (double)data->longitude); //stores longitude in string
    widget_add_string_element(
        app->widget, lX + 2, lY + 13, AlignLeft, AlignBottom, FontPrimary, t);
}

void pod_display_view_redraw_altitude(App* app, uint8_t aX, uint8_t aY) //Draw Altitude
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(
        app->widget,
        aX,
        aY,
        61,
        19,
        0); //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* altitudeWidget = (Icon*)&I_altitude_10x11;
    widget_add_icon_element(app->widget, aX + 2, aY + 4, altitudeWidget);
    Icon* altitudeUnitWidget = (Icon*)&I_m_10x15;
    widget_add_icon_element(app->widget, aX + 50, aY + 2, altitudeUnitWidget);
    length =
        snprintf(NULL, 0, "%.1f", (double)data->altitude) + 1; //finds num of digits in altitude
    char t[length]; //creates string for altitude
    snprintf(t, length, "%.1f", (double)data->altitude); //stores altitude in string
    widget_add_string_element(
        app->widget, aX + 14, aY + 13, AlignLeft, AlignBottom, FontPrimary, t);
}

void pod_display_view_redraw_satellites(App* app, uint8_t sX, uint8_t sY) //Draw Satellites
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(
        app->widget,
        sX,
        sY,
        61,
        19,
        0); //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* satelliteWidget =
        (Icon*)&I_satellites_15x15; //Temperature's units is the only one included in the icon symbol
    widget_add_icon_element(app->widget, sX + 2, sY + 3, satelliteWidget);
    length =
        snprintf(NULL, 0, "%d", data->satellites_tracked) + 1; //finds num of digits in satellites
    char t[length]; //creates string for satellites
    snprintf(t, length, "%d", data->satellites_tracked); //stores satellites in string
    widget_add_string_element(
        app->widget, sX + 20, sY + 13, AlignLeft, AlignBottom, FontPrimary, t);
}

void pod_display_view_redraw_temperature(App* app, uint8_t tX, uint8_t tY) //Draw Temperature
{
    int length;
    Bme280Data* data = app->bme280->data;

    widget_add_frame_element(
        app->widget,
        tX,
        tY,
        61,
        19,
        0); //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* temperatureUnitWidget; //Temperature's units is the only one included in the icon symbol
    float tempGPIO = data->temperature;
    switch(app->settings->temperature) {
    case F:
        temperatureUnitWidget = (Icon*)&I_temp_F_11x14;
        tempGPIO = temperature_conversion(C, F, tempGPIO);
        break;
    case C:
        temperatureUnitWidget = (Icon*)&I_temp_C_11x14;
        break;
    case K:
        temperatureUnitWidget = (Icon*)&I_temp_K_11x14;
        tempGPIO = temperature_conversion(C, K, tempGPIO);
        break;
    default:
        FURI_LOG_E(TAG, "Unrecognised temperature type in pod_display_view_redraw_widget");
        app_quit(app);
        return;
    }
    widget_add_icon_element(app->widget, tX + 2, tY + 3, temperatureUnitWidget);
    length = snprintf(NULL, 0, "%6.2f", (double)tempGPIO) + 1; //finds num of digits in temperature
    char t[length]; //creates string for temp
    snprintf(t, length, "%6.2f", (double)tempGPIO); //stores temp in string
    widget_add_string_element(
        app->widget, tX + 14, tY + 13, AlignLeft, AlignBottom, FontPrimary, t);
}

void pod_display_view_redraw_humidity(App* app, uint8_t hX, uint8_t hY) //Draw Humidity
{
    int length;
    Bme280Data* data = app->bme280->data;

    widget_add_frame_element(app->widget, hX, hY, 61, 19, 0);
    //Humidity Icon and Pressure Icon are static, units then get put to the right
    Icon* humidityWidget = (Icon*)&I_hum_9x15;
    widget_add_icon_element(app->widget, hX + 2, hY + 2, humidityWidget);
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* humidityUnitWidget;
    float humid = data->humidity;
    switch(app->settings->humidity) {
    case relative:
        humidityUnitWidget = (Icon*)&I_percent_11x15;
        break;
    case absolute:
        humidityUnitWidget = (Icon*)&I_g_m3_11x15;
        humid = humidity_conversion(relative, C, absolute, humid, data->temperature);
        break;
    default:
        FURI_LOG_E(TAG, "Unrecognised humidity type in pod_display_view_redraw_widget");
        app_quit(app);
        return;
    }
    widget_add_icon_element(app->widget, hX + 49, hY + 2, humidityUnitWidget);
    length = snprintf(NULL, 0, "%6.2f", (double)humid) + 1; //finds num of digits in humidity
    char h[length]; //creates string for humidity
    snprintf(h, length, "%6.2f", (double)humid); //stores humidity in string
    widget_add_string_element(
        app->widget, hX + 11, hY + 13, AlignLeft, AlignBottom, FontPrimary, h);
}

void pod_display_view_redraw_pressure(App* app, uint8_t pX, uint8_t pY) //Draw Pressure
{
    int length;
    Bme280Data* data = app->bme280->data;
    widget_add_frame_element(app->widget, pX, pY, 61, 19, 0);

    //Humidity Icon and Pressure Icon are static, units then get put to the right
    Icon* pressureWidget = (Icon*)&I_pressure_7x13;
    widget_add_icon_element(app->widget, pX + 2, pY + 4, pressureWidget);
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* pressureUnitWidget;
    float press = data->pressure;
    switch(app->settings->pressure) {
    case mbar:
        pressureUnitWidget = (Icon*)&I_mbar_15x15;
        break;
    case hPa:
        pressureUnitWidget = (Icon*)&I_hpa_15x15;
        break;
    case Torr:
        pressureUnitWidget = (Icon*)&I_torr_15x15;
        press = pressure_conversion(mbar, Torr, press);
        break;
    case PSI:
        pressureUnitWidget = (Icon*)&I_PSI_15x15;
        press = pressure_conversion(mbar, PSI, press);
        break;
    case mmHg:
        pressureUnitWidget = (Icon*)&I_mm_hg_15x15;
        press = pressure_conversion(mbar, mmHg, press);
        break;
    case inHg:
        pressureUnitWidget = (Icon*)&I_in_hg_15x15;
        press = pressure_conversion(mbar, inHg, press);
        break;
    default:
        FURI_LOG_E(TAG, "Unrecognised pressure type in pod_display_view_redraw_widget");
        app_quit(app);
        return;
    }
    widget_add_icon_element(app->widget, pX + 45, pY + 2, pressureUnitWidget);
    length = snprintf(NULL, 0, "%6.2f", (double)press) + 1; //finds num of digits in pressure
    char p[length]; //creates pressure for temp
    snprintf(p, length, "%6.2f", (double)press); //stores temp in pressure
    widget_add_string_element(
        app->widget, pX + 9, pY + 13, AlignLeft, AlignBottom, FontPrimary, p);
}

void pod_display_view_redraw_widget(App* app) {
    FURI_LOG_I(TAG, "Redrawing POD View Widgets");
    widget_reset(app->widget);
    widget_add_frame_element(
        app->widget,
        0,
        0,
        128,
        64,
        0); //Flipper screen size is 128x64, this draws a border around it
    widget_add_frame_element(app->widget, 126, app->canvas_y_offset + 2, 2, 15, 0); //Scroll Bar
    if(app->canvas_y_offset == 0) //Banner
    {
        Icon* podBanner = (Icon*)&I_pod_display_banner_top_90x15;
        widget_add_icon_element(app->widget, 18, 5, podBanner);
    }
    if(app->bme280->state == BME_Disabled) //If initialization failed
    {
        FURI_LOG_I(TAG, "BME disabled");
        bme_free(app->bme280); //free
        app->bme280 = bme_init(); //reattempt initialization
    }

    if(app->bme280->state == BME_Active && app->pws->txrx->history_size > 0) //If init is working
    {
        FURI_LOG_I(TAG, "BME active and Weather Stations active");

        //Build the canvas
        uint8_t tempCombinedX = 2;
        uint8_t tempCombinedY = 23;
        uint8_t humCombinedX = 2;
        uint8_t humCombinedY = 43;
        uint8_t pressX = 2;
        uint8_t pressY = 63;
        uint8_t timerX = 64;
        uint8_t timerY = 63;
        uint8_t latX = 2;
        uint8_t latY = 83;
        uint8_t longX = 64;
        uint8_t longY = 83;
        uint8_t timeX = 2;
        uint8_t timeY = 103;
        uint8_t satX = 64;
        uint8_t satY = 103;

        if(tempCombinedY > app->canvas_y_offset) //Should you draw temp?
        {
            pod_widgets_redraw_temperature(
                app,
                tempCombinedX,
                tempCombinedY - app->canvas_y_offset,
                Pod_Display_Scene,
                app->deltaState);
        }
        if(humCombinedY > app->canvas_y_offset) //Should you draw humidity?
        {
            pod_widgets_redraw_humidity(
                app,
                humCombinedX,
                humCombinedY - app->canvas_y_offset,
                Pod_Display_Scene,
                app->deltaState);
        }
        if(pressY > app->canvas_y_offset) // should you draw pressure?
        {
            pod_widgets_redraw_pressure(app, pressX, pressY - app->canvas_y_offset);
        }
        if(timerY > app->canvas_y_offset) // should you draw timer?
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
        /*if(altY > app->canvas_y_offset) //Should you draw altitude?
        {
            pod_widgets_redraw_altitude(app, altX, altY - app->canvas_y_offset);
        }*/
        if(satY > app->canvas_y_offset) //Should you draw satellites?
        {
            pod_widgets_redraw_satellites(app, satX, satY - app->canvas_y_offset);
        }
    } else {
        widget_add_string_element(
            app->widget,
            3,
            3,
            AlignLeft,
            AlignBottom,
            FontPrimary,
            "Flipper Failed to Connect to BME280 or PWS");
    }
}
static bool pod_display_input_callback(
    InputEvent* input_event,
    void* context) //called when button is pressed
{
    App* app = context;
    bool consumed = false;
    furi_assert(app->queue);
    PodDisplayEvent event = {.type = POD_Display_Key_Event, .input = *input_event};
    if(event.input.type == InputTypeShort && event.input.key == InputKeyBack) {
        view_dispatcher_send_custom_event(app->view_dispatcher, POD_Display_Exit_Event);
        consumed = true;
    } else if(
        event.input.type == InputTypeShort && event.input.key == InputKeyUp &&
        app->canvas_y_offset > 0) {
        app->canvas_y_offset -= 20;
        view_dispatcher_send_custom_event(app->view_dispatcher, POD_Display_Scroll_Event);
        consumed = true;
    } else if(
        event.input.type == InputTypeShort && event.input.key == InputKeyDown &&
        app->canvas_y_offset < 60) {
        app->canvas_y_offset += 20;
        view_dispatcher_send_custom_event(app->view_dispatcher, POD_Display_Scroll_Event);
        consumed = true;
    } else if(event.input.type == InputTypeShort && event.input.key == InputKeyOk) {
        view_dispatcher_send_custom_event(app->view_dispatcher, POD_Display_Log_Event);
        consumed = true;
    } else if(event.input.type == InputTypePress && event.input.key == InputKeyLeft) {
        view_dispatcher_send_custom_event(app->view_dispatcher, POD_Display_Left_Held_Event);
        consumed = true;
    } else if(event.input.type == InputTypePress && event.input.key == InputKeyRight) {
        view_dispatcher_send_custom_event(app->view_dispatcher, POD_Display_Right_Held_Event);
        consumed = true;
    } else if(event.input.type == InputTypeRelease && event.input.key == InputKeyLeft) {
        view_dispatcher_send_custom_event(app->view_dispatcher, POD_Display_Left_Release_Event);
        consumed = true;
    } else if(event.input.type == InputTypeRelease && event.input.key == InputKeyRight) {
        view_dispatcher_send_custom_event(app->view_dispatcher, POD_Display_Right_Release_Event);
        consumed = true;
    }

    //furi_message_queue_put(app->queue, &event, FuriWaitForever);
    return consumed;
    //return false;
}

void pod_display_tick_callback(
    void* context) //every second this is called to read the bme and update the display
{
    App* app = context;
    furi_assert(app->queue);
    FuriMessageQueue* queue = app->queue;
    PodDisplayEvent event = {.type = POD_Display_Tick_Event};
    // It's OK to lose this event if system overloaded (so we don't pass a wait value for 3rd parameter.)
    FURI_LOG_I(TAG, "Updating Message Queue");
    furi_message_queue_put(queue, &event, 0);
    scene_manager_handle_tick_event(app->scene_manager);
}
void pod_display_scene_on_enter(void* context) {
    FURI_LOG_I(TAG, "POD POD_Display Scene entered");
    App* app = context;

    widget_reset(app->widget);
    app->canvas_y_offset = 0;
    app->gps_uart = simple_gps_uart_enable(app);
    app->bme280 = bme_init();
    app->pws = ws_init(app);
    app->initialization_states->pws_initialized = true;
    app->initialization_states->gps_initialized = true;
    app->initialization_states->bme_initialized = true;

    if(app->pws->txrx->rx_key_state == WSRxKeyStateIDLE) {
        ws_preset_init(
            app->pws, "AM650", subghz_setting_get_default_frequency(app->pws->setting), NULL, 0);
        ws_history_reset(app->pws->txrx->history);
        app->pws->txrx->rx_key_state = WSRxKeyStateStart;
    }
    if(app->pws->txrx->txrx_state == WSTxRxStateRx) {
        ws_rx_end(app->pws);
    }

    if((app->pws->txrx->txrx_state == WSTxRxStateIDLE) ||
       (app->pws->txrx->txrx_state == WSTxRxStateSleep)) {
        ws_begin(
            app->pws,
            subghz_setting_get_preset_data_by_name(
                app->pws->setting, furi_string_get_cstr(app->pws->txrx->preset->name)));
        ws_rx(app->pws, app->pws->txrx->preset->frequency);
    }

    //Queue for events(Ticks or input)
    app->queue = furi_message_queue_alloc(8, sizeof(PodDisplayEvent));

    pod_display_view_redraw_widget(app);
    view_set_context(widget_get_view(app->widget), app);
    //view_set_draw_callback(widget_get_view(app->widget),pod_display_render_callback);
    view_set_input_callback(widget_get_view(app->widget), pod_display_input_callback);
    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Widget_View);

    // Update the screen fairly frequently (every 1000 milliseconds = 1 second.)
    app->timer = furi_timer_alloc(pod_display_tick_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 1000);
}

bool pod_display_scene_on_event(void* context, SceneManagerEvent event) {
    App* app = context;
    bool consumed = false;
    //UNUSED(app);
    //UNUSED(event);
    switch(event.type) {
    case SceneManagerEventTypeTick:
        FURI_LOG_I(TAG, "Scene Manager Event Type Tick");
        PodDisplayEvent pod_event;
        if(furi_message_queue_get(app->queue, &pod_event, FuriWaitForever) == FuriStatusOk) {
            switch(pod_event.type) {
            case POD_Display_Tick_Event:
                FURI_LOG_I(TAG, "POD Tick Event");
                FURI_LOG_I(TAG, "Updating Screen");
                furi_mutex_acquire(app->mutex, FuriWaitForever);
                pod_display_view_redraw_widget(app); //redraw widgets
                furi_mutex_release(app->mutex);
                consumed = true;
                //furi_mutex_release(app->mutex);
                break;
            default:
                break;
            }
        } else {
            // We had an issue getting message from the queue, so exit application.
            FURI_LOG_E(TAG, "Scene Manger Event Type Tick Error");
        }
        break;
    case SceneManagerEventTypeCustom:
        FURI_LOG_I(TAG, "Custom Event");
        switch(event.event) {
        case POD_Display_Exit_Event:
            //scene_manager_next_scene(app->scene_manager,Pod_Main_Menu_Scene);
            FURI_LOG_I(TAG, "POD Exit Event");
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
            break;
        case POD_Display_Scroll_Event:
            FURI_LOG_I(TAG, "Scroll Event");
            furi_mutex_acquire(app->mutex, FuriWaitForever);
            pod_display_view_redraw_widget(app); //redraw widgets
            furi_mutex_release(app->mutex);
            consumed = true;
            break;
        case POD_Display_Log_Event:
            FURI_LOG_I(TAG, "Log Event");
            logger_stream_append(app); // Add new log line using current data
            consumed = false;
            break;
        case POD_Display_Left_Held_Event:
            FURI_LOG_I(TAG, "Left held event");
            app->deltaState = POD_Left_Side_Delta;
            break;
        case POD_Display_Right_Held_Event:
            FURI_LOG_I(TAG, "Right held event");
            app->deltaState = POD_Right_Side_Delta;
            break;
        case POD_Display_Left_Release_Event:
            FURI_LOG_I(TAG, "Left release event");
            app->deltaState = POD_Neutral_Delta;
            break;
        case POD_Display_Right_Release_Event:
            FURI_LOG_I(TAG, "Right release event");
            app->deltaState = POD_Neutral_Delta;
            break;
        }
        break;
    default:
        break;
    }
    return consumed;
}

void pod_display_scene_on_exit(void* context) {
    FURI_LOG_I(TAG, "Exiting POD POD_Display Scene");
    App* app = context;
    ws_free(app->pws);
    bme_free(app->bme280);
    gps_uart_disable(app->gps_uart);
    app->initialization_states->pws_initialized = false;
    app->initialization_states->bme_initialized = false;
    app->initialization_states->gps_initialized = false;
    furi_message_queue_free(app->queue);
    furi_timer_free(app->timer);
    widget_reset(app->widget);
}