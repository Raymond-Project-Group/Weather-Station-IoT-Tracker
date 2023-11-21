#include "weather_station.h"
#include "../pod.h"
#include "../logger/logger.h"

static int32_t ws_worker(void* context)
{
    WeatherStationContext* ws = (WeatherStationContext*) context;
    UNUSED(ws);
    while(1)
    {
        uint32_t events = furi_thread_flags_wait(WS_WORKER_ALL_EVENTS, FuriFlagWaitAny, FuriWaitForever);
        furi_check((events & FuriFlagError) == 0);
        if(events & WsWorkerEvtStop) 
        {
            break;
        }
        if(events & WsWorkerEvtTick) 
        {   
            //FURI_LOG_I(TAG,"Increasing PWS Timer Var");
            //ws_block_generic_deserialize(ws->data->generic ,ws_history_get_raw_data(ws->txrx->history,ws->txrx->idx_menu_chosen));//updates data i think?
            continue;
        }
    }
    return 0;
}

static void ws_subghz_general_callback(SubGhzReceiver* receiver,SubGhzProtocolDecoderBase* decoder_base,void* context) //PWS Read 
{
    furi_assert(context);
    WeatherStationContext* ws = context;
    uint8_t ws_hist = ws_history_add_to_history(ws->txrx->history, decoder_base, ws->txrx->preset);
    if(ws_hist == WSHistoryStateAddKeyNewDada) //Is it new?, if so, add it to history
    {
        FURI_LOG_I(TAG,"Added to History");
        ws->txrx->history_size ++;
        ws->txrx->rx_key_state = WSRxKeyStateAddKey;
        
        FlipperFormat* fff = ws_history_get_raw_data(ws->txrx->history,ws->txrx->idx_menu_chosen);//Gets Flipper Format and Raw Data
        flipper_format_rewind(fff);
        flipper_format_read_string(fff, "Protocol", ws->data->protocol_name);//gets protocol name
        
        if(ws_block_generic_deserialize(ws->data->generic , fff) == SubGhzProtocolStatusOk) { //updates data i think?
            FURI_LOG_I(TAG,"Deserialized Data");
            logger_auto_append(ws->parentApp);
            FURI_LOG_I(TAG,"Logged Data");
        } 
    }
    else if(ws_hist == WSHistoryStateAddKeyUpdateData)//old one
    {
        FURI_LOG_I(TAG,"Updated Records");
        if(ws_block_generic_deserialize(ws->data->generic ,ws_history_get_raw_data(ws->txrx->history,ws->txrx->idx_menu_chosen)) == SubGhzProtocolStatusOk) {
            logger_auto_append(ws->parentApp);
        } 
    }

    subghz_receiver_reset(receiver);
}

void ws_update_tickback(void* context)//collect from BME
{
    WeatherStationContext* ws = (WeatherStationContext*) context;
    furi_thread_flags_set(furi_thread_get_id(ws->thread), WsWorkerEvtTick);
}

void ws_preset_init(void* context,const char* preset_name,uint32_t frequency,uint8_t* preset_data,size_t preset_data_size) 
{
    furi_assert(context);
    WeatherStationContext* ws = context;
    furi_string_set(ws->txrx->preset->name, preset_name);
    ws->txrx->preset->frequency = frequency;
    ws->txrx->preset->data = preset_data;
    ws->txrx->preset->data_size = preset_data_size;
}

bool ws_set_preset(WeatherStationContext* ws, const char* preset) 
{
    if(!strcmp(preset, "FuriHalSubGhzPresetOok270Async")) {
        furi_string_set(ws->txrx->preset->name, "AM270");
    } else if(!strcmp(preset, "FuriHalSubGhzPresetOok650Async")) {
        furi_string_set(ws->txrx->preset->name, "AM650");
    } else if(!strcmp(preset, "FuriHalSubGhzPreset2FSKDev238Async")) {
        furi_string_set(ws->txrx->preset->name, "FM238");
    } else if(!strcmp(preset, "FuriHalSubGhzPreset2FSKDev476Async")) {
        furi_string_set(ws->txrx->preset->name, "FM476");
    } else if(!strcmp(preset, "FuriHalSubGhzPresetCustom")) {
        furi_string_set(ws->txrx->preset->name, "CUSTOM");
    } else {
        //FURI_LOG_E(TAG, "Unknown preset");
        return false;
    }
    return true;
}

