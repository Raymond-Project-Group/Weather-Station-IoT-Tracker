#include "ws_transmit.h"
#include "../pod.h"
#include <lib/subghz/devices/cc1101_configs.h>

int get_preset_from_name(const char* preset_name) {
    if(!strcmp(preset_name, "FuriHalSubGhzPresetOok270Async")) {
        return FuriHalSubGhzPresetOok270Async;
    } else if(!strcmp(preset_name, "FuriHalSubGhzPresetOok650Async")) {
        return FuriHalSubGhzPresetOok650Async;
    } else if(!strcmp(preset_name, "FuriHalSubGhzPreset2FSKDev238Async")) {
        return FuriHalSubGhzPreset2FSKDev238Async;
    } else if(!strcmp(preset_name, "FuriHalSubGhzPreset2FSKDev476Async")) {
        return FuriHalSubGhzPreset2FSKDev476Async;
    } else {
        return FuriHalSubGhzPresetCustom;
    }
}

bool message_set_preset(Message* m, const char* preset) 
{
    if(!strcmp(preset, "FuriHalSubGhzPresetOok270Async")) {
        furi_string_set(m->preset->name, "AM270");
    } else if(!strcmp(preset, "FuriHalSubGhzPresetOok650Async")) {
        furi_string_set(m->preset->name, "AM650");
    } else if(!strcmp(preset, "FuriHalSubGhzPreset2FSKDev238Async")) {
        furi_string_set(m->preset->name, "FM238");
    } else if(!strcmp(preset, "FuriHalSubGhzPreset2FSKDev476Async")) {
        furi_string_set(m->preset->name, "FM476");
    } else if(!strcmp(preset, "FuriHalSubGhzPresetCustom")) {
        furi_string_set(m->preset->name, "CUSTOM");
    } else {
        //FURI_LOG_E(TAG, "Unknown preset");
        return false;
    }
    return true;
}
Message* message_test()
{
    Message* message = malloc(sizeof(Message));
    //init setting
    message->settings = subghz_setting_alloc();

    //ToDo FIX  file name setting
    subghz_setting_load(message->settings, EXT_PATH("subghz/assets/setting_user"));

    message->preset = malloc(sizeof(SubGhzRadioPreset));
    message->preset->name = furi_string_alloc();
    furi_string_set(message->preset->name,"AM650");
    //furi_string_set(message->preset->name,"FuriHalSubGhzPresetOok650Async");
    message->preset->data = NULL;
    message->preset->data_size = 0;
    message->preset->frequency = subghz_setting_get_default_frequency(message->settings);

    message->generic = malloc(sizeof(WSBlockGeneric));

    message->generic->protocol_name = "Acurite 592TXR";
    message->generic->id = 0x4be;
    message->generic->data_count_bit = 56;
    message->generic->battery_low = 0;
    message->generic->humidity = 55;
    message->generic->timestamp = 0;
    message->generic->channel = 1;
    message->generic->btn = 0xff;
    message->generic->temp = 25.6;
    ws_protocol_acurite_592txr_reconstructor(message->generic);

    
    //init environment
    message->environment = subghz_environment_alloc();
    subghz_environment_set_protocol_registry(message->environment,(void*)&weather_station_protocol_registry);//loads protocols from protocol files into environment
    //init transmitter    
    message->transmitter = subghz_transmitter_alloc_init(message->environment,message->generic->protocol_name);

    message->selection = 0;
    message->edit = false;
    return message;
}
Message* message_init()
{
    Message* message = malloc(sizeof(Message));
    //init setting
    message->settings = subghz_setting_alloc();

    //ToDo FIX  file name setting
    subghz_setting_load(message->settings, EXT_PATH("subghz/assets/setting_user"));

    message->preset = malloc(sizeof(SubGhzRadioPreset));
    message->preset->name = furi_string_alloc();
    furi_string_set(message->preset->name,"AM650");
    //furi_string_set(message->preset->name,"FuriHalSubGhzPresetOok650Async");
    message->preset->data = NULL;
    message->preset->data_size = 0;
    message->preset->frequency = subghz_setting_get_default_frequency(message->settings);

    message->generic = malloc(sizeof(WSBlockGeneric));

    message->generic->protocol_name = "Acurite 592TXR";
    message->generic->id = 0x4be;
    message->generic->data_count_bit = 56;
    message->generic->battery_low = 0;
    message->generic->humidity = 55;
    message->generic->timestamp = 0;
    message->generic->channel = 1;
    message->generic->btn = 0xff;
    message->generic->temp = 25.6;
    //ws_protocol_acurite_592txr_reconstructor(message->generic);

    
    //init environment
    message->environment = subghz_environment_alloc();
    subghz_environment_set_protocol_registry(message->environment,(void*)&weather_station_protocol_registry);//loads protocols from protocol files into environment
    
    //for the scene
    message->edit = false;
    message->selection = 0;

    //protocol length
    size_t i = 0;
    const char* protocol_name;
    do
    {
        protocol_name = subghz_environment_get_protocol_name_registry(message->environment, i++);
    }
    while(protocol_name!=NULL);
    message->protocol_reg_length=(uint16_t) (i-1);
    return message;
}

