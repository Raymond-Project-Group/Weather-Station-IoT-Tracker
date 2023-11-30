#ifndef WEATHER_TRANSMIT_CONSTRAINTS_H
#define WEATHER_TRANSMIT_CONSTRAINTS_H
#include "../flipper.h"
#include "helpers/weather_station_event.h"
#include "helpers/weather_station_types.h"
#include "protocols/ws_generic.h"
#include "protocols/protocol_items.h"
#include <stdio.h>
#include <string.h>
typedef struct
{
    char* protocol;
    uint8_t channel;
    uint8_t battery;
    uint16_t device_id;
    uint8_t humidity;
    float temp_min;
    float temp_max;
}protocol_constraints;

protocol_constraints Find_Transmit_Constraints(const char* protocol_name);
extern const protocol_constraints weather_station_protocol_constraints[18];
#endif