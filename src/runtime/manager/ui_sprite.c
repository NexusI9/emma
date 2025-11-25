#include "ui_sprite.h"

const TextureAtlasRegion *ui_sprite(const UISprite type) {

  static const float tex_res = 2048.0f;
  static const float scale = 2.0f;

  static const TextureAtlasRegion region[UISprite_COUNT] = {
      [UISprite_ToolBar] =
          {
              .label = "Tool Bar",
              .uv0 = {scale * 0.0f / tex_res, scale * 0.0f / tex_res},
              .uv1 = {scale * 698.0f / tex_res, scale * 178.0f / tex_res},
              .size = {scale * 698.0f, scale * 178.0f},
          },
      [UISprite_ToolBar_Icon_Frame] =
          {
              .label = "Icon Frame",
              .uv0 = {scale * 698.0f / tex_res, scale * 0.0f / tex_res},
              .uv1 = {scale * 773.0f / tex_res, scale * 75.0f / tex_res},
              .size = {scale * 75.0f, scale * 75.0f},
          },
      [UISprite_ToolBar_Icon_Module] =
          {
              .label = "Icon Module",
              .uv0 = {scale * 773.0f / tex_res, scale * 0.0f / tex_res},
              .uv1 = {scale * 848.0f / tex_res, scale * 75.0f / tex_res},
              .size = {scale * 75.0f, scale * 75.0f},
          },
      [UISprite_ToolBar_Icon_Shape] =
          {
              .label = "Icon Shape",
              .uv0 = {scale * 848.0f / tex_res, scale * 0.0f / tex_res},
              .uv1 = {scale * 923.0f / tex_res, scale * 75.0f / tex_res},
              .size = {scale * 75.0f, scale * 75.0f},
          },
      [UISprite_ToolBar_Icon_Note] =
          {
              .label = "Icon Note",
              .uv0 = {scale * 923.0f / tex_res, scale * 0.0f / tex_res},
              .uv1 = {scale * 998.0f / tex_res, scale * 75.0f / tex_res},
              .size = {scale * 75.0f, scale * 75.0f},
          },
      [UISprite_ToolBar_Icon_Persona] =
          {
              .label = "Icon Persona",
              .uv0 = {scale * 698.0f / tex_res, scale * 75.0f / tex_res},
              .uv1 = {scale * 773.0f / tex_res, scale * 150.0f / tex_res},
              .size = {scale * 75.0f, scale * 75.0f},
          },

  };

  return &region[type];
}
