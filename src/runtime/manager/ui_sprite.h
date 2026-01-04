#ifndef _EMMA_UI_SPRITE_H_
#define _EMMA_UI_SPRITE_H_

#include "nkengine/include/texture.h"

typedef enum {
  UISprite_Toolbar_Glow,
  UISprite_Toolbar_Pad,
  UISprite_Toolbar_Icon_Frame,
  UISprite_Toolbar_Icon_Shape,
  UISprite_Toolbar_Icon_Note,
  UISprite_Toolbar_Icon_Persona,
  UISprite_Toolbar_Icon_Selector,
  UISprite_Pod_Base,
  UISprite_Pod_Window,
  UISprite_Persona_Pioneer,
  UISprite_Persona_Striver,
  UISprite_Persona_Giver,
  UISprite_Persona_Artisan,
  UISprite_Persona_Operator,
  UISprite_Persona_Explorer,
  UISprite_Sidebar_Icon_Module,
  UISprite_Sidebar_Icon_Persona,
  UISprite_Statbar_Icon_Bored,
  UISprite_Statbar_Icon_Excited,
  UISprite_Statbar_Icon_Coin,
  UISprite_Statbar_Icon_Gift,
  UISprite_Statbar_Icon_Solo,
  UISprite_Statbar_Icon_Group,
  UISprite_Statbar_Icon_Weight,
  UISprite_Statbar_Icon_Leaf,
  UISprite_Statbar_Window,
  UISprite_Statbar_Gradient_Excitement,
  UISprite_Statbar_Gradient_Reward,
  UISprite_Statbar_Gradient_Socialbounding,
  UISprite_Statbar_Gradient_Friction,
  UISprite_Slider_Knob,
  UISprite_Icon_Info,
  UISprite_Button_Gradient,
} UISprite;

