#ifndef _EMMA_UI_SPRITE_H_
#define _EMMA_UI_SPRITE_H_

#include "nkengine/include/texture.h"

typedef enum {
  UISprite_ToolBar,
  UISprite_IconFrame,
  UISprite_IconModule,
  UISprite_COUNT,
} UISprite;

EXTERN_C_BEGIN

const TextureAtlasRegion *ui_sprite(const UISprite);

EXTERN_C_END

#endif
