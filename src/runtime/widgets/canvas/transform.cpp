#include "transform.hpp"
#include "nkengine/include/gui.hpp"
#include "runtime/widgets/canvas/core.hpp"
#include "runtime/widgets/utils.hpp"

Widget::CanvasTransform::CanvasTransform(Gui *gui, Canvas *node)
    : node(node), transform_box(gui), CanvasModule(gui, node) {

  transform_box.update_bound(ImVec2(20, 20), ImVec2(900, 300));

  {
    // Frame transform config
    Configuration *fm_conf = &transform_configuration[ConfigurationType_Frame];

    fm_conf->transform_mode = TransformBoxMode_All;
    fm_conf->get_position = canvas_shape_get_frame_position;
    fm_conf->set_position = canvas_shape_set_frame_position;
    fm_conf->get_size = canvas_shape_get_frame_size;
    fm_conf->set_size = canvas_shape_set_frame_size;
    fm_conf->selection_list = &node->frames[CanvasFrameState_Selected];
    fm_conf->button = ImGuiMouseButton_Right;
    // fm_conf->parent_list = (group list)
  }

  {
    // Module transform config
    Configuration *md_conf = &transform_configuration[ConfigurationType_Module];

    md_conf->transform_mode = TransformBoxMode_Move;
    md_conf->get_position = canvas_shape_get_frame_position;
    md_conf->get_size = canvas_shape_get_frame_size;
    md_conf->set_position = canvas_shape_set_module_position;
    md_conf->set_size = canvas_shape_set_module_size;
    md_conf->session_end = canvas_shape_on_module_session_end;
    md_conf->selection_list = &node->modules[CanvasModuleState_Selected];
    md_conf->parent_list = &node->frames[CanvasModuleState_Default];
    md_conf->button = ImGuiMouseButton_Left;
  }

  {
    // Pod transform config
    Configuration *pod_conf = &transform_configuration[ConfigurationType_Pod];

    pod_conf->transform_mode = TransformBoxMode_Move;
    pod_conf->get_position = canvas_shape_get_frame_position;
    pod_conf->set_position = canvas_shape_set_pod_position;
    pod_conf->get_size = canvas_shape_get_frame_size;
    pod_conf->set_size = canvas_shape_set_pod_size;
    pod_conf->selection_list = &node->pods[CanvasPodState_Selected];
    pod_conf->button = ImGuiMouseButton_Left;
  }
}

/**
   Handle the boundbox interaction along with the transformation for frames
   and modules.
 */
void Widget::CanvasTransform::listen_frame(FrameShape *frame,
                                           const ConfigurationType type) {

  if (state & State_Freeze)
    return;

  const Configuration *conf = &transform_configuration[type];

  Frame *frame_node = frame->get_node();

  // no matter the button, if a click happened and hit a frame *area*, we update
  // the transform session status to "Has Hit"
  gui_selection_hit(&transform_box.selection,
                    (ImGui::IsMouseClicked(ImGuiMouseButton_Right) ||
                     ImGui::IsMouseClicked(ImGuiMouseButton_Left)) &&
                        frame->boundbox_hovered());

  // but we only add to selection if the button actually matches the
  // configuration button and hit the
  if (ImGui::IsMouseClicked(conf->button) && frame->boundbox_hovered()) {

    FrameData data;
    data.frame = frame_node;
    data.canvas = node;
    data.parent_list = conf->parent_list;

    // register the frame for transform callbacks
    if (stli_insert(transform_frame_data.entries, ALLOCATOR_MAX_FRAMES,
                    &transform_frame_data.count, sizeof(FrameData), &data,
                    "Canvas Transform Frame Data") != StaticListStatus_Success)
      return;

    TransformBoxObjectDescriptor object = {
        .handle = &transform_frame_data.entries[transform_frame_data.count - 1],
        .get_position = conf->get_position,
        .set_position = conf->set_position,
        .get_size = conf->get_size,
        .set_size = conf->set_size,
        .session_end = conf->session_end,
    };

    TransformBoxObject *found_obj =
        transform_box.find_object(object.handle, NULL);

    // remove object TODO DEBUG: NEVER CALLS THIS CONDITION
    if (found_obj) {

      // mark as unselected
      allocator_id_list_pop(conf->selection_list->entries,
                            &conf->selection_list->length, frame_node->id);

      transform_box.remove_object(found_obj->handle, NULL);

      // remove it from the cached callback data
      for (size_t i = 0; i < transform_frame_data.count; i++)
        if (transform_frame_data.entries[i].frame == frame_node)
          stli_remove_at_index(transform_frame_data.entries,
                               &transform_frame_data.count, sizeof(FrameData),
                               i, NULL);

      // add object
    } else {

      // prevent selecting the inner content of the frame when the frame is
      // already selected, in the case  we click inside (on the content) to
      // move the frame (and not selecting the content).
      if (transform_box.objects_count() &&
          transform_box.button != conf->button) {
        return;
      }

      transform_box.mode = conf->transform_mode;
      transform_box.button = conf->button;

      // if not CAP input or if the configuration's Transform Mode is
      // empty and different from the current one, we empty the selection.
      if (input_key(INPUT_KEY_SHIFT) == false) {
        transform_box.empty_objects();
        allocator_id_list_empty(conf->selection_list->entries,
                                &conf->selection_list->length);
      }

      // mark as selected
      allocator_id_list_push(conf->selection_list->entries,
                             ALLOCATOR_MAX_FRAMES,
                             &conf->selection_list->length, frame_node->id);

      transform_box.add_object(&object);
    }

    transform_box.update_bound_from_selection();
  }
}

