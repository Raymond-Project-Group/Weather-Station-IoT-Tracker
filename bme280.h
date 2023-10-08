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
    Bme280TempCal temp_cal; //used for calibrating collection
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
    BmeState state;//Not currently used, but could be
    uint8_t address;//address of I2C
    FuriThread* thread;//setup thread of BME280 collection.
    FuriTimer* timer; //Used for tick callbacks
} Bme280Context;

Bme280Context* bme_init();
void bme_update_status(Bme280Context* bme);
bool bme_read_sensors(Bme280Context* bme);
bool bme_ready(Bme280Context* bme);
void bme_free(Bme280Context* bme);
bool bme_sensor_callibration_values(Bme280Context* bme);
bool bme_find_address(Bme280Context* bme);//it should be 0x76 or 0x77, but only 0xEC has been working for me
bool bme_sleep_mode(Bme280Context* bme);
bool bme_set_operating_modes(Bme280Context* bme);
void bme_update_tickback(void* context); //Used for timer callback, for thread
void bme_init_thread(Bme280Context* bme);
void bme_uart_deinit_thread(Bme280Context* bme);

#endif