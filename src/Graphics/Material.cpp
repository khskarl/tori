#include "Material.hpp"


#include <Assets/AssetsManager.hpp>

Material::Material (std::string _albedo,
                    std::string _normal,
                    std::string _roughness,
                    std::string _metalness,
                    std::string _occlusion)
{
	p_albedo =    AssetsManager::Get().GetTexture(_albedo);
	p_normal =    AssetsManager::Get().GetTexture(_normal);
	p_roughness = AssetsManager::Get().GetTexture(_roughness);
	p_metalness = AssetsManager::Get().GetTexture(_metalness);
	p_occlusion = AssetsManager::Get().GetTexture(_occlusion);
}
Material::~Material () {}
