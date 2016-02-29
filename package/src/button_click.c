#include <pebble.h>

// global vars
static Window* g_window;
static TextLayer* g_time_text_layer;
static TextLayer* g_date_text_layer;
static Layer* g_line_layer;

static void ignore_click_handler(ClickRecognizerRef recognizer, void* context) 
{
  // ignore
}

static void click_config_provider(void* context) 
{ 
  window_single_click_subscribe(BUTTON_ID_BACK, ignore_click_handler);
}

static void line_layer_update_callback(Layer* layer, GContext* ctx) 
{
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static void window_load(Window* window) 
{
  Layer* window_layer = window_get_root_layer(window);

  // x offset is used for different form factors to display time correctly
  int x_offset = 0;

#if defined(PBL_PLATFORM_CHALK)
  x_offset = 15;
#endif

  g_date_text_layer = text_layer_create(GRect(8+x_offset, 68, 130, 28));
  text_layer_set_font(g_date_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_color(g_date_text_layer, GColorWhite);
  text_layer_set_background_color(g_date_text_layer, GColorBlack);
  text_layer_set_text_alignment(g_date_text_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(g_date_text_layer));

  g_time_text_layer = text_layer_create(GRect(7+x_offset, 92, 130, 49));
  text_layer_set_font(g_time_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_color(g_time_text_layer, GColorWhite);
  text_layer_set_background_color(g_time_text_layer, GColorBlack);
  text_layer_set_text_alignment(g_time_text_layer, GTextAlignmentLeft);  
  layer_add_child(window_layer, text_layer_get_layer(g_time_text_layer));

  GRect line_frame = GRect(8+x_offset, 97, 139, 2);
  g_line_layer = layer_create(line_frame);
  layer_set_update_proc(g_line_layer, line_layer_update_callback);
  layer_add_child(window_layer, g_line_layer);
}

static void tick_handler(struct tm* tick_time, TimeUnits units_changed)
{
  static char time[] = "1337";
  static char date[] = "08152342";

  if( (units_changed & MINUTE_UNIT) != 0 ) 
  {
    strftime(time, sizeof(time), "%H%M", tick_time);
    text_layer_set_text(g_time_text_layer, time);
  }

  if( (units_changed & DAY_UNIT) != 0 ) 
  {
    strftime(date, sizeof(date), "%Y%m%d", tick_time);
    text_layer_set_text(g_date_text_layer, date);
  }  
}


static void window_unload(Window* window) 
{
  text_layer_destroy(g_time_text_layer);
  text_layer_destroy(g_date_text_layer);
}

static void init(void) 
{ 
  g_window = window_create();
  window_set_click_config_provider(g_window, click_config_provider);
  window_set_window_handlers(g_window, (WindowHandlers) 
      {
      .load = window_load,
      .unload = window_unload,
      });

  window_set_background_color(g_window, GColorBlack);
  window_stack_push(g_window, false);  

  // call method once to display current time right away
  // generate own time info
  time_t tempTime = time(NULL); 
  struct tm* tick_time = localtime(&tempTime);  
  tick_handler(tick_time, MINUTE_UNIT | DAY_UNIT);

  tick_timer_service_subscribe(MINUTE_UNIT | DAY_UNIT, tick_handler);
}

static void deinit(void) 
{
  window_destroy(g_window);
  tick_timer_service_unsubscribe();
}

int main(void) 
{
  init();
  app_event_loop();
  deinit();
}
