#ifndef WS_TRANSMIT_H
#define WS_TRANSMIT_H
#include "../flipper.h"
#include "helpers/weather_station_event.h"
#include "helpers/weather_station_types.h"
#include "protocols/ws_generic.h"
#include "protocols/protocol_items.h"
#include "weather_station_history.h"

#include <lib/subghz/subghz_setting.h>
#include <lib/subghz/subghz_worker.h>
#include <lib/subghz/transmitter.h>
#include <lib/subghz/devices/cc1101_int/cc1101_int_interconnect.h>
#include <lib/subghz/devices/devices.h>
#include <lib/subghz/registry.h>
#include <lib/flipper_format/flipper_format.h>

typedef struct
{
    WSBlockGeneric* generic;
    SubGhzRadioPreset* preset;
    SubGhzEnvironment* environment;
    SubGhzTransmitter* transmitter;
    SubGhzSetting* settings;
    bool edit;
    uint8_t selection;
    uint16_t protocol_reg_length;

}Message;

int get_preset_from_name(const char* preset_name);
Message* message_test();
Message* message_init();
void message_free(Message* message);
bool ws_transmit_message(Message* message);
#endif