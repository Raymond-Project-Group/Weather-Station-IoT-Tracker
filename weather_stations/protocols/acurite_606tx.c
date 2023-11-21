#include "acurite_606tx.h"

#define TAG "WSProtocolAcurite_606TX"

/*
 * Help
 * https://github.com/merbanan/rtl_433/blob/5bef4e43133ac4c0e2d18d36f87c52b4f9458453/src/devices/acurite.c#L1644
 * 
 *     0000 1111 | 0011 0000 | 0101 1100 | 1110 0111
 *     iiii iiii | buuu tttt | tttt tttt | cccc cccc
 * - i: identification; changes on battery switch
 * - c: lfsr_digest8;
 * - u: unknown;
 * - b: battery low; flag to indicate low battery voltage
 * - t: Temperature; in °C
 * 
 */

static const SubGhzBlockConst ws_protocol_acurite_606tx_const = {
    .te_short = 500,
    .te_long = 2000,
    .te_delta = 150,
    .min_count_bit_for_found = 32,
};

struct WSProtocolDecoderAcurite_606TX {
    SubGhzProtocolDecoderBase base;

    SubGhzBlockDecoder decoder;
    WSBlockGeneric generic;
};

struct WSProtocolEncoderAcurite_606TX {
    SubGhzProtocolEncoderBase base;

    SubGhzProtocolBlockEncoder encoder;
    WSBlockGeneric generic;
};

typedef enum {
    Acurite_606TXDecoderStepReset = 0,
    Acurite_606TXDecoderStepSaveDuration,
    Acurite_606TXDecoderStepCheckDuration,
} Acurite_606TXDecoderStep;

const SubGhzProtocolDecoder ws_protocol_acurite_606tx_decoder = {
    .alloc = ws_protocol_decoder_acurite_606tx_alloc,
    .free = ws_protocol_decoder_acurite_606tx_free,

    .feed = ws_protocol_decoder_acurite_606tx_feed,
    .reset = ws_protocol_decoder_acurite_606tx_reset,

    .get_hash_data = ws_protocol_decoder_acurite_606tx_get_hash_data,
    .serialize = ws_protocol_decoder_acurite_606tx_serialize,
    .deserialize = ws_protocol_decoder_acurite_606tx_deserialize,
    .get_string = ws_protocol_decoder_acurite_606tx_get_string,
};

const SubGhzProtocolEncoder ws_protocol_acurite_606tx_encoder = {
    .alloc = ws_protocol_encoder_acurite_606tx_alloc,
    .free = ws_protocol_encoder_acurite_606tx_free,

    .deserialize = ws_protocol_encoder_acurite_606tx_deserialize,
    .stop = ws_protocol_encoder_acurite_606tx_stop,
    .yield = ws_protocol_encoder_acurite_606tx_yield,
};

const SubGhzProtocol ws_protocol_acurite_606tx = {
    .name = WS_PROTOCOL_ACURITE_606TX_NAME,
    .type = SubGhzProtocolWeatherStation,
    .flag = SubGhzProtocolFlag_433 | SubGhzProtocolFlag_315 | SubGhzProtocolFlag_868 |
            SubGhzProtocolFlag_AM | SubGhzProtocolFlag_Decodable | SubGhzProtocolFlag_Send,

    .decoder = &ws_protocol_acurite_606tx_decoder,
    .encoder = &ws_protocol_acurite_606tx_encoder,
};


void* ws_protocol_encoder_acurite_606tx_alloc(SubGhzEnvironment* environment) {
    UNUSED(environment);
    WSProtocolEncoderAcurite_606TX* instance = malloc(sizeof(WSProtocolEncoderAcurite_606TX));

    instance->base.protocol = &ws_protocol_acurite_606tx;
    instance->generic.protocol_name = instance->base.protocol->name;

    instance->encoder.repeat = 1; //dont repeat, only send once
    instance->encoder.size_upload = 136;
    instance->encoder.upload = malloc(instance->encoder.size_upload * sizeof(LevelDuration));
    instance->encoder.is_running = false;
    return instance;
}

void ws_protocol_encoder_acurite_606tx_free(void* context) {
    furi_assert(context);
    WSProtocolEncoderAcurite_606TX* instance = context;
    free(instance->encoder.upload);//free upload before freeing instance
    free(instance);
}

