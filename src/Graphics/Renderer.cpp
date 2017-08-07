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
	m_screenProgram->SetUniform("screenTexture", 0);

	// Load and setup Meshes and Textures
	Data::LoadAllMeshes();
	Data::LoadAllTextures();
	m_skyboxMesh = Data::LoadMesh("cube.obj");
	m_skyboxTexture = Data::LoadTexture("default.png");

	// Setup lights here because lazy
	m_lightSources.push_back(LightSource(LightSource::Type::Directional));
	m_lightSources.push_back(LightSource(LightSource::Type::Point));
	m_lightSources.push_back(LightSource(LightSource::Type::Point));
	m_lightSources.push_back(LightSource(LightSource::Type::Point));

	m_lightSources[0].direction = glm::normalize(glm::vec3(0.3f, -1.f, 0.5f));
	m_lightSources[1].position = glm::vec3(-3,  0,  0);
	m_lightSources[2].position = glm::vec3( 2, -2, 10);
	m_lightSources[3].position = glm::vec3( 2,  -5,  5);
	m_lightSources[1].color    = glm::vec3(20.f, 20.f, 50.f);
	m_lightSources[2].color    = glm::vec3(20.f, 40.f, 40.f);
	m_lightSources[3].color    = glm::vec3(50.f, 10.f, 10.f);

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
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
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
	m_mainProgram->SetUniform("texAlbedo",    0);
	m_mainProgram->SetUniform("texNormal",    1);
	m_mainProgram->SetUniform("texRoughness", 2);
	m_mainProgram->SetUniform("texMetalness", 3);
	m_mainProgram->SetUniform("texOcclusion", 4);

	m_mainProgram->SetUniform("gNumLights", m_lightSources.size());
	for (size_t i = 0; i < m_lightSources.size(); i++) {
		LightSource* light = &m_lightSources[i];
		std::string lightsStr = "gLights[" + std::to_string(i) + "]";
		m_mainProgram->SetUniform(lightsStr + ".position",  light->position);
		m_mainProgram->SetUniform(lightsStr + ".direction", light->direction);
		m_mainProgram->SetUniform(lightsStr + ".color",     light->color);
		m_mainProgram->SetUniform(lightsStr + ".type",      light->type);
	}

	for (GameObject* const object : m_renderQueue) {
		Model* const model = &object->m_model;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model->m_albedo->m_id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, model->m_normal->m_id);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, model->m_roughness->m_id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, model->m_metalness->m_id);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, model->m_ao->m_id);

		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, object->m_position);
		m_mainProgram->SetUniform("model", modelMatrix);
		model->m_mesh->Render();
	}

	// Render skybox
	glFrontFace(GL_CW);
	glDepthMask(GL_FALSE);
	m_mainProgram->SetUniform("view",  glm::mat4(glm::mat3(p_activeCamera->GetViewMatrix())));
	m_mainProgram->SetUniform("model", glm::mat4());
	m_skyboxMesh->Render();
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CCW);

	// Draw main framebuffer
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	//
	m_screenProgram->Use();
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
