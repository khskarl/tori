#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <Types.hpp>

#include "Mesh.hpp"
#include "Texture.hpp"
#include "Program.hpp"
#include "Camera.hpp"

class Cubemap {
private:
	// Model* m_skyboxModel = nullptr;
	Mesh*    m_mesh    = nullptr;
	Texture* m_texture = nullptr;
	Program* m_program = nullptr;

	void SetupEquirectangular (std::string texture_name);
public:
	Cubemap (std::string texture_name);
	~Cubemap ();

	void Bind (uint32_t bind_position);
	void Render (Camera* camera);
};

#endif
