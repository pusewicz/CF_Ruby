#include "mrb_cf_sprite.h"

// TODO:
// - check
// https://github.com/mruby/mruby/blob/master/mrbgems/mruby-time/src/time.c#L565
// and https://dev.to/roryo/storing-c-data-in-an-mruby-class-50k4
// - extract data initialization to a separate function

static struct mrb_data_type const mrb_cute_sprite_data_type = {
    "Sprite",
    mrb_free,
};

static CF_Sprite *s_data_get_ptr(mrb_state *mrb, mrb_value self) {
  CF_Sprite *sprite_ptr =
      DATA_GET_PTR(mrb, self, &mrb_cute_sprite_data_type, CF_Sprite);
  if (sprite_ptr == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "uninitialized data");
  }
  return sprite_ptr;
}

static mrb_value mrb_cute_sprite_draw(mrb_state *mrb, mrb_value self) {
  CF_Sprite *sprite_ptr = s_data_get_ptr(mrb, self);

  cf_draw_sprite(sprite_ptr);

  return self;
}

static mrb_value mrb_cute_sprite_play(mrb_state *mrb, mrb_value self) {
  const char *animation = NULL;
  mrb_get_args(mrb, "z", &animation);
  if (animation == NULL) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "missing animation name");
  }

  CF_Sprite *sprite_ptr = s_data_get_ptr(mrb, self);

  cf_sprite_play(sprite_ptr, animation);

  return self;
}

static mrb_value mrb_cute_sprite_is_playing(mrb_state *mrb, mrb_value self) {
  const char *animation = NULL;
  mrb_get_args(mrb, "z", &animation);
  if (animation == NULL) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "missing animation name");
  }

  CF_Sprite *sprite_ptr = s_data_get_ptr(mrb, self);

  return mrb_bool_value(cf_sprite_is_playing(sprite_ptr, animation));
}

static mrb_value mrb_cute_sprite_update(mrb_state *mrb, mrb_value self) {
  CF_Sprite *sprite_ptr = s_data_get_ptr(mrb, self);

  cf_sprite_update(sprite_ptr);

  return self;
}

static mrb_value mrb_cute_sprite_make_demo_sprite(mrb_state *mrb,
                                                  mrb_value self) {
  // TODO: https://github.com/mruby/mruby/issues/9#issuecomment-147070792
  CF_Sprite *sprite_ptr = (CF_Sprite *)DATA_PTR(self);

  if (sprite_ptr) {
    mrb_free(mrb, sprite_ptr);
  }
  mrb_data_init(self, NULL, &mrb_cute_sprite_data_type);

  CF_Sprite *sprite = (CF_Sprite *)cf_alloc(sizeof(CF_Sprite));
  *sprite = cf_make_demo_sprite();
  sprite_ptr = mrb_malloc(mrb, sizeof(*sprite_ptr));
  sprite_ptr = sprite;

  mrb_data_init(self, sprite_ptr, &mrb_cute_sprite_data_type);

  return self;
}

void mrb_cute_sprite_define(mrb_state *mrb, struct RClass *mrb_cute) {
  struct RClass *mrb_cute_sprite =
      mrb_define_class_under(mrb, mrb_cute, "Sprite", mrb->object_class);
  ;

  MRB_SET_INSTANCE_TT(mrb_cute_sprite, MRB_TT_DATA);

  mrb_define_class_method(mrb, mrb_cute_sprite, "make_demo_sprite",
                          mrb_cute_sprite_make_demo_sprite, MRB_ARGS_NONE());

  mrb_define_method(mrb, mrb_cute_sprite, "initialize",
                    mrb_cute_sprite_make_demo_sprite, MRB_ARGS_NONE());
  mrb_define_method(mrb, mrb_cute_sprite, "draw", mrb_cute_sprite_draw,
                    MRB_ARGS_NONE());
  mrb_define_method(mrb, mrb_cute_sprite, "play", mrb_cute_sprite_play,
                    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb_cute_sprite, "playing?",
                    mrb_cute_sprite_is_playing, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb_cute_sprite, "update", mrb_cute_sprite_update,
                    MRB_ARGS_NONE());
}
