#include "../bme280/bme280.h"
#include "../pod.h"
#include "../logger/logger.h"
#include "../unit_conversion/unit_conversion.h"
#include "../weather_stations/weather_station.h"
#include "../helpers/settings_helper.h"
#include "scene_widgets.h"
#include <applications/services/gui/modules/widget.h>
#include <applications/services/gui/modules/widget_elements/widget_element.h>
#include <applications/services/gui/view.h>

void pod_widgets_redraw_timer(App* app,uint8_t tX, uint8_t tY) //Draw Timer
{
    int length;
    FuriHalRtcDateTime curr_dt;
    furi_hal_rtc_get_datetime(&curr_dt); //get flipper date time
    uint32_t curr_ts = furi_hal_rtc_datetime_to_timestamp(&curr_dt); //get flipper time stamp
    WSBlockGeneric* data = app->pws->data->generic;
    
    widget_add_frame_element(app->widget,tX,tY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* timer = (Icon *)&I_Timer_11x11;//Timer image
    widget_add_icon_element(app->widget, tX+2, tY+3, timer);
    length = snprintf(NULL,0,"%d sec",(int)curr_ts-(int)data->timestamp)+1;//finds time since last transmission
    char t[length];//creates string for time
    snprintf(t,length,"%d sec",(int)curr_ts-(int)data->timestamp);//stores time since last transmission
    widget_add_string_element(app->widget,tX+14,tY+13,AlignLeft,AlignBottom,FontPrimary,t);
}


void pod_widgets_redraw_time(App* app,uint8_t tX, uint8_t tY) //Draw Time
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(app->widget,tX,tY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* timeWidgetUnits;//Time's units is the included in the icon symbol
    int8_t hours = data->time_hours;
    switch(app->settings->time)
    {
        case UTC:
            timeWidgetUnits = (Icon *)&I_time_UTC_11x13;
            break;
        case EST:
            timeWidgetUnits = (Icon *)&I_time_EST_11x13;
            hours -=4; //This will might be added to unit conversions, but right now it doesnt need to be
            if(hours<0){
                hours+=24;
            }
            break;
        default:
            FURI_LOG_E(TAG, "Unrecognised time type in pod_widgets_redraw_widget");
            app_quit(app);
            return;
    }
    widget_add_icon_element(app->widget, tX+2, tY+3, timeWidgetUnits);
    length = snprintf(NULL,0,"%02d:%02d:%02d",hours,data->time_minutes,data->time_seconds)+1;//finds num of digits in time
    char t[length];//creates string for time
    if (data->time_seconds < 0) { // checks for illegal time
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
    widget_add_string_element(app->widget,tX+14,tY+13,AlignLeft,AlignBottom,FontPrimary,t);
}

void pod_widgets_redraw_latitude(App* app,uint8_t lX, uint8_t lY) //Draw Latitude
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(app->widget,lX,lY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* latitudeUnitWidget = (Icon*)&I_latitude_N_10x15;
    widget_add_icon_element(app->widget, lX+48, lY+2, latitudeUnitWidget);
    length = snprintf(NULL,0,"%7.4f",(double)data->latitude)+1;//finds num of digits in latitude
    char t[length];//creates string for latitude
    snprintf(t,length,"%7.4f",(double)data->latitude);//stores latitude in string
    widget_add_string_element(app->widget,lX+2,lY+13,AlignLeft,AlignBottom,FontPrimary,t);
}

void pod_widgets_redraw_longitude(App* app,uint8_t lX, uint8_t lY) //Draw Longitude
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(app->widget,lX,lY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* longitudeUnitWidget = (Icon*)&I_longitude_W_10x15;
    widget_add_icon_element(app->widget, lX+48, lY+2, longitudeUnitWidget);
    length = snprintf(NULL,0,"%7.4f",(double)data->longitude)+1;//finds num of digits in longitude
    char t[length];//creates string for longitude
    snprintf(t,length,"%7.4f",(double)data->longitude);//stores longitude in string
    widget_add_string_element(app->widget,lX+2,lY+13,AlignLeft,AlignBottom,FontPrimary,t);
}

void pod_widgets_redraw_altitude(App* app,uint8_t aX, uint8_t aY) //Draw Altitude
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(app->widget,aX,aY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* altitudeWidget = (Icon*)&I_altitude_10x11;
    widget_add_icon_element(app->widget, aX+2, aY+4, altitudeWidget);
    Icon* altitudeUnitWidget = (Icon*)&I_m_10x15;
    widget_add_icon_element(app->widget, aX+50, aY+2, altitudeUnitWidget);
    length = snprintf(NULL,0,"%.1f",(double)data->altitude)+1;//finds num of digits in altitude
    char t[length];//creates string for altitude
    snprintf(t,length,"%.1f",(double)data->altitude);//stores altitude in string
    widget_add_string_element(app->widget,aX+14,aY+13,AlignLeft,AlignBottom,FontPrimary,t);
}

void pod_widgets_redraw_satellites(App* app,uint8_t sX, uint8_t sY) //Draw Satellites
{
    int length;
    GpsStatus* data = app->gps_uart->status;

    widget_add_frame_element(app->widget,sX,sY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* satelliteWidget = (Icon*) &I_satellites_15x15;//Temperature's units is the only one included in the icon symbol
    widget_add_icon_element(app->widget, sX+2, sY+3, satelliteWidget);
    length = snprintf(NULL,0,"%d",data->satellites_tracked)+1;//finds num of digits in satellites
    char t[length];//creates string for satellites
    snprintf(t,length,"%d",data->satellites_tracked);//stores satellites in string
    widget_add_string_element(app->widget,sX+20,sY+13,AlignLeft,AlignBottom,FontPrimary,t);
}

void pod_widgets_redraw_temperature(App* app,uint8_t tX, uint8_t tY, int page, PodDeltaState deltaState) //Draw Temperature
{
    int length;
    if(page != Pod_Display_Scene)
    {
        float temp = 0.0/0.0;
        if(page==Pod_Gpio_Display_Scene)
        {
            temp = get_calibrated_temperature(app);
        }
        else if (page == Pod_Pws_Display_Scene)
        {
            WSBlockGeneric* data = app->pws->data->generic;
            temp = data->temp;
        }
        widget_add_frame_element(app->widget,tX,tY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
        //Temperature, Humidity, and Pressure Units can change off settings
        Icon* temperatureUnitWidget;//Temperature's units is the only one included in the icon symbol
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
                FURI_LOG_E(TAG, "Unrecognised temperature type in pod_widgets_redraw_widget");
                app_quit(app);
                return;
        }
        widget_add_icon_element(app->widget, tX+2, tY+3, temperatureUnitWidget);
        length = snprintf(NULL,0,"%6.2f",(double)temp)+1;//finds num of digits in temperature
        char t[length];//creates string for temp
        snprintf(t,length,"%6.2f",(double)temp);//stores temp in string
        widget_add_string_element(app->widget,tX+14,tY+13,AlignLeft,AlignBottom,FontPrimary,t);
    }
    else {

        float tempGPIO = 0.0/0.0;
        float tempPWS = 0.0/0.0;

        tempGPIO = get_calibrated_temperature(app);
        WSBlockGeneric* dataPWS = app->pws->data->generic;
        tempPWS = dataPWS->temp;

        widget_add_frame_element(app->widget,tX,tY,123,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
        //Temperature, Humidity, and Pressure Units can change off settings
        Icon* temperatureUnitWidget;//Temperature's units is the only one included in the icon symbol
        switch(app->settings->temperature)
        {
            case F:
                temperatureUnitWidget = (Icon*)&I_temp_F_11x14;
                tempGPIO = temperature_conversion(C,F,tempGPIO);
                tempPWS = temperature_conversion(C,F,tempPWS);
                break;
            case C:
                temperatureUnitWidget = (Icon*)&I_temp_C_11x14;
                break;
            case K:
                temperatureUnitWidget = (Icon*)&I_temp_K_11x14;
                tempGPIO = temperature_conversion(C,K,tempGPIO);
                tempPWS = temperature_conversion(C,K,tempPWS);
                break;
            default:
                FURI_LOG_E(TAG, "Unrecognised temperature type in pod_widgets_redraw_widget");
                app_quit(app);
                return;
        }
        switch (deltaState) {
            case POD_Left_Side_Delta:
                tempGPIO = tempGPIO - tempPWS;
                break;
            case POD_Right_Side_Delta:
                tempPWS = tempGPIO - tempPWS;
                break;
            default:
                break;
        }
        
        length = snprintf(NULL,0,"%6.2f",(double)tempGPIO)+1;//finds num of digits in temperature
        char tLeft[length];//creates string for temp
        snprintf(tLeft,length,"%6.2f",(double)tempGPIO);//stores temp in string
        widget_add_string_element(app->widget,tX+2,tY+13,AlignLeft,AlignBottom,FontPrimary,tLeft);

        widget_add_icon_element(app->widget, tX+56, tY+3, temperatureUnitWidget);

        length = snprintf(NULL,0,"%6.2f",(double)tempPWS)+1;//finds num of digits in temperature
        char tRight[length];//creates string for temp
        snprintf(tRight,length,"%6.2f",(double)tempPWS);//stores temp in string
        widget_add_string_element(app->widget,tX+121,tY+13,AlignRight,AlignBottom,FontPrimary,tRight);
    }
}

void pod_widgets_redraw_humidity(App* app,uint8_t hX, uint8_t hY,int page, PodDeltaState deltaState) //Draw Humidity
{
    int length;
    if(page != Pod_Display_Scene)
    {
        float humid = 0.0/0.0;
        float temp = 0.0/0.0;
        if(page==Pod_Gpio_Display_Scene)
        {
            humid = get_calibrated_humidity(app);
            temp = get_calibrated_temperature(app);
        }
        else if (page == Pod_Pws_Display_Scene)
        {
            WSBlockGeneric* data = app->pws->data->generic;
            humid = data->humidity*1.0;
            temp = data->temp;
        }

        widget_add_frame_element(app->widget,hX,hY,61,19,0);  
        //Humidity Icon and Pressure Icon are static, units then get put to the right
        Icon* humidityWidget = (Icon*)&I_hum_9x15;
        widget_add_icon_element(app->widget,hX+2,hY+2,humidityWidget);
        //Temperature, Humidity, and Pressure Units can change off settings
        Icon* humidityUnitWidget;
        switch(app->settings->humidity)
        {
            case relative:
                humidityUnitWidget = (Icon*)&I_percent_11x15;
                break;
            case absolute:
                humidityUnitWidget = (Icon*)&I_g_m3_11x15;
                humid = humidity_conversion(relative,C,absolute,humid,temp);
                break;
            default:
                FURI_LOG_E(TAG, "Unrecognised humidity type in pod_widgets_redraw_widget");
                app_quit(app);
                return;
        }
        widget_add_icon_element(app->widget,hX+49,hY+2,humidityUnitWidget);
        length = snprintf(NULL,0,"%6.2f",(double)humid)+1;//finds num of digits in humidity
        char h[length];//creates string for humidity
        snprintf(h,length,"%6.2f",(double)humid);//stores humidity in string
        widget_add_string_element(app->widget,hX+11,hY+13,AlignLeft,AlignBottom,FontPrimary,h);

    }
    else {

        float humidGPIO = 0.0/0.0;
        float humidPWS = 0.0/0.0;
        float tempGPIO = 0.0/0.0;
        float tempPWS = 0.0/0.0;

        humidGPIO = get_calibrated_humidity(app);
        tempGPIO = get_calibrated_temperature(app);
        WSBlockGeneric* dataPWS = app->pws->data->generic;
        humidPWS = dataPWS->humidity*1.0;
        tempPWS = dataPWS->temp;

        widget_add_frame_element(app->widget,hX,hY,123,19,0);  
        //Humidity Icon and Pressure Icon are static, units then get put to the right
        Icon* humidityWidget = (Icon*)&I_hum_9x15;
        widget_add_icon_element(app->widget,hX+57,hY+2,humidityWidget);
        //Temperature, Humidity, and Pressure Units can change off settings
        Icon* humidityUnitWidget;
        switch(app->settings->humidity)
        {
            case relative:
                humidityUnitWidget = (Icon*)&I_percent_11x15;
                break;
            case absolute:
                humidityUnitWidget = (Icon*)&I_g_m3_11x15;
                humidGPIO = humidity_conversion(relative,C,absolute,humidGPIO,tempGPIO);
                humidPWS = humidity_conversion(relative,C,absolute,humidPWS,tempPWS);
                break;
            default:
                FURI_LOG_E(TAG, "Unrecognised humidity type in pod_widgets_redraw_widget");
                app_quit(app);
                return;
        }

        switch (deltaState) {
            case POD_Left_Side_Delta:
                humidGPIO = humidGPIO - humidPWS;
                break;
            case POD_Right_Side_Delta:
                humidPWS = humidGPIO - humidPWS;
                break;
            default:
                break;
        }

        widget_add_icon_element(app->widget,hX+40,hY+2,humidityUnitWidget);
        length = snprintf(NULL,0,"%6.2f",(double)humidGPIO)+1;//finds num of digits in humidity
        char h[length];//creates string for humidity
        snprintf(h,length,"%6.2f",(double)humidGPIO);//stores humidity in string
        widget_add_string_element(app->widget,hX+2,hY+13,AlignLeft,AlignBottom,FontPrimary,h);

        widget_add_icon_element(app->widget,hX+110,hY+2,humidityUnitWidget);
        length = snprintf(NULL,0,"%6.2f",(double)humidPWS)+1;//finds num of digits in humidity
        char hRight[length];//creates string for humidity
        snprintf(hRight,length,"%6.2f",(double)humidPWS);//stores humidity in string
        widget_add_string_element(app->widget,hX+108,hY+13,AlignRight,AlignBottom,FontPrimary,hRight);
    }
}

void pod_widgets_redraw_pressure(App* app, uint8_t pX, uint8_t pY) //Draw Pressure
{
    int length;
    widget_add_frame_element(app->widget,pX,pY,61,19,0);  
    
    //Humidity Icon and Pressure Icon are static, units then get put to the right
    Icon* pressureWidget = (Icon*)&I_pressure_7x13;
    widget_add_icon_element(app->widget,pX+2,pY+4,pressureWidget);
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* pressureUnitWidget;
    float press = get_calibrated_pressure(app);
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
            FURI_LOG_E(TAG, "Unrecognised pressure type in pod_widgets_redraw_widget");
            app_quit(app);
            return;
    }
    widget_add_icon_element(app->widget,pX+45,pY+2,pressureUnitWidget);
    length = snprintf(NULL,0,"%6.2f",(double)press)+1;//finds num of digits in pressure
    char p[length];//creates pressure for temp
    snprintf(p,length,"%6.2f",(double)press);//stores temp in pressure
    widget_add_string_element(app->widget,pX+9,pY+13,AlignLeft,AlignBottom,FontPrimary,p);
}


void pod_widgets_redraw_temperature_offset(App* app, uint8_t x, uint8_t y) {
    Icon* select = (Icon *)&I_selected_5x17;
    int radius = 0;
    int selection = 0;
    if(app->offsetState->selection == 0)
    {
        radius = 3;
        if(app->offsetState->edit)
        {
            switch (app->offsetState->digit)
            {
                case 3:
                    selection = 13;
                    break;
                case 2:
                    selection = 20;
                    break;
                case 1:
                    selection = 29;
                    break;
                case 0:
                    selection = 35;
                    break;
                default:
                    break;
            }
            widget_add_icon_element(app->widget,x+selection,y+1,select);
        }
    }

    widget_add_frame_element(app->widget,x,y,123,19,radius);  
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* unitWidget = (Icon*)&I_temp_C_11x14;
    widget_add_icon_element(app->widget, x+2, y+3, unitWidget);
    char t[14];//creates string for raw value
    snprintf(t,14,"%+01d.%02d",app->offsetState->rawTemp / 100, abs(app->offsetState->rawTemp % 100));//stores value in string
    widget_add_string_element(app->widget,x+14,y+13,AlignLeft,AlignBottom,FontPrimary,t);
}

void pod_widgets_redraw_humidity_offset(App* app, uint8_t x, uint8_t y) {
    Icon* select = (Icon *)&I_selected_5x17;
    int radius = 0;
    int selection = 0;
    if(app->offsetState->selection == 1)
    {
        radius = 3;
        if(app->offsetState->edit)
        {
            switch (app->offsetState->digit)
            {
                case 3:
                    selection = 12;
                    break;
                case 2:
                    selection = 18;
                    break;
                case 1:
                    selection = 27;
                    break;
                case 0:
                    selection = 33;
                    break;
                default:
                    break;
            }
            widget_add_icon_element(app->widget,x+selection,y+1,select);
        }
    }

    widget_add_frame_element(app->widget,x,y,123,19,radius);  
    Icon* valueWidget = (Icon*) &I_hum_9x15;
    Icon* unitWidget = (Icon*)&I_percent_11x15;
    widget_add_icon_element(app->widget, x+2, y+3, valueWidget);
    widget_add_icon_element(app->widget, x+49, y+3, unitWidget);
    char t[14];//creates string for raw value
    snprintf(t,14,"%+01d.%02d",app->offsetState->rawHum / 100, abs(app->offsetState->rawHum % 100));//stores value in string
    widget_add_string_element(app->widget,x+12,y+13,AlignLeft,AlignBottom,FontPrimary,t);
}

void pod_widgets_redraw_pressure_offset(App* app, uint8_t x, uint8_t y) {
    Icon* select = (Icon *)&I_selected_5x17;
    int radius = 0;
    int selection = 0;
    if(app->offsetState->selection == 2)
    {
        radius = 3;
        if(app->offsetState->edit)
        {
            switch (app->offsetState->digit)
            {
                case 5:
                    selection = 12;
                    break;
                case 4:
                    selection = 18;
                    break;
                case 3:
                    selection = 24;
                    break;
                case 2:
                    selection = 30;
                    break;
                case 1:
                    selection = 39;
                    break;
                case 0:
                    selection = 45;
                    break;
                default:
                    break;
            }
            widget_add_icon_element(app->widget,x+selection,y+1,select);
        }
    }

    widget_add_frame_element(app->widget,x,y,123,19,radius);  
    Icon* valueWidget = (Icon*) &I_pressure_7x13;
    Icon* unitWidget = (Icon*)&I_mbar_15x15;
    widget_add_icon_element(app->widget, x+2, y+3, valueWidget);
    widget_add_icon_element(app->widget, x+59, y+3, unitWidget);
    char t[14];//creates string for raw value
    snprintf(t,14,"%+04d.%02d",app->offsetState->rawPres / 100, abs(app->offsetState->rawPres % 100));//stores value in string
    widget_add_string_element(app->widget,x+12,y+13,AlignLeft,AlignBottom,FontPrimary,t);
}
