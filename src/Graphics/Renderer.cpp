#include "Renderer.hpp"

#include <iostream>
#include <Types.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

#include "TextureLoader.hpp"
#include "MeshLoader.hpp"

Renderer::Renderer () {}
Renderer::~Renderer () {}

void Renderer::Setup () {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Load and setup Programs
	m_mainProgram = new Program("pbr.vs", "pbr.fs");

	// Load and setup Meshes and Textures
	Data::LoadAllMeshes();
	Data::LoadAllTextures();

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
}

void Renderer::RenderFrame () {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
		// std::cout << light->direction.x << " " << light->direction.y << " " << light->direction.z << "\n";
		std::string lightsStr = "gLights[" + std::to_string(i) + "]";
		m_mainProgram->SetUniform(lightsStr + ".position",  light->position);
		m_mainProgram->SetUniform(lightsStr + ".direction", light->direction);
		m_mainProgram->SetUniform(lightsStr + ".color",     light->color);
		m_mainProgram->SetUniform(lightsStr + ".type",      light->type);

		// std::cout << lightsStr + ".position" << "\n";

	}
	m_mainProgram->SetUniform("gLights[0].direction",  glm::vec3(0, -1, 0));

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

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	ImGui::Render();
}

void Renderer::Submit (GameObject* const object) {
	m_renderQueue.push_back(object);
}

// void Renderer::RequestResources (GameObject* const object) {
//
// }

void Renderer::SetActiveCamera (Camera* const camera) {
	p_activeCamera = camera;
}