/**
   In this function we check if a handle either from an existing connector or
   a newly created one is active and transform it according to the mouse
   position.
 */
void Widget::CanvasTransform::listen_active_connector_handle(
    ConnectorHandle *handle, Connector *connector) {
  if (handle) {
    ImVec2 mouse = vp_im2_scene(ImGui::GetIO().MousePos);
    connector_handle_set_position(handle, (vec2){mouse.x, mouse.y});
    connector_update_corners(connector);
  }
}

/**
   On Mouse release we check if some frames that have parent moved out of the
   parent bound to break the relationship. And and vice versa, i.e. if a child
   is included in a parent bound then add it as child.
 */
void Widget::canvas_shape_on_module_session_end(void *data) {

  CanvasTransform::FrameData *frame_data = (CanvasTransform::FrameData *)data;

  Frame *frame = allocator_frame_entry(frame_data->frame->id);

  // check if still within parent bound
  if (frame_data->frame->parent != ID_UNDEFINED) {

    Frame *parent = allocator_frame_entry(frame->parent);

    if (!frame_collide(parent, frame))
      frame_remove_child(parent, frame->id);
  }

  // check if the frame is within a parent frame scope
  if (frame_data->parent_list)
    for (size_t i = 0; i < frame_data->parent_list->length; i++) {
      Frame *parent =
          allocator_frame_entry(frame_data->parent_list->entries[i]);
      if (frame_collide(parent, frame)) {
        frame_add_child(parent, frame->id);
      }
    }
}

void Widget::canvas_shape_get_frame_position(void *data, ImVec2 &value) {

  CanvasTransform::FrameData *frame_data = (CanvasTransform::FrameData *)data;

  Frame *frame = frame_data->frame;

  const float *world_pos = frame_get_world_position(frame);
  value = ImVec2(world_pos[0], world_pos[1]);
}

void Widget::canvas_shape_get_frame_size(void *data, ImVec2 &value) {

  CanvasTransform::FrameData *frame_data = (CanvasTransform::FrameData *)data;

  Frame *frame = frame_data->frame;

  value = im_vec2(frame->size);
}

void Widget::canvas_shape_set_frame_position(void *data, ImVec2 value) {

  CanvasTransform::FrameData *frame_data = (CanvasTransform::FrameData *)data;

  Frame *frame = frame_data->frame;
  canvas_set_frame_position(frame_data->canvas, frame,
                            (vec2){value.x, value.y});

  canvas_update_frame_connectors(frame_data->canvas, frame_data->frame);
}

void Widget::canvas_shape_set_frame_size(void *data, ImVec2 value) {

  CanvasTransform::FrameData *frame_data = (CanvasTransform::FrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_frame_size(frame_data->canvas, frame, (vec2){value.x, value.y});
}

void Widget::canvas_shape_set_module_size(void *data, ImVec2 value) {

  CanvasTransform::FrameData *frame_data = (CanvasTransform::FrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_module_size(frame_data->canvas, frame, (vec2){value.x, value.y});
}

void Widget::canvas_shape_set_module_position(void *data, ImVec2 value) {

  CanvasTransform::FrameData *frame_data = (CanvasTransform::FrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_module_world_position(frame_data->canvas, frame,
                                   (vec2){value.x, value.y});

  /*
    Since we crop the frames children, we need to check if the module is
   still within the parent area when moving cause if we move the module out
   of the parent, it will still disapear being cropped out by the parent
   frame.

    As a result to make the user understand that the frame is being
   'unlinked' from parent, we need to remove the parent from the child so it
   is not cropped anymore. However note that we do NOT relink back the child
   here cause on transform session end, we already traverse all the frames
   to check if the module is within one of them.

   Also it would by costly to check on every frame here if the child is within
   any frame node; that's why we only handle the unlink phase here as it is fast
   to target the parent (node->parent) and contribute greatly to the user
   experience.
   */
  if (frame_data->frame->parent != ID_UNDEFINED) {
    Frame *parent = allocator_frame_entry(frame->parent);
    if (!frame_collide(parent, frame))
      frame_remove_child(parent, frame->id);
  }
}

void Widget::canvas_shape_set_pod_size(void *data, ImVec2 value) {

  CanvasTransform::FrameData *frame_data = (CanvasTransform::FrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_pod_size(frame_data->canvas, frame, (vec2){value.x, value.y});
}

void Widget::canvas_shape_set_pod_position(void *data, ImVec2 value) {

  CanvasTransform::FrameData *frame_data = (CanvasTransform::FrameData *)data;

  Frame *frame = frame_data->frame;

  canvas_set_pod_position(frame_data->canvas, frame, (vec2){value.x, value.y});
  canvas_update_frame_connectors(frame_data->canvas, frame_data->frame);
}
