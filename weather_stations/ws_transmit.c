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
    //init transmitter    
    message->transmitter = subghz_transmitter_alloc_init(message->environment,message->generic->protocol_name);
    
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
    subghz_transmitter_free(message->transmitter);
    subghz_environment_free(message->environment);
    free(message);
}
bool ws_transmit_message(Message* message)
{
    bool ret = false;
    // Populate the CC101 device list.
    //subghz_devices_init();

    // Get the internal radio device.
    //const SubGhzDevice* device = subghz_devices_get_by_name(SUBGHZ_DEVICE_CC1101_INT_NAME);

    //SubGhzEnvironment* environment = subghz_environment_alloc();
    //subghz_environment_set_protocol_registry(environment,(void*)&weather_station_protocol_registry);//loads protocols from protocol files into environment
    //SubGhzTransmitter* transmitter = subghz_transmitter_alloc_init(environment,message->generic->protocol_name);

    // Use one of the presets in subghz_device_cc1101_int_interconnect_load_preset.  If the first argument is FuriHalSubGhzPresetCustom, then the second argument is
    // a custom register table (Reg, value, reg, value, ...,0, 0, PATable [0..7] entries).
    // Set the frequency, RF switch path (band), calibrates the oscillator on the CC1101.
    furi_hal_subghz_load_custom_preset(subghz_setting_get_preset_data_by_name(message->settings, furi_string_get_cstr(message->preset->name)));//DO PRESET THEN FREQUENCY, THE ORDER MATTERS
    furi_hal_subghz_set_frequency_and_path(message->preset->frequency);

    furi_hal_subghz_idle();
    furi_hal_gpio_init(&gpio_cc1101_g0, GpioModeOutputPushPull, GpioPullNo, GpioSpeedLow);
    furi_hal_gpio_write(&gpio_cc1101_g0, true);

    /*uint8_t same = 1;
    uint8_t * test = subghz_setting_get_preset_data_by_name(message->settings, furi_string_get_cstr(message->preset->name));
    for(int i=0; i<44;i++)
    {
        if(test[i]!=subghz_device_cc1101_preset_ook_650khz_async_regs[i]){
            same = 0;
            break;
        }
    }    
    FURI_LOG_I(TAG,"%lu\t%u",message->preset->frequency,same);*/
    
    if(!furi_hal_subghz_tx()) {
        FURI_LOG_E(TAG, "Sending not allowed");
    }
    else
    {
        // Stop charging the battery while transmitting.
        furi_hal_power_suppress_charge_enter();

        FlipperFormat* payload = flipper_format_string_alloc();
        ws_block_generic_serialize(message->generic,payload,message->preset);//serializes message->generic;

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
    }

    furi_hal_subghz_set_path(FuriHalSubGhzPathIsolate);
    furi_hal_subghz_idle();
    furi_hal_subghz_sleep();

    // Free resources we allocated.
    //subghz_transmitter_free(transmitter);
    //subghz_environment_free(environment);

    return ret;

}