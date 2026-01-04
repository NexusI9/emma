#include "motivation_overview.hpp"
#include "resources/theme.emma.h"
#include "runtime/manager/theme.h"
#include "runtime/node/motivation.h"

Widget::MotivationOverview::Component::Component(Gui *gui,
                                                 ImVec2 window_position)
    : Widget(gui), window_position(window_position), octagon(new_octagon()),
      gauges{
          // === Excitement ===
          {
              .label = "Excitement",
              .gradient =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Gradient_Excitement),
                  },
              .window =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Window),
                  },
              .leading_icon =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Icon_Bored),
                  },
              .trailing_icon =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Icon_Excited),
                  },
          },
          // === Bounding ===
          {
              .label = "Bounding",
              .gradient =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Gradient_Socialbounding),
                  },
              .window =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Window),
                  },
              .leading_icon =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Icon_Solo),
                  },
              .trailing_icon =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Icon_Group),
                  },
          },

          // === Friction ===
          {
              .label = "Friction",
              .gradient =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Gradient_Friction),
                  },
              .window =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Window),
                  },
              .leading_icon =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Icon_Leaf),
                  },
              .trailing_icon =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Icon_Weight),
                  },
          },
          // === Reward ===
          {
              .label = "Reward",
              .gradient =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Gradient_Reward),
                  },
              .window =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Window),
                  },
              .leading_icon = {texture_atlas_layer_view(&g_atlas,
                                                        TextureAtlasLayer_UI),
                               ui_sprite(UISprite_Statbar_Icon_Coin)},
              .trailing_icon =
                  {
                      texture_atlas_layer_view(&g_atlas, TextureAtlasLayer_UI),
                      ui_sprite(UISprite_Statbar_Icon_Gift),
                  },
          },
      } {

  {
    OctagonDescriptor oct_desc = {
        .label = "Motivation Overivew Octagon",
        .inner_color = emma_color(ThemeEmmaColor_Surface_Lowest),
        .position = {WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 3},
        .scale = 80.0f,
        .inner_radius = 0.6f,
    };
    octagon_create(octagon.get_node(), &oct_desc);
    octagon_set_labels(octagon.get_node(), OCTAGON_LABELS_OCTALYSIS);
    octagon_update_labels_coordinates(octagon.get_node());
  }

  layout();
}

void Widget::MotivationOverview::Component::layout() {

  const int OCT_HEIGHT = gui_scale(gui, 200);

  const int BASE_GAUGE_Y = OCT_HEIGHT + ROW_GAP;
  const int BASE_GAUGE_ICON_X = gui_scale(gui, 86);
  const int GAUGE_WIDTH = gui_scale(gui, 200);

  for (uint8_t i = 0; i < GAUGES_COUNT; i++) {
    struct Gauge *g = &gauges[i];
    const int GAUGE_Y = BASE_GAUGE_Y + i * (GAUGE_HEIGHT + GAUGE_GAP);

    g->label_p = ImVec2(PADDING.x, GAUGE_Y);

    g->leading_icon.set_position(ImVec2(PADDING.x + BASE_GAUGE_ICON_X, GAUGE_Y),
                                 GuiSpriteAnchor_TopLeft);

    g->gradient.set_position(
        ImVec2(g->leading_icon.get_end().x + ICON_GAP, GAUGE_Y),
        GuiSpriteAnchor_TopLeft);

    g->window.set_position(
        ImVec2(g->leading_icon.get_end().x + ICON_GAP, GAUGE_Y),
        GuiSpriteAnchor_TopLeft);

    g->trailing_icon.set_position(
        ImVec2(g->window.get_end().x + ICON_GAP, GAUGE_Y),
        GuiSpriteAnchor_TopLeft);

    g->mask_p1 = g->gradient.get_end();
  }
}

void Widget::MotivationOverview::Component::update_gauges_mask() {

  for (uint8_t i = 0; i < GAUGES_COUNT; i++) {

    struct Gauge *g = &gauges[i];

    const float value = motivation_get_element(
        this->motivation,
        (MotivationType)(i + (MotivationType_COUNT - GAUGES_COUNT)));

    const int size =
        g->gradient.region->size[0] - g->gradient.region->size[0] * (1 - value);

    g->mask_p0 =
        ImVec2(g->gradient.get_start().x + size, g->gradient.get_start().y);
  }
}

void Widget::MotivationOverview::Component::draw() {

  ImGui::SetNextWindowSize(WINDOW_SIZE);
  ImGui::BeginChild("MotivationOverview");
  {

    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 p_min = ImGui::GetWindowPos();
    ImVec2 p_max = im_vec2_add(ImGui::GetWindowPos(), ImGui::GetWindowSize());

    dl->AddRectFilled(p_min, p_max,
                      im_color(emma_color(ThemeEmmaColor_Surface_Lowest)),
                      BORDER_RADIUS);

    set_origin(p_min);
    octagon.draw();

    for (uint8_t i = 0; i < GAUGES_COUNT; i++)
      draw_gauge(dl, &gauges[i]);
  }
  ImGui::EndChild();
}

void Widget::MotivationOverview::Component::draw_gauge(ImDrawList *dl,
                                                       struct Gauge *gauge) {

  dl->AddText(im_vec2_add(gauge->label_p, origin),
              im_color(emma_color(ThemeEmmaColor_Text_On_Dark)), gauge->label);

  gauge->leading_icon.draw_at(origin);
  gauge->gradient.draw_at(origin);

  dl->AddRectFilled(im_vec2_add(gauge->mask_p0, origin),
                    im_vec2_add(gauge->mask_p1, origin),
                    im_color(emma_color(ThemeEmmaColor_Surface_Lowest)),
                    BORDER_RADIUS, ImDrawFlags_RoundCornersRight);

  gauge->window.draw_at(origin);
  gauge->trailing_icon.draw_at(origin);
}
