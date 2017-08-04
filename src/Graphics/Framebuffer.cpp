#include "Framebuffer.hpp"

#include <GL/glew.h>

#include <iostream>

#include "../Settings.hpp"

Framebuffer::Framebuffer () {
	Setup(Settings::ScreenWidth, Settings::ScreenHeight);
}

void Framebuffer::Setup (uint32_t width, uint32_t height) {
	m_width = width;
	m_height = height;

	// Generate color texture
	glGenTextures(1, &m_colorTexture_id);
	glBindTexture(GL_TEXTURE_2D, m_colorTexture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate Depth+Stencil texture
	glGenTextures(1, &m_depthTexture_id);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,
							 width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach Color Texture to Framebuffer
	glGenFramebuffers(1, &m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER,
												 GL_COLOR_ATTACHMENT0,
												 GL_TEXTURE_2D,
												 m_colorTexture_id, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,
												 GL_DEPTH_STENCIL_ATTACHMENT,
												 GL_TEXTURE_2D,
												 m_depthTexture_id, 0);
	std::cout << m_id << "\n";

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "[ERROR] Framebuffer not complete \n";

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer () {
	glDeleteTextures(1, &m_colorTexture_id);
	glDeleteTextures(1, &m_depthTexture_id);
	glDeleteFramebuffers(1, &m_id);
}

void Framebuffer::Bind () {
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

uint32_t Framebuffer::GetColorTextureHandle () {
	return m_colorTexture_id;
}
