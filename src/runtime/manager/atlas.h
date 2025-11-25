#ifndef _EMMA_ATLAS_H_
#define _EMMA_ATLAS_H_

#include "nkengine/include/texture.h"

extern TextureAtlas g_atlas;

typedef enum {
  TextureAtlasLayer_Module,
  TextureAtlasLayer_UI,
} TextureAtlasLayer;

EXTERN_C_BEGIN

void atlas_create();

EXTERN_C_END

#endif
