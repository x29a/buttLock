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
  window_single_click_subscribe(BUTTON_ID_UP, ignore_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, ignore_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, ignore_click_handler);
}

static void line_layer_update_callback(Layer* layer, GContext* ctx)
{
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

static void window_load(Window* window)
{
  Layer* window_layer = window_get_root_layer(window);

  int date_x = 8;
  int date_y = 68;
  int date_w = 130;
  int date_h = 28;
  int time_x = 7;
  int time_y = 92;
  int time_w = 130;
  int time_h = 49;
  int line_x = 8;
  int line_y = 97;
  int line_w = 139;
  int line_h = 2;

#if defined(PBL_PLATFORM_CHALK)
  date_x = 8 + 15;
  time_x = 7 + 15;
  line_x = 8 + 15;
#elif PBL_DISPLAY_HEIGHT == 228
  date_x = (8 * 200) / 144;
  date_y = (68 * 228) / 168;
  date_w = (130 * 200) / 144;
  date_h = (28 * 228) / 168;
  time_x = (7 * 200) / 144;
  time_y = (92 * 228) / 168;
  time_w = (130 * 200) / 144;
  time_h = (49 * 228) / 168;
  line_x = (8 * 200) / 144;
  line_y = (97 * 228) / 168;
  line_w = (139 * 200) / 144;
#elif PBL_DISPLAY_WIDTH == 260
  date_x = ((8 + 15) * 260) / 180;
  date_y = (68 * 260) / 180;
  date_w = (130 * 260) / 180;
  date_h = (28 * 260) / 180;
  time_x = ((7 + 15) * 260) / 180;
  time_y = (92 * 260) / 180;
  time_w = (130 * 260) / 180;
  time_h = (49 * 260) / 180;
  line_x = ((8 + 15) * 260) / 180;
  line_y = (97 * 260) / 180;
  line_w = (139 * 260) / 180;
#endif

  g_date_text_layer = text_layer_create(GRect(date_x, date_y, date_w, date_h));
  text_layer_set_font(g_date_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_color(g_date_text_layer, GColorWhite);
  text_layer_set_background_color(g_date_text_layer, GColorBlack);
  text_layer_set_text_alignment(g_date_text_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(g_date_text_layer));

  g_time_text_layer = text_layer_create(GRect(time_x, time_y, time_w, time_h));
  text_layer_set_font(g_time_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_color(g_time_text_layer, GColorWhite);
  text_layer_set_background_color(g_time_text_layer, GColorBlack);
  text_layer_set_text_alignment(g_time_text_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(g_time_text_layer));

  g_line_layer = layer_create(GRect(line_x, line_y, line_w, line_h));
  layer_set_update_proc(g_line_layer, line_layer_update_callback);
  layer_add_child(window_layer, g_line_layer);
}

static void tick_handler(struct tm* tick_time, TimeUnits units_changed)
{
  static char time[] = "1337";
  static char date[] = "08152342";

  if ((units_changed & MINUTE_UNIT) != 0)
  {
    strftime(time, sizeof(time), "%H%M", tick_time);
    text_layer_set_text(g_time_text_layer, time);
  }

  if ((units_changed & DAY_UNIT) != 0)
  {
    strftime(date, sizeof(date), "%Y%m%d", tick_time);
    text_layer_set_text(g_date_text_layer, date);
  }
}

static void window_unload(Window* window)
{
  text_layer_destroy(g_time_text_layer);
  text_layer_destroy(g_date_text_layer);
  layer_destroy(g_line_layer);
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
