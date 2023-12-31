#include "../flipper.h"
#include "../app.h"
#include "../pod.h"
#include "scenes.h"
#include "scene_settings.h"

void pod_settings_change_temp_units(VariableItem* tempChoice)//update temp settings
{
    App* app = variable_item_get_context(tempChoice);
    uint8_t index = variable_item_get_current_value_index(tempChoice);
    variable_item_set_current_value_text(tempChoice, TemperatureNames[index]);
    app->settings->temperature = index;
}

void pod_settings_change_humid_units(VariableItem* humidChoice)//update humid settings
{
    App* app = variable_item_get_context(humidChoice);
    uint8_t index = variable_item_get_current_value_index(humidChoice);
    variable_item_set_current_value_text(humidChoice, HumidityNames[index]);
    app->settings->humidity = index;
}

void pod_settings_change_pressure_units(VariableItem* pressureChoice)//update pressure settings
{
    App* app = variable_item_get_context(pressureChoice);
    uint8_t index = variable_item_get_current_value_index(pressureChoice);
    variable_item_set_current_value_text(pressureChoice, PressureNames[index]);
    app->settings->pressure = index;
}

void pod_settings_change_coordinate_units(VariableItem* coordinateChoice)//update pressure settings
{
    App* app = variable_item_get_context(coordinateChoice);
    uint8_t index = variable_item_get_current_value_index(coordinateChoice);
    variable_item_set_current_value_text(coordinateChoice, CoordinateNames[index]);
    app->settings->coordinates = index;
}

void pod_settings_change_time_units(VariableItem* timeChoice)//update time settings
{
    App* app = variable_item_get_context(timeChoice);
    uint8_t index = variable_item_get_current_value_index(timeChoice);
    variable_item_set_current_value_text(timeChoice, TimeNames[index]);
    app->settings->time = index;
}

void pod_settings_change_log_mode(VariableItem* logChoice) {
    App* app = variable_item_get_context(logChoice);
    uint8_t index = variable_item_get_current_value_index(logChoice);
    variable_item_set_current_value_text(logChoice, LogModeNames[index]);
    app->settings->logMode = index;
}

void pod_settings_change_baud_rate(VariableItem* rateChoice) {
    App* app = variable_item_get_context(rateChoice);
    uint8_t index = variable_item_get_current_value_index(rateChoice);
    variable_item_set_current_value_text(rateChoice, GPSBaudRateLabels[index]);
    app->settings->gps_baudrate = index;
}

void pod_settings_change_frequency(VariableItem* freqChoice) {
    App* app = variable_item_get_context(freqChoice);
    uint8_t index = variable_item_get_current_value_index(freqChoice);
    variable_item_set_current_value_text(freqChoice, FrequencyLables[index]);
    app->settings->freq = index;
}
void pod_settings_scene_on_enter(void* context) {
    FURI_LOG_I(TAG, "Settings Scene entered");
    App* app = context;
    variable_item_list_reset(app->variable_item_list);//Reset variable item list
    
    VariableItem* temp = variable_item_list_add(app->variable_item_list, "Temperature", Temp_Count, pod_settings_change_temp_units, app); //set to temperature
    variable_item_set_current_value_text(temp, TemperatureNames[app->settings->temperature]); //update to reflect current temp choice
    
    VariableItem* humid = variable_item_list_add(app->variable_item_list, "Humidity", Humid_Count, pod_settings_change_humid_units, app); //set to humidity
    variable_item_set_current_value_text(humid, HumidityNames[app->settings->humidity]); //update to reflect current humidity choice
    
    VariableItem* press = variable_item_list_add(app->variable_item_list, "Pressure", Pressure_Count, pod_settings_change_pressure_units, app); //set to pressure
    variable_item_set_current_value_text(press, PressureNames[app->settings->pressure]); //update to reflect current pressure choice

    VariableItem* coord = variable_item_list_add(app->variable_item_list, "Coordinates", Coordinate_Count, pod_settings_change_coordinate_units, app); //set to coordinates
    variable_item_set_current_value_text(coord, CoordinateNames[app->settings->coordinates]); //update to reflect current coordinate choice
    
    VariableItem* time = variable_item_list_add(app->variable_item_list, "Time", Time_Count, pod_settings_change_time_units, app); //set to pressure
    variable_item_set_current_value_text(time, TimeNames[app->settings->time]); //update to reflect current pressure choice

    VariableItem* log = variable_item_list_add(app->variable_item_list, "Log Mode", Log_Mode_Count, pod_settings_change_log_mode, app);
    variable_item_set_current_value_text(log, LogModeNames[app->settings->logMode]);

    VariableItem* rate = variable_item_list_add(app->variable_item_list, "Baud Rate", Baud_Rates_Count, pod_settings_change_baud_rate, app);
    variable_item_set_current_value_text(rate, GPSBaudRateLabels[app->settings->gps_baudrate]);

    VariableItem* freq = variable_item_list_add(app->variable_item_list, "Freq(MHz)", Frequency_Count, pod_settings_change_frequency, app);
    variable_item_set_current_value_text(freq, FrequencyLables[app->settings->freq]);

    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Variable_Item_List_View);

    FURI_LOG_I(TAG, "Settings Scene entrance complete");
}
bool pod_settings_scene_on_event(void* context, SceneManagerEvent event) {
    App* app = context;
    bool consumed = false;
    UNUSED(app);
    UNUSED(event);
    // if(event.type == SceneManagerEventTypeCustom && event.event == ResistorsEditViewUpdateEvent) {
    //     ...
    //     consumed = true;
    // }
    return consumed;
}
void pod_settings_scene_on_exit(void* context) {
    App* app = context;
    variable_item_list_reset(app->variable_item_list); //Reset variable item list
}