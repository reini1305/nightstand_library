
#include "nightstand.h"
#include <pebble-effect-layer/pebble-effect-layer.h>

static EffectLayer* effect_layer;
static AccelData a_data;
static BatteryChargeState b_data;
static char time_text[] = "00:00";
static TextLayer *night_time_layer;
static Window *window;
static bool loaded;

static void loadWindow(Window *window) {
  loaded = true;
}
  
static void unloadWindow(Window *window) {
  loaded = false;
}
void nightstand_window_init(void){
  window = window_create();
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  window_set_window_handlers(window, (WindowHandlers) {
    .load = loadWindow,
    .unload = unloadWindow
  });
  loaded = false;
  window_set_background_color(window,GColorBlack);
  effect_layer = effect_layer_create(GRect(0,(bounds.size.h-bounds.size.w)/2-1,bounds.size.w,bounds.size.w));
  night_time_layer = text_layer_create(GRect(0, bounds.size.h/2-25, bounds.size.w, 50));
  text_layer_set_background_color(night_time_layer, GColorBlack);
  text_layer_set_text_color(night_time_layer, GColorWhite);
  text_layer_set_font(night_time_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  text_layer_set_text_alignment(night_time_layer, GTextAlignmentCenter);
  text_layer_set_text(night_time_layer,time_text);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(night_time_layer));
  layer_add_child(window_get_root_layer(window), effect_layer_get_layer(effect_layer));
}

bool nightstand_window_update(void){
  char *time_format;
  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }
  time_t now = time(NULL);
  struct tm* tick_time = localtime(&now);
  strftime(time_text, sizeof(time_text), time_format, tick_time);
  
  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }
  layer_mark_dirty(text_layer_get_layer(night_time_layer));
  accel_service_peek(&a_data);
  b_data = battery_state_service_peek();
  if(b_data.is_plugged) {
    if(a_data.x<-900){ // rotate screen left
      effect_layer_remove_effect(effect_layer);
      effect_layer_add_effect(effect_layer,effect_rotate_90_degrees,(void*)true);
      if(!loaded)
        window_stack_push(window,false);
      return true;
    }
    else if(a_data.x>900) { //rotate screen right
      effect_layer_remove_effect(effect_layer);
      effect_layer_add_effect(effect_layer,effect_rotate_90_degrees,(void*)false);
      if(!loaded)
        window_stack_push(window,false);
      return true;
    }
  }
  if(loaded)
    window_stack_pop(false);
  return false;
}

void nightstand_window_deinit(void){
  text_layer_destroy(night_time_layer);
  effect_layer_destroy(effect_layer);
  window_destroy(window);
}