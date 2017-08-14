#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <Types.hpp>

#include "Texture.hpp"
#include "Program.hpp"


class Material {
public:
	std::string m_filename = "";

	Texture* p_albedo = nullptr;
	Texture* p_normal = nullptr;
	Texture* p_roughness = nullptr;
	Texture* p_metalness = nullptr;
	Texture* p_occlusion = nullptr;

	Material (std::string _albedo,
	          std::string _normal,
	          std::string _roughness,
	          std::string _metalness,
	          std::string _occlusion);
	~Material ();
};

#endif
