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
	m_skyboxProgram = new Program("skybox.vs", "skybox.fs");

	// Load and setup Meshes and Textures
	Data::LoadAllMeshes();
	Data::LoadAllTextures();

	SetupSkybox("newport_loft_env.hdr");

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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFrontFace(GL_CW);
	glDepthMask(GL_FALSE);
	m_skyboxProgram->Use();
	m_skyboxProgram->SetUniform("view",  glm::mat4(glm::mat3(p_activeCamera->GetViewMatrix())));
	m_skyboxProgram->SetUniform("projection",  p_activeCamera->GetProjectionMatrix());
	m_skyboxProgram->SetUniform("skyboxTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture->m_id);
	m_skyboxMesh->Render();
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CCW);

	// Draw main framebuffer
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

void Renderer::SetupSkybox (std::string texture_name) {
	m_skyboxMesh = Data::LoadMesh("cube.obj");
	Program* conversionProgram = new Program("equirectangular_to_cube.vs", "equirectangular_to_cube.fs");
	Texture* equirectangularTex = Data::LoadPanorama(texture_name);

	uint32_t captureFBO, captureRBO;
	glGenFramebuffers(1,  &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	uint32_t cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Render skybox

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3( 0.0f, 0.0f, 0.0f),
		            glm::vec3( 1.0f, 0.0f, 0.0f),
		            glm::vec3( 0.0f,-1.0f, 0.0f)),
		glm::lookAt(glm::vec3( 0.0f, 0.0f, 0.0f),
		            glm::vec3(-1.0f, 0.0f, 0.0f),
		            glm::vec3( 0.0f,-1.0f, 0.0f)),
		glm::lookAt(glm::vec3( 0.0f, 0.0f, 0.0f),
		            glm::vec3( 0.0f, 1.0f, 0.0f),
		            glm::vec3( 0.0f, 0.0f, 1.0f)),
		glm::lookAt(glm::vec3( 0.0f, 0.0f, 0.0f),
		            glm::vec3( 0.0f,-1.0f, 0.0f),
		            glm::vec3( 0.0f, 0.0f,-1.0f)),
		glm::lookAt(glm::vec3( 0.0f, 0.0f, 0.0f),
		            glm::vec3( 0.0f, 0.0f, 1.0f),
		            glm::vec3( 0.0f,-1.0f, 0.0f)),
		glm::lookAt(glm::vec3( 0.0f, 0.0f, 0.0f),
		            glm::vec3( 0.0f, 0.0f,-1.0f),
		            glm::vec3( 0.0f,-1.0f, 0.0f))
	};

	// convert HDR equirectangular environment map to cubemap equivalent
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFrontFace(GL_CW);
	glDepthMask(GL_FALSE);
	conversionProgram->Use();
	conversionProgram->SetUniform("projection", captureProjection);
	conversionProgram->SetUniform("equirectangularMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, equirectangularTex->m_id);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (size_t i = 0; i < 6; ++i) {
		conversionProgram->SetUniform("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		                       GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_skyboxMesh->Render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_skyboxTexture = new Texture();
	m_skyboxTexture->m_filename = texture_name;
	m_skyboxTexture->m_type     = Texture::Type::Cubemap;
	m_skyboxTexture->m_id       = cubemap;
	m_skyboxTexture->m_width    = 512;
	m_skyboxTexture->m_height   = 512;
	m_skyboxTexture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
	m_skyboxTexture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
	m_skyboxTexture->m_edgeSampleModeS  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
	m_skyboxTexture->m_edgeSampleModeT  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
}

void Renderer::Submit (GameObject* const object) {
	m_renderQueue.push_back(object);
}

void Renderer::SetActiveCamera (Camera* const camera) {
	p_activeCamera = camera;
}
