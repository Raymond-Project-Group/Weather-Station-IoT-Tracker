#include "flipper.h"
#include "app.h"
#include "scenes/scenes.h"
#include "scenes/scene_main_menu.h"
#include "scenes/scene_settings.h"
#include "scenes/scene_gpio_display.h"
/* generated by fbt from .png files in images folder */
#include <pod_icons.h>

const char* TAG = "PODS";

int32_t pod_app(void* p) {
    UNUSED(p);
    FURI_LOG_I("TEST", "Hello world");
    FURI_LOG_I("TEST", "I'm pod!");
    
    
    FURI_LOG_I(TAG, "pod app launched");
    
    App* app = app_alloc();
    //app->bme280 = bme_init();
    
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, Pod_Main_Menu_Scene);
    view_dispatcher_run(app->view_dispatcher);

    app_free(app);
    return 0;
}
