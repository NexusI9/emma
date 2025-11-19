#include "transform_box.hpp"
#include "utils/id.h"
#include <climits>

int Widget::TransformBox::add_configuration(
    const TransformBoxConfigurationDescriptor *config) {

  for (uint8_t i = 0; i < transform_box_configuration_capacity; i++) {
    if (!configurations[i].active) {
      configurations[i].get_position = config->get_position;
      configurations[i].set_position = config->set_position;
      configurations[i].get_size = config->get_size;
      configurations[i].set_size = config->set_size;
      return i;
    }
  }

  return INT_MAX;
}

Widget::TransformBoxStatus
Widget::TransformBox::remove_configuration(const int id) {

  if (id > transform_box_configuration_capacity)
    return TransformBoxStatus_OutOfBound;

  configurations[id] = {0};

  return TransformBoxStatus_Success;
}

StaticListStatus Widget::TransformBox::add_target(const void *target,
                                                  const int conf_id) {
  if (conf_id > transform_box_configuration_capacity)
    return StaticListStatus_OutOfRange;

  return stli_insert(configurations[conf_id].targets,
                     transform_box_targets_capacity,
                     &configurations[conf_id].target_count, sizeof(void *),
                     target, "Transform Box Target List");
}

StaticListStatus Widget::TransformBox::remove_target(const void *target,
                                                     const int conf_id) {

  if (conf_id > transform_box_configuration_capacity)
    return StaticListStatus_OutOfRange;

  return stli_remove(configurations[conf_id].targets,
                     &configurations[conf_id].target_count, sizeof(void *),
                     target, "Transform Box Target List");
}
