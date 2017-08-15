#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Framebuffer.hpp"
#include "Camera.hpp"
#include "Program.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "LightSource.hpp"
#include "Cubemap.hpp"
#include "../GameObject.hpp"

#include <cstdint>
#include <vector>

class Renderer
{
public:
	std::vector<LightSource> m_lightSources;
	std::vector<GameObject*> m_renderQueue;
	std::vector<Texture*> m_textures;

	Mesh* p_quadMesh = nullptr;
	Framebuffer m_mainFramebuffer;
	Program* m_screenProgram = nullptr;

	float m_exposureLevel = 1.0f;

	void SetupCubemap (std::string texture_name);

public:
	bool m_bRenderWireframe = false;

	Program* m_mainProgram = nullptr;
	Camera* p_activeCamera = nullptr;

	// Skybox related variables
	Cubemap* m_cubemap = nullptr;

	Renderer ();
	~Renderer ();

	void Setup ();
	void RenderFrame ();

	void Submit (GameObject* const object);

	void SetActiveCamera (Camera* const camera);

	// GUI Functions
	void RendererWindow (bool* p_open);

};

#endif
