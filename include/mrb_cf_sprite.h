#pragma once

#include <cute_alloc.h>
#include <cute_sprite.h>
#include <mruby.h>
#include <mruby/string.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <stdbool.h>

void mrb_cute_sprite_define(mrb_state *mrb, struct RClass *mrb_cute);
