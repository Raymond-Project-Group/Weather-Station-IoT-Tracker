#include "flipper.h"
#include "app.h"
#include "scenes.h"
#include "scene_gpio_display.h"

void pod_gpio_display_scene_on_enter(void* context)
{
    App* app = context;
    text_box_reset(app->text_box);
    char* text = PressureNames[app->settings->pressure];
    text_box_set_text(app->text_box,text);
    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Text_Box_View);
}

bool pod_gpio_display_scene_on_event(void* context, SceneManagerEvent event)
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

void pod_gpio_display_scene_on_exit(void* context)
{
    App* app = context;
    text_box_reset(app->text_box);//Reset variable item list
}