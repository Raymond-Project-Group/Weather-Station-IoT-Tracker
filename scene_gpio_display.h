#ifndef SCENE_GPIO_DISPLAY_H
#define SCENE_GPIO_DISPLAY_H

#include "flipper.h"

void pod_gpio_display_scene_on_enter(void* context);
bool pod_gpio_display_scene_on_event(void* context, SceneManagerEvent event);
void pod_gpio_display_scene_on_exit(void* context);
#endif