void ws_get_frequency_modulation(WeatherStationContext* ws,FuriString* frequency,FuriString* modulation) 
{
    furi_assert(ws);
    if(frequency != NULL) {
        furi_string_printf(frequency,"%03ld.%02ld",ws->txrx->preset->frequency / 1000000 % 1000,ws->txrx->preset->frequency / 10000 % 100);
    }
    if(modulation != NULL) {
        furi_string_printf(modulation, "%.2s", furi_string_get_cstr(ws->txrx->preset->name));
    }
}

void ws_begin(WeatherStationContext* ws, uint8_t* preset_data) 
{
    furi_assert(ws);
    UNUSED(preset_data);
    furi_hal_subghz_reset();
    furi_hal_subghz_idle();
    furi_hal_subghz_load_custom_preset(preset_data);
    furi_hal_gpio_init(&gpio_cc1101_g0, GpioModeInput, GpioPullNo, GpioSpeedLow);
    ws->txrx->txrx_state = WSTxRxStateIDLE;
}

uint32_t ws_rx(WeatherStationContext* ws, uint32_t frequency) 
{
    furi_assert(ws);
    if(!furi_hal_subghz_is_frequency_valid(frequency)) {
        furi_crash("WeatherStation: Incorrect RX frequency.");
    }
    furi_assert(ws->txrx->txrx_state != WSTxRxStateRx && ws->txrx->txrx_state != WSTxRxStateSleep);

    furi_hal_subghz_idle();
    uint32_t value = furi_hal_subghz_set_frequency_and_path(frequency);
    furi_hal_gpio_init(&gpio_cc1101_g0, GpioModeInput, GpioPullNo, GpioSpeedLow);
    furi_hal_subghz_flush_rx();
    furi_hal_subghz_rx();

    furi_hal_subghz_start_async_rx(subghz_worker_rx_callback, ws->txrx->worker);
    subghz_worker_start(ws->txrx->worker);
    ws->txrx->txrx_state = WSTxRxStateRx;
    return value;
}

void ws_idle(WeatherStationContext* ws) 
{
    furi_assert(ws);
    furi_assert(ws->txrx->txrx_state != WSTxRxStateSleep);
    furi_hal_subghz_idle();
    ws->txrx->txrx_state = WSTxRxStateIDLE;
}

void ws_rx_end(WeatherStationContext* ws) 
{
    furi_assert(ws);
    furi_assert(ws->txrx->txrx_state == WSTxRxStateRx);
    if(subghz_worker_is_running(ws->txrx->worker)) {
        subghz_worker_stop(ws->txrx->worker);
        furi_hal_subghz_stop_async_rx();
    }
    furi_hal_subghz_idle();
    ws->txrx->txrx_state = WSTxRxStateIDLE;
}

void ws_sleep(WeatherStationContext* ws) 
{
    furi_assert(ws);
    furi_hal_subghz_sleep();
    ws->txrx->txrx_state = WSTxRxStateSleep;
}