static bool ws_protocol_encoder_acurite_606tx_get_upload(WSProtocolEncoderAcurite_606TX* instance) {
    furi_assert(instance);
    size_t index = 0;
    size_t size_upload = (instance->generic.data_count_bit * 2) + 8;  //Data Count*2 because there is an on/off command, +8 because of the four header bits
    if(size_upload > instance->encoder.size_upload) {
        FURI_LOG_E(TAG, "Size upload exceeds allocated encoder buffer. upload: %d, size_upload: %d", size_upload,instance->encoder.size_upload);
        return false;
    } 
    else 
    {
        instance->encoder.size_upload = size_upload;
    }
    for(uint8_t i = 0; i<4;i++)//header is 4 long bits
    {
        //FURI_LOG_I("transmitting","%u,-%u,0",ws_protocol_acurite_606tx_const.te_long+ws_protocol_acurite_606tx_const.te_short,ws_protocol_acurite_606tx_const.te_long+ws_protocol_acurite_606tx_const.te_short);
        instance->encoder.upload[index++] = level_duration_make(true, (uint32_t)(ws_protocol_acurite_606tx_const.te_short+ws_protocol_acurite_606tx_const.te_long));
        instance->encoder.upload[index++] = level_duration_make(false, (uint32_t)(ws_protocol_acurite_606tx_const.te_short+ws_protocol_acurite_606tx_const.te_long));
    }
    for(uint8_t i = instance->generic.data_count_bit; i > 0; i--) {
        if(bit_read(instance->generic.data, i - 1)) {
            //send bit 1
            //FURI_LOG_I("transmitting","%u,-%u,1",ws_protocol_acurite_606tx_const.te_long,ws_protocol_acurite_606tx_const.te_short);
            instance->encoder.upload[index++] = level_duration_make(true, (uint32_t)ws_protocol_acurite_606tx_const.te_long);
            instance->encoder.upload[index++] = level_duration_make(false, (uint32_t)ws_protocol_acurite_606tx_const.te_short);
        } 
        else {
            //send bit 0
            //FURI_LOG_I("transmitting","%u,-%u,0",ws_protocol_acurite_606tx_const.te_short,ws_protocol_acurite_606tx_const.te_long);
            instance->encoder.upload[index++] = level_duration_make(true, (uint32_t)ws_protocol_acurite_606tx_const.te_short);
            instance->encoder.upload[index++] = level_duration_make(false, (uint32_t)ws_protocol_acurite_606tx_const.te_long);
        }
    }
    return true;
}



SubGhzProtocolStatus ws_protocol_encoder_acurite_606tx_deserialize(void* context, FlipperFormat* flipper_format) {
    furi_assert(context);
    WSProtocolEncoderAcurite_606TX* instance = context;
    SubGhzProtocolStatus ret = SubGhzProtocolStatusError;
    //FURI_LOG_I(TAG,"Updating instance->data");
    //ws_protocol_acurite_606tx_reconstructor(&instance->generic);
    FURI_LOG_I(TAG,"Sending Deserializing Acurite Message");
    do
    {
        ret = ws_block_generic_deserialize_check_count_bit(&instance->generic,flipper_format,ws_protocol_acurite_606tx_const.min_count_bit_for_found);
        if(ret != SubGhzProtocolStatusOk){
            break;
        }
        //flipper_format_read_uint32(flipper_format, "Repeat", (uint32_t*)&instance->encoder.repeat, 1); //Optional Parameter(Dont know what it does)
        if(!ws_protocol_encoder_acurite_606tx_get_upload(instance)){
            ret = SubGhzProtocolStatusErrorEncoderGetUpload;
            break;
        }
        instance->encoder.is_running = true;
    } while (false);
    
    return ret;
}

void ws_protocol_encoder_acurite_606tx_stop(void* context){
    furi_assert(context);
    WSProtocolEncoderAcurite_606TX* instance = context;
    instance->encoder.is_running = false;
}

LevelDuration ws_protocol_encoder_acurite_606tx_yield(void* context) {
    WSProtocolEncoderAcurite_606TX* instance = context;

    if(instance->encoder.repeat == 0 || !instance->encoder.is_running) {
        instance->encoder.is_running = false;
        return level_duration_reset();
    }

    LevelDuration ret = instance->encoder.upload[instance->encoder.front];

    if(++instance->encoder.front == instance->encoder.size_upload) {
        instance->encoder.repeat--;
        instance->encoder.front = 0;
    }

    return ret;
}

