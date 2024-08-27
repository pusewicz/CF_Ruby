#include "sprite.h"

// TODO:
// - double check the DATA_PTR use, rather than the mrb_data_get_ptr
// - check
// https://github.com/mruby/mruby/blob/master/mrbgems/mruby-time/src/time.c#L565
// and https://dev.to/roryo/storing-c-data-in-an-mruby-class-50k4

typedef struct mrb_cute_sprite_data_t {
  CF_Sprite *sprite;
} mrb_cute_sprite_data_t;

static void mrb_cute_sprite_data_free(mrb_state *mrb, void *ptr) {
  // TODO: Do I need to free the sprite?
  mrb_free(mrb, ptr);
}

static struct mrb_data_type const mrb_cute_sprite_data_type = {
    "Sprite",
    mrb_cute_sprite_data_free,
};

// void cf_draw_sprite(const CF_Sprite* sprite);
static mrb_value mrb_cute_sprite_draw(mrb_state *mrb, mrb_value self) {
  mrb_cute_sprite_data_t *data = (mrb_cute_sprite_data_t *)mrb_data_get_ptr(
      mrb, self, &mrb_cute_sprite_data_type);
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "uninitialized data");
  }

  cf_draw_sprite(data->sprite);

  return self;
}

// bool cf_sprite_play(CF_Sprite* sprite, const char* animation)
static mrb_value mrb_cute_sprite_play(mrb_state *mrb, mrb_value self) {
  const char *animation = NULL;
  mrb_get_args(mrb, "z", &animation);
  if (animation == NULL) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "missing animation name");
  }

  mrb_cute_sprite_data_t *data = (mrb_cute_sprite_data_t *)mrb_data_get_ptr(
      mrb, self, &mrb_cute_sprite_data_type);
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "uninitialized data");
  }

  cf_sprite_play(data->sprite, animation);

  return self;
}

// bool cf_sprite_is_playing(CF_Sprite* sprite, const char* animation)
static mrb_value mrb_cute_sprite_is_playing(mrb_state *mrb, mrb_value self) {
  const char *animation = NULL;
  mrb_get_args(mrb, "z", &animation);
  if (animation == NULL) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "missing animation name");
  }

  mrb_cute_sprite_data_t *data = (mrb_cute_sprite_data_t *)mrb_data_get_ptr(
      mrb, self, &mrb_cute_sprite_data_type);
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "uninitialized data");
  }

  return mrb_bool_value(cf_sprite_is_playing(data->sprite, animation));
}

// void cf_sprite_update(CF_Sprite* sprite)
static mrb_value mrb_cute_sprite_update(mrb_state *mrb, mrb_value self) {
  mrb_cute_sprite_data_t *data = (mrb_cute_sprite_data_t *)mrb_data_get_ptr(
      mrb, self, &mrb_cute_sprite_data_type);
  if (data == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "uninitialized data");
  }

  cf_sprite_update(data->sprite);

  return self;
}

static mrb_value mrb_cute_sprite_make_demo_sprite(mrb_state *mrb,
                                                  mrb_value self) {
  // TODO: https://github.com/mruby/mruby/issues/9#issuecomment-147070792
  mrb_cute_sprite_data_t *data =
      (mrb_cute_sprite_data_t *)mrb_malloc(mrb, sizeof(mrb_cute_sprite_data_t));

  CF_Sprite *sprite = malloc(sizeof(CF_Sprite));
  *sprite = cf_make_demo_sprite();
  data->sprite = sprite;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_cute_sprite_data_type;

  // initialize the Sprite object
  // TODO:
  // https://github.com/mruby/mruby/blob/master/mrbgems/mruby-time/src/time.c

  return self;
}

void mrb_cute_sprite_init(mrb_state *mrb, struct RClass *mrb_cute) {
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

void mrb_cute_sprite_free(mrb_state *mrb, mrb_value self) {}
