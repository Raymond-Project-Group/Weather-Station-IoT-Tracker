#include "../bme280/bme280.h"
#include "../pod.h"
#include "../logger/logger.h"
#include "../unit_conversion/unit_conversion.h"
#include "../weather_stations/weather_station.h"
#include "scene_widgets.h"
#include <applications/services/gui/modules/widget.h>
#include <applications/services/gui/modules/widget_elements/widget_element.h>
#include <applications/services/gui/view.h>

void pod_widgets_redraw_humidity_transmit(App* app,uint8_t hX, uint8_t hY) //Draw Humidity
{   
    Icon* select = (Icon *)&I_selected_5x17;
    int radius = 0;
    int selection = 0;
    if(app->message->selection/10 == 6)
    {
        radius = 3;
        if(app->message->edit)
        {
            switch (app->message->selection%10)
            {
                case 0:
                    selection = 12;
                    break;
                case 1:
                    selection = 18;
                    break;
                case 2:
                    selection = 24;
                    break;
                default:
                    break;
            }
            widget_add_icon_element(app->widget,hX+selection,hY+1,select);
        }
    }
    widget_add_frame_element(app->widget,hX,hY,61,19,radius);  
    //Humidity Icon and Pressure Icon are static, units then get put to the right
    Icon* humidityWidget = (Icon*)&I_hum_9x15;
    widget_add_icon_element(app->widget,hX+2,hY+2,humidityWidget);
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* humidityUnitWidget = (Icon*)&I_percent_11x15;
    widget_add_icon_element(app->widget,hX+49,hY+2,humidityUnitWidget);
    char h[5];//creates string for humidity
    snprintf(h,5,"%03d",app->message->generic->humidity);//stores humidity in string
    widget_add_string_element(app->widget,hX+12,hY+13,AlignLeft,AlignBottom,FontPrimary,h);
}

void pod_widgets_redraw_temperature_transmit(App* app,uint8_t tX, uint8_t tY) //Draw temperature
{   
    Icon* select = (Icon *)&I_selected_5x17;
    int radius = 0;
    int selection = 0;
    if(app->message->selection/10 == 5)
    {
        radius = 3;
        if(app->message->edit)
        {
            switch (app->message->selection%10)
            {
                case 0:
                    selection = 13;
                    break;
                case 1:
                    selection = 20;
                    break;
                case 2:
                    selection = 26;
                    break;
                case 3:
                    selection = 31;
                    break;
                case 4:
                    selection = 40;
                    break;
                case 5:
                    selection = 46;
                    break;
                default:
                    break;
            }
            widget_add_icon_element(app->widget,tX+selection,tY+1,select);
        }
    }
    widget_add_frame_element(app->widget,tX,tY,61,19,radius);  
    //Temperature, Humidity, and Pressure Units can change off settings
    Icon* temperatureUnitWidget = (Icon*)&I_temp_C_11x14;
    widget_add_icon_element(app->widget, tX+2, tY+3, temperatureUnitWidget);
    char t[10];//creates string for temp
    if(app->message->generic->temp>=0){
        snprintf(t,10,"+%06.2f",(double)app->message->generic->temp);//stores temp in string
    }
    else{
        snprintf(t,10,"-%06.2f",(double)app->message->generic->temp*-1);//stores temp in string
    }
    widget_add_string_element(app->widget,tX+14,tY+13,AlignLeft,AlignBottom,FontPrimary,t);
}
void pod_widgets_redraw_protocol(App* app, uint8_t pX, uint8_t pY)//Draw Protocol
{
    Icon* lra = (Icon *)&I_left_right_7x5;
    int radius = 0;
    if(app->message->selection/10 == 0){
        radius = 3;
        if(app->message->edit)
        {
            widget_add_icon_element(app->widget,pX+2,pY+13,lra);
        }
    }
    widget_add_frame_element(app->widget,pX,pY,106,19,radius);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    widget_add_string_element(app->widget,pX+9,pY+13,AlignLeft,AlignBottom,FontPrimary,app->message->generic->protocol_name);

}

void pod_widgets_redraw_id(App* app,uint8_t iX, uint8_t iY) //Draw ID
{
    Icon* lra = (Icon *)&I_left_right_7x5;
    Icon* select = (Icon *)&I_selected_5x17;
    //Icon* la = (Icon *)&I_arrow_left_6x9;
    //Icon* ra = (Icon *)&I_arrow_right_6x9;
    //widget_add_icon_element(app->widget,iX+2,iY+13,da);
    //widget_add_icon_element(app->widget,iX+11,iY+15,la);
    //widget_add_icon_element(app->widget,iX+17,iY+15,ra);

    int radius = 0;
    int selection = 0;
    if(app->message->selection/10 == 2){
        radius = 3;
        if(app->message->edit)
        {
            widget_add_icon_element(app->widget,iX+2,iY+13,lra);
            switch (app->message->selection%10)
            {
                case 0:
                    selection = 23;
                    break;
                case 1:
                    selection = 29;
                    break;
                case 2:
                    selection = 35;
                    break;
                default:
                    break;
            }
            widget_add_icon_element(app->widget,iX+selection,iY+1,select);
        }
    }
    widget_add_frame_element(app->widget,iX,iY,45,19,radius);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* id = (Icon *)&I_id_7x5;//id image
    widget_add_icon_element(app->widget, iX+2, iY+2, id);
    char i[7];
    snprintf(i,7,"0x%03lx",app->message->generic->id);
    widget_add_string_element(app->widget,iX+10,iY+13,AlignLeft,AlignBottom,FontPrimary,i);
}

