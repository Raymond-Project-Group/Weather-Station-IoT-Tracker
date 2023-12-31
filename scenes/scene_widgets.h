#ifndef SCENE_WIDGETS_H
#define SCENE_WIDGETS_H

#include "../flipper.h"
#include "../app.h"
#include "scenes.h"
void pod_widgets_redraw_pressure(App* app, uint8_t pX, uint8_t pY); //Draw Pressure
void pod_widgets_redraw_humidity(App* app,uint8_t hX, uint8_t hY, int page, PodDeltaState deltaState); //Draw Humidity
void pod_widgets_redraw_temperature(App* app,uint8_t tX, uint8_t tY, int page, PodDeltaState deltaState); //Draw Temperature
void pod_widgets_redraw_satellites(App* app,uint8_t sX, uint8_t sY); //Draw Satellites
void pod_widgets_redraw_altitude(App* app,uint8_t aX, uint8_t aY); //Draw Altitude
void pod_widgets_redraw_longitude(App* app,uint8_t lX, uint8_t lY); //Draw Longitude
void pod_widgets_redraw_latitude(App* app,uint8_t lX, uint8_t lY); //Draw Latitude
void pod_widgets_redraw_time(App* app,uint8_t tX, uint8_t tY); //Draw Time
void pod_widgets_redraw_timer(App* app,uint8_t tX, uint8_t tY); //Draw Timer

void pod_widgets_redraw_temperature_offset(App* app, uint8_t x, uint8_t y); // Draw temp offset
void pod_widgets_redraw_humidity_offset(App* app, uint8_t x, uint8_t y); // Draw humid offset
void pod_widgets_redraw_pressure_offset(App* app, uint8_t x, uint8_t y); // Draw pressure offset

void pod_widgets_redraw_battery(App* app,uint8_t bX, uint8_t bY); //Draw Battery
void pod_widgets_redraw_id(App* app,uint8_t iX, uint8_t iY); //Draw ID
void pod_widgets_redraw_channel(App* app,uint8_t cX, uint8_t cY); //Draw Channel
void pod_widgets_redraw_frequency(App* app,uint8_t fX, uint8_t fY); //Draw Frequency
void pod_widgets_redraw_protocol(App* app, uint8_t pX, uint8_t pY); //Draw Protocol
void pod_widgets_redraw_humidity_transmit(App* app,uint8_t hX, uint8_t hY);
void pod_widgets_redraw_temperature_transmit(App* app,uint8_t tX, uint8_t tY); //Draw temperature 
void pod_widgets_redraw_rssi(App* app,uint8_t rX, uint8_t rY);
#endif