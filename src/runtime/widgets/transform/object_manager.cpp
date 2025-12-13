#include "object_manager.hpp"

Widget::Transform::ObjectManager::Status
Widget::Transform::ObjectManager::toggle_object(const ObjectDescriptor *desc) {

  size_t index;
  if (!find_object(desc->handle, &index)) {
    add_object(desc);
    return Transform::ObjectManager::Status_ObjectAdded;
  }

  remove_object(desc->handle, &index);
  return Transform::ObjectManager::Status_ObjectRemoved;
}

Widget::Transform::ObjectManager::Object *
Widget::Transform::ObjectManager::find_object(const void *handle,
                                              size_t *index) {

  for (size_t i = 0; i < objects.count; i++) {

    if (objects.entries[i].handle == handle) {
      if (index)
        *index = i;
      return &objects.entries[i];
    }
  }

  if (index)
    *index = SIZE_MAX;

  return NULL;
}

StaticListStatus
Widget::Transform::ObjectManager::add_object(const ObjectDescriptor *desc) {

  if (objects.count == OBJECT_CAPACITY)
    return StaticListStatus_MaxCapacity;

  Object object = {
      .handle = desc->handle,
      .get_position = desc->get_position,
      .set_position = desc->set_position,
      .get_size = desc->get_size,
      .set_size = desc->set_size,
      .session_end = desc->session_end,
  };

  return stli_insert(objects.entries, OBJECT_CAPACITY, &objects.count,
                     sizeof(Object), &object,
                     "Transform Object Manager Target List");
}

StaticListStatus
Widget::Transform::ObjectManager::remove_object(const void *target,
                                                size_t *index) {

  size_t found_index = index ? (*index) : SIZE_MAX;

  if (index == NULL)
    find_object(target, &found_index);

  if (found_index == SIZE_MAX)
    return StaticListStatus_UnfoundEntry;

  return stli_remove_at_index(objects.entries, &objects.count, sizeof(Object),
                              found_index, NULL);
}

StaticListStatus Widget::Transform::ObjectManager::clear() {

  return stli_empty(objects.entries, &objects.count, sizeof(Object),
                    "Transform ObjectManager Target List");
}
