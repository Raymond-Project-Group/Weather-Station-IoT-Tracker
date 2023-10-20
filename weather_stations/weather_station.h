#ifndef WEATHER_STATION_H
#define WEATHER_STATION_H
#include "../flipper.h"
#include "helpers/weather_station_event.h"
#include "helpers/weather_station_types.h"
#include "protocols/ws_generic.h"
#include "protocols/protocol_items.h"
#include "weather_station_history.h"
#include <lib/subghz/subghz_setting.h>
#include <lib/subghz/subghz_worker.h>
#include <lib/subghz/receiver.h>
#include <lib/subghz/transmitter.h>
#include <lib/subghz/registry.h>
#include <lib/flipper_format/flipper_format.h>

typedef enum {
    WsWorkerEvtStop = (1 << 0),
    WsWorkerEvtTick = (1 << 1),
} WsWorkerEvtFlags;
#define WS_WORKER_ALL_EVENTS (WsWorkerEvtStop | WsWorkerEvtTick)

typedef struct {
    FuriString* protocol_name;
    WSBlockGeneric* generic;
} WSReceiverInfoModel;

typedef struct {
    SubGhzWorker* worker;
    SubGhzEnvironment* environment;
    SubGhzReceiver* receiver;
    SubGhzRadioPreset* preset;
    WSHistory* history;
    uint16_t idx_menu_chosen;
    uint16_t history_size;
    WSTxRxState txrx_state;
    WSHopperState hopper_state;
    uint8_t hopper_timeout;
    uint8_t hopper_idx_frequency;
    WSRxKeyState rx_key_state;
}WeatherStationTxRx;



typedef struct {
    WeatherStationTxRx* txrx;
    WSReceiverInfoModel* data;
    //NotificationApp* notifications;
    SubGhzSetting* setting;
    FuriThread* thread;//setup thread of ws
    //FuriTimer* timer; //Used for tick callbacks
}WeatherStationContext;

WeatherStationContext* ws_init();
void ws_free(WeatherStationContext* ws);

bool ws_set_preset(WeatherStationContext* ws, const char* preset);
void ws_preset_init(void* context,const char* preset_name,uint32_t frequency,uint8_t* preset_data,size_t preset_data_size);
void ws_get_frequency_modulation(WeatherStationContext* ws,FuriString* frequency,FuriString* modulation);
void ws_begin(WeatherStationContext* ws, uint8_t* preset_data);
uint32_t ws_rx(WeatherStationContext* ws, uint32_t frequency);
void ws_idle(WeatherStationContext* ws);
void ws_rx_end(WeatherStationContext* ws);
void ws_sleep(WeatherStationContext* ws);

void ws_init_thread(WeatherStationContext* ws); 
void ws_deinit_thread(WeatherStationContext* ws);



/*void ws_preset_init(void* context,const char* preset_name,uint32_t frequency,uint8_t* preset_data,size_t preset_data_size);
bool ws_set_preset(WeatherStationContext* app, const char* preset);
void ws_get_frequency_modulation(WeatherStationContext* app,FuriString* frequency,FuriString* modulation);
void ws_begin(WeatherStationContext* app, uint8_t* preset_data);
uint32_t ws_rx(WeatherStationContext* app, uint32_t frequency);
void ws_idle(WeatherStationContext* app);
void ws_rx_end(WeatherStationContext* app);
void ws_sleep(WeatherStationContext* app);
void ws_hopper_update(WeatherStationContext* app);*/


#endif