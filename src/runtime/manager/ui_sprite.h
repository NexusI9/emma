#ifndef _EMMA_UI_SPRITE_H_
#define _EMMA_UI_SPRITE_H_

#include "nkengine/include/texture.h"

typedef enum{
	UISprite_Toolbar_Glow,
	UISprite_Toolbar_Pad,
	UISprite_Toolbar_Icon_Frame,
	UISprite_Toolbar_Icon_Cursor,
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
	UISprite_Sidebar_Icon_Module,
	UISprite_Sidebar_Icon_Persona,
} UISprite;


static const TextureAtlasRegion sprites[] = {
	[UISprite_Toolbar_Glow] = {
		.label = "Toolbar Glow",
		.uv0 = { 0.000f, 0.000f },
		.uv1 = { 0.682f, 0.174f },
		.size = { 1396, 356 },
	},
	[UISprite_Toolbar_Pad] = {
		.label = "Toolbar Pad",
		.uv0 = { 0.469f, 0.174f },
		.uv1 = { 0.970f, 0.235f },
		.size = { 1026, 126 },
	},
	[UISprite_Toolbar_Icon_Frame] = {
		.label = "Toolbar Icon Frame",
		.uv0 = { 0.682f, 0.000f },
		.uv1 = { 0.755f, 0.073f },
		.size = { 150, 150 },
	},
	[UISprite_Toolbar_Icon_Cursor] = {
		.label = "Toolbar Icon Cursor",
		.uv0 = { 0.755f, 0.000f },
		.uv1 = { 0.828f, 0.073f },
		.size = { 150, 150 },
	},
	[UISprite_Toolbar_Icon_Shape] = {
		.label = "Toolbar Icon Shape",
		.uv0 = { 0.828f, 0.000f },
		.uv1 = { 0.901f, 0.073f },
		.size = { 150, 150 },
	},
	[UISprite_Toolbar_Icon_Note] = {
		.label = "Toolbar Icon Note",
		.uv0 = { 0.901f, 0.000f },
		.uv1 = { 0.975f, 0.073f },
		.size = { 150, 150 },
	},
	[UISprite_Toolbar_Icon_Persona] = {
		.label = "Toolbar Icon Persona",
		.uv0 = { 0.682f, 0.073f },
		.uv1 = { 0.755f, 0.146f },
		.size = { 150, 150 },
	},
	[UISprite_Toolbar_Icon_Selector] = {
		.label = "Toolbar Icon Selector",
		.uv0 = { 0.312f, 0.174f },
		.uv1 = { 0.469f, 0.330f },
		.size = { 320, 320 },
	},
	[UISprite_Pod_Base] = {
		.label = "Pod Base",
		.uv0 = { 0.000f, 0.174f },
		.uv1 = { 0.156f, 0.330f },
		.size = { 320, 320 },
	},
	[UISprite_Pod_Window] = {
		.label = "Pod Window",
		.uv0 = { 0.156f, 0.174f },
		.uv1 = { 0.312f, 0.330f },
		.size = { 320, 320 },
	},
	[UISprite_Persona_Striver] = {
		.label = "Persona Striver",
		.uv0 = { 0.756f, 0.076f },
		.uv1 = { 0.805f, 0.125f },
		.size = { 100, 100 },
	},
	[UISprite_Persona_Giver] = {
		.label = "Persona Giver",
		.uv0 = { 0.805f, 0.076f },
		.uv1 = { 0.854f, 0.125f },
		.size = { 100, 100 },
	},
	[UISprite_Persona_Artisan] = {
		.label = "Persona Artisan",
		.uv0 = { 0.854f, 0.076f },
		.uv1 = { 0.902f, 0.125f },
		.size = { 100, 100 },
	},
	[UISprite_Persona_Operator] = {
		.label = "Persona Operator",
		.uv0 = { 0.902f, 0.076f },
		.uv1 = { 0.951f, 0.125f },
		.size = { 100, 100 },
	},
	[UISprite_Persona_Explorer] = {
		.label = "Persona Explorer",
		.uv0 = { 0.951f, 0.076f },
		.uv1 = { 1.000f, 0.125f },
		.size = { 100, 100 },
	},
	[UISprite_Persona_Pioneer] = {
		.label = "Persona Pioneer",
		.uv0 = { 0.756f, 0.125f },
		.uv1 = { 0.805f, 0.174f },
		.size = { 100, 100 },
	},
	[UISprite_Sidebar_Icon_Module] = {
		.label = "Sidebar Icon Module",
		.uv0 = { 0.682f, 0.148f },
		.uv1 = { 0.707f, 0.174f },
		.size = { 52, 52 },
	},
	[UISprite_Sidebar_Icon_Persona] = {
		.label = "Sidebar Icon Persona",
		.uv0 = { 0.707f, 0.148f },
		.uv1 = { 0.732f, 0.174f },
		.size = { 52, 52 },
	},
};


EXTERN_C_BEGIN

const TextureAtlasRegion *ui_sprite(const UISprite);

EXTERN_C_END

#endif
