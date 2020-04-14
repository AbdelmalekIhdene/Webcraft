#pragma once
#include <SDL_opengles2.h>
#include <string>
#include <tuple>
#include <vector>

class Program {
public:
	std::string name;
	GLuint identifier;
	Program(std::string name, GLuint identifier);
	void deleteProgram();
	void useProgram();
};

class Shader {
public:
	std::string name;
	GLenum type;
	GLuint identifier;
	Shader(std::string name, GLenum type, GLuint identifier);
	void deleteShader();
};

class ShaderManager {
public:
	std::vector<Program> programs;
	Shader static createShader(std::string name, GLenum type, char const *path);
	void appendProgram(std::string name, std::initializer_list<Shader> shaders,
	std::initializer_list<std::tuple<GLuint, std::string>> attributes);
	~ShaderManager();
};
