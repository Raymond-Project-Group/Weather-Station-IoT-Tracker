#include "../flipper.h"
#include "../app.h"
#include "scenes.h"
#include "scene_pws_transmit_display.h"
#include "../pod.h"
#include "../logger/logger.h"
#include "../unit_conversion/unit_conversion.h"
#include "../weather_stations/ws_transmit.h"
#include <applications/services/gui/modules/widget.h>
#include <applications/services/gui/modules/widget_elements/widget_element.h>
#include <applications/services/gui/view.h>
#include "scene_widgets.h"

void pod_pws_transmit_display_view_redraw_widget(App* app)
{
    FURI_LOG_I(TAG, "Redrawing PWS Transmit View Widgets");
    //int length;
    widget_reset(app->widget);
    widget_add_frame_element(app->widget,0,0,128,64,0); //Flipper screen size is 128x64, this draws a border around it
    //widget_add_frame_element(app->widget,126,app->canvas_y_offset+2,2,20,0);//Scroll Bar

    //Icon* pwsBanner = (Icon*)&I_pws_display_banner_90x15;//Banner
    //widget_add_icon_element(app->widget,18,5,pwsBanner);

    //widget_add_string_element(app->widget, 63, 3, AlignCenter, AlignTop, FontPrimary, app->message->generic->protocol_name);

    /*length = snprintf(NULL,0,"0x%lx",app->message->generic->id)+1;//finds num of digits 
    char id[length];//creates string var
    snprintf(id,length,"0x%lx",app->message->generic->id);
    widget_add_string_element(app->widget, 3, 13, AlignLeft, AlignTop, FontPrimary, "ID: ");
    widget_add_string_element(app->widget, 63, 13, AlignLeft, AlignTop, FontPrimary, id);

    length = snprintf(NULL,0,"%2d",app->message->generic->channel)+1;//finds num of digits 
    char channel[length];//creates string var
    snprintf(channel,length,"%2d",app->message->generic->channel);
    widget_add_string_element(app->widget, 3, 23, AlignLeft, AlignTop, FontPrimary, "Channel: ");
    widget_add_string_element(app->widget, 63, 23, AlignLeft, AlignTop, FontPrimary, channel);

   
    length = snprintf(NULL,0,"%d",app->message->generic->battery_low)+1;//finds num of digits 
    char battery_low[length];//creates string var
    snprintf(battery_low,length,"%d",app->message->generic->battery_low);
    widget_add_string_element(app->widget, 3, 33, AlignLeft, AlignTop, FontPrimary, "Low Volt: ");
    widget_add_string_element(app->widget, 63, 33, AlignLeft, AlignTop, FontPrimary, battery_low);

    length = snprintf(NULL,0,"%6.2f",(double)app->message->generic->temp)+1;//finds num of digits 
    char temp[length];//creates string var
    snprintf(temp,length,"%6.2f",(double)app->message->generic->temp);
    widget_add_string_element(app->widget, 3, 43, AlignLeft, AlignTop, FontPrimary, "Temp(C): ");
    widget_add_string_element(app->widget, 63, 43, AlignLeft, AlignTop, FontPrimary, temp);

    length = snprintf(NULL,0,"%3d",app->message->generic->humidity)+1;//finds num of digits 
    char humid[length];//creates string var
    snprintf(humid,length,"%3d",app->message->generic->humidity);
    widget_add_string_element(app->widget, 3, 53, AlignLeft, AlignTop, FontPrimary, "Humid(%): ");
    widget_add_string_element(app->widget, 63, 53, AlignLeft, AlignTop, FontPrimary, humid);*/

    /*Icon* arrowUp = (Icon*)&I_arrow_up_9x6;
    Icon* arrowDown = (Icon*)&I_arrow_down_9x6;
    Icon* arrowLeft = (Icon*)&I_arrow_left_6x9;
    Icon* arrowRight = (Icon*)&I_arrow_right_6x9;

    Icon* arrowUpInv = (Icon*)&I_arrow_up_inv_9x6;
    Icon* arrowDownInv = (Icon*)&I_arrow_down_inv_9x6;
    Icon* arrowLeftInv = (Icon*)&I_arrow_left_inv_6x9;
    Icon* arrowRightInv = (Icon*)&I_arrow_right_inv_6x9;

    widget_add_icon_element(app->widget,3,3,arrowLeft);
    widget_add_icon_element(app->widget,118,3,arrowRight);
    widget_add_icon_element(app->widget,3,30,arrowUp);
    widget_add_icon_element(app->widget,118,30,arrowDown);
    switch(app->canvas_y_offset == 01)
    {
        case 00://Protocol Left Selected
            widget_add_icon_element(app->widget,3,3,arrowLeftInv);
            break;
        case 01:
            widget_add_icon_element(app->widget,118,3,arrowRightInv);
            break;
        case 10:

    }*/
    uint8_t protX = 2;
    uint8_t protY = 3;
    uint8_t idX = 2;
    uint8_t idY = 23;
    uint8_t chanX = 48;
    uint8_t chanY = 23;
    uint8_t freqX = 74;
    uint8_t freqY = 23;
    uint8_t battX = 110;
    uint8_t battY = 3;
    uint8_t tempX = 2;
    uint8_t tempY = 43;
    uint8_t humX = 64;
    uint8_t humY = 43;
    if (protY > app->canvas_y_offset)//Should you draw temp?
    {
        pod_widgets_redraw_protocol(app,protX,protY - app->canvas_y_offset);
    }
    if (tempY > app->canvas_y_offset)//Should you draw temp?
    {
        pod_widgets_redraw_temperature_transmit(app,tempX,tempY - app->canvas_y_offset);
    }
    if (humY > app->canvas_y_offset)//Should you draw humidity?
    {
        pod_widgets_redraw_humidity_transmit(app,humX,humY - app->canvas_y_offset);
    }
    if (idY > app->canvas_y_offset)//Should you draw ID?
    {
        pod_widgets_redraw_id(app,idX,idY - app->canvas_y_offset);
    }
    if (chanY > app->canvas_y_offset)//Should you draw Channel?
    {
        pod_widgets_redraw_channel(app,chanX,chanY - app->canvas_y_offset);
    }
    if (battY > app->canvas_y_offset)//Should you draw Battery?
    {
        pod_widgets_redraw_battery(app,battX,battY - app->canvas_y_offset);
    }
    if (freqY > app->canvas_y_offset)//Should you draw Frequency?
    {
        pod_widgets_redraw_frequency(app,freqX,freqY - app->canvas_y_offset);
    }
}
static bool pod_pws_transmit_display_input_callback(InputEvent* input_event, void*context)//called when button is pressed
{
    App* app = context;
    bool consumed = false;
    //furi_assert(app->queue);
    
    //Has to be declared up here else error thrown
    size_t i = 0;
    const char* protocol_name;
    //const SubGhzProtocol* protocol;
    //const SubGhzProtocolRegistry* protocol_registry_items = subghz_environment_get_protocol_registry(app->message->environment);
    PwsTransmitDisplayEvent event = {.type = PWS_Transmit_Display_Key_Event, .input = *input_event};
    switch(event.input.type)
    {
        case InputTypeLong:
            if(event.input.key==InputKeyOk)
            {
                ws_transmit_message(app->message);
                view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                consumed=true;//we need to de-highlight selection
            }
            break;
        case InputTypeShort:
            if(app->message->edit)//We have already selected what to edit
            {
                if(event.input.key==InputKeyBack || event.input.key==InputKeyOk)
                {
                    app->message->edit=false;
                    view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                    consumed=true;//we need to de-highlight selection
                    break;
                }
                switch((app->message->selection/10))
                {
                    case 0://Protocols
                        protocol_name = subghz_environment_get_protocol_name_registry(app->message->environment, i++);
                        FURI_LOG_I(TAG,app->message->generic->protocol_name);
                        while(protocol_name!=NULL && strcmp(protocol_name,app->message->generic->protocol_name))//While protocol name is not NULL and we havent found the message->generic->protocol_name  
                        {
                            FURI_LOG_I(TAG,protocol_name);
                            protocol_name = subghz_environment_get_protocol_name_registry(app->message->environment, i++);
                        }
                        if(protocol_name==NULL)//This shouldn't happen, the protocol name should always be in the registry
                        {
                            FURI_LOG_E(TAG,"Protocol Name Not Found in Registry");
                            return false;
                        }
                        FURI_LOG_I(TAG,"%u %u",app->message->protocol_reg_length,(uint16_t)i);
                        switch(event.input.key)
                        {
                            case InputKeyLeft:
                                if(i == 1)
                                {
                                    app->message->generic->protocol_name = subghz_environment_get_protocol_name_registry(app->message->environment, (size_t)(app->message->protocol_reg_length - 1));//set equal to last protocol in list
                                }
                                else
                                {
                                    app->message->generic->protocol_name = subghz_environment_get_protocol_name_registry(app->message->environment, i-2);//shifts to protocol left one
                                }
                                break;
                            case InputKeyRight:
                                if(i == (size_t)(app->message->protocol_reg_length-1))
                                {
                                    app->message->generic->protocol_name = subghz_environment_get_protocol_name_registry(app->message->environment, 0);//set equal to first protocol in list
                                }
                                else
                                {
                                    app->message->generic->protocol_name = subghz_environment_get_protocol_name_registry(app->message->environment, i);//shifts to protocol right one
                                }
                                break;
                            default:
                                break;
                        }
                        consumed = true;
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        break;
                    case 1: //Battery
                        switch(event.input.key)
                        {
                            case InputKeyUp:
                                app->message->generic->battery_low = !app->message->generic->battery_low;
                                break;
                            case InputKeyDown:
                                app->message->generic->battery_low = !app->message->generic->battery_low;
                                break;
                            default:
                                break;
                        }
                        break;
                    case 2://ID
                        switch(event.input.key)
                        {
                            case InputKeyLeft:
                                if(app->message->selection > 20)
                                {
                                    app->message->selection--;//decrease selection/move cursor over to the left
                                }
                                else
                                {
                                    app->message->selection = 22;//wrap around
                                }
                                break;
                            case InputKeyRight:
                                if(app->message->selection < 22)
                                {
                                    app->message->selection++;//increase selection/move cursor over to the right
                                }
                                else
                                {
                                    app->message->selection = 20;//wrap around
                                }
                                break;
                            case InputKeyUp:
                                switch(app->message->selection)
                                {
                                    case 20://increase first digit
                                        if((app->message->generic->id & 0xF00) == 0xF00) 
                                            app->message->generic->id = app->message->generic->id & 0x0FF; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->id = (app->message->generic->id & 0x0FF) | ((app->message->generic->id & 0xF00) + 0x100);
                                        break;
                                    case 21://increase second digit
                                        if((app->message->generic->id & 0xF0) == 0xF0) 
                                            app->message->generic->id = app->message->generic->id & 0xF0F; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->id = (app->message->generic->id & 0xF0F) | ((app->message->generic->id & 0xF0) + 0x10);
                                        break;
                                    case 22://increase third digit
                                        if((app->message->generic->id & 0xF) == 0xF) 
                                            app->message->generic->id = app->message->generic->id & 0xFF0; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->id = (app->message->generic->id & 0xFF0) | ((app->message->generic->id & 0xF) + 1);
                                        break;
                                    default:
                                        FURI_LOG_I(TAG,"ID Increase Transmit Message Builder Error");
                                        break;
                                }
                                break;
                            case InputKeyDown:
                                switch(app->message->selection)
                                {
                                    case 20://Decrease first digit
                                        if((app->message->generic->id & 0xF00) == 0x0) 
                                            app->message->generic->id = app->message->generic->id | 0xF00; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->id = (app->message->generic->id & 0x0FF) | ((app->message->generic->id & 0xF00) - 0x100);
                                        break;
                                    case 21://Decrease second digit
                                        if((app->message->generic->id & 0xF0) == 0x0) 
                                            app->message->generic->id = app->message->generic->id | 0xF0; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->id = (app->message->generic->id & 0xF0F) | ((app->message->generic->id & 0xF0) - 0x10);
                                        break;
                                    case 22://Decrease third digit
                                        if((app->message->generic->id & 0xF) == 0x0)
                                            app->message->generic->id = app->message->generic->id | 0xF; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->id = (app->message->generic->id & 0xFF0) | ((app->message->generic->id & 0xF) - 1);
                                        break;
                                    default:
                                        FURI_LOG_I(TAG,"ID Decrease Transmit Message Builder Error");
                                        break;
                                }
                                break;
                            default:
                                break;
                        }
                        consumed = true;
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        break;
                    case 3: //Channel
                        switch(event.input.key)
                        {
                            case InputKeyLeft:
                                if(app->message->selection > 30)
                                {
                                    app->message->selection--;//decrease selection/move cursor over to the left
                                }
                                else
                                {
                                    app->message->selection = 31;//wrap around
                                }
                                break;
                            case InputKeyRight:
                                if(app->message->selection < 31)
                                {
                                    app->message->selection++;//increase selection/move cursor over to the right
                                }
                                else
                                {
                                    app->message->selection = 30;//wrap around
                                }
                                break;
                            case InputKeyUp:
                                switch(app->message->selection)
                                {
                                    case 30://increase first digit
                                        if(app->message->generic->channel >= 10) 
                                            app->message->generic->channel -= 10; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->channel += 10;
                                        break;
                                    case 31://increase second digit
                                        if(app->message->generic->channel == 19 || app->message->generic->channel  == 9 ) 
                                            app->message->generic->channel -= 9; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->channel += 1;
                                        break;
                                    default:
                                        FURI_LOG_I(TAG,"CH Increase Transmit Message Builder Error");
                                        break;
                                }
                                break;
                            case InputKeyDown:
                                switch(app->message->selection)
                                {
                                    case 30://Decrease first digit
                                        if(app->message->generic->channel < 10) 
                                            app->message->generic->channel += 10; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->channel -= 10;
                                        break;
                                    case 31://Decrease second digit
                                        if(app->message->generic->channel == 0 || app->message->generic->channel == 10) 
                                            app->message->generic->channel += 9; //it's annoying we cant make delcartions in switch statements in c
                                        else
                                            app->message->generic->channel -= 1;
                                        break;
                                    default:
                                        FURI_LOG_I(TAG,"CH Decrease Transmit Message Builder Error");
                                        break;
                                }
                                break;
                            default:
                                break;
                        }
                        consumed = true;
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        break;
                    
                    case 4: //Frequency
                        //protocol = subghz_protocol_registry_get_by_name(protocol_registry_items,app->message->generic->protocol_name);
                        switch(event.input.key)
                        {
                            case InputKeyLeft:
                                if(app->message->selection%10==0)
                                {
                                    app->message->selection+=5;
                                }
                                else
                                {
                                    app->message->selection--;
                                }
                                break;
                            case InputKeyRight:
                                if(app->message->selection%10==5)
                                {
                                    app->message->selection-=5;
                                }
                                else
                                {
                                    app->message->selection++;
                                }
                                break;
                            case InputKeyUp:
                                switch(app->message->selection%10)
                                {
                                    case 0:
                                        switch(app->message->preset->frequency/100000000)//first digit can be 3,4,9
                                        {
                                            case 3://up to 4
                                                app->message->preset->frequency += 100000000;
                                                break;
                                            case 4://up to 9
                                                app->message->preset->frequency += 500000000;
                                                break;
                                            case 9://loop back to 3
                                                app->message->preset->frequency -= 600000000;
                                                break;
                                            default:
                                                FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;
                                    case 1:
                                        switch(app->message->preset->frequency/10000000-((app->message->preset->frequency/100000000)*10))//second digit can be 0-4
                                        {
                                            case 4://loop back to 4
                                                app->message->preset->frequency -= 40000000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/10000000-((app->message->preset->frequency/100000000)*10)< 4) //add 1
                                                    app->message->preset->frequency += 10000000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;  
                                    case 2:
                                        //FURI_LOG_I(TAG,"%lu",app->message->preset->frequency/1000000-((app->message->preset->frequency/10000000)*10));
                                        switch(app->message->preset->frequency/1000000-(app->message->preset->frequency/10000000)*10)//third digit
                                        {
                                            case 9://loop back to 0
                                                app->message->preset->frequency -= 9000000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/1000000-((app->message->preset->frequency/10000000)*10) < 9) //add 1
                                                    app->message->preset->frequency += 1000000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;
                                    case 3:
                                        switch(app->message->preset->frequency/100000-(app->message->preset->frequency/1000000)*10)//fourth digit
                                        {
                                            case 9://loop back to 0
                                                app->message->preset->frequency -= 900000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/100000-(app->message->preset->frequency/1000000)*10 < 9) //add 1
                                                    app->message->preset->frequency += 100000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;   
                                    case 4:
                                        switch(app->message->preset->frequency/10000-(app->message->preset->frequency/100000)*10)//fifth digit
                                        {
                                            case 9://loop back to 0
                                                app->message->preset->frequency -= 90000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/10000-(app->message->preset->frequency/100000)*10 < 9) //add 1
                                                    app->message->preset->frequency += 10000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break; 
                                    case 5:
                                        switch(app->message->preset->frequency/1000-(app->message->preset->frequency/10000)*10)//sixth digit
                                        {
                                            case 9://loop back to 0
                                                app->message->preset->frequency -= 9000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/1000-(app->message->preset->frequency/10000)*10 < 9) //add 1
                                                    app->message->preset->frequency += 1000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;
                                    default:
                                        FURI_LOG_E(TAG,"Frequency Builder No Digit Error");
                                        break;                                            
                                }
                                break;
                            case InputKeyDown:
                                switch(app->message->selection%10)
                                {
                                    case 0:
                                        switch(app->message->preset->frequency/100000000)//first digit can be 3,4,9
                                        {
                                            case 3://loop back to 9
                                                app->message->preset->frequency += 600000000;
                                                break;
                                            case 4://back to 3
                                                app->message->preset->frequency -= 100000000;
                                                break;
                                            case 9:
                                                app->message->preset->frequency -= 500000000;
                                                break;
                                            default:
                                                FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;
                                    case 1:
                                        switch(app->message->preset->frequency/10000000-((app->message->preset->frequency/100000000)*10))//second digit can be 0-4
                                        {
                                            case 0://loop back to 4
                                                app->message->preset->frequency += 40000000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/10000000-((app->message->preset->frequency/100000000)*10) <= 4) //subtract 1
                                                    app->message->preset->frequency -= 10000000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;    
                                    case 2:
                                        switch(app->message->preset->frequency/1000000-(app->message->preset->frequency/10000000)*10)//third digit
                                        {
                                            case 0://loop back to 9
                                                app->message->preset->frequency += 9000000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/100000-(app->message->preset->frequency/10000000)*10 > 0) //subtract 1
                                                    app->message->preset->frequency -= 1000000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;
                                    case 3:
                                        switch(app->message->preset->frequency/100000-(app->message->preset->frequency/1000000)*10)//fourth digit
                                        {
                                            case 0://loop back to 9
                                                app->message->preset->frequency += 900000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/100000-(app->message->preset->frequency/1000000)*10 > 0) //subtract 1
                                                    app->message->preset->frequency -= 100000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;   
                                    case 4:
                                        switch(app->message->preset->frequency/10000-(app->message->preset->frequency/100000)*10)//fifth digit
                                        {
                                            case 0://loop back to 9
                                                app->message->preset->frequency += 90000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/10000-(app->message->preset->frequency/100000)*10 > 0) //subtract 1
                                                    app->message->preset->frequency -= 10000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break; 
                                    case 5:
                                        switch(app->message->preset->frequency/1000-(app->message->preset->frequency/10000)*10)//sixth digit
                                        {
                                            case 0://loop back to 9
                                                app->message->preset->frequency += 9000;
                                                break;
                                            default:
                                                if(app->message->preset->frequency/1000 - (app->message->preset->frequency/10000)*10> 0) //subtract 1
                                                    app->message->preset->frequency -= 1000;
                                                else
                                                    FURI_LOG_E(TAG,"Frequency Builder Error");
                                                break;
                                        }
                                        break;
                                    default:
                                        FURI_LOG_E(TAG,"Frequency Builder No Digit Error");
                                        break;                         
                                }
                                break;
                            default:
                                break;
                        }
                        consumed = true;
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        break;
                    case 5: //Temperature
                        //protocol = subghz_protocol_registry_get_by_name(protocol_registry_items,app->message->generic->protocol_name);
                        switch(event.input.key)
                        {
                            case InputKeyLeft:
                                if(app->message->selection%10==0)
                                {
                                    app->message->selection+=5;
                                }
                                else
                                {
                                    app->message->selection--;
                                }
                                break;
                            case InputKeyRight:
                                if(app->message->selection%10==5)
                                {
                                    app->message->selection-=5;
                                }
                                else
                                {
                                    app->message->selection++;
                                }
                                break;
                            case InputKeyUp:
                                switch(app->message->selection%10)
                                {
                                    case 0:
                                        app->message->generic->temp*=-1;
                                        break;
                                    case 1:
                                        if(app->message->generic->temp>=0)//If not negative, first digit
                                        {
                                            if(app->message->generic->temp<900)
                                                app->message->generic->temp+=100;
                                            else
                                                app->message->generic->temp-=900;
                                        }
                                        else
                                        {
                                            if(app->message->generic->temp>-900)
                                                app->message->generic->temp-=100;
                                            else
                                                app->message->generic->temp+=900;
                                        }
                                        break;  
                                    case 2:
                                        if(app->message->generic->temp>=0)//If not negative, second digit
                                        {
                                            if((int)(app->message->generic->temp)%100<90)
                                                app->message->generic->temp+=10;
                                            else
                                                app->message->generic->temp-=90;
                                        }
                                        else
                                        {
                                            if((int)(app->message->generic->temp)%100>-90)
                                                app->message->generic->temp-=10;
                                            else
                                                app->message->generic->temp+=90;
                                        }
                                        break;  
                                    case 3:
                                        if(app->message->generic->temp>=0)//If not negative, third digit
                                        {
                                            if((int)(app->message->generic->temp)%10<9)
                                                app->message->generic->temp+=1;
                                            else
                                                app->message->generic->temp-=9;
                                        }
                                        else
                                        {
                                            if((int)(app->message->generic->temp)%10>-9)
                                                app->message->generic->temp-=1;
                                            else
                                                app->message->generic->temp+=9;
                                        }
                                        break;
                                    case 4:
                                        if(app->message->generic->temp>=0)//If not negative, fourth digit
                                        {
                                            if((int)(app->message->generic->temp*10)%10<9)
                                                app->message->generic->temp+=.1;
                                            else
                                                app->message->generic->temp-=.9;
                                        }
                                        else
                                        {
                                            if((int)(app->message->generic->temp*10)%10!=-9)
                                                app->message->generic->temp-=.1;
                                            else
                                                app->message->generic->temp+=.9;
                                        }
                                        break;
                                    case 5:
                                        if(app->message->generic->temp>=0)//If not negative, fifth digit
                                        {
                                            if((int)(app->message->generic->temp*100)%10<9)
                                                app->message->generic->temp+=.01;
                                            else
                                                app->message->generic->temp-=.09;
                                        }
                                        else
                                        {
                                            if((int)(app->message->generic->temp*100)%10!=-9)
                                                app->message->generic->temp-=.01;
                                            else
                                                app->message->generic->temp+=.09;
                                        }
                                        break;
                                    default:
                                        FURI_LOG_E(TAG,"Temp Builder No Digit Error");
                                        break;                                            
                                }
                                break;
                            case InputKeyDown:
                                switch(app->message->selection%10)
                                {
                                    case 0:
                                        app->message->generic->temp*=-1;
                                        break;
                                    case 1:
                                        if(app->message->generic->temp>=0)//If not negative, first digit
                                        {
                                            if(app->message->generic->temp>99)
                                                app->message->generic->temp-=100;
                                            else
                                                app->message->generic->temp+=900;
                                        }
                                        else
                                        {
                                            if(app->message->generic->temp<-99)
                                                app->message->generic->temp+=100;
                                            else
                                                app->message->generic->temp-=900;
                                        }
                                        break;  
                                    case 2:
                                        if(app->message->generic->temp>=0)//If not negative, second digit
                                        {
                                            if((int)(app->message->generic->temp)%100>9)
                                                app->message->generic->temp-=10;
                                            else
                                                app->message->generic->temp+=90;
                                        }
                                        else
                                        {
                                            if((int)(app->message->generic->temp)%100<-9)
                                                app->message->generic->temp+=10;
                                            else
                                                app->message->generic->temp-=90;
                                        }
                                        break;  
                                    case 3:
                                        if(app->message->generic->temp>=0)//If not negative, third digit
                                        {
                                            if((int)(app->message->generic->temp)%10>0)
                                                app->message->generic->temp-=1;
                                            else
                                                app->message->generic->temp+=9;
                                        }
                                        else
                                        {
                                            if((int)(app->message->generic->temp)%10<0)
                                                app->message->generic->temp+=1;
                                            else
                                                app->message->generic->temp-=9;
                                        }
                                        break;
                                    case 4:
                                        if(app->message->generic->temp>=0)//If not negative, fourth digit
                                        {
                                            if((int)(app->message->generic->temp*10)%10>0)
                                                app->message->generic->temp-=.1;
                                            else
                                                app->message->generic->temp+=.9;
                                        }
                                        else
                                        {
                                            if((int)(app->message->generic->temp*10)%10<0)
                                                app->message->generic->temp+=.1;
                                            else
                                                app->message->generic->temp-=.9;
                                        }
                                        break;
                                    case 5:
                                        if(app->message->generic->temp>=0)//If not negative, fifth digit
                                        {
                                            if((int)(app->message->generic->temp*100)%10>0)
                                                app->message->generic->temp-=.01;
                                            else
                                                app->message->generic->temp+=.09;
                                        }
                                        else
                                        {
                                            if((int)(app->message->generic->temp*100)%10<0)
                                                app->message->generic->temp+=.01;
                                            else
                                                app->message->generic->temp-=.09;
                                        }
                                        break;
                                    default:
                                        FURI_LOG_E(TAG,"Temp Builder No Digit Error");
                                        break;  
                                }
                                break;
                            default:
                                break;
                        }
                        consumed = true;
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        break;
                    
                    case 6: //Humidity
                        //protocol = subghz_protocol_registry_get_by_name(protocol_registry_items,app->message->generic->protocol_name);
                        switch(event.input.key)
                        {
                            case InputKeyLeft:
                                if(app->message->selection%10==0)
                                {
                                    app->message->selection+=2;
                                }
                                else
                                {
                                    app->message->selection--;
                                }
                                break;
                            case InputKeyRight:
                                if(app->message->selection%10==5)
                                {
                                    app->message->selection-=2;
                                }
                                else
                                {
                                    app->message->selection++;
                                }
                                break;
                            case InputKeyUp:
                                switch(app->message->selection%10)
                                {
                                    case 0:
                                        if(app->message->generic->humidity>=100){
                                            app->message->generic->humidity-=100;
                                        }
                                        else{
                                            app->message->generic->humidity+=100;
                                        }
                                        break;
                                    case 1:
                                        if(app->message->generic->humidity%100<90)
                                            app->message->generic->humidity+=10;
                                        else
                                            app->message->generic->humidity-=90;
                                        break;  
                                    case 2:
                                        if(app->message->generic->humidity%10<9)
                                            app->message->generic->humidity+=1;
                                        else
                                            app->message->generic->humidity-=9;
                                        break;  
                                    default:
                                        FURI_LOG_E(TAG,"Temp Builder No Digit Error");
                                        break;                                            
                                }
                                break;
                            case InputKeyDown:
                                switch(app->message->selection%10)
                                {
                                    case 0:
                                        if(app->message->generic->humidity>=100){
                                            app->message->generic->humidity-=100;
                                        }
                                        else{
                                            app->message->generic->humidity+=100;
                                        }
                                        break;
                                    case 1:
                                        if(app->message->generic->humidity%100>=10)
                                            app->message->generic->humidity-=10;
                                        else
                                            app->message->generic->humidity+=90;
                                        break;  
                                    case 2:
                                        if(app->message->generic->humidity%10>0)
                                            app->message->generic->humidity-=1;
                                        else
                                            app->message->generic->humidity+=9;
                                        break;  
                                    default:
                                        FURI_LOG_E(TAG,"Temp Builder No Digit Error");
                                        break;    
                                }
                                break;
                            default:
                                break;
                        }
                        consumed = true;
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                switch(event.input.key)
                {
                    case InputKeyBack:
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Exit_Event);
                        consumed = true;
                        break;
                    case InputKeyOk:
                        app->message->edit=true;
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        consumed = true;
                        break;
                    case InputKeyLeft:
                        if(app->message->selection >= 10)//Shift Left to the Rounded to the 10's
                        {
                            app->message->selection = ((app->message->selection/10) - 1) * 10;
                        }
                        else
                        {
                            app->message->selection = 60; //Last selection(Humidity)
                        }
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        consumed = true;
                        break;
                    case InputKeyRight:
                        if(app->message->selection < 60)//Shift Right Rounded to the 10's
                        {
                            app->message->selection = ((app->message->selection/10) + 1) * 10;
                        }
                        else
                        {
                            app->message->selection = 0; //First selection(Protocols)
                        }
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        consumed = true;
                        break;
                    case InputKeyUp:
                        if(app->message->selection < 20)//Loop
                        {
                            app->message->selection = 60;
                        }
                        else if(app->message->selection < 40)//Shift Up to 0(Protocol)
                        {
                            app->message->selection = 00;
                        }
                        else if(app->message->selection < 50)//Shift Up to 10(Battery)
                        {
                            app->message->selection = 10;
                        }
                        else if(app->message->selection < 60)//Shift Up to 20(ID)
                        {
                            app->message->selection = 20;
                        }
                        else{
                            app->message->selection = 30;//Shift Up to 30(Channel)
                        }
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        consumed = true;
                        break;
                    case InputKeyDown:
                        if(app->message->selection >= 50)//Loop
                        {
                            app->message->selection = 0;
                        }
                        else if(app->message->selection >= 40)
                        {
                            app->message->selection = 60; //Last selection(Humidity)
                        }
                        else if(app->message->selection >= 20)
                        {
                            app->message->selection = 50; //Shift Down to 50(Temp)
                        }
                        else if(app->message->selection >= 10)
                        {
                            app->message->selection = 40; //Shift Down to 30(Freq)
                        }
                        else{
                            app->message->selection = 20; //Shift down to 20(ID)
                        }
                        
                        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Input_Event);
                        consumed = true;
                        break;
                    default:
                        break;

                }
            }
            break;
        default:
            break;
    }
    /*if(event.input.type == InputTypeShort && event.input.key == InputKeyBack) {
        view_dispatcher_send_custom_event(app->view_dispatcher,PWS_Transmit_Display_Exit_Event);
        consumed = true;
    }
    else if (event.input.type == InputTypeShort && event.input.key == InputKeyOk) {
        view_dispatcher_send_custom_event(app->view_dispatcher, PWS_Transmit_Display_Log_Event);
        consumed = false;
    }*/
    //furi_message_queue_put(app->queue, &event, FuriWaitForever);
    return consumed;
    //return false;
}

void pod_pws_transmit_display_tick_callback(void* context)//every second this is called to read the bme and update the display
{
    App* app = context;
    furi_assert(app->queue);
    FuriMessageQueue* queue = app->queue;
    PwsTransmitDisplayEvent event = {.type = PWS_Transmit_Display_Tick_Event};
    // It's OK to lose this event if system overloaded (so we don't pass a wait value for 3rd parameter.)
    FURI_LOG_I(TAG,"Updating Message Queue");
    furi_message_queue_put(queue, &event, 0);
    scene_manager_handle_tick_event(app->scene_manager);
}

void pod_pws_transmit_display_scene_on_enter(void* context)
{
    FURI_LOG_I(TAG, "PWS Transmit Display Scene entered");
    App* app = context;
    //app->gps_uart = gps_uart_enable();

    widget_reset(app->widget);
    app->canvas_y_offset = 0;

    app->message = message_init();
    //Queue for events(Ticks or input)
    app->queue = furi_message_queue_alloc(8,sizeof(PwsTransmitDisplayEvent));

    pod_pws_transmit_display_view_redraw_widget(app);
    view_set_context(widget_get_view(app->widget), app);
    //view_set_draw_callback(widget_get_view(app->widget),pod_pws_transmit_display_render_callback);
    view_set_input_callback(widget_get_view(app->widget),pod_pws_transmit_display_input_callback);
    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Widget_View);

    // Update the screen fairly frequently (every 1000 milliseconds = 1 second.)
    //app->timer = furi_timer_alloc(pod_pws_transmit_display_tick_callback, FuriTimerTypePeriodic, app);
    //furi_timer_start(app->timer, 1000);
}

