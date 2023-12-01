#ifndef WEATHER_STATION_TYPES_H
#define WEATHER_STATION_TYPES_H

#include <furi.h>
#include <furi_hal.h>

#define WS_VERSION_APP "1.1"
#define WS_DEVELOPED "SkorP"
#define WS_GITHUB "https://github.com/flipperdevices/flipperzero-good-faps"

#define WS_KEY_FILE_VERSION 1
#define WS_KEY_FILE_TYPE "Flipper Weather Station Key File"

/** WSRxKeyState state */
typedef enum {
    WSRxKeyStateIDLE,
    WSRxKeyStateBack,
    WSRxKeyStateStart,
    WSRxKeyStateAddKey,
} WSRxKeyState;

/** WSHopperState state */
typedef enum {
    WSHopperStateOFF,
    WSHopperStateRunning,
    WSHopperStatePause,
    WSHopperStateRSSITimeOut,
} WSHopperState;

/** WSLock */
typedef enum {
    WSLockOff,
    WSLockOn,
} WSLock;

/*typedef enum {
    WeatherStationViewVariableItemList,
    WeatherStationViewSubmenu,
    WeatherStationViewReceiver,
    WeatherStationViewReceiverInfo,
    WeatherStationViewWidget,
} WeatherStationView;*/

/** WeatherStationTxRx state */
typedef enum {
    WSTxRxStateIDLE,
    WSTxRxStateRx,
    WSTxRxStateTx,
    WSTxRxStateSleep,
} WSTxRxState;

#endif