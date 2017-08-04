#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <Types.hpp>

class Texture {
public:
	enum Type
	{
		Texture1D = GL_TEXTURE_1D,
		Texture2D = GL_TEXTURE_2D,
		Cubemap   = GL_TEXTURE_CUBE_MAP,
	};

	enum FilteringMode
	{
		Nearest = GL_NEAREST,
		Linear  = GL_LINEAR,
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipmapNearest  = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipmapLinear  = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapLinear   = GL_LINEAR_MIPMAP_LINEAR
	};

	enum EdgeSampleMode
	{
		Repeat         = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge    = GL_CLAMP_TO_EDGE
	};

	enum Format
	{
		Red  = GL_RED,
		RG   = GL_RG,
		RGB  = GL_RGB,
		RGBA = GL_RGBA,
		DepthComponent = GL_DEPTH_COMPONENT,
		StencilIndex   = GL_STENCIL_INDEX
	};

	uint16_t m_id = 0;
	uint16_t m_width  = 0;
	uint16_t m_height = 0;

	std::string m_filename = "";

	Type m_type = Type::Texture2D;

	FilteringMode  m_minFilteringMode = FilteringMode::Linear;
	FilteringMode  m_magFilteringMode = FilteringMode::Linear;
	EdgeSampleMode m_edgeSampleModeS = EdgeSampleMode::Repeat;
	EdgeSampleMode m_edgeSampleModeT = EdgeSampleMode::Repeat;

	Texture();
	~Texture ();
};

#endif // TEXTURE_HPP
