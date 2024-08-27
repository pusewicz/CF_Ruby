#include "cf.h"

struct RClass *mrb_cute_sprite;

static mrb_value mrb_cf_display_count(mrb_state *mrb, mrb_value self) {
  return mrb_fixnum_value(cf_display_count());
}

static mrb_value mrb_cf_version_string_linked(mrb_state *mrb, mrb_value self) {
  return mrb_str_new_cstr(mrb, cf_version_string_linked());
}

// CF_Result cf_make_app(const char* window_title, int display_index, int x, int
// y, int w, int h, int options, const char* argv0);
static mrb_value mrb_cf_make_app(mrb_state *mrb, mrb_value self) {
  char *window_title;
  mrb_int display_index, x, y, w, h, options;
  mrb_int argv0;
  mrb_get_args(mrb, "ziiiiiii", &window_title, &display_index, &x, &y, &w, &h,
               &options, &argv0);
  cf_make_app(window_title, display_index, x, y, w, h, options, NULL);
  return mrb_nil_value();
}

// bool cf_app_is_running();
static mrb_value mrb_cf_app_is_running(mrb_state *mrb, mrb_value self) {
  return mrb_bool_value(cf_app_is_running());
}

// void cf_app_update(CF_OnUpdateFn* on_update);
static mrb_value mrb_cf_app_update(mrb_state *mrb, mrb_value self) {
  cf_app_update(NULL);
  return mrb_nil_value();
}

// int cf_app_draw_onto_screen(bool clear);
static mrb_value mrb_cf_app_draw_onto_screen(mrb_state *mrb, mrb_value self) {
  mrb_bool clear;
  mrb_get_args(mrb, "b", &clear);
  return mrb_fixnum_value(cf_app_draw_onto_screen(clear));
}

static void mrb_cf_init(mrb_state *mrb, struct RClass *mrb_cute) {
  mrb_define_module_function(mrb, mrb_cute, "display_count",
                             mrb_cf_display_count, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mrb_cute, "version",
                             mrb_cf_version_string_linked, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mrb_cute, "app_make", mrb_cf_make_app,
                             MRB_ARGS_REQ(7));
  mrb_define_module_function(mrb, mrb_cute, "app_is_running",
                             mrb_cf_app_is_running, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mrb_cute, "app_update", mrb_cf_app_update,
                             MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mrb_cute, "app_draw_onto_screen",
                             mrb_cf_app_draw_onto_screen, MRB_ARGS_REQ(1));
}

void mrb_mruby_cf_gem_init(mrb_state *mrb) {
  struct RClass *mrb_cute = mrb_define_module(mrb, "Cute");

  mrb_cf_init(mrb, mrb_cute);
  mrb_cute_sprite_init(mrb, mrb_cute);
}

void mrb_mruby_cf_gem_final(mrb_state *mrb) {}
