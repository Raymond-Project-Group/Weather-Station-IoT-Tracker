#include "ws_transmit_constraints.h"

const protocol_constraints acurite_592txr = (protocol_constraints){.protocol="Acurite 592TXR",.channel = 3,.battery = 1,.device_id = 4095,.humidity = 127,.temp_min = -40,.temp_max = 70};
const protocol_constraints acurite_606tx = (protocol_constraints){.protocol = "Acurite-606TX",.channel = 0,.battery = 1,.device_id = 255,.humidity = 0,.temp_min = -100};
const protocol_constraints acurite_609txc = (protocol_constraints){.protocol = "Acurite-609TXC",.channel = 0,.battery = 1,.device_id = 255,.humidity = 255,.temp_min = -204.8};
const protocol_constraints ambient_weather = (protocol_constraints){.protocol = "Ambient_Weather",.channel = 7,.battery = 1,.device_id = 255,.humidity = 255,.temp_min = -40};
const protocol_constraints auriol_ahfl = (protocol_constraints){.protocol = "Auriol AHFL",.channel = 3,.battery = 1,.device_id = 255,.humidity = 127,.temp_min = -204.8};
const protocol_constraints auriol_hg0601a = (protocol_constraints){.protocol = "Auriol HG06061",.channel = 3,.battery = 1,.device_id = 255,.humidity = 255,.temp_min = -204.8};
const protocol_constraints gt_wt_02 = (protocol_constraints){.protocol = "GT-WT02",.channel = 3,.battery = 1,.device_id = 255,.humidity = 127,.temp_min = -204.8};
const protocol_constraints gt_wt_03 = (protocol_constraints){.protocol = "GT-WT03",.channel = 3,.battery = 1,.device_id = 255,.humidity = 255,.temp_min = -204.8};
const protocol_constraints infactory = (protocol_constraints){.protocol = "inFactory-TH",.channel = 3,.battery = 1,.device_id = 255,.humidity = 100,.temp_min = -67};
const protocol_constraints lacrosse_tx = (protocol_constraints){.protocol = "LaCrosse_TX",.channel = 0,.battery = 0,.device_id = 15,.humidity = 0,.temp_min = -50};
const protocol_constraints lacrosse_tx141thbv2 = (protocol_constraints){.protocol = "TX141THBv2",.channel = 3,.battery = 1,.device_id = 255,.humidity = 255,.temp_min = -45};
const protocol_constraints nexus_th = (protocol_constraints){.protocol = "Nexus-TH",.channel = 3,.battery = 1,.device_id = 255,.humidity = 255,.temp_min = -204.8};
const protocol_constraints oregon_v1 = (protocol_constraints){.protocol = "Oregon-v1",.channel = 3,.battery = 1,.device_id = 15,.humidity = 0,.temp_min = -99};
const protocol_constraints oregon2 = (protocol_constraints){.protocol = "Oregon2",.channel = 3,.battery = 1,.device_id = 65535,.humidity = 0,.temp_min = -99};
const protocol_constraints oregon3 = (protocol_constraints){.protocol = "Oregon3",.channel = 3,.battery = 1,.device_id = 65535,.humidity = 128,.temp_min = -99};
const protocol_constraints thermopro_tx4 = (protocol_constraints){.protocol = "ThermoPRO-TX4",.channel = 3,.battery = 1,.device_id = 255,.humidity = 255,.temp_min = -204.8};
const protocol_constraints tx_8300 = (protocol_constraints){.protocol = "TX8300",.channel = 3,.battery = 1,.device_id = 127,.humidity = 100,.temp_min = -99.9};
const protocol_constraints wendox_w6726 = (protocol_constraints){.protocol = "Wendox W6726",.channel = 3,.battery = 1,.device_id = 31,.humidity = 0,.temp_min = -102.3};

const protocol_constraints weather_station_protocol_constraints[18] = {
    acurite_592txr,
    acurite_606tx,
    acurite_609txc,
    ambient_weather,
    auriol_ahfl,
    auriol_hg0601a,
    gt_wt_02,
    gt_wt_03,
    infactory,
    lacrosse_tx,
    lacrosse_tx141thbv2,
    nexus_th,
    oregon_v1,
    oregon2,
    oregon3,
    thermopro_tx4,
    tx_8300,
    wendox_w6726
};

protocol_constraints Find_Transmit_Constraints(const char* protocol_name)
{
    for(int i = 0; i < 18; i++)
    {
        if(!strcmp(protocol_name,weather_station_protocol_constraints[i].protocol))
            return weather_station_protocol_constraints[i];
    }
    return weather_station_protocol_constraints[0];
}