void message_free(Message* message)
{
    furi_string_free(message->preset->name);
    //setting
    subghz_setting_free(message->settings);
    free(message->preset);
    free(message->generic);
    // Free subghz resources we allocated.
    //subghz_transmitter_free(message->transmitter);
    subghz_environment_free(message->environment);
    free(message);
}
void ws_check_freq(Message* message){//checks frequency and if not applicable makes it closest viable value
    int min_freq = 0;
    int freqs[6] = {304100,321950,433050,434790,915000,927950};
    if(!(((message->preset->frequency >= 304100000) && (message->preset->frequency <= 321950000)) || 
    ((message->preset->frequency >= 433050000) && (message->preset->frequency <= 434790000)) || 
    ((message->preset->frequency >= 915000000) && (message->preset->frequency <= 927950000))))
    {
        for(int i = 1; i<6; i++)
        {
            int a = (message->preset->frequency/1000)-freqs[i];
            int b = (message->preset->frequency/1000)-freqs[min_freq];
            
            if(abs(a)<abs(b))
            {
                min_freq = i;
            }
        }
        message->preset->frequency = freqs[min_freq]*1000;
    }
}

bool ws_reconstruct_message(Message* message)
{
    if(!strcmp(message->generic->protocol_name,"Acurite 592TXR"))
    {
        message->generic->data_count_bit = 56;
        ws_protocol_acurite_592txr_reconstructor(message->generic);

    }
    else if(!strcmp(message->generic->protocol_name,"Acurite-606TX"))
    {
        message->generic->data_count_bit = 32;
        ws_protocol_acurite_606tx_reconstructor(message->generic);
    }/*
    else if(!strcmp(message->generic->protocol_name,"Acurite-609TXC"))
    {

    }*/
    else if(!strcmp(message->generic->protocol_name,"Ambient_Weather"))
    {
        message->generic->data_count_bit = 48;
        ws_protocol_ambient_weather_reconstructor(message->generic);
    }/*
    else if(!strcmp(message->generic->protocol_name,"Auriol AHFL"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"Auriol HG06061"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"GT-WT02"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"GT-WT03"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"inFactory-TH"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"LaCrosse_TX"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"TX141THBv2"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"Nexus-TH"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"Oregon-v1"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"Oregon2"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"Oregon3"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"ThermoPRO-TX4"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"TX8300"))
    {

    }
    else if(!strcmp(message->generic->protocol_name,"Wendox W6726"))
    {

    }*/
    else{
        FURI_LOG_E(TAG,"Protocol Reconstruction Not Exist yet for this protocol");
        return false;
    }
    return true;
}

bool ws_transmit_message(Message* message)
{
    bool ret = false;

    protocol_constraints constraints = Find_Transmit_Constraints(message->generic->protocol_name);
   
    if(strcmp(constraints.protocol,message->generic->protocol_name)){
        FURI_LOG_E(TAG, "Protocol Constraints Not Found");
        return false;
    }
    if(message->generic->id > constraints.device_id){
        message->generic->id = constraints.device_id;
    }
    if(message->generic->temp < constraints.temp_min){
        message->generic->temp = constraints.temp_min;
    }
    if(message->generic->channel > constraints.channel){
        message->generic->channel = constraints.channel;
    }
    ws_check_freq(message);
    if(!ws_reconstruct_message(message)){
        return false;
    }
    // Use one of the presets in subghz_device_cc1101_int_interconnect_load_preset.  If the first argument is FuriHalSubGhzPresetCustom, then the second argument is
    // a custom register table (Reg, value, reg, value, ...,0, 0, PATable [0..7] entries).
    // Set the frequency, RF switch path (band), calibrates the oscillator on the CC1101.
    furi_hal_subghz_load_custom_preset(subghz_setting_get_preset_data_by_name(message->settings, furi_string_get_cstr(message->preset->name)));//DO PRESET THEN FREQUENCY, THE ORDER MATTERS
    furi_hal_subghz_set_frequency_and_path(message->preset->frequency);
    
    furi_hal_subghz_idle();
    furi_hal_gpio_init(&gpio_cc1101_g0, GpioModeOutputPushPull, GpioPullNo, GpioSpeedLow);
    furi_hal_gpio_write(&gpio_cc1101_g0, true);
    if(!furi_hal_subghz_tx()) {
        FURI_LOG_E(TAG, "Sending not allowed");
        return false;
    }
    else
    {
        // Stop charging the battery while transmitting.
        furi_hal_power_suppress_charge_enter();

        FlipperFormat* payload = flipper_format_string_alloc();
        ws_block_generic_serialize(message->generic,payload,message->preset);//serializes message->generic;
        //init transmitter    
        message->transmitter = subghz_transmitter_alloc_init(message->environment,message->generic->protocol_name);

        SubGhzProtocolStatus status = subghz_transmitter_deserialize(message->transmitter,payload);

        furi_assert(status == SubGhzProtocolStatusOk);

        // Start transmitting (keeps the DMA buffer filled with the encoder.upload[] data)
        if(furi_hal_subghz_start_async_tx(subghz_transmitter_yield, message->transmitter)) {
            // Wait for the transmission to complete.
            while(!(furi_hal_subghz_is_async_tx_complete())) {
                furi_delay_ms(100);
            }

            // Stop transmitting, debug log (tag="FuriHalSubGhz") the duty cycle information.
            furi_hal_subghz_stop_async_tx();
            ret = true;
        }
        // Allow the battery to charge again.
        furi_hal_power_suppress_charge_exit();
        //free payload
        flipper_format_free(payload);
        //free transmitter
        subghz_transmitter_free(message->transmitter);
    }

    furi_hal_subghz_set_path(FuriHalSubGhzPathIsolate);
    furi_hal_subghz_idle();
    furi_hal_subghz_sleep();

    // Free resources we allocated.
    //subghz_transmitter_free(transmitter);
    //subghz_environment_free(environment);

    return ret;

}