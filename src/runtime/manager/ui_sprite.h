#ifndef _EMMA_UI_SPRITE_H_
#define _EMMA_UI_SPRITE_H_

#include "nkengine/include/texture.h"

// automatially generated with UISprite Plugin
typedef enum {
  UISprite_Toolbar,
  UISprite_Toolbar_Icon_Frame,
  UISprite_Toolbar_Icon_Module,
  UISprite_Toolbar_Icon_Shape,
  UISprite_Toolbar_Icon_Note,
  UISprite_Toolbar_Icon_Persona,
  UISprite_Toolbar_Icon_Selector,
  UISprite_Pod_Base,
  UISprite_Pod_Window,
  UISprite_Persona_Striver,
  UISprite_Persona_Giver,
  UISprite_Persona_Artisan,
  UISprite_Persona_Operator,
  UISprite_Persona_Explorer,
  UISprite_Persona_Pioneer,
} UISprite;

static const TextureAtlasRegion sprites[] = {
    [UISprite_Toolbar] =
        {
            .label = "Toolbar",
            .uv0 = {0.000f, 0.000f},
            .uv1 = {0.682f, 0.174f},
            .size = {1396, 356},
        },
    [UISprite_Toolbar_Icon_Frame] =
        {
            .label = "Toolbar Icon Frame",
            .uv0 = {0.682f, 0.000f},
            .uv1 = {0.755f, 0.073f},
            .size = {150, 150},
        },
    [UISprite_Toolbar_Icon_Module] =
        {
            .label = "Toolbar Icon Module",
            .uv0 = {0.755f, 0.000f},
            .uv1 = {0.828f, 0.073f},
            .size = {150, 150},
        },
    [UISprite_Toolbar_Icon_Shape] =
        {
            .label = "Toolbar Icon Shape",
            .uv0 = {0.828f, 0.000f},
            .uv1 = {0.901f, 0.073f},
            .size = {150, 150},
        },
    [UISprite_Toolbar_Icon_Note] =
        {
            .label = "Toolbar Icon Note",
            .uv0 = {0.901f, 0.000f},
            .uv1 = {0.975f, 0.073f},
            .size = {150, 150},
        },
    [UISprite_Toolbar_Icon_Persona] =
        {
            .label = "Toolbar Icon Persona",
            .uv0 = {0.682f, 0.073f},
            .uv1 = {0.755f, 0.146f},
            .size = {150, 150},
        },
    [UISprite_Toolbar_Icon_Selector] =
        {
            .label = "Toolbar Icon Selector",
            .uv0 = {0.312f, 0.174f},
            .uv1 = {0.469f, 0.330f},
            .size = {320, 320},
        },
    [UISprite_Pod_Base] =
        {
            .label = "Pod Base",
            .uv0 = {0.000f, 0.174f},
            .uv1 = {0.156f, 0.330f},
            .size = {320, 320},
        },
    [UISprite_Pod_Window] =
        {
            .label = "Pod Window",
            .uv0 = {0.156f, 0.174f},
            .uv1 = {0.312f, 0.330f},
            .size = {320, 320},
        },
    [UISprite_Persona_Striver] =
        {
            .label = "Persona Striver",
            .uv0 = {0.756f, 0.078f},
            .uv1 = {0.805f, 0.127f},
            .size = {100, 100},
        },
    [UISprite_Persona_Giver] =
        {
            .label = "Persona Giver",
            .uv0 = {0.805f, 0.078f},
            .uv1 = {0.854f, 0.127f},
            .size = {100, 100},
        },
    [UISprite_Persona_Artisan] =
        {
            .label = "Persona Artisan",
            .uv0 = {0.854f, 0.078f},
            .uv1 = {0.902f, 0.127f},
            .size = {100, 100},
        },
    [UISprite_Persona_Operator] =
        {
            .label = "Persona Operator",
            .uv0 = {0.902f, 0.078f},
            .uv1 = {0.951f, 0.127f},
            .size = {100, 100},
        },
    [UISprite_Persona_Explorer] =
        {
            .label = "Persona Explorer",
            .uv0 = {0.951f, 0.078f},
            .uv1 = {1.000f, 0.127f},
            .size = {100, 100},
        },
    [UISprite_Persona_Pioneer] =
        {
            .label = "Persona Pioneer",
            .uv0 = {0.756f, 0.127f},
            .uv1 = {0.805f, 0.176f},
            .size = {100, 100},
        },
};

EXTERN_C_BEGIN

const TextureAtlasRegion *ui_sprite(const UISprite);

EXTERN_C_END

#endif
