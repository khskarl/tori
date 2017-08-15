#include "Primitives.hpp"

namespace Primitives {
void Quad (Mesh* out_mesh) {
// 	float quadVertices[] = {
// // Positions   | UVs
// 	-1.0f,  1.0f,  0.0f, 1.0f,
// 	-1.0f, -1.0f,  0.0f, 0.0f,
// 	 1.0f, -1.0f,  1.0f, 0.0f,
//
// 	-1.0f,  1.0f,  0.0f, 1.0f,
// 	 1.0f, -1.0f,  1.0f, 0.0f,
// 	 1.0f,  1.0f,  1.0f, 1.0f
// 	};

	out_mesh->m_vertices.positions = {
		glm::vec3(-1, 1, 0),
		glm::vec3(-1,-1, 0),
		glm::vec3( 1,-1, 0),
		glm::vec3( 1, 1, 0),
	};

	out_mesh->m_indices = {
		0, 1, 2,
		0, 2, 3,
	};

	out_mesh->m_vertices.texCoords = {
		glm::vec2( 0, 1),
		glm::vec2( 0, 0),
		glm::vec2( 1, 0),
		glm::vec2( 1, 1),
	};

	out_mesh->m_vertices.normals = {
		glm::vec3( 1, 0, 0),
		glm::vec3( 1, 0, 0),
		glm::vec3( 1, 0, 0),
		glm::vec3( 1, 0, 0),
	};

	out_mesh->m_vertices.colors = {
		glm::vec4( 1, 1, 1, 1),
		glm::vec4( 1, 1, 1, 1),
		glm::vec4( 1, 1, 1, 1),
		glm::vec4( 1, 1, 1, 1),
	};

	out_mesh->m_filename = "generated_quad";
	out_mesh->Setup();
}

void Grid (size_t const width, size_t const depth, Mesh* out_mesh) {
	for (size_t i = 0; i <= depth; i++) {
		for (size_t j = 0; j <= width; j++) {
			glm::vec3 position = glm::vec3(j, 0, i);
			float u = j / (float) width;
			float v = i / (float) depth;
			glm::vec2 texCoord = glm::vec2(u, v);
			glm::vec3 normal   = glm::vec3(0, 1, 0);
			glm::vec4 color    = glm::vec4(1, 1, 1, 1);

			out_mesh->m_vertices.positions.push_back(position);
			out_mesh->m_vertices.texCoords.push_back(texCoord);
			out_mesh->m_vertices.normals.push_back(normal);
			out_mesh->m_vertices.colors.push_back(color);
		}
	}

	// const size_t num_quads = (width * depth - 1) / 2;
	const size_t quads_width = width - 1;
	const size_t quads_depth = depth - 1;
	for (size_t i = 0; i < quads_depth; i++) {
		for (size_t j = 0; j < quads_width; j++) {
			size_t index = i * quads_width + j;
			out_mesh->m_indices.push_back(index);
			out_mesh->m_indices.push_back(index + 1);
			out_mesh->m_indices.push_back(index + quads_width);

			out_mesh->m_indices.push_back(index + 1);
			out_mesh->m_indices.push_back(index + quads_width + 1);
			out_mesh->m_indices.push_back(index + quads_width);
		}
	}

	out_mesh->m_filename = "generated_grid";
	out_mesh->Setup();
}

}