void ws_protocol_acurite_606tx_reconstructor(WSBlockGeneric* instance) { //added for encoding  NOT DONE YET
    uint64_t data=0;
    data = (data<<8) | instance->id; //rolling code(id) added to data
    FURI_LOG_I("Id Reconstructing","%llx",data);

    //instance->id = (instance->data >> 40) & 0x3FFF;
    uint8_t battery = (!instance->battery_low)*8;
    data = (data<<4) | battery; //battery added to data
    FURI_LOG_I("Battery Reconstructing","%llx",data);
    uint16_t temp_raw;
    if(instance->temp<0)
    {
        temp_raw = (~(uint16_t)(instance->temp*-10))+1;
    }
    else
    {
        temp_raw = (uint16_t)(instance->temp*10);
    }
    data = (data<<12) | temp_raw;  //temp added to data
    FURI_LOG_I("Temperature Reconstructing","%llx",data);
    
    uint8_t msg[] = {
        data >> 16,
        data >> 8,
        data};
    UNUSED(msg);
    //uint8_t checksum = (msg[0]+msg[1]+msg[2]+msg[3]+msg[4]+msg[5]) % 256; //Modulo 256 Checksum
    //FURI_LOG_I("Checksum Reconstructing","%x",checksum);
    //data = (data<<8) | checksum; //added checksum to data
    FURI_LOG_I("Done Reconstructing","%llx",data);
    instance->data = data;
}


void* ws_protocol_decoder_acurite_606tx_alloc(SubGhzEnvironment* environment) {
    UNUSED(environment);
    WSProtocolDecoderAcurite_606TX* instance = malloc(sizeof(WSProtocolDecoderAcurite_606TX));
    instance->base.protocol = &ws_protocol_acurite_606tx;
    instance->generic.protocol_name = instance->base.protocol->name;
    return instance;
}

void ws_protocol_decoder_acurite_606tx_free(void* context) {
    furi_assert(context);
    WSProtocolDecoderAcurite_606TX* instance = context;
    free(instance);
}

void ws_protocol_decoder_acurite_606tx_reset(void* context) {
    furi_assert(context);
    WSProtocolDecoderAcurite_606TX* instance = context;
    instance->decoder.parser_step = Acurite_606TXDecoderStepReset;
}

static bool ws_protocol_acurite_606tx_check(WSProtocolDecoderAcurite_606TX* instance) {
    if(!instance->decoder.decode_data) return false;
    uint8_t msg[] = {
        instance->decoder.decode_data >> 24,
        instance->decoder.decode_data >> 16,
        instance->decoder.decode_data >> 8};

    uint8_t crc = subghz_protocol_blocks_lfsr_digest8(msg, 3, 0x98, 0xF1);
    return (crc == (instance->decoder.decode_data & 0xFF));
}

/**
 * Analysis of received data
 * @param instance Pointer to a WSBlockGeneric* instance
 */
static void ws_protocol_acurite_606tx_remote_controller(WSBlockGeneric* instance) {
    instance->id = (instance->data >> 24) & 0xFF;
    instance->battery_low = (instance->data >> 23) & 1;

    instance->channel = WS_NO_CHANNEL;

    if(!((instance->data >> 19) & 1)) {
        instance->temp = (float)((instance->data >> 8) & 0x07FF) / 10.0f;
    } else {
        instance->temp = (float)((~(instance->data >> 8) & 0x07FF) + 1) / -10.0f;
    }
    instance->btn = WS_NO_BTN;
    instance->humidity = WS_NO_HUMIDITY;
}

