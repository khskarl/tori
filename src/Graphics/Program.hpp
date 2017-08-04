#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <Types.hpp>

#include <GL/glew.h>

class Shader
{
public:
	enum Type {
		Vertex  = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
	 };

	 std::string m_filename = "";
	 Type m_type;
	 uint32_t m_id = 0;

	 Shader  ();
	 ~Shader ();

	 bool CompileFromFile (std::string filename, Type type);
	 bool CompileFromText (std::string text, Type type);
	 void Reload ();
	 void DeleteShader ();
};

class Program
{
public:
	Program ();
	Program (std::string filename_vs, std::string filename_fs);
	~Program ();

	void Reload ();

	void Use ();
	void SetUniform (std::string uniformName, glm::vec3 value);
	void SetUniform (std::string uniformName, glm::mat4x4 value);
	void SetUniform (std::string uniformName, uint32_t value);
private:
	GLint m_id = 0;

	Shader m_vertexShader;
	Shader m_fragmentShader;

	void LoadFromFiles (std::string filename_vs, std::string filename_fs);
	void Link ();
	void DeleteProgram ();

	int32_t CompileShaderFromString (const char* sourcecode, Shader::Type type);
	int32_t CompileShaderFromFile   (std::string filename,   Shader::Type type);

	GLint GetUniformLocation(std::string uniformName);
};

#endif
