#include <cute.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <stdio.h>

int main(int argc, char **argv) {
  mrb_state *mrb = mrb_open();
  if (!mrb) { /* handle error */
  }

  // Get data from JSON
  char *title = "Fancy Window Title";
  int display_index = 0;
  int width = 640;
  int height = 480;
  int options = CF_APP_OPTIONS_WINDOW_POS_CENTERED_BIT | CF_APP_OPTIONS_RESIZABLE_BIT;

  // Create multiline Ruby scripa
  const char *ruby_code = "def update\n"
                            "@sprite ||= Cute::Sprite.new\n"
                            "@sprite.play('spin') unless @sprite.playing?('spin')\n"
                            "@sprite.update\n"
                            "@sprite.draw\n"
                          "end\n";

  // Load and execute your Ruby game script
  mrb_load_string(mrb, ruby_code);

  if (mrb->exc) {
    mrb_print_error(mrb);
  }

  cf_make_app(title, 0, 0, 0, width, height, options, argv[0]);

  while(cf_app_is_running()) {
    cf_app_update(NULL);
    mrb_funcall(mrb, mrb_top_self(mrb), "update", 0);
    if (mrb->exc) {
      mrb_print_error(mrb);
    }

    cf_app_draw_onto_screen(true);
  }

  mrb_close(mrb);
  return 0;
}
