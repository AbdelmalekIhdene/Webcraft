#include "ShaderManager.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <SDL_opengles2.h>
#include <string>
#include <sys/stat.h>
#include <tuple>
#include <vector>

void assertFatal(bool condition, char const *format, ...);

char *readFile(char const *path) {
	FILE *file = fopen(path, "r");
	assertFatal(file != NULL,
	"Could not open file %s\n", path);
	struct stat statistics;
	assertFatal(fstat(fileno(file), &statistics) != -1,
	"Could not fetch size of file %s\n", path);
	char *buffer = (char*) calloc(statistics.st_size + 1, sizeof(char));
	fread(buffer, statistics.st_size, 1, file);
	fclose(file);
	return buffer;
}

Program::Program(std::string name, GLuint identifier) {
	this->name = name;
	this->identifier = identifier;
}

void Program::useProgram() {
	glUseProgram(identifier);
}

void Program::deleteProgram() {
	printf("Deleting program \"%s\"\n", name.c_str());
	glDeleteProgram(identifier);
}

Shader::Shader(std::string name, GLenum type, GLuint identifier) {
	this->name = name;
	this->type = type;
	this->identifier = identifier;
}

void Shader::deleteShader() {
	printf("Deleting shader \"%s\"\n", name.c_str());
	glDeleteShader(identifier);
}

Shader ShaderManager::createShader(std::string name, GLenum type, char const *path) {
	GLuint const shader = glCreateShader(type);
	char const *source = readFile(path);
	glShaderSource(shader, 1, &source, NULL);
	free((char*) source);
	printf("Compiling shader %s (%s)\n", name.c_str(), path);
	glCompileShader(shader);
	char infoLog[1024];
	glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
	assertFatal(strlen(infoLog) == 0,
	"Could not initialise shader %s (%s): %s\n", name.c_str(), path, infoLog);
	return Shader(name, type, shader);
}

void ShaderManager::appendProgram(std::string name, std::initializer_list<Shader> shaders,
std::initializer_list<std::tuple<GLuint, std::string>> attributes) {
	GLuint const program = glCreateProgram();
	for(auto s : shaders) {
		printf("Linking shader \"%s\" to program \"%s\"\n", s.name.c_str(), name.c_str());
		glAttachShader(program, s.identifier);
	}
	for(auto a : attributes) {
		printf("Binding program \"%s\" vertex attribute location %d to attribute variable \"%s\"\n",
		name.c_str(), std::get<0>(a), std::get<1>(a).c_str());
		glBindAttribLocation(program, std::get<0>(a), std::get<1>(a).c_str());
	}
	printf("Linking program \"%s\"\n", name.c_str());
	glLinkProgram(program);
	char infoLog[1024];
	glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
	assertFatal(strlen(infoLog) == 0,
	"Could not link program %s: %s\n", name.c_str(), infoLog);
	for(auto s : shaders) glDetachShader(program, s.identifier);
	programs.push_back(Program(name, program));
}

ShaderManager::~ShaderManager() {
	for(auto p : programs) p.deleteProgram();
}
