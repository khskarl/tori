#include "Cubemap.hpp"

#include <Log.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Assets/AssetsManager.hpp>
#include <Assets/TextureLoader.hpp>

Cubemap::Cubemap (std::string texture_name, std::string irradiance_name) {
	m_mesh = AssetsManager::Get().GetMesh("cube.obj");
	m_program = new Program("skybox.vs", "skybox.fs");
	Setup(texture_name, irradiance_name);
}

void Cubemap::BindColor (const uint32_t bind_position) {
	m_colorTexture->Bind(bind_position);
}

void Cubemap::BindIrradiance (const uint32_t bind_position) {
	m_irradianceTexture->Bind(bind_position);
}

void Cubemap::Render (Camera* const camera) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFrontFace(GL_CW);
	glDepthMask(GL_FALSE);
	m_program->Use();
	m_program->SetUniform("view",  glm::mat4(glm::mat3(camera->GetViewMatrix())));
	m_program->SetUniform("projection", camera->GetProjectionMatrix());
	m_program->SetUniform1ui("skyboxTexture", 0);
	m_colorTexture->Bind(0);
	m_mesh->Render();
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CCW);
}

Texture* Cubemap::ConvertToCubemap (Texture* equirectangularTex) {
	Program* conversionProgram = new Program("equirectangular_to_cube.vs",
	                                         "equirectangular_to_cube.fs");

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
	for (size_t i = 0; i < 6; ++i) {
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
	glm::mat4 captureViews[] = {
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
	conversionProgram->SetUniform1ui("equirectangularMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, equirectangularTex->m_id);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (size_t i = 0; i < 6; ++i) {
		conversionProgram->SetUniform("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		                       GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_mesh->Render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Texture* texture = new Texture();
	texture->m_filename = equirectangularTex->m_filename;
	texture->m_type     = Texture::Type::Cubemap;
	texture->m_id       = cubemap;
	texture->m_width    = 512;
	texture->m_height   = 512;
	texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
	texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
	texture->m_edgeSampleModeS  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
	texture->m_edgeSampleModeT  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
	return texture;
}

void Cubemap::Setup (std::string texture_name, std::string irradiance_name) {
	Texture* color2D = Loader::LoadPanorama(texture_name);
	m_colorTexture = ConvertToCubemap(color2D);

	Texture* irradiance2D = Loader::LoadPanorama(irradiance_name);
	m_irradianceTexture = ConvertToCubemap(irradiance2D);
}