void pod_widgets_redraw_channel(App* app,uint8_t cX, uint8_t cY) //Draw Channel
{
    int radius = 0;
    int selection = 0;
    Icon* lra = (Icon *)&I_left_right_7x5;
    Icon* select = (Icon *)&I_selected_5x17;
    if(app->message->selection/10 == 3){
        radius = 3;
        if(app->message->edit)
        {
            widget_add_icon_element(app->widget,cX+2,cY+13,lra);
            switch (app->message->selection%10)
            {
                case 0:
                    selection = 10;
                    break;
                case 1:
                    selection = 16;
                    break;
                default:
                    break;
            }
            widget_add_icon_element(app->widget,cX+selection,cY+1,select);
        }
    }
    widget_add_frame_element(app->widget,cX,cY,25,19,radius);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* channel = (Icon *)&I_channel_7x5;//Channel image
    widget_add_icon_element(app->widget, cX+2, cY+2, channel);
    char c[4];
    snprintf(c,4,"%02d",app->message->generic->channel);
    widget_add_string_element(app->widget,cX+10,cY+13,AlignLeft,AlignBottom,FontPrimary,c);
}

void pod_widgets_redraw_frequency(App* app,uint8_t fX, uint8_t fY) //Draw Frequency
{    
    Icon* select = (Icon *)&I_selected_5x17;
    int radius = 0;
    int selection = 0;
    if(app->message->selection/10 == 4)
    {
        radius = 3;
        if(app->message->edit)
        {
            switch (app->message->selection%10)
            {
                case 0:
                    selection = 3;
                    break;
                case 1:
                    selection = 8;
                    break;
                case 2:
                    selection = 14;
                    break;
                case 3:
                    selection = 20;
                    break;
                case 4:
                    selection = 25;
                    break;
                case 5:
                    selection = 32;
                    break;
                case 6:
                    selection = 38;
                    break;
                default:
                    break;
            }
            widget_add_icon_element(app->widget,fX+selection,fY+1,select);
        }
    }
    widget_add_frame_element(app->widget,fX,fY,51,19,radius);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    Icon* frequency = (Icon *)&I_hz_11x13;//frequency image
    widget_add_icon_element(app->widget, fX+38, fY+4, frequency);
    uint32_t freq = app->message->preset->frequency/1000;
    // FURI_LOG_I("HERE","%lu %lu",app->message->preset->frequency,freq);
    char f[9];
    snprintf(f,9,"%lu",freq);
    widget_add_string_element(app->widget,fX+2,fY+13,AlignLeft,AlignBottom,FontPrimary,f);
}

void pod_widgets_redraw_battery(App* app,uint8_t bX, uint8_t bY) //Draw Battery
{ 
    int radius = 0;
    Icon* select = (Icon *)&I_selected_5x17;
    if(app->message->selection/10 == 1){
        radius = 3;
        if(app->message->edit)
        {
            widget_add_icon_element(app->widget,bX+9,bY+1,select);
        }
    }
    widget_add_frame_element(app->widget,bX,bY,15,19,radius);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.Reacher
    Icon* battery = (Icon *)&I_battery_6x12;//Battery image
    widget_add_icon_element(app->widget, bX+2, bY+2, battery);
    char b[2];
    snprintf(b,2,"%d",!app->message->generic->battery_low);
    widget_add_string_element(app->widget,bX+9,bY+13,AlignLeft,AlignBottom,FontPrimary,b);
}

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
    FURI_LOG_E(TAG, "Time Drawn");
    GpsStatus* data = app->gps_uart->status;
    FURI_LOG_E(TAG, "Time Drawn");

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
void pod_widgets_redraw_rssi(App* app,uint8_t rX, uint8_t rY) //Draw rssi
{
    int length;
    float rssi = app->pws->data->rssi;

    widget_add_frame_element(app->widget,rX,rY,61,19,0);  //WE HAVE 126x62 available canvas.  Three 19p boxes(borders one p apart) would take up 60 pixels, 1p off of canvas border would be all 62p.
    //Icon* altitudeWidget = (Icon*)&I_altitude_10x11;
    //widget_add_icon_element(app->widget, rX+2, rY+4, altitudeWidget);
    //Icon* altitudeUnitWidget = (Icon*)&I_m_10x15;
    //widget_add_icon_element(app->widget, rX+50, rY+2, altitudeUnitWidget);
    length = snprintf(NULL,0,"%.1f",(double)rssi)+1;//finds num of digits in rssi
    char t[length];//creates string for rssi
    snprintf(t,length,"%.1f",(double)rssi);//stores rssi in string
    widget_add_string_element(app->widget,rX+14,rY+13,AlignLeft,AlignBottom,FontPrimary,t);
}

void pod_widgets_redraw_temperature(App* app,uint8_t tX, uint8_t tY, int page, PodDeltaState deltaState) //Draw Temperature
{
    int length;
    if(page != Pod_Display_Scene)
    {
        float temp = 0.0/0.0;
        if(page==Pod_Gpio_Display_Scene)
        {
            Bme280Data* data = app->bme280->data;
            temp = data->temperature;
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

        Bme280Data* dataGPIO = app->bme280->data;
        tempGPIO = dataGPIO->temperature;
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
//}
//void pod_widgets_redraw_humidity(App* app,uint8_t hX, uint8_t hY,int page) //Draw Humidity
{
    int length;
    if(page != Pod_Display_Scene)
    {
        float humid = 0.0/0.0;
        float temp = 0.0/0.0;
        if(page==Pod_Gpio_Display_Scene)
        {
            Bme280Data* data = app->bme280->data;
            humid = data->humidity;
            temp = data->temperature;
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

        Bme280Data* dataGPIO = app->bme280->data;
        humidGPIO = dataGPIO->humidity;
        tempGPIO = dataGPIO->temperature;
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
