#include "mrb_cf.h"

struct RClass *mrb_cute_sprite_class;

static mrb_value mrb_cute_display_count(mrb_state *mrb, mrb_value self) {
  CF_UNUSED(mrb);
  CF_UNUSED(self);
  return mrb_fixnum_value(cf_display_count());
}

static mrb_value mrb_cute_version_string_linked(mrb_state *mrb,
                                                mrb_value self) {
  CF_UNUSED(self);
  return mrb_str_new_cstr(mrb, cf_version_string_linked());
}

static mrb_value mrb_cute_make_app(mrb_state *mrb, mrb_value self) {
  CF_UNUSED(self);
  char *window_title;
  mrb_int display_index, x, y, w, h, options;
  mrb_int argv0;
  mrb_get_args(mrb, "ziiiiiii", &window_title, &display_index, &x, &y, &w, &h,
               &options, &argv0);
  cf_make_app(window_title, display_index, x, y, w, h, options, NULL);
  return mrb_nil_value();
}

static mrb_value mrb_cute_app_is_running(mrb_state *mrb, mrb_value self) {
  CF_UNUSED(mrb);
  CF_UNUSED(self);
  return mrb_bool_value(cf_app_is_running());
}

static mrb_value mrb_cute_app_update(mrb_state *mrb, mrb_value self) {
  CF_UNUSED(mrb);
  CF_UNUSED(self);
  cf_app_update(NULL);
  return mrb_nil_value();
}

static mrb_value mrb_cute_app_draw_onto_screen(mrb_state *mrb, mrb_value self) {
  CF_UNUSED(self);
  mrb_bool clear;
  mrb_get_args(mrb, "b", &clear);
  return mrb_fixnum_value(cf_app_draw_onto_screen(clear));
}

void mrb_cute_init(mrb_state *mrb) {
  struct RClass *mrb_cute = mrb_define_module(mrb, "Cute");

  mrb_define_module_function(mrb, mrb_cute, "display_count",
                             mrb_cute_display_count, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mrb_cute, "version",
                             mrb_cute_version_string_linked, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mrb_cute, "app_make", mrb_cute_make_app,
                             MRB_ARGS_REQ(7));
  mrb_define_module_function(mrb, mrb_cute, "app_is_running",
                             mrb_cute_app_is_running, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mrb_cute, "app_update", mrb_cute_app_update,
                             MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mrb_cute, "app_draw_onto_screen",
                             mrb_cute_app_draw_onto_screen, MRB_ARGS_REQ(1));
  mrb_cute_sprite_init(mrb, mrb_cute);
}
