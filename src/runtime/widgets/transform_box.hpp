#ifndef _WIDGET_TRANSFORM_BOX_H_
#define _WIDGET_TRANSFORM_BOX_H_

#include "nkengine/include/gui.h"
#include "runtime/canvas/core.h"
#include <cglm/cglm.h>
#include <imgui/imgui.h>

namespace Widget {

typedef void (*transform_box_set_position_callback)(void *, ImVec2);
typedef void (*transform_box_get_position_callback)(void *, ImVec2);

typedef void (*transform_box_set_size_callback)(void *, ImVec2);
typedef void (*transform_box_get_size_callback)(void *, ImVec2);

static constexpr uint8_t transform_box_configuration_capacity = 32;
static constexpr uint8_t transform_box_targets_capacity = 32;

typedef enum {
  TransformBoxStatus_Success,
  TransformBoxStatus_MaxCapacity,
  TransformBoxStatus_OutOfBound,
  TransformBoxStatus_UndefError,
} TransformBoxStatus;

typedef struct {
  bool active;

  transform_box_set_position_callback set_position;
  transform_box_set_size_callback set_size;
  transform_box_get_position_callback get_position;
  transform_box_get_size_callback get_size;

  void *targets[transform_box_targets_capacity];
  size_t target_count;

} TransformBoxConfiguration;

typedef struct {
  transform_box_set_position_callback set_position;
  transform_box_set_size_callback set_size;
  transform_box_get_position_callback get_position;
  transform_box_get_size_callback get_size;
} TransformBoxConfigurationDescriptor;

class TransformBox {

public:
  TransformBox(){};
  TransformBox(Gui *gui) : gui(gui) {}

  int add_configuration(const TransformBoxConfigurationDescriptor *);
  TransformBoxStatus remove_configuration(const int);

  StaticListStatus add_target(const void *, const int);
  StaticListStatus remove_target(const void *, const int);

  TransformBoxStatus update_bound_from_selection();

  TransformBoxStatus draw();

private:
  ImVec2 p0, p1;
  Gui *gui;
  static constexpr uint8_t transform_box_handles_count = 6;

  Handle handles[transform_box_handles_count];

  TransformBoxConfiguration
      configurations[transform_box_configuration_capacity] = {0};
};

} // namespace Widget

#endif
