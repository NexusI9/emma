#include "ui_sprite.h"

const TextureAtlasRegion *ui_sprite(const UISprite type) {

  static const float tex_res = 2048.0f;

  static const TextureAtlasRegion region[UISprite_COUNT] = {
      [UISprite_ToolBar] =
          {
              .label = "Navigation bar",
              .uv0 = {0.0f / tex_res, 0.0f / tex_res},
              .uv1 = {698.0f / tex_res, 136.0f / tex_res},
              .size = {698.0f, 136.0f},
          },

  };

  return &region[type];
}
