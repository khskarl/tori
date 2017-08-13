#include "Cubemap.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "TextureLoader.hpp"
#include "MeshLoader.hpp"

Cubemap::Cubemap (std::string texture_name) {
	m_program = new Program("skybox.vs", "skybox.fs");
	SetupEquirectangular(texture_name);
}

void Cubemap::Bind (uint32_t bind_position) {
	m_texture->Bind(bind_position);
}

void Cubemap::Render (Camera* camera) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFrontFace(GL_CW);
	glDepthMask(GL_FALSE);
	m_program->Use();
	m_program->SetUniform("view",  glm::mat4(glm::mat3(camera->GetViewMatrix())));
	m_program->SetUniform("projection", camera->GetProjectionMatrix());
	m_program->SetUniform1ui("skyboxTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture->m_id);
	m_mesh->Render();
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CCW);
}

void Cubemap::SetupEquirectangular (std::string texture_name) {
	m_mesh = Data::LoadMesh("cube.obj");
	Program* conversionProgram = new Program("equirectangular_to_cube.vs",
	                                         "equirectangular_to_cube.fs");
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

	m_texture = new Texture();
	m_texture->m_filename = texture_name;
	m_texture->m_type     = Texture::Type::Cubemap;
	m_texture->m_id       = cubemap;
	m_texture->m_width    = 512;
	m_texture->m_height   = 512;
	m_texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
	m_texture->m_minFilteringMode = (Texture::FilteringMode)  GL_LINEAR;
	m_texture->m_edgeSampleModeS  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
	m_texture->m_edgeSampleModeT  = (Texture::EdgeSampleMode) GL_CLAMP_TO_EDGE;
}
