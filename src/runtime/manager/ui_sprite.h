#ifndef _EMMA_UI_SPRITE_H_
#define _EMMA_UI_SPRITE_H_

#include "nkengine/include/texture.h"

typedef enum {
  UISprite_ToolBar,
  UISprite_ToolBar_Icon_Frame,
  UISprite_ToolBar_Icon_Module,
  UISprite_ToolBar_Icon_Shape,
  UISprite_ToolBar_Icon_Note,
  UISprite_ToolBar_Icon_Persona,
  UISprite_COUNT,
} UISprite;

EXTERN_C_BEGIN

const TextureAtlasRegion *ui_sprite(const UISprite);

EXTERN_C_END

#endif
