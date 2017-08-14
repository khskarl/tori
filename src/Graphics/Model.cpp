#include "Model.hpp"

#include <Assets/AssetsManager.hpp>

Model::Model (std::string mesh_name, Material* material_name) {
	p_mesh    = AssetsManager::Get().GetMesh(mesh_name);
	p_material = material_name;
}

Model::Model (std::string mesh_name) {
	p_mesh = AssetsManager::Get().GetMesh(mesh_name);
	p_material = new Material("default.png",
	                          "default.png",
	                          "default.png",
	                          "default.png",
	                          "default.png");
}

Model::~Model () {}
