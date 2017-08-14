#include "Renderer.hpp"

#include <iostream>
#include <Types.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

#include "TextureLoader.hpp"
#include "MeshLoader.hpp"
#include "../Settings.hpp"

Renderer::Renderer () {}
Renderer::~Renderer () {}

void Renderer::Setup () {

	// Load and setup Programs
	m_mainProgram = new Program("pbr.vs", "pbr.fs");
	m_screenProgram = new Program("passthrough.vs", "passthrough.fs");
	m_screenProgram->SetUniform1ui("screenTexture", 0);

	// Load and setup Meshes and Textures
	Data::LoadAllMeshes();
	Data::LoadAllTextures();

	m_cubemap = new Cubemap("envmaps/loft.hdr", "envmaps/loft_irradiance.hdr");

	// Setup lights here because lazy
	// m_lightSources.push_back(LightSource(LightSource::Type::Directional));
	// m_lightSources[0].direction = glm::normalize(glm::vec3(0.3f, -1.f, 0.5f));
	// m_lightSources[0].color    = glm::vec3(10.f, 10.f, 10.f);
	// m_lightSources.push_back(LightSource(LightSource::Type::Point));
	// m_lightSources[1].position = glm::vec3( 2,  -5,  5);
	// m_lightSources[1].color    = glm::vec3(50.f, 10.f, 10.f);

	// Setup main framebuffer
	float quadVertices[] = {
// Positions   | UVs
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	// screen quad VAO
	uint32_t quadVBO;
	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Setup main framebuffer
	m_mainFramebuffer.Setup(Settings::ScreenWidth, Settings::ScreenHeight);
	m_mainFramebuffer.Bind();

	// Setup OpenGL settings
	glClearColor(0.005f, 0.005f, 0.005f, 1.0f);
	glViewport(0, 0, Settings::ScreenWidth, Settings::ScreenHeight);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Renderer::RenderFrame () {
	// Draw all objects into main framebuffer
	m_mainFramebuffer.Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (m_bRenderWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	m_mainProgram->Use();
	m_mainProgram->SetUniform("view",       p_activeCamera->GetViewMatrix());
	m_mainProgram->SetUniform("projection", p_activeCamera->GetProjectionMatrix());
	m_mainProgram->SetUniform("cameraPos",  p_activeCamera->GetPosition());
	m_mainProgram->SetUniform1i("texAlbedo",    0);
	m_mainProgram->SetUniform1i("texNormal",    1);
	m_mainProgram->SetUniform1i("texRoughness", 2);
	m_mainProgram->SetUniform1i("texMetalness", 3);
	m_mainProgram->SetUniform1i("texOcclusion", 4);
	m_mainProgram->SetUniform1i("irradianceMap", 5);

	m_mainProgram->SetUniform1i("gNumLights", (int) m_lightSources.size());
	for (size_t i = 0; i < m_lightSources.size(); i++) {
		LightSource* light = &m_lightSources[i];
		std::string lightsStr = "gLights[" + std::to_string(i) + "]";
		m_mainProgram->SetUniform(lightsStr + ".position",  light->position);
		m_mainProgram->SetUniform(lightsStr + ".direction", light->direction);
		m_mainProgram->SetUniform(lightsStr + ".color",     light->color);
		m_mainProgram->SetUniform1i(lightsStr + ".type",   light->type);
	}

	m_cubemap->BindIrradiance(5);

	for (GameObject* const object : m_renderQueue) {
		Model*    const model    = &object->m_model;
		Material* const material = model->p_material;

		material->p_albedo   ->Bind(0);
		material->p_normal   ->Bind(1);
		material->p_roughness->Bind(2);
		material->p_metalness->Bind(3);
		material->p_occlusion->Bind(4);

		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, object->m_position);
		m_mainProgram->SetUniform("model", modelMatrix);
		model->p_mesh->Render();
	}

	// Render skybox
	m_cubemap->Render(p_activeCamera);

	// Draw main framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	//
	m_screenProgram->Use();
	m_screenProgram->SetUniform1f("exposureLevel", m_exposureLevel);
	glBindVertexArray(m_quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mainFramebuffer.GetColorTextureHandle());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	ImGui::Render();
}

void Renderer::Submit (GameObject* const object) {
	m_renderQueue.push_back(object);
}

void Renderer::SetActiveCamera (Camera* const camera) {
	p_activeCamera = camera;
}

void Renderer::RendererWindow (bool* p_open) {
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiSetCond_FirstUseEver);

	if (ImGui::Begin("Renderer", p_open))
	{
		if (ImGui::Button("Reload shaders")) m_mainProgram->Reload();
		ImGui::Checkbox("Wireframe", &m_bRenderWireframe);
		ImGui::Text("%.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
		ImGui::Image((void*)m_mainFramebuffer.GetColorTextureHandle(),
		             ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SliderFloat("Exposure", &m_exposureLevel, 0.1f, 5.0f);

		ImGui::End();
	}
}
