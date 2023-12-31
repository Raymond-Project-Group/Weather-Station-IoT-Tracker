#ifndef WEATHER_STATION_HISTORY_H
#define WEATHER_STATION_HISTORY_H
#include <math.h>
#include <furi.h>
#include <furi_hal.h>
#include <lib/flipper_format/flipper_format.h>
#include <lib/subghz/types.h>

typedef struct {
    FuriString* item_str;
    FlipperFormat* flipper_string;
    uint8_t type;
    uint32_t id;
    SubGhzRadioPreset* preset;
} WSHistoryItem;

ARRAY_DEF(WSHistoryItemArray, WSHistoryItem, M_POD_OPLIST)

#define M_OPL_WSHistoryItemArray_t() ARRAY_OPLIST(WSHistoryItemArray, M_POD_OPLIST)

typedef struct {
    WSHistoryItemArray_t data;
} WSHistoryStruct;

struct WSHistory {
    uint32_t last_update_timestamp;
    uint16_t last_index_write;
    uint8_t code_last_hash_data;
    FuriString* tmp_string;
    WSHistoryStruct* history;
};

typedef struct WSHistory WSHistory;
/** History state add key */
typedef enum {
    WSHistoryStateAddKeyUnknown,
    WSHistoryStateAddKeyTimeOut,
    WSHistoryStateAddKeyNewDada,
    WSHistoryStateAddKeyUpdateData,
    WSHistoryStateAddKeyOverflow,
} WSHistoryStateAddKey;

/** Allocate WSHistory
 * 
 * @return WSHistory* 
 */
WSHistory* ws_history_alloc(void);

/** Free WSHistory
 * 
 * @param instance - WSHistory instance
 */
void ws_history_free(WSHistory* instance);

/** Clear history
 * 
 * @param instance - WSHistory instance
 */
void ws_history_reset(WSHistory* instance);

/** Get frequency to history[idx]
 * 
 * @param instance  - WSHistory instance
 * @param idx       - record index  
 * @return frequency - frequency Hz
 */
uint32_t ws_history_get_frequency(WSHistory* instance, uint16_t idx);

SubGhzRadioPreset* ws_history_get_radio_preset(WSHistory* instance, uint16_t idx);

/** Get preset to history[idx]
 * 
 * @param instance  - WSHistory instance
 * @param idx       - record index  
 * @return preset   - preset name
 */
const char* ws_history_get_preset(WSHistory* instance, uint16_t idx);

/** Get history index write 
 * 
 * @param instance  - WSHistory instance
 * @return idx      - current record index  
 */
uint16_t ws_history_get_item(WSHistory* instance);

/** Get type protocol to history[idx]
 * 
 * @param instance  - WSHistory instance
 * @param idx       - record index  
 * @return type      - type protocol  
 */
uint8_t ws_history_get_type_protocol(WSHistory* instance, uint16_t idx);

/** Get name protocol to history[idx]
 * 
 * @param instance  - WSHistory instance
 * @param idx       - record index  
 * @return name      - const char* name protocol  
 */
const char* ws_history_get_protocol_name(WSHistory* instance, uint16_t idx);

/** Get string item menu to history[idx]
 * 
 * @param instance  - WSHistory instance
 * @param output    - FuriString* output
 * @param idx       - record index
 */
void ws_history_get_text_item_menu(WSHistory* instance, FuriString* output, uint16_t idx);

/** Get string the remaining number of records to history
 * 
 * @param instance  - WSHistory instance
 * @param output    - FuriString* output
 * @return bool - is FUUL
 */
bool ws_history_get_text_space_left(WSHistory* instance, FuriString* output);

/** Add protocol to history
 * 
 * @param instance  - Weather Station Context instance
 * @param context    - SubGhzProtocolCommon context
 * @param preset    - SubGhzRadioPreset preset
 * @return WSHistoryStateAddKey;
 */
WSHistoryStateAddKey ws_history_add_to_history(void* wsContext, void* context, SubGhzRadioPreset* preset);

/** Get SubGhzProtocolCommonLoad to load into the protocol decoder bin data
 * 
 * @param instance  - WSHistory instance
 * @param idx       - record index
 * @return SubGhzProtocolCommonLoad*
 */
FlipperFormat* ws_history_get_raw_data(WSHistory* instance, uint16_t idx);

#endif