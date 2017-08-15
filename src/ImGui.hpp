#ifndef IMGUI_HPP
#define IMGUI_HPP

#include <imgui/imgui.h>

class Texture;
class Material;

namespace ImGui {
void TextureInfo (Texture const * texture);
void MaterialInfo (Material const * material);
}

#endif
