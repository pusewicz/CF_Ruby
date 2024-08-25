#include <mruby.h>
#include <mruby/compile.h>
#include <stdio.h>

int main(int argc, char **argv) {
  mrb_state *mrb = mrb_open();
  if (!mrb) { /* handle error */
  }

  // Create multiline Ruby scripa
  const char *ruby_code = "puts 'Hello, MRuby!'\n"
        "Cute.app_make('Fancy Window Title', 0, 50, 50, 640, 480, 0, 0)\n"
        "while Cute.app_is_running\n"
        "Cute.app_update\n"
        "Cute.app_draw_onto_screen(true)\n"
        "end\n"
                          "puts Cute.version\n";

  // Load and execute your Ruby game script
  mrb_load_string(mrb, ruby_code);

  if (mrb->exc) {
    mrb_print_error(mrb);
  }

  mrb_close(mrb);
  return 0;
}
