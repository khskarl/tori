#ifndef MODEL_HPP
#define MODEL_HPP

#include <Types.hpp>

#include "Mesh.hpp"
#include "Texture.hpp"

class Model {
private:
public:
	Mesh*    m_mesh    = nullptr;
	Texture* m_albedo = nullptr;
	Texture* m_normal = nullptr;
	Texture* m_roughness = nullptr;
	Texture* m_metalness = nullptr;
	Texture* m_ao = nullptr;
	
	Model  (std::string mesh_name,
	            std::string albedo_name,
	            std::string normal_name,
	            std::string roughness_name,
	            std::string metalness_name,
	            std::string ao_name);
	~Model ();
};

#endif // MODEL_HPP
