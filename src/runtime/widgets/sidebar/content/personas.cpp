#include "personas.hpp"
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
                  ui_sprite(UISprite_Persona_Artisan)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Explorer)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Giver)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Operator)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Pioneer)),
          },
          {
              .avatar = ::Component::Sprite(
                  texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                  ui_sprite(UISprite_Persona_Striver)),
          },
      } {

  layout();
}

void Widget::SideBar::Content::Personas::Component::draw() {

  draw_header("Personas");

  for (uint8_t i = 0; i < PersonaType_COUNT; i++)
    draw_card(&personas[i], active_persona == i);
}

void Widget::SideBar::Content::Personas::Component::draw_card(struct persona *p,
                                                              bool active) {

  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 win_pos = ImGui::GetWindowPos();

  ImVec2 frame_p0 =
      ImVec2(win_pos.x + p->frame_coo.p0[0], win_pos.y + p->frame_coo.p0[1]);

  ImVec2 frame_p1 =
      ImVec2(win_pos.x + p->frame_coo.p1[0], win_pos.y + p->frame_coo.p1[1]);

  dl->AddRectFilled(frame_p0, frame_p1,
                    im_color(emma_color(ThemeEmmaColor_Surface_Low)), radius);

  dl->AddRect(frame_p0, frame_p1,
              im_color(emma_color(ThemeEmmaColor_Border_Subtle_On_Dark)),
              radius, 0, thickness);

  dl->AddImage((ImTextureRef)p->avatar.get_view(),
               ImVec2(win_pos.x + p->avatar.get_start().x,
                      win_pos.y + p->avatar.get_start().y),
               ImVec2(win_pos.x + p->avatar.get_end().x,
                      win_pos.y + p->avatar.get_end().y),
               im_vec2((float *)p->avatar.region->uv0),
               im_vec2((float *)p->avatar.region->uv1));

  dl->AddText(ImVec2(win_pos.x + p->label_coo.x, win_pos.y + p->label_coo.y),
              im_color(emma_color(ThemeEmmaColor_Text_On_Dark)), p->label);
}

void Widget::SideBar::Content::Personas::Component::layout() {

  static const char *persona_labels[] = {
      "Artisan", "Explorer", "Giver", "Operator", "Pioneer", "Striver",
  };

  for (uint8_t i = 0; i < PersonaType_COUNT; i++) {

    struct persona *p = &personas[i];

    p->label = persona_labels[i];

    ImVec2 frame_start = ImVec2(0.0f, base_y + i * (size.y + gap));

    // frame coordinates
    glm_vec2_copy((vec2){frame_start.x, frame_start.y}, p->frame_coo.p0);
    glm_vec2_copy((vec2){size.x, frame_start.y + size.y}, p->frame_coo.p1);

    // avatar coordinates
    p->avatar.set_position(
        ImVec2(frame_start.x + padding.x, frame_start.y + padding.y),
        GuiSpriteAnchor_TopLeft);

    // label coordinates
    p->label_coo = ImVec2(p->avatar.get_end().x + gap,
                          frame_start.y + size.y / 2 -
                              (float)gui_scale(gui, ImGui::GetFontSize()) / 2);
  }
}
