#include "ui_sprite.h"

const TextureAtlasRegion *ui_sprite(const UISprite type) {

  return &sprites[type];
}
