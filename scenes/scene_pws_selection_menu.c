#include "../flipper.h"
#include "../app.h"
#include "scenes.h"
#include "scene_pws_selection_menu.h"
#include "../weather_stations/weather_station_history.h"
//#include "../weather_stations/WSHistory.h"
#include "../weather_stations/weather_station.h"
/* main menu scene */

/** main menu callback - sends custom events to the scene manager based on the selection */
void pod_pws_selection_callback(void* context, uint32_t index)
{
    App* app = context;
    scene_manager_handle_custom_event(app->scene_manager,index);
}

void pod_pws_selection_tick_callback(void* context)//every second this is called to read the bme and update the display
{
    App* app = context;
    furi_assert(app->queue);
    FuriMessageQueue* queue = app->queue;
    PwsSelectionEvent event = {.type = PWS_Selection_Tick_Event};
    // It's OK to lose this event if system overloaded (so we don't pass a wait value for 3rd parameter.)
    FURI_LOG_I(TAG,"Updating Message Queue");
    furi_message_queue_put(queue, &event, 0);
    scene_manager_handle_tick_event(app->scene_manager);
}

/** main menu scene - resets the submenu, and gives it content, callbacks and selection enums */
void pod_pws_selection_scene_on_enter(void* context)
{
    FURI_LOG_I("PODS", "PWS Selection Enter");
    App* app = context;
    if(!app->initialization_states->pws_initialized)
    {
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
    app->sub_menu_size = 0; // used to confirm that we're showing all PWS options
    app->clear_mem = true; // if false, we are accessing a PWS. Otherwise, clear memory 
    FuriString* str_buff;
    str_buff = furi_string_alloc();
    WSHistoryItem* item_menu;
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "PWS Selection Menu"); 
    for(size_t i = 0; i < app->pws->txrx->history_size; ++i) {
        //size_t idx = CLAMP((uint16_t)(i + model->list_offset), model->history_item, 0);
        item_menu = WSHistoryItemArray_get(app->pws->txrx->history->history->data, i);
        furi_string_set(str_buff, item_menu->item_str);
        submenu_add_item(app->submenu, furi_string_get_cstr(str_buff), i, pod_pws_selection_callback, app);
        furi_string_reset(str_buff);
        app->sub_menu_size++;
    }
    furi_string_free(str_buff);
    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Submenu_View);

    //Queue for events(Ticks)
    app->queue = furi_message_queue_alloc(8,sizeof(PwsSelectionEvent));

    // Update the screen fairly frequently (every 1000 milliseconds = 1 second.)
    app->timer = furi_timer_alloc(pod_pws_selection_tick_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 1000);
}

void pod_pws_selection_add_new_ws(void* context)
{
    App* app = context;
    FuriString* str_buff;
    str_buff = furi_string_alloc();
    WSHistoryItem* item_menu;
    for(int i = 0; app->sub_menu_size < app->pws->txrx->history_size; app->sub_menu_size++)//Increase Sub Menu Size until it is equal to history size
    {
        i++;
        FURI_LOG_I(TAG,"Sub Menu: %d",app->sub_menu_size);
        item_menu = WSHistoryItemArray_get(app->pws->txrx->history->history->data, app->sub_menu_size);
        furi_string_set(str_buff, item_menu->item_str);
        submenu_add_item(app->submenu, furi_string_get_cstr(str_buff), app->sub_menu_size, pod_pws_selection_callback, app);  
        furi_string_reset(str_buff);  
    }
    furi_string_free(str_buff);
}

/** main menu event handler - switches scene based on the event */
bool pod_pws_selection_scene_on_event(void* context, SceneManagerEvent event)
{
    App* app = context;
    bool consumed = false;
    switch(event.type) {
        case SceneManagerEventTypeTick:
            FURI_LOG_I("Select Page","Scene Manager Event Type Tick");
            PwsSelectionEvent pws_event;
            if(furi_message_queue_get(app->queue, &pws_event, FuriWaitForever) == FuriStatusOk) {
                switch(pws_event.type) {
                    case PWS_Selection_Tick_Event:
                        FURI_LOG_I(TAG,"PWS Selection Tick Event");
                        if(app->sub_menu_size < app->pws->txrx->history_size)
                        {
                            FURI_LOG_I(TAG,"Updating Screen");
                            furi_mutex_acquire(app->mutex, FuriWaitForever);
                            pod_pws_selection_add_new_ws(app);
                            furi_mutex_release(app->mutex);
                            consumed = true;
                        }
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
            FURI_LOG_I("PODS", "Selected Menu Item");
            app->pws->txrx->idx_menu_chosen = event.event;
            //furi_message_queue_free(app->queue);
            //furi_timer_free(app->timer);
            app->clear_mem = false;
            scene_manager_next_scene(app->scene_manager, app->next_pws_scene);//Go to the next Scene, either PWS or POD
            consumed = true;
            break;
        default: // eg. SceneManagerEventTypeBack
            consumed = false;
            break;
    }
    return consumed;
}

/** main menu exit*/
void pod_pws_selection_scene_on_exit(void* context)
{
    FURI_LOG_I("PODS", "PWS Selection Exit");
    App* app = context;
    if(app->clear_mem)//if we are not selecting weather station
    {
        FURI_LOG_I("PODS", "PWS Clear Mem");
        ws_free(app->pws);
        app->initialization_states->pws_initialized = false;
    }
    furi_message_queue_free(app->queue);
    furi_timer_free(app->timer);
    submenu_reset(app->submenu);
}
