#include "mrb_cf.h"
#include "mrb_cf_sprite.h"
#include <cute.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <stdio.h>

mrb_state *mrb;

void update() {
  mrb_funcall(mrb, mrb_top_self(mrb), "update", 0);
  if (mrb->exc) {
    mrb_print_error(mrb);
    mrb->exc = 0;
  }
}

void load_main_rb() {
  const char *filename = "main.rb";
  FILE *f = fopen(filename, "r");
  if (!f) {
    fprintf(stderr, "Error: Could not open %s\n", filename);
    return;
  }

  mrb_load_file(mrb, f);
  fclose(f);

  if (mrb->exc) {
    mrb_print_error(mrb);
    mrb->exc = 0;
  }

  // Check if init function exists
  /* if (mrb_func_basic_p(mrb, mrb_top_self(mrb), mrb_intern_lit(mrb, "init")))
   * { */
  /*   mrb_funcall(mrb, mrb_top_self(mrb), "init", 0); */
  /* } */
}

int main(int argc, char **argv) {
  mrb = mrb_open();
  if (!mrb) {
    fprintf(stderr, "Error: Could not initialize MRuby\n");
    return 1;
  }
  mrb_cute_define(mrb);

  // TODO: Get data from some config file
  char title[256];
  snprintf(title, sizeof(title), "CF_Ruby (MRuby/%s Cute/%s)", MRUBY_VERSION,
           cf_version_string_linked());
  int display_index = 0;
  int width = 640;
  int height = 480;
  int options =
      CF_APP_OPTIONS_WINDOW_POS_CENTERED_BIT | CF_APP_OPTIONS_RESIZABLE_BIT;

  // Load and execute your Ruby game script
  const char *filename = "main.rb";
  FILE *f = fopen(filename, "r");
  if (!f) {
    fprintf(stderr, "Error: Could not open %s\n", filename);
    mrb_close(mrb);
    return 1;
  }

  mrb_load_file(mrb, f);
  fclose(f);

  if (mrb->exc) {
    mrb_print_error(mrb);
    mrb->exc = 0;
  }

  cf_make_app(title, display_index, 0, 0, width, height, options, argv[0]);
  cf_set_fixed_timestep(60);
  cf_set_target_framerate(60);

  while (cf_app_is_running()) {
    cf_app_update(update);
    cf_app_draw_onto_screen(true);
  }

  mrb_close(mrb);
  return 0;
}
