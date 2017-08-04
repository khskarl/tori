#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <Types.hpp>

class Framebuffer {
private:
	uint32_t m_id = 0;
	uint32_t m_colorTexture_id = 0;
	uint32_t m_depthTexture_id = 0;

	uint32_t m_width  = 512;
	uint32_t m_height = 512;


public:
	Framebuffer ();
	// Framebuffer (uint32_t width, uint32_t height);
	~Framebuffer ();

	void Setup (uint32_t width, uint32_t height);
	void Bind ();
	uint32_t GetColorTextureHandle ();
};

#endif
