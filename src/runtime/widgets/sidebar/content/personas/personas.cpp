#include "personas.hpp"
#include "nkengine/vendor/imgui/imgui_internal.h"
#include "resources/theme.emma.h"
#include "runtime/manager/atlas.h"
#include "runtime/manager/theme.h"
#include "runtime/manager/ui_sprite.h"
#include "runtime/node/persona.h"
#include "runtime/widgets/sidebar/content/content.hpp"
#include <cglm/cglm.h>

Widget::SideBar::Content::Personas::Component::Component(Gui *gui)
    : Content::Component(gui),
      personas{
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Operator)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Artisan)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Giver)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Explorer)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Striver)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Pioneer)),
          },
      } {

  layout();
}

void Widget::SideBar::Content::Personas::Component::draw() {

  draw_header("Personas");

  for (uint8_t i = 0; i < PersonaType_COUNT; i++) {
    if (draw_card(&personas[i], active_persona == i)) {

      active_persona = (active_persona == i) ? -1 : i;

      for (uint8_t j = 0; j < persona_click_callbacks.count; j++)
        persona_click_callbacks.entries[j].callback(
            (PersonaType)i, active_persona == i,
            persona_click_callbacks.entries[j].data);
    }
    ImGui::Dummy(ImVec2(0, ROW_GAP));
  }
}

bool Widget::SideBar::Content::Personas::Component::draw_card(struct persona *p,
                                                              bool active) {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 origin = ImGui::GetCursorScreenPos();

  ImVec2 frame_p0 = im_vec2_add(im_vec2(p->frame_coo.p0), origin);
  ImVec2 frame_p1 = im_vec2_add(im_vec2(p->frame_coo.p1), origin);

  ImGui::ItemSize(SIZE);

  dl->AddRectFilled(
      frame_p0, frame_p1,
      im_color(emma_color(active ? ThemeEmmaColor_Background_Brand_Strong
                                 : ThemeEmmaColor_Surface_Low)),
      RADIUS);

  dl->AddRect(
      frame_p0, frame_p1,
      im_color(emma_color(active ? ThemeEmmaColor_Border_Brand_Base
                                 : ThemeEmmaColor_Border_Subtle_On_Dark)),
      RADIUS, 0, THICKNESS);

  p->avatar.draw_at(origin);

  dl->AddText(im_vec2_add(p->label_coo, origin),
              im_color(emma_color(ThemeEmmaColor_Text_On_Dark)), p->label);

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
      ImGui::IsMouseHoveringRect(frame_p0, frame_p1))
    return true;

  return false;
}

void Widget::SideBar::Content::Personas::Component::layout() {

  for (uint8_t i = 0; i < PersonaType_COUNT; i++) {

    struct persona *p = &personas[i];

    p->label = PERSONA_INTRO[i].label;

    // frame coordinates
    glm_vec2_copy(GLM_VEC2_ZERO, p->frame_coo.p0);
    glm_vec2_copy((vec2){SIZE.x, SIZE.y}, p->frame_coo.p1);

    // avatar coordinates
    p->avatar.set_position(ImVec2(PADDING.x, PADDING.y / 2.0f),
                           GuiSpriteAnchor_TopLeft);

    // label coordinates
    p->label_coo = ImVec2(p->avatar.get_end().x + GAP,
                          SIZE.y / 2 - (float)gui_scale(gui, 8));
  }
}
