#include "scene_offsets_display.h"

#include "scene_widgets.h"
#include "../helpers/float_representation.h"
#include "../helpers/settings_helper.h"

#include "../pod.h"


void offsets_display_view_redraw_widget(App* app) {

	widget_reset(app->widget);
	widget_add_frame_element(app->widget,0,0,128,64,0);


	uint8_t widgetX = 2;

	uint8_t tempOffY = 3;
	uint8_t humOffY = 23;
	uint8_t pressOffY = 43;

	pod_widgets_redraw_temperature_offset(app, widgetX, tempOffY);
	pod_widgets_redraw_humidity_offset(app, widgetX, humOffY);
	pod_widgets_redraw_pressure_offset(app, widgetX, pressOffY);

}

static bool offsets_display_input_callback(InputEvent* input_event, void* context) {

    App* app = (App*) context;
    bool consumed = false;

    OffsetsDisplayEvent event = {.type = Offsets_Display_Key_Event, .input = *input_event};
    
    int* targetOffset;
    int maxDigit;
    int maxOffset;

    if (event.input.type == InputTypeShort) {
        if (app->offsetState->edit) {
            switch (app->offsetState->selection) {
                case 0: // temp
                    targetOffset = &app->offsetState->rawTemp;
                    maxDigit = 3;
                    maxOffset = 999;
                    break;
                case 1: // humidity
                    targetOffset = &app->offsetState->rawHum;
                    maxDigit = 3;
                    maxOffset = 999;
                    break;
                case 2: // pressure
                    targetOffset = &app->offsetState->rawPres;
                    maxDigit = 5;
                    maxOffset = 99999;
                    break;
                default:
                    FURI_LOG_E(TAG, "Offset selector out of range");
                    targetOffset = &app->offsetState->rawTemp;
                    maxDigit = 0;
                    maxOffset = 0;
                    break;
            }
            switch (event.input.key) {
                case InputKeyBack:
                case InputKeyOk:
                    app->offsetState->edit=false;
                    view_dispatcher_send_custom_event(app->view_dispatcher,Offsets_Display_Input_Event);
                    consumed=true;//we need to de-highlight selection
                    break;
                case InputKeyUp:
                case InputKeyDown:
                    if (app->offsetState->digit == maxDigit) { // If sign is selected
                        *targetOffset *= -1; // Negate value
                        break;
                    } // otherwise
                    int delta;
                    switch (app->offsetState->digit) {
                        case 0:
                            delta = 1;
                            break;
                        case 1:
                            delta = 10;
                            break;
                        case 2:
                            delta = 100;
                            break;
                        case 3:
                            delta = 1000;
                            break;
                        case 4:
                            delta = 10000;
                            break;
                        default:
                            delta = 0;
                            break;
                    }
                    FURI_LOG_I(TAG, "%d, %d, %d", *targetOffset, delta, maxOffset);
                    if (event.input.key == InputKeyUp) {
                        *targetOffset += delta;
                        if (*targetOffset > maxOffset)
                            *targetOffset = maxOffset;
                    }
                    else {
                        *targetOffset -= delta;
                        if (*targetOffset < -maxOffset)
                            *targetOffset = -maxOffset;
                    }
                    break;
                case InputKeyLeft:
                    if (app->offsetState->digit < maxDigit)
                        app->offsetState->digit++;
                    break;
                case InputKeyRight:
                    if (app->offsetState->digit > 0)
                        app->offsetState->digit--;
                    break;
                default:
                    FURI_LOG_E(TAG, "Unexpected input in bagging area");
                    break;
            }

            view_dispatcher_send_custom_event(app->view_dispatcher, Offsets_Display_Input_Event);
            consumed = true;
        }
        else {
            switch (event.input.key) {
                case InputKeyBack:
                    view_dispatcher_send_custom_event(app->view_dispatcher, Offsets_Display_Exit_Event);
                    consumed = true;
                    break;
                case InputKeyOk:
                    app->offsetState->edit = true;
                    view_dispatcher_send_custom_event(app->view_dispatcher, Offsets_Display_Input_Event);
                    consumed = true;
                    break;
                case InputKeyUp:
                    if (app->offsetState->selection > 0) {
                        app->offsetState->selection--;
                        app->offsetState->digit = 0;
                    }
                    view_dispatcher_send_custom_event(app->view_dispatcher, Offsets_Display_Input_Event);
                    consumed = true;
                    break;
                case InputKeyDown:
                    if (app->offsetState->selection < 2) {
                        app->offsetState->selection++;
                        app->offsetState->digit = 0;
                    }
                    view_dispatcher_send_custom_event(app->view_dispatcher, Offsets_Display_Input_Event);
                    consumed = true;
                    break;
                default:
                    view_dispatcher_send_custom_event(app->view_dispatcher, Offsets_Display_Input_Event);
                    consumed = false;
                    break;
            }
        }
    }
    else {
        view_dispatcher_send_custom_event(app->view_dispatcher, Offsets_Display_Input_Event);
    }

    return consumed;
}

