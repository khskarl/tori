#include "Material.hpp"


#include "TextureLoader.hpp"

Material::Material (std::string _albedo,
                    std::string _normal,
                    std::string _roughness,
                    std::string _metalness,
                    std::string _occlusion)
{
	p_albedo = Data::LoadTexture(_albedo);
	p_normal = Data::LoadTexture(_normal);
	p_roughness = Data::LoadTexture(_roughness);
	p_metalness = Data::LoadTexture(_metalness);
	p_occlusion = Data::LoadTexture(_occlusion);
}
Material::~Material () {}
