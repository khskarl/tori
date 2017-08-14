#include "MaterialLoader.hpp"

#include <Log.hpp>
#include <vector>

#include <INIReader.h>

namespace Loader {

Material* LoadMaterial (const std::string filepath) {
	LOG_INFO("Loading Material " + filepath);

	INIReader reader(filepath);

	if (reader.ParseError() < 0) {
		LOG_ERROR("Can't load " + filepath);
		return nullptr;
	}

	std::string albedo_name    = reader.Get("material", "albedo", "UNKNOWN");
	std::string normal_name    = reader.Get("material", "normal", "UNKNOWN");
	std::string roughness_name = reader.Get("material", "roughness", "UNKNOWN");
	std::string metalness_name = reader.Get("material", "metalness", "UNKNOWN");
	std::string occlusion_name = reader.Get("material", "occlusion", "UNKNOWN");

	Material* material = new Material(albedo_name,
	                                  normal_name,
	                                  roughness_name,
	                                  metalness_name,
	                                  occlusion_name);

	return material;
}


}
