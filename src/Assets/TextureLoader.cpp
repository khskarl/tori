#include "TextureLoader.hpp"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>
#include <Types.hpp>
#include <Log.hpp>
#include <vector>

namespace Loader {

Texture* LoadTexture (const fs::path filepath) {
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
		texture->m_filename = filepath.filename().string();
		texture->m_type     = Texture::Type::Texture2D;
		texture->m_id       = textureID;
		texture->m_width    = (uint16_t) width;
		texture->m_height   = (uint16_t) height;
		texture->m_edgeSampleModeS  = (Texture::EdgeSampleMode) GL_REPEAT;
		texture->m_edgeSampleModeT  = (Texture::EdgeSampleMode) GL_REPEAT;
		texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
		texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
		return texture;
	}	else {
		LOG_ERROR("Failed to load texture " + filepath.string());
		stbi_image_free(data);
		return nullptr;
	}
}

Texture* LoadPanorama (const fs::path filepath) {
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
		texture->m_filename = filepath.filename().string();
		texture->m_type     = Texture::Type::Cubemap;
		texture->m_id       = textureID;
		texture->m_width    = (uint16_t) width;
		texture->m_height   = (uint16_t) height;
		texture->m_edgeSampleModeS  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
		texture->m_edgeSampleModeT  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
		texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
		texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
		return texture;
	} else {
		LOG_ERROR("Failed to load texture " + filepath.string());
		stbi_image_free(data);
		return nullptr;
	}
}
}
