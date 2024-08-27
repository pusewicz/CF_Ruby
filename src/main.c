#include "mrb_cf.h"
#include "mrb_cf_sprite.h"
#include <cute.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <stdio.h>
#include <unistd.h>

#define MAIN_RB "main.rb"

mrb_state *mrb;

static void s_update(void *);
static void s_load_main_rb(void);

int main(int argc, char **argv) {
  CF_UNUSED(argc);
  mrb = mrb_open();
  if (!mrb) {
    fprintf(stderr, "Error: Could not initialize MRuby\n");
    return EXIT_FAILURE;
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

  s_load_main_rb();
  cf_make_app(title, display_index, 0, 0, width, height, options, argv[0]);
  cf_set_fixed_timestep(60);
  cf_set_target_framerate(60);

  while (cf_app_is_running()) {
    cf_app_update(s_update);
    cf_app_draw_onto_screen(true);
  }

  mrb_close(mrb);
  cf_destroy_app();
  return EXIT_SUCCESS;
}

static void s_update(void *data) {
  CF_UNUSED(data);
  mrb_funcall(mrb, mrb_top_self(mrb), "update", 0);
  if (mrb->exc) {
    mrb_print_error(mrb);
    mrb->exc = 0;
  }
}

static void s_load_main_rb(void) {
  FILE *f = fopen(MAIN_RB, "r");
  if (!f) {
    fprintf(stderr, "Error: Could not open %s\n", MAIN_RB);
    exit(EXIT_FAILURE);
  }

  mrb_load_file(mrb, f);
  fclose(f);

  if (mrb->exc) {
    mrb_print_error(mrb);
    mrb->exc = 0;
  }
}
