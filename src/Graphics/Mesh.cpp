#include "Mesh.hpp"

#include "MeshLoader.hpp"

#include <vector>
#include <iostream>

Mesh::Mesh () {}

Mesh::~Mesh () {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers     (1, &m_EBO);
	glDeleteBuffers(1, &m_positionVBO);
	glDeleteBuffers(1, &m_texCoordVBO);
	glDeleteBuffers(1, &m_normalVBO);
	glDeleteBuffers(1, &m_colorVBO);
}

void Mesh::Setup () {
	if (m_isSetup == true) {
		std::cerr << "[ERROR] " << m_filename << " is already setup\n";
		return;
	}
	m_isSetup = true;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers     (1, &m_EBO);
	glGenBuffers(1, &m_positionVBO);
	glGenBuffers(1, &m_texCoordVBO);
	glGenBuffers(1, &m_normalVBO);
	glGenBuffers(1, &m_colorVBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             m_indices.size() * sizeof(uint32_t),
	             &m_indices[0],
	             GL_STATIC_DRAW);

	// (0) Position Attribute
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
	glBufferData(GL_ARRAY_BUFFER,
	             m_vertices.positions.size() * sizeof(glm::vec3),
	             &m_vertices.positions[0],
	             GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// (1) Texture Cordinate Attribute
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER,
	             m_vertices.texCoords.size() * sizeof(glm::vec2),
	             &m_vertices.texCoords[0],
	             GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	// (2) Normal attribute
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
	glBufferData(GL_ARRAY_BUFFER,
	             m_vertices.normals.size() * sizeof(glm::vec3),
	             &m_vertices.normals[0],
	             GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);


	// (3) Color attribute
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER,
	             m_vertices.colors.size() * sizeof(glm::vec4),
	             &m_vertices.colors[0],
	             GL_STATIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Render () {
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
