#ifndef SETTINGS_HUB
#define SETTINGS_HUB

#include "../flipper.h"

/** main menu options enum */
typedef enum {
    Pod_General_Settings_Selection,
    Pod_Offsets_Selection,
} PodSettingsHubSceneIndex;

/** main menu events */
typedef enum {
    Pod_General_Settings_Selection_Event,
    Pod_Offsets_Selection_Event,
} PodSettingsHubEvent;

void pod_settings_hub_callback(void* context, uint32_t index);
void pod_settings_hub_scene_on_enter(void* context);
bool pod_settings_hub_scene_on_event(void* context, SceneManagerEvent event);
void pod_settings_hub_scene_on_exit(void* context);







#endif