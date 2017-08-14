#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <cstdint>
#include <glm/glm.hpp>

#include "Graphics/Model.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Texture.hpp"

class GameObject {
private:
public:
	GameObject ();
	~GameObject ();
	// TODO: Transform Component
	glm::vec3 m_position  = glm::vec3(0);

	// TODO: Rendering Component
	Model m_model = Model("sphere.obj");
};

#endif // GAME_OBJECT_HPP
