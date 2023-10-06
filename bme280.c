#include "bme280.h"

static float bme_compensate_temperature(Bme280Data* data,int32_t uncal_temp) 
{ //adapted callibration given in https://cdn-learn.adafruit.com/assets/assets/000/115/588/original/bst-bme280-ds002.pdf?1664822559 and https://github.com/quen0n/unitemp-flipperzero/blob/1a157681ff64a8828d8a062fcc592aeb1253d6a0/sensors/BMx280.h
    int32_t var1, var2;
    var1 = ((((uncal_temp >> 3) - ((int32_t)data->temp_cal.dig_T1 << 1))) *
            ((int32_t)data->temp_cal.dig_T2)) >>
           11;
    var2 = (((((uncal_temp >> 4) - ((int32_t)data->temp_cal.dig_T1)) *
              ((uncal_temp >> 4) - ((int32_t)data->temp_cal.dig_T1))) >>
             12) *
            ((int32_t)data->temp_cal.dig_T3)) >>
           14;
    data->temp_fine = var1 + var2;//temperature correction value
    return ((data->temp_fine * 5 + 128) >> 8) / 100.0f;
}

static float bme_compensate_pressure(Bme280Data* data, int32_t uncal_press) {
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t)data->temp_fine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)data->press_cal.dig_P6);
    var2 = var2 + ((var1 * ((int32_t)data->press_cal.dig_P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t)data->press_cal.dig_P4) << 16);
    var1 = (((data->press_cal.dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) +
            ((((int32_t)data->press_cal.dig_P2) * var1) >> 1)) >>
           18;
    var1 = ((((32768 + var1)) * ((int32_t)data->press_cal.dig_P1)) >> 15);
    if(var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = (((uint32_t)(((int32_t)1048576) - uncal_press) - (var2 >> 12))) * 3125;
    if(p < 0x80000000) {
        p = (p << 1) / ((uint32_t)var1);
    } else {
        p = (p / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)data->press_cal.dig_P9) *
            ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >>
           12;
    var2 = (((int32_t)(p >> 2)) * ((int32_t)data->press_cal.dig_P8)) >> 13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + data->press_cal.dig_P7) >> 4));
    return p;
}

static float bme_compensate_humidity(Bme280Data* data, int32_t uncal_humid) {
    int32_t v_x1_u32r;
    v_x1_u32r = (data->temp_fine - ((int32_t)76800));

    v_x1_u32r =
        (((((uncal_humid << 14) - (((int32_t)data->hum_cal.dig_H4) << 20) -
            (((int32_t)data->hum_cal.dig_H5) * v_x1_u32r)) +
           ((int32_t)16384)) >>
          15) *
         (((((((v_x1_u32r * ((int32_t)data->hum_cal.dig_H6)) >> 10) *
              (((v_x1_u32r * ((int32_t)data->hum_cal.dig_H3)) >> 11) +
               ((int32_t)32768))) >>
             10) +
            ((int32_t)2097152)) *
               ((int32_t)data->hum_cal.dig_H2) +
           8192) >>
          14));

    v_x1_u32r =
        (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                       ((int32_t)data->hum_cal.dig_H1)) >>
                      4));

    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    return ((uint32_t)(v_x1_u32r >> 12)) / 1024.0f;
}



Bme280Context* bme_init()
{
    Bme280Context* bme = malloc(sizeof(Bme280Context));
    Bme280Data* data = malloc(sizeof(Bme280Data));
    bme->address = 0xEC;
    data->temperature = 0xFF;
    //data->temp_xlsb = 0xFF;
    data->humidity = 0xFF;
    data->pressure = 0xFF;
    //data->press_xlsb = 0xFF;
    data->buffer = furi_string_alloc();
    //data->state = I2C_State_Not_Found;
    bme->tick_count = 0;
    bme->data = data;
    bme->state = BME_Disabled;
    bme_sensor_callibration_values(bme);
    return bme;
}

void bme_update_status(Bme280Context* bme)
{
    if(bme_ready(bme) && bme->state == BME_Ready){
        bme->state = BME_Active;
        //bme->data->state = I2C_State_Found;
        if(bme_read_sensors(bme)){
            bme->state = BME_Finished;
        }
    }
}
bool bme_find_address(Bme280Context* bme)//it should be 0x76 or 0x77, but only 0xEC has been working for me
{
    bool t = false;
    furi_hal_i2c_acquire(&furi_hal_i2c_handle_external);

    for(uint8_t try_addr = 0; try_addr != 0xff; try_addr++) {
        if(furi_hal_i2c_is_device_ready(&furi_hal_i2c_handle_external, try_addr, 100)) {
            bme->address= try_addr;
            //bme->data->temperature = addr;
            t = true;
            break;
        }
    }

    furi_hal_i2c_release(&furi_hal_i2c_handle_external);
    return t;
}

