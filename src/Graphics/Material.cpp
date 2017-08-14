#include "Material.hpp"


#include <Assets/AssetManager.hpp>

Material::Material (std::string _albedo,
                    std::string _normal,
                    std::string _roughness,
                    std::string _metalness,
                    std::string _occlusion)
{
	p_albedo =    AssetManager::Get().GetTexture(_albedo);
	p_normal =    AssetManager::Get().GetTexture(_normal);
	p_roughness = AssetManager::Get().GetTexture(_roughness);
	p_metalness = AssetManager::Get().GetTexture(_metalness);
	p_occlusion = AssetManager::Get().GetTexture(_occlusion);
}
Material::~Material () {}
