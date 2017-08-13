#include "Texture.hpp"

#include <GL/glew.h>

Texture::Texture () {}
Texture::~Texture () {}

void Texture::Bind (uint32_t bind_position) {
	glActiveTexture(GL_TEXTURE0 + bind_position);
	glBindTexture(m_type, m_id);
}
