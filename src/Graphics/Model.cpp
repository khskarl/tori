#include "Model.hpp"

#include "MeshLoader.hpp"
#include "TextureLoader.hpp"

Model::Model (std::string mesh_name,
	            std::string albedo_name,
	            std::string normal_name,
	            std::string roughness_name,
	            std::string metalness_name,
	            std::string ao_name) {
	m_mesh    = Data::LoadMesh(mesh_name);
	m_albedo    = Data::LoadTexture(albedo_name);
	m_normal    = Data::LoadTexture(normal_name);
	m_roughness = Data::LoadTexture(roughness_name);
	m_metalness = Data::LoadTexture(metalness_name);
	m_ao        = Data::LoadTexture(ao_name);
}

Model::~Model () {}
