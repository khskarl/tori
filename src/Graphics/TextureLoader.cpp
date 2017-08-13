#include "TextureLoader.hpp"

#include <imgui/imgui.h>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>
#include <iostream>
#include <vector>

namespace Data {

std::vector<Texture*> m_textures;

Texture* GetLoadedTexture (const std::string filename) {
	for (Texture* const texture : m_textures) {
		if (texture->m_filename == filename) {
			return texture;
		}
	}
	return nullptr;
}

void LoadAllTextures () {
  Data::LoadTexture ("default.png");
	Data::LoadTexture ("woodfloor_albedo.png");
	Data::LoadTexture ("woodfloor_roughness.png");
	Data::LoadTexture ("woodfloor_normal.png");
	Data::LoadTexture ("woodfloor_metalness.png");
	Data::LoadTexture ("woodfloor_ao.png");
}

Texture* LoadTexture (const std::string filename) {
	// Check if texture is already loaded
	Texture* loadedTexture = GetLoadedTexture(filename);
	if (loadedTexture != nullptr) {
		return loadedTexture;
	}

	// If texture wasn't loaded, load it
	std::string filepath = "data/textures/" + filename;
	// stbi_set_flip_vertically_on_load(true);
	int32_t width, height, numChannels;
	uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &numChannels, 0);

	if (data)	{
		uint32_t textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int32_t gl_channel;
		switch (numChannels) {
			case 1:	gl_channel = GL_RED;
			break;
			case 2: gl_channel = GL_RG;
			break;
			case 3: gl_channel = GL_RGB;
			break;
			case 4: gl_channel = GL_RGBA;
			break;
			default: gl_channel = GL_RGB;
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, gl_channel, width, height, 0, gl_channel, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		Texture* texture = new Texture();
		texture->m_filename = filename;
		texture->m_type     = Texture::Type::Texture2D;
		texture->m_id       = (uint16_t) textureID;
		texture->m_width    = (uint16_t) width;
		texture->m_height   = (uint16_t) height;
		m_textures.push_back(texture);
		return texture;
	}	else {
		std::cerr << "[ERROR] Failed to load texture " << filepath << "\n";
		stbi_image_free(data);
		return nullptr;
	}
}

Texture* LoadPanorama (const std::string filename) {
	// Check if texture is already loaded
	Texture* loadedTexture = GetLoadedTexture(filename);
	if (loadedTexture != nullptr) {
		return loadedTexture;
	}

	// If texture wasn't loaded, load it
	std::string filepath = "data/textures/" + filename;
	stbi_set_flip_vertically_on_load(true);
	int32_t width, height, numChannels;
	float* data = stbi_loadf(filepath.c_str(), &width, &height, &numChannels, 0);

	if (data) {
		uint32_t textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

		Texture* texture = new Texture();
		texture->m_filename = filename;
		texture->m_type     = Texture::Type::Cubemap;
		texture->m_id       = (uint16_t) textureID;
		texture->m_width    = (uint16_t) width;
		texture->m_height   = (uint16_t) height;
		texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
		texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
		texture->m_edgeSampleModeS  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
		texture->m_edgeSampleModeT  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
		m_textures.push_back(texture);
		return texture;
	} else {
		std::cerr << "[ERROR] Failed to load texture " << filepath << "\n";
		stbi_image_free(data);
		return nullptr;
	}
}

Texture* LoadCubemap (const std::string filename) {
	std::cerr << "LoadCubemap (const std::string filename) not implemented\n";
	return nullptr;
}

// --------------- //
// IMGUI Functions //
// --------------- //
bool ImHelpTexturesNamesGetter (void* data, int n, const char** out_text) {
	const std::vector<Texture*>* v = (std::vector<Texture*>*) data;
	*out_text = ((*v)[n])->m_filename.c_str();
	return true;
}

void TextureWindow () {
	// ImGui::SetNextWindowSize(ImVec2(300, 400));

	if (ImGui::Begin("Textures"))
	{
		static int selected_item_index = 0;
		ImGui::BeginChild("texture list", ImVec2(200, 0), true);
			ImGui::PushItemWidth(-1);
			ImGui::ListBox("", &selected_item_index, ImHelpTexturesNamesGetter, &m_textures, m_textures.size(), m_textures.size());
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("texture view", ImVec2(-1, 0));
			ImGui::Text("Filename: %s", m_textures[selected_item_index]->m_filename.c_str());
			ImGui::Separator();
			ImGui::Image((void*)m_textures[selected_item_index]->m_id, ImVec2(128, 128));
			ImGui::Text("Textures D:");
		ImGui::EndChild();
	}
	ImGui::End();
}

}
