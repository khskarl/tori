#ifndef MODEL_HPP
#define MODEL_HPP

#include <Types.hpp>

#include "Mesh.hpp"
#include "Material.hpp"

class Model {
private:
public:
	Mesh*     p_mesh     = nullptr;
	Material* p_material = nullptr;

	Model  (std::string mesh_name, std::string material_name);
	Model  (std::string mesh_name, Material* material);
	Model  (Mesh* mesh, Material* material);
	Model  (std::string mesh_name);
	~Model ();
};

#endif // MODEL_HPP
