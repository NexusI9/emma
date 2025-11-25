#include "atlas.h"

TextureAtlas g_atlas = {0};

void atlas_create() {

  texture_atlas_create(&g_atlas,
                       &(TextureAtlasDescriptor){
                           .label = "Emma Atlas",
                           .resolution = TextureResolution_2048,
                           .layers =
                               {
                                   .count = 2,
                                   .paths = {"textures/module_atlas.jpg",
                                             "textures/ui_atlas.png"},
                               },
                       });
}
