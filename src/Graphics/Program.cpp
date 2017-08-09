#include "Program.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader () {}
Shader::~Shader () {
	glDeleteShader(m_id);
}

bool Shader::CompileFromFile (std::string filename, Type type) {
	m_filename = filename;
	m_type = type;

	std::string filepath = "data/shaders/" + filename;
	std::ifstream file;
	file.open(filepath);
	if (!file.is_open()) {
		std::cerr << "[ERROR] Failed to load '" << filepath << "'\n";
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	return CompileFromText(stream.str(), type);
}

bool Shader::CompileFromText (std::string source, Type type) {
	const char* source_str = source.c_str();
	int32_t id = glCreateShader(type);
	glShaderSource(id, 1, &source_str, NULL);
	glCompileShader(id);

	// Check for errors
	int32_t success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == false)
	{
		char error_log[512];
		glGetShaderInfoLog(id, 512, NULL, error_log);
		switch (type) {
			case Type::Vertex:
			std::cerr << "[VERTEX SHADER COMPILATION FAILED]\n" << error_log << "\n";
			break;
			case Type::Fragment:
			std::cerr << "[FRAGMENT SHADER COMPILATION FAILED]\n" << error_log << "\n";
			break;
			default:
			std::cerr << "[UNKNOWN SHADER COMPILATION FAILED]\n" << error_log << "\n";
			break;
		}

		glDeleteShader(id);

		return false;
	}
	else {
		if (m_id != 0) DeleteShader();

		m_id = id;

		return true;
	}

}

void Shader::DeleteShader () {
	glDeleteShader(m_id);
	m_id = 0;
}

void Shader::Reload () {
	CompileFromFile(m_filename, m_type);
}

Program::Program () {}

Program::Program (std::string filename_vs, std::string filename_fs) {
	m_vertexShader.CompileFromFile   (filename_vs, Shader::Type::Vertex);
	m_fragmentShader.CompileFromFile (filename_fs, Shader::Type::Fragment);
	Link();
}

Program::~Program () {
	if (m_id != 0) {
		DeleteProgram();
	}
}

void Program::Link () {
	int32_t new_program_id = glCreateProgram();
	glAttachShader(new_program_id, m_vertexShader.m_id);
	glAttachShader(new_program_id, m_fragmentShader.m_id);
	glLinkProgram(new_program_id);

	// Check for linking errors
	int success;
	glGetProgramiv(new_program_id, GL_LINK_STATUS, &success);
	if (success == false)
	{
		char error_log[512];
		glGetProgramInfoLog(new_program_id, 512, NULL, error_log);
		std::cerr << "[LINKING FAILED]\n" << error_log << std::endl;
	}
	else
	{
		if (m_id != 0) {
			DeleteProgram();
		}
		m_id = new_program_id;
	}
}

void Program::Reload () {
	DeleteProgram();
	m_vertexShader  .Reload();
	m_fragmentShader.Reload();
	Link();
}

void Program::Use () {
	glUseProgram(m_id);
}

void Program::DeleteProgram () {
	glDeleteProgram(m_id);
	m_id = 0;
}

void Program::SetUniform(std::string uniformName, glm::vec3 value) {
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform3f(uniformLocation, value.x, value.y, value.z);
}

void Program::SetUniform(std::string uniformName, glm::mat4x4 value) {
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetUniform1i(std::string uniformName, int32_t value) {
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform1i(uniformLocation, value);
}

void Program::SetUniform1ui(std::string uniformName, uint32_t value) {
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform1ui(uniformLocation, value);
}

void Program::SetUniform1f(std::string uniformName, float value) {
	GLint uniformLocation = GetUniformLocation(uniformName);
	glUniform1f(uniformLocation, value);
}

GLint Program::GetUniformLocation(std::string uniformName) {
	return glGetUniformLocation(m_id, uniformName.c_str());
}
