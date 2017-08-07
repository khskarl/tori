#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Framebuffer.hpp"
#include "Camera.hpp"
#include "Program.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "LightSource.hpp"
#include "../GameObject.hpp"

#include <cstdint>
#include <vector>

class Renderer
{
public:
	std::vector<LightSource> m_lightSources;
	std::vector<GameObject*> m_renderQueue;
	std::vector<Texture*> m_textures;

	uint32_t m_quadVAO;
	Framebuffer m_mainFramebuffer;
	Program* m_screenProgram;

public:
	bool m_bRenderWireframe = false;

	Program* m_mainProgram = nullptr;
	Camera* p_activeCamera = nullptr;

	Mesh*    m_skyboxMesh = nullptr;
	Texture* m_skyboxTexture = nullptr;
	Program* m_skyboxProgram = nullptr;


	Renderer ();
	~Renderer ();

	void Setup ();
	void RenderFrame ();

	void Submit (GameObject* const object);

	void SetActiveCamera (Camera* const camera);

};

#endif
