#include "Model.hpp"

#include <Assets/AssetManager.hpp>

Model::Model (std::string mesh_name, std::string material_name) {
	p_mesh     = AssetManager::Get().GetMesh(mesh_name);
	p_material = AssetManager::Get().GetMaterial(material_name);
}

Model::Model (std::string mesh_name, Material* material) {
	p_mesh    = AssetManager::Get().GetMesh(mesh_name);
	p_material = material;
}

Model::Model (Mesh* mesh, Material* material) {
	p_mesh     = mesh;
	p_material = material;
}

Model::Model (std::string mesh_name) {
	p_mesh = AssetManager::Get().GetMesh(mesh_name);
	p_material = new Material("default.png",
	                          "default.png",
	                          "default.png",
	                          "default.png",
	                          "default.png");
}

Model::~Model () {}
