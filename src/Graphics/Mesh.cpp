#include "Mesh.hpp"

#include <Assets/MeshLoader.hpp>


#include <vector>
#include <Log.hpp>

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
		LOG_ERROR(m_filename + " is already setup");
		return;
	}
	m_isSetup = true;

	CalculateTangentsAndBinormals();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers     (1, &m_EBO);
	glGenBuffers(1, &m_positionVBO);
	glGenBuffers(1, &m_texCoordVBO);
	glGenBuffers(1, &m_normalVBO);
	glGenBuffers(1, &m_tangentVBO);
	glGenBuffers(1, &m_binormalVBO);
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

	// (3) Tangent attribute
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
	glBufferData(GL_ARRAY_BUFFER,
	             m_vertices.tangents.size() * sizeof(glm::vec3),
	             &m_vertices.tangents[0],
	             GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);


	// (4) Binormal attribute
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, m_binormalVBO);
	glBufferData(GL_ARRAY_BUFFER,
	             m_vertices.binormals.size() * sizeof(glm::vec3),
	             &m_vertices.binormals[0],
	             GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);


	// (5) Color attribute
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER,
	             m_vertices.colors.size() * sizeof(glm::vec4),
	             &m_vertices.colors[0],
	             GL_STATIC_DRAW);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::CalculateTangentsAndBinormals () {
	for (size_t i = 0; i < m_vertices.positions.size(); i++) {
		m_vertices.tangents .push_back(glm::vec3(0));
		m_vertices.binormals.push_back(glm::vec3(0));
	}

	for (size_t f = 0; f < m_indices.size() - 2; f += 3) {
		// std::cout << f << " " << m_indices.size() << "\n";
		size_t iV0 = m_indices[f];
		size_t iV1 = m_indices[f + 1];
		size_t iV2 = m_indices[f + 2];

		glm::vec3 pos0 = m_vertices.positions[iV0];
		glm::vec3 pos1 = m_vertices.positions[iV1];
		glm::vec3 pos2 = m_vertices.positions[iV2];

		glm::vec2 uv0  = m_vertices.texCoords[iV0];
		glm::vec2 uv1  = m_vertices.texCoords[iV1];
		glm::vec2 uv2  = m_vertices.texCoords[iV2];

		glm::vec3 edge0 = pos1 - pos0;
		glm::vec3 edge1 = pos2 - pos0;
		glm::vec2 deltaUV0 = uv1 - uv0;
		glm::vec2 deltaUV1 = uv2 - uv0;

		float denominator = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

		glm::vec3 tangent = glm::normalize(
			glm::vec3(deltaUV1.y * edge0.x - deltaUV0.y * edge1.x,
			          deltaUV1.y * edge0.y - deltaUV0.y * edge1.y,
			          deltaUV1.y * edge0.z - deltaUV0.y * edge1.z) * denominator);

		glm::vec3 binormal = glm::normalize(
			glm::vec3(-deltaUV1.x * edge0.x + deltaUV0.x * edge1.x,
			          -deltaUV1.x * edge0.y + deltaUV0.x * edge1.y,
			          -deltaUV1.x * edge0.z + deltaUV0.x * edge1.z) * denominator);

		m_vertices.tangents[iV0] += tangent;
		m_vertices.tangents[iV1] += tangent;
		m_vertices.tangents[iV2] += tangent;
		m_vertices.binormals[iV0] += binormal;
		m_vertices.binormals[iV1] += binormal;
		m_vertices.binormals[iV2] += binormal;
	}

	for (size_t i = 0; i < m_vertices.tangents.size(); i++) {
		m_vertices.normals  [i] = glm::normalize(m_vertices.normals  [i]);
		m_vertices.tangents [i] = glm::normalize(m_vertices.tangents [i]);
		m_vertices.binormals[i] = glm::normalize(m_vertices.binormals[i]);
	}

}

void Mesh::Render () {
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
