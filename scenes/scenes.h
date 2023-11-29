#ifndef SCENE_H
#define SCENE_H

#include "../flipper.h"
#include "scene_main_menu.h"
#include "scene_settings.h"
#include "scene_gpio_display.h"
#include "scene_pws_selection_menu.h"
#include "scene_pws_display.h"
#include "scene_pod_display.h"
#include "scene_pws_transmit_display.h"

//Current Scene
typedef enum {
    Pod_Main_Menu_Scene,
    Pod_Gpio_Display_Scene,
    Pod_Pws_Display_Scene,
    Pod_Pws_Selection_Scene,
    Pod_Display_Scene,
    Pod_Pws_Transmit_Display_Scene,
    Pod_Settings_Scene,
    Pod_Scene_Count,
} PodScenes;

//Current View
typedef enum {
    Pod_Popup_View,
    Pod_Widget_View,
    Pod_Text_Input_View,
    Pod_Text_Box_View,
    Pod_Submenu_View,
    Pod_Variable_Item_List_View,
} PodViews;

extern void (*const pod_scene_on_enter_handlers[])(void*);
extern bool (*const pod_scene_on_event_handlers[])(void*, SceneManagerEvent);
extern void (*const pod_scene_on_exit_handlers[])(void*);
extern const SceneManagerHandlers pod_scene_manager_handlers;

bool pod_custom_callback(void* context, uint32_t custom_event);
bool pod_back_event_callback(void* context);

#endif