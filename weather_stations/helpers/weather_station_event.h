#ifndef WEATHER_STATION_EVENT_H
#define WEATHER_STATION_EVENT_H

typedef enum {
    //WSCustomEvent
    WSCustomEventStartId = 100,

    WSCustomEventSceneSettingLock,

    WSCustomEventViewReceiverOK,
    WSCustomEventViewReceiverConfig,
    WSCustomEventViewReceiverBack,
    WSCustomEventViewReceiverOffDisplay,
    WSCustomEventViewReceiverUnlock,
} WSCustomEvent;

#endif