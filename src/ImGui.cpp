#include "ImGui.hpp"

#include <Types.hpp>
#include <vector>

#include <Graphics/Texture.hpp>

namespace ImGui {

void TextureInfo (Texture const * texture) {
	ImGui::BeginChild("texture info", ImVec2(-1, 0));
		ImGui::Text("Filename: %s", texture->m_filename.c_str());
		ImGui::Separator();
		ImGui::Image((void*)texture->m_id, ImVec2(128, 128));
	ImGui::EndChild();
}

}