void offsets_display_tick_callback(void* context) {

	App* app = context;
	furi_assert(app->queue);
	FuriMessageQueue* queue = app->queue;

	OffsetsDisplayEvent event = {.type = Offsets_Display_Tick_Event};
	FURI_LOG_I(TAG, "Updating Message Queue");
	furi_message_queue_put(queue, &event, 0);
	scene_manager_handle_tick_event(app->scene_manager);

}

void offsets_display_scene_on_enter(void* context) {

	FURI_LOG_I(TAG, "Offsets Display Scene entered");
    App* app = context;

    widget_reset(app->widget);
    app->canvas_y_offset = 0;

    //Queue for events(Ticks or input)
    app->queue = furi_message_queue_alloc(8,sizeof(OffsetsDisplayEvent));

    app->offsetState->rawTemp = toIntRep(app->settings->temp_offset);
    app->offsetState->rawPres = toIntRep(app->settings->pressure_offset);
    app->offsetState->rawHum = toIntRep(app->settings->humidity_offset);

    offsets_display_view_redraw_widget(app);
    view_set_context(widget_get_view(app->widget), app);

    view_set_input_callback(widget_get_view(app->widget), offsets_display_input_callback);
    view_dispatcher_switch_to_view(app->view_dispatcher, Pod_Widget_View);

}

bool offsets_display_scene_on_event(void* context, SceneManagerEvent event) {

	App* app = context;
    bool consumed = false;

    switch(event.type){
        case SceneManagerEventTypeTick:
            FURI_LOG_I(TAG,"Scene Manager Event Type Tick");
            OffsetsDisplayEvent offsets_event;
            if(furi_message_queue_get(app->queue, &offsets_event, FuriWaitForever) == FuriStatusOk) {
                switch(offsets_event.type) {
                    case Offsets_Display_Tick_Event:
                        FURI_LOG_I(TAG,"PWS Tick Event");
                        FURI_LOG_I(TAG,"Updating Screen");
                        furi_mutex_acquire(app->mutex, FuriWaitForever);
                        offsets_display_view_redraw_widget(app);//redraw widgets
                        furi_mutex_release(app->mutex);
                        consumed = true;
                        //furi_mutex_release(app->mutex);
                        break;
                    default:
                        break;
                }
            } 
            else {
                // We had an issue getting message from the queue, so exit application.
                FURI_LOG_E(TAG,"Scene Manger Event Type Tick Error");
            }
            break;
        case SceneManagerEventTypeCustom:
            switch(event.event){
                case Offsets_Display_Exit_Event:
                    //scene_manager_next_scene(app->scene_manager,Pod_Main_Menu_Scene);
                    FURI_LOG_I(TAG,"Offsets Exit Event");
                    consumed = true;
                    scene_manager_previous_scene(app->scene_manager);
                    break;
                case Offsets_Display_Input_Event:
                    offsets_display_view_redraw_widget(app);//redraw widgets
                    consumed = true;
                    break;
                default:
                    consumed = false;
                    break;
            }
            break;
        default:
            break;
    }
    return consumed;
}

void offsets_display_scene_on_exit(void* context) {
    FURI_LOG_I(TAG, "Exiting Offsets Scene");
    App* app = (App*) context;
    furi_message_queue_free(app->queue);
    widget_reset(app->widget);

    app->settings->temp_offset = fromIntRep(app->offsetState->rawTemp);
    app->settings->humidity_offset = fromIntRep(app->offsetState->rawHum);
    app->settings->pressure_offset = fromIntRep(app->offsetState->rawPres);
}
