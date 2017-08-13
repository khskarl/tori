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
	Texture* m_colorTexture = nullptr;
	Texture* m_irradianceTexture = nullptr;
	Program* m_program = nullptr;

	Texture* ConvertToCubemap (Texture* texture);
	void Setup (std::string texture_name, std::string irradiance_name);
public:
	Cubemap (std::string texture_name, std::string irradiance_name);
	~Cubemap ();

	void BindColor (const uint32_t bind_position);
	void BindIrradiance (const uint32_t bind_position);
	void Render (Camera* const camera);
};

#endif