static const TextureAtlasRegion sprites[] =
    {
        [UISprite_Toolbar_Glow] =
            {
                .label = "Toolbar Glow",
                .uv0 = {0.000f, 0.000f},
                .uv1 = {0.682f, 0.174f},
                .size = {1396, 356},
            },
        [UISprite_Toolbar_Pad] =
            {
                .label = "Toolbar Pad",
                .uv0 = {0.469f, 0.174f},
                .uv1 = {0.970f, 0.235f},
                .size = {1026, 126},
            },
        [UISprite_Toolbar_Icon_Frame] =
            {
                .label = "Toolbar Icon Frame",
                .uv0 = {0.682f, 0.000f},
                .uv1 = {0.755f, 0.073f},
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
                .uv0 = {0.755f, 0.000f},
                .uv1 = {0.828f, 0.073f},
                .size = {150, 150},
            },
        [UISprite_Toolbar_Icon_Selector] =
            {
                .label = "Toolbar Icon Selector",
                .uv0 = {0.313f, 0.174f},
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
                .uv1 = {0.313f, 0.330f},
                .size = {320, 320},
            },
        [UISprite_Persona_Pioneer] =
            {
                .label = "Persona Pioneer",
                .uv0 = {0.691f, 0.075f},
                .uv1 = {0.740f, 0.124f},
                .size = {100, 100},
            },
        [UISprite_Persona_Striver] =
            {
                .label = "Persona Striver",
                .uv0 = {0.743f, 0.075f},
                .uv1 = {0.792f, 0.124f},
                .size = {100, 100},
            },
        [UISprite_Persona_Giver] =
            {
                .label = "Persona Giver",
                .uv0 = {0.795f, 0.075f},
                .uv1 = {0.844f, 0.124f},
                .size = {100, 100},
            },
        [UISprite_Persona_Artisan] =
            {
                .label = "Persona Artisan",
                .uv0 = {0.847f, 0.075f},
                .uv1 = {0.896f, 0.124f},
                .size = {100, 100},
            },
        [UISprite_Persona_Operator] =
            {
                .label = "Persona Operator",
                .uv0 = {0.898f, 0.075f},
                .uv1 = {0.947f, 0.124f},
                .size = {100, 100},
            },
        [UISprite_Persona_Explorer] =
            {
                .label = "Persona Explorer",
                .uv0 = {0.950f, 0.075f},
                .uv1 = {0.999f, 0.124f},
                .size = {100, 100},
            },
        [UISprite_Sidebar_Icon_Module] =
            {
                .label = "Sidebar Icon Module",
                .uv0 = {0.682f, 0.124f},
                .uv1 = {0.707f, 0.149f},
                .size = {52, 52},
            },
        [UISprite_Sidebar_Icon_Persona] =
            {
                .label = "Sidebar Icon Persona",
                .uv0 = {0.707f, 0.124f},
                .uv1 = {0.732f, 0.149f},
                .size = {52, 52},
            },
        [UISprite_Statbar_Icon_Bored] =
            {
                .label = "Statbar Icon Bored",
                .uv0 = {0.756f, 0.124f},
                .uv1 = {0.771f, 0.140f},
                .size = {32, 32},
            },
        [UISprite_Statbar_Icon_Excited] =
            {
                .label = "Statbar Icon Excited",
                .uv0 = {0.771f, 0.124f},
                .uv1 = {0.787f, 0.140f},
                .size = {32, 32},
            },
        [UISprite_Statbar_Icon_Coin] =
            {
                .label = "Statbar Icon Coin",
                .uv0 = {0.787f, 0.124f},
                .uv1 = {0.803f, 0.140f},
                .size = {32, 32},
            },
        [UISprite_Statbar_Icon_Gift] =
            {
                .label = "Statbar Icon Gift",
                .uv0 = {0.803f, 0.124f},
                .uv1 = {0.818f, 0.140f},
                .size = {32, 32},
            },
        [UISprite_Statbar_Icon_Solo] =
            {
                .label = "Statbar Icon Solo",
                .uv0 = {0.756f, 0.140f},
                .uv1 = {0.771f, 0.155f},
                .size = {32, 32},
            },
        [UISprite_Statbar_Icon_Group] =
            {
                .label = "Statbar Icon Group",
                .uv0 = {0.771f, 0.140f},
                .uv1 = {0.787f, 0.155f},
                .size = {32, 32},
            },
        [UISprite_Statbar_Icon_Weight] =
            {
                .label = "Statbar Icon Weight",
                .uv0 = {0.787f, 0.140f},
                .uv1 = {0.803f, 0.155f},
                .size = {32, 32},
            },
        [UISprite_Statbar_Icon_Leaf] =
            {
                .label = "Statbar Icon Leaf",
                .uv0 = {0.803f, 0.140f},
                .uv1 = {0.818f, 0.155f},
                .size = {32, 32},
            },
        [UISprite_Statbar_Window] =
            {
                .label = "Statbar Window",
                .uv0 = {0.682f, 0.155f},
                .uv1 = {0.877f, 0.171f},
                .size = {400, 32},
            },
        [UISprite_Statbar_Gradient_Excitement] =
            {
                .label = "Statbar Gradient Excitement",
                .uv0 = {0.469f, 0.235f},
                .uv1 = {0.664f, 0.251f},
                .size = {400, 32},
            },
        [UISprite_Statbar_Gradient_Reward] =
            {
                .label = "Statbar Gradient Reward",
                .uv0 = {0.664f, 0.235f},
                .uv1 = {0.859f, 0.251f},
                .size = {400, 32},
            },
        [UISprite_Statbar_Gradient_Socialbounding] =
            {
                .label = "Statbar Gradient Socialbounding",
                .uv0 = {0.469f, 0.251f},
                .uv1 = {0.664f, 0.267f},
                .size = {400, 32},
            },
        [UISprite_Statbar_Gradient_Friction] =
            {
                .label = "Statbar Gradient Friction",
                .uv0 = {0.664f, 0.251f},
                .uv1 = {0.859f, 0.267f},
                .size = {400, 32},
            },
        [UISprite_Slider_Knob] =
            {
                .label = "Slider Knob",
                .uv0 = {0.732f, 0.124f},
                .uv1 = {0.756f, 0.147f},
                .size = {48, 48},
            },
        [UISprite_Icon_Info] =
            {
                .label = "Icon Info",
                .uv0 = {0.818f, 0.124f},
                .uv1 = {0.834f, 0.140f},
                .size = {32, 32},
            },
        [UISprite_Button_Gradient] =
            {
                .label = "Button Gradient",
                .uv0 = {0.975f, 0.009f},
                .uv1 = {0.987f, 0.054f},
                .size = {26, 92},
            },
};

EXTERN_C_BEGIN

const TextureAtlasRegion *ui_sprite(const UISprite);

EXTERN_C_END

#endif
