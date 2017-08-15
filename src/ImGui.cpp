#include "ImGui.hpp"

#include <Types.hpp>
#include <vector>

#include <Graphics/Texture.hpp>
#include <Graphics/Material.hpp>

namespace ImGui {

void TextureInfo (Texture const* texture) {
	ImGui::BeginChild("texture info", ImVec2(-1, 0));
		ImGui::Text("Filename: %s", texture->m_filename.c_str());
		ImGui::Separator();
		ImGui::Image((void*)texture->m_id, ImVec2(128, 128));
	ImGui::EndChild();
}


void MaterialInfo (Material const* material) {
	ImGui::BeginChild("material info", ImVec2(-1, 0));
		ImGui::Text("Material: %s", material->m_filename.c_str());
		ImGui::Separator();
		ImGui::Text("Albedo: %s", material->p_albedo->m_filename.c_str());
	ImGui::EndChild();
}
}
