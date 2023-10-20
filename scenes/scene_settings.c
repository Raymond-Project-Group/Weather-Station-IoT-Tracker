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

void pod_settings_change_time_units(VariableItem* timeChoice)//update time settings
{
    App* app = variable_item_get_context(timeChoice);
    uint8_t index = variable_item_get_current_value_index(timeChoice);
    variable_item_set_current_value_text(timeChoice, TimeNames[index]);
    app->settings->time = index;
}

void pod_settings_scene_on_enter(void* context)
{
    FURI_LOG_I(TAG, "Settings Scene entered");
    App* app = context;
    variable_item_list_reset(app->variable_item_list);//Reset variable item list
    
    VariableItem* temp = variable_item_list_add(app->variable_item_list, "Temperature", Temp_Count, pod_settings_change_temp_units, app); //set to temperature
    variable_item_set_current_value_text(temp, TemperatureNames[app->settings->temperature]); //update to reflect current temp choice
    
    VariableItem* humid = variable_item_list_add(app->variable_item_list, "Humidity", Humid_Count, pod_settings_change_humid_units, app); //set to humidity
    variable_item_set_current_value_text(humid, HumidityNames[app->settings->humidity]); //update to reflect current humidity choice
    
    VariableItem* press = variable_item_list_add(app->variable_item_list, "Pressure", Pressure_Count, pod_settings_change_pressure_units, app); //set to pressure
    variable_item_set_current_value_text(press, PressureNames[app->settings->pressure]); //update to reflect current pressure choice
    
    VariableItem* time = variable_item_list_add(app->variable_item_list, "Time", Time_Count, pod_settings_change_time_units, app); //set to pressure
    variable_item_set_current_value_text(time, TimeNames[app->settings->time]); //update to reflect current pressure choice

    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Variable_Item_List_View);
}
bool pod_settings_scene_on_event(void* context, SceneManagerEvent event)
{
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
void pod_settings_scene_on_exit(void* context)
{
    App* app = context;
    variable_item_list_reset(app->variable_item_list);//Reset variable item list
}