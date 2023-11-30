#include "scene_settings_hub.h"

#include "../app.h"
#include "scenes.h"

void pod_settings_hub_callback(void* context, uint32_t index) {
	App* app = context;
    switch(index) {
		case Pod_General_Settings_Selection:
			scene_manager_handle_custom_event(
				app->scene_manager, Pod_General_Settings_Selection_Event);
			break;
		case Pod_Offsets_Selection:
			scene_manager_handle_custom_event(
				app->scene_manager, Pod_Offsets_Selection_Event);
			break;
		}
}

void pod_settings_hub_scene_on_enter(void* context) {
	App* app = context;
	submenu_reset(app->submenu);
	submenu_set_header(app->submenu, "Settings");
	submenu_add_item(app->submenu, "General Settings", Pod_General_Settings_Selection, pod_settings_hub_callback, app);
	submenu_add_item(app->submenu, "Offsets", Pod_Offsets_Selection, pod_settings_hub_callback, app);
	view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Submenu_View);
}

bool pod_settings_hub_scene_on_event(void* context, SceneManagerEvent event) {
	App* app = context;
	if (event.type == SceneManagerEventTypeCustom) {
		switch (event.event) {
			case Pod_General_Settings_Selection_Event:
				FURI_LOG_I("POD", "Entering General Settings Scene");
				scene_manager_next_scene(app->scene_manager, Pod_Settings_Scene);
				return true;
			case Pod_Offsets_Selection_Event:
				FURI_LOG_I("POD", "Entering Offsets Scene");
				scene_manager_next_scene(app->scene_manager, Pod_Offsets_Scene);
				return true;
		}
	}
    return false;
}

void pod_settings_hub_scene_on_exit(void* context) {
	App* app = context;
	submenu_reset(app->submenu);
}