bool pod_pws_transmit_display_scene_on_event(void* context, SceneManagerEvent event)
{
    App* app = context;
    bool consumed = false;
    //UNUSED(app);
    //UNUSED(event);
    switch(event.type){
        case SceneManagerEventTypeTick:
            FURI_LOG_I(TAG,"Scene Manager Event Type Tick");
            PwsTransmitDisplayEvent pws_transmit_event;
            if(furi_message_queue_get(app->queue, &pws_transmit_event, FuriWaitForever) == FuriStatusOk) {
                switch(pws_transmit_event.type) {
                    case PWS_Transmit_Display_Tick_Event:
                        FURI_LOG_I(TAG,"PWS Tick Event");
                        FURI_LOG_I(TAG,"Updating Screen");
                        furi_mutex_acquire(app->mutex, FuriWaitForever);
                        pod_pws_transmit_display_view_redraw_widget(app);//redraw widgets
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
                case PWS_Transmit_Display_Exit_Event:
                    //scene_manager_next_scene(app->scene_manager,Pod_Main_Menu_Scene);
                    FURI_LOG_I(TAG,"PWS Exit Event");
                    consumed = true;
                    scene_manager_previous_scene(app->scene_manager);
                    break;
                case PWS_Transmit_Display_Input_Event:
                    pod_pws_transmit_display_view_redraw_widget(app);//redraw widgets
                    consumed = true;
                    break;
                case PWS_Transmit_Display_Log_Event:
                    FURI_LOG_I(TAG,"Send Message");
                    Message* message = message_test();
                    if(ws_transmit_message(message)){
                        FURI_LOG_I(TAG,"Pog???  0x%llX",message->generic->data);
                    }
                    message_free(message);
                    consumed = false;
                    break;
            }
            break;
        default:
            break;
    }
    return consumed;
}

void pod_pws_transmit_display_scene_on_exit(void* context)
{
    FURI_LOG_I(TAG, "Exiting PWS Display Scene");
    App* app = context;
    furi_message_queue_free(app->queue);
    message_free(app->message);
    //furi_timer_free(app->timer);
    widget_reset(app->widget);
    //gps_uart_disable(app->gps_uart);
}