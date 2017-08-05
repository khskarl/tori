#ifndef MESH_HPP
#define MESH_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <cstdint>
#include <vector>
#include <string>

struct Vertices {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> binormals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec4> colors;
};

class Mesh
{
public:
	std::string m_filename = "";

	Vertices m_vertices;
	std::vector<uint32_t> m_indices;

	Mesh ();
	~Mesh ();

	void Setup ();
	void Render ();

// private:
	GLuint m_VAO, m_EBO;
	GLuint m_positionVBO;
	GLuint m_texCoordVBO;
	GLuint m_normalVBO;
	GLuint m_tangentVBO;
	GLuint m_binormalVBO;
	GLuint m_colorVBO;

	bool m_isSetup = false;

	void CalculateTangentsAndBinormals ();
};

#endif
