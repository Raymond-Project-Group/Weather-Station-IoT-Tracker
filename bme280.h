//This is heavily based off of https://github.com/quen0n/unitemp-flipperzero/blob/master/sensors/BMx280.h 
#ifndef BME280_H
#define BME280_H
#include "flipper.h"
//#include "app.h"
/*typedef enum {
    I2C_State_Not_Found,
    I2C_State_Found,
    I2C_State_Write_Success,
    I2C_State_Read_Success,
    I2C_State_Write_Read_Success,
} I2cState;*/

typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
} Bme280TempCal;

typedef struct {
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
} Bme280PressCal;

typedef struct {
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;
} Bme280HumCal;

typedef enum {
    BME_Disabled,
    BME_Ready,
    BME_Active,
    BME_Finished,
} BmeState;


typedef struct {
    FuriString* buffer;
    // You can add additional state here.
    //I2cDemoState state;
    Bme280TempCal temp_cal;
    Bme280HumCal hum_cal;
    Bme280PressCal press_cal;
    float temperature;
    float humidity;
    float pressure;
    int32_t temp_fine;
    //uint8_t temp_xlsb;
    //uint8_t press_xlsb;
} Bme280Data;

typedef struct {
    Bme280Data* data; // Data accessed by multiple threads (acquire the mutex before accessing!)
    BmeState state;
    uint8_t address;
    uint16_t tick_count;
} Bme280Context;

Bme280Context* bme_init();
void bme_update_status(Bme280Context* bme);
bool bme_read_sensors(Bme280Context* bme);
bool bme_ready(Bme280Context* bme);
void bme_free(Bme280Context* bme);
bool bme_sensor_callibration_values(Bme280Context* bme);

#endif