bool bme_sensor_callibration_values(Bme280Context* bme){
    //bool result = false;
    uint8_t buffer[7] = {0}; 
    uint8_t  temp_addr= 0x88;//temp callibration address
    uint8_t  press_addr = 0x8E;//pressure callibration address
    uint8_t  hum_h1_addr = 0xA1;//humidity callibration address
    uint8_t hum_h2_addr = 0xE1;
    furi_hal_i2c_acquire(&furi_hal_i2c_handle_external);
    if(!furi_hal_i2c_read_mem(&furi_hal_i2c_handle_external,bme->address,temp_addr,(uint8_t*)&bme->data->temp_cal,6,10))
    {
        furi_hal_i2c_release(&furi_hal_i2c_handle_external);
        return false;
    }
    if(!furi_hal_i2c_read_mem(&furi_hal_i2c_handle_external,bme->address,press_addr,(uint8_t*)&bme->data->press_cal,18,10))
    {
        furi_hal_i2c_release(&furi_hal_i2c_handle_external);
        return false;
    }
    if(!furi_hal_i2c_read_mem(&furi_hal_i2c_handle_external,bme->address,hum_h1_addr,buffer,1,10))
    {
        furi_hal_i2c_release(&furi_hal_i2c_handle_external);
        return false;
    }
    bme->data->hum_cal.dig_H1 = buffer[0];
    if(!furi_hal_i2c_read_mem(&furi_hal_i2c_handle_external,bme->address,hum_h2_addr,buffer,7,10))
    {
        furi_hal_i2c_release(&furi_hal_i2c_handle_external);
        return false;
    }
    bme->data->hum_cal.dig_H2 = (uint16_t)(buffer[0] | ((uint16_t)buffer[1] << 8));
    bme->data->hum_cal.dig_H3 = buffer[2];
    bme->data->hum_cal.dig_H4 = ((int16_t)buffer[3] << 4) | (buffer[4] & 0x0F);
    bme->data->hum_cal.dig_H5 = (buffer[4] & 0x0F) | ((int16_t)buffer[5] << 4);
    bme->data->hum_cal.dig_H6 = buffer[6];
    bme->data->pressure=0x04;
    furi_hal_i2c_release(&furi_hal_i2c_handle_external);
    return true;
}

bool bme_read_sensors(Bme280Context* bme){
    //bool result = false;
    uint8_t buffer[3] = {0}; 
    uint8_t  temp_addr= 0xFA;
    uint8_t  press_addr = 0xF7;
    uint8_t  hum_addr = 0xFD;
    furi_hal_i2c_acquire(&furi_hal_i2c_handle_external);
    
    //acquire temperature
    if(!furi_hal_i2c_read_mem(&furi_hal_i2c_handle_external,bme->address,temp_addr,buffer,3,10))
    {
        furi_hal_i2c_release(&furi_hal_i2c_handle_external);
        return false;
    }
    int32_t uncal_temp = ((int32_t)buffer[0] << 12) | ((int32_t)buffer[1] << 4) | ((int32_t)buffer[2] >> 4);//store uncallibrated temp

    //acquire pressure
    if(!furi_hal_i2c_read_mem(&furi_hal_i2c_handle_external,bme->address,press_addr,buffer,3,10))
    {
        furi_hal_i2c_release(&furi_hal_i2c_handle_external);
        return false;
    }
    int32_t uncal_press = ((int32_t)buffer[0] << 12) | ((int32_t)buffer[1] << 4) | ((int32_t)buffer[2] >> 4);

    //acquire humidity
    if(!furi_hal_i2c_read_mem(&furi_hal_i2c_handle_external,bme->address,hum_addr,buffer,2,10))
    {
        furi_hal_i2c_release(&furi_hal_i2c_handle_external);
        return false;
    }
    int32_t uncal_hum = ((uint16_t)buffer[0] << 8) | buffer[1];
    
    bme->data->temperature = bme_compensate_temperature(bme->data,uncal_temp);
    bme->data->pressure = bme_compensate_pressure(bme->data,uncal_press);
    bme->data->humidity = bme_compensate_humidity(bme->data,uncal_hum);
    
    furi_hal_i2c_release(&furi_hal_i2c_handle_external);
    return true;
}
void bme_free(Bme280Context* bme)
{
    furi_string_free(bme->data->buffer);
    free(bme->data);
    free(bme);
}