void ws_protocol_decoder_acurite_606tx_feed(void* context, bool level, uint32_t duration) {
    furi_assert(context);
    WSProtocolDecoderAcurite_606TX* instance = context;

    switch(instance->decoder.parser_step) {
    case Acurite_606TXDecoderStepReset:
        if((!level) && (DURATION_DIFF(duration, ws_protocol_acurite_606tx_const.te_short * 17) <
                        ws_protocol_acurite_606tx_const.te_delta * 8)) {
            //Found syncPrefix
            instance->decoder.parser_step = Acurite_606TXDecoderStepSaveDuration;
            instance->decoder.decode_data = 0;
            instance->decoder.decode_count_bit = 0;
        }
        break;

    case Acurite_606TXDecoderStepSaveDuration:
        if(level) {
            instance->decoder.te_last = duration;
            instance->decoder.parser_step = Acurite_606TXDecoderStepCheckDuration;
        } else {
            instance->decoder.parser_step = Acurite_606TXDecoderStepReset;
        }
        break;

    case Acurite_606TXDecoderStepCheckDuration:
        if(!level) {
            if(DURATION_DIFF(instance->decoder.te_last, ws_protocol_acurite_606tx_const.te_short) <
               ws_protocol_acurite_606tx_const.te_delta) {
                if((DURATION_DIFF(duration, ws_protocol_acurite_606tx_const.te_short) <
                    ws_protocol_acurite_606tx_const.te_delta) ||
                   (duration > ws_protocol_acurite_606tx_const.te_long * 3)) {
                    //Found syncPostfix
                    instance->decoder.parser_step = Acurite_606TXDecoderStepReset;
                    if((instance->decoder.decode_count_bit ==
                        ws_protocol_acurite_606tx_const.min_count_bit_for_found) &&
                       ws_protocol_acurite_606tx_check(instance)) {
                        instance->generic.data = instance->decoder.decode_data;
                        instance->generic.data_count_bit = instance->decoder.decode_count_bit;
                        ws_protocol_acurite_606tx_remote_controller(&instance->generic);
                        if(instance->base.callback)
                            instance->base.callback(&instance->base, instance->base.context);
                    }
                    instance->decoder.decode_data = 0;
                    instance->decoder.decode_count_bit = 0;
                } else if(
                    DURATION_DIFF(duration, ws_protocol_acurite_606tx_const.te_long) <
                    ws_protocol_acurite_606tx_const.te_delta * 2) {
                    subghz_protocol_blocks_add_bit(&instance->decoder, 0);
                    instance->decoder.parser_step = Acurite_606TXDecoderStepSaveDuration;
                } else if(
                    DURATION_DIFF(duration, ws_protocol_acurite_606tx_const.te_long * 2) <
                    ws_protocol_acurite_606tx_const.te_delta * 4) {
                    subghz_protocol_blocks_add_bit(&instance->decoder, 1);
                    instance->decoder.parser_step = Acurite_606TXDecoderStepSaveDuration;
                } else {
                    instance->decoder.parser_step = Acurite_606TXDecoderStepReset;
                }
            } else {
                instance->decoder.parser_step = Acurite_606TXDecoderStepReset;
            }
        } else {
            instance->decoder.parser_step = Acurite_606TXDecoderStepReset;
        }
        break;
    }
}

uint8_t ws_protocol_decoder_acurite_606tx_get_hash_data(void* context) {
    furi_assert(context);
    WSProtocolDecoderAcurite_606TX* instance = context;
    return subghz_protocol_blocks_get_hash_data(
        &instance->decoder, (instance->decoder.decode_count_bit / 8) + 1);
}

SubGhzProtocolStatus ws_protocol_decoder_acurite_606tx_serialize(
    void* context,
    FlipperFormat* flipper_format,
    SubGhzRadioPreset* preset) {
    furi_assert(context);
    WSProtocolDecoderAcurite_606TX* instance = context;
    return ws_block_generic_serialize(&instance->generic, flipper_format, preset);
}

SubGhzProtocolStatus
    ws_protocol_decoder_acurite_606tx_deserialize(void* context, FlipperFormat* flipper_format) {
    furi_assert(context);
    WSProtocolDecoderAcurite_606TX* instance = context;
    return ws_block_generic_deserialize_check_count_bit(
        &instance->generic,
        flipper_format,
        ws_protocol_acurite_606tx_const.min_count_bit_for_found);
}

void ws_protocol_decoder_acurite_606tx_get_string(void* context, FuriString* output) {
    furi_assert(context);
    WSProtocolDecoderAcurite_606TX* instance = context;
    furi_string_printf(
        output,
        "%s %dbit\r\n"
        "Key:0x%lX%08lX\r\n"
        "Sn:0x%lX Ch:%d  Bat:%d\r\n"
        "Temp:%3.1f C Hum:%d%%",
        instance->generic.protocol_name,
        instance->generic.data_count_bit,
        (uint32_t)(instance->generic.data >> 32),
        (uint32_t)(instance->generic.data),
        instance->generic.id,
        instance->generic.channel,
        instance->generic.battery_low,
        (double)instance->generic.temp,
        instance->generic.humidity);
}
