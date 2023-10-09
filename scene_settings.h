#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H

#include "flipper.h"
void pod_settings_change_temp_units(VariableItem* tempChoice);
void pod_settings_change_humid_units(VariableItem* humidChoice);
void pod_settings_change_pressure_units(VariableItem* pressureChoice);
void pod_settings_change_time_units(VariableItem* timeChoice);//items

void pod_settings_scene_on_enter(void* context);
bool pod_settings_scene_on_event(void* context, SceneManagerEvent event);
void pod_settings_scene_on_exit(void* context);

#endif