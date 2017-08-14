#include "Model.hpp"

#include "MeshLoader.hpp"
#include "TextureLoader.hpp"

Model::Model (std::string mesh_name, Material* material_name) {
	p_mesh    = Data::LoadMesh(mesh_name);
	p_material = material_name;
}

Model::Model (std::string mesh_name) {
	p_mesh = Data::LoadMesh(mesh_name);
	p_material = new Material("default.png",
	                          "default.png",
	                          "default.png",
	                          "default.png",
	                          "default.png");
}

Model::~Model () {}