WeatherStationContext* ws_init(void* app)
{
    WeatherStationContext* ws = malloc(sizeof(WeatherStationContext));

    ws->parentApp = app;

    //init setting
    ws->setting = subghz_setting_alloc();
    //ToDo FIX  file name setting
    subghz_setting_load(ws->setting, EXT_PATH("subghz/assets/setting_user"));
    
    //init Worker & Protocol & History
    ws->txrx = malloc(sizeof(WeatherStationTxRx));
    ws->txrx->preset = malloc(sizeof(SubGhzRadioPreset));
    ws->txrx->preset->name = furi_string_alloc();
    ws_preset_init(ws, "AM650", subghz_setting_get_default_frequency(ws->setting), NULL, 0);

    ws->txrx->hopper_state = WSHopperStateOFF;
    ws->txrx->history = ws_history_alloc();
    ws->txrx->worker = subghz_worker_alloc();
    ws->txrx->environment = subghz_environment_alloc();
    subghz_environment_set_protocol_registry(ws->txrx->environment, (void*)&weather_station_protocol_registry);//loads protocols from protocol files into environment
    ws->txrx->receiver = subghz_receiver_alloc_init(ws->txrx->environment);
    ws->txrx->idx_menu_chosen = 0;
    ws->txrx->history_size = 0;//start with no weather stations found
    subghz_receiver_set_filter(ws->txrx->receiver, SubGhzProtocolFlag_Decodable);
    subghz_worker_set_overrun_callback(ws->txrx->worker, (SubGhzWorkerOverrunCallback)subghz_receiver_reset);
    subghz_worker_set_pair_callback(ws->txrx->worker, (SubGhzWorkerPairCallback)subghz_receiver_decode);
    subghz_worker_set_context(ws->txrx->worker, ws->txrx->receiver);

    //init data storage
    ws->data = malloc(sizeof(WSReceiverInfoModel));
    ws->data->generic = malloc(sizeof(WSBlockGeneric));
    ws->data->protocol_name = furi_string_alloc();


    // All the subghz examples i found disable charging; so we do too.
    furi_hal_power_suppress_charge_enter();


    ws_begin(ws,subghz_setting_get_preset_data_by_name(ws->setting, furi_string_get_cstr(ws->txrx->preset->name)));
    ws_rx(ws, ws->txrx->preset->frequency);

    //init thread
    ws_init_thread(ws);

    return ws;

}
void ws_free(WeatherStationContext* ws)
{
    //disable it from running
    ws_rx_end(ws);
    ws_sleep(ws);

    //setting
    subghz_setting_free(ws->setting);

    //data storage
    free(ws->data->generic);
    furi_string_free(ws->data->protocol_name);
    free(ws->data);

    //Worker & Protocol & History
    subghz_receiver_free(ws->txrx->receiver);
    subghz_environment_free(ws->txrx->environment);
    ws_history_free(ws->txrx->history);
    subghz_worker_free(ws->txrx->worker);
    furi_string_free(ws->txrx->preset->name);
    free(ws->txrx->preset);
    free(ws->txrx);
    
    //free thread
    ws_deinit_thread(ws);

    free(ws);
    
    // Reenable charging.
    furi_hal_power_suppress_charge_exit();
}
void ws_init_thread(WeatherStationContext* ws)
{
    furi_assert(ws);
    //ws->timer = furi_timer_alloc(ws_update_tickback, FuriTimerTypePeriodic, ws);
    //furi_timer_start(ws->timer, 1000);
    ws->thread = furi_thread_alloc();
    furi_thread_set_name(ws->thread,"WeatherStation_Worker");
    furi_thread_set_stack_size(ws->thread,2048); //I don't know how big the stack size should be
    furi_thread_set_context(ws->thread,ws); //Context is the ws
    furi_thread_set_callback(ws->thread, ws_worker);
    furi_thread_start(ws->thread);
    subghz_receiver_set_rx_callback(ws->txrx->receiver, ws_subghz_general_callback, ws); //callback for thread
}
void ws_deinit_thread(WeatherStationContext* ws)
{
    furi_assert(ws);
    //furi_timer_free(ws->timer);//Stop the timer
    furi_thread_flags_set(furi_thread_get_id(ws->thread), WsWorkerEvtStop);
    furi_thread_join(ws->thread);
    furi_thread_free(ws->thread);
}