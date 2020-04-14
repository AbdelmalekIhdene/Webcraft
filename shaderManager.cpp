#include "shaderManager.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <SDL_opengles2.h>
#include <sys/stat.h>
#include <vector>
using namespace std;

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

void ShaderManager::appendShader(GLenum type, char const *path) {
	GLuint const shader = glCreateShader(type);
	char const *source = readFile(path);
	glShaderSource(shader, 1, &source, NULL);
	free((char*) source);
	glCompileShader(shader);
	char infoLog[1024];
	glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
	assertFatal(strlen(infoLog) == 0,
	"[%s Shader Initialisation] %s\n", path, infoLog);
	shaders.push_back(shader);
}

void ShaderManager::appendProgram(int shaderCount, ...) {
	GLuint const program = glCreateProgram();
	va_list arguments;
	va_start(arguments, shaderCount);
	for(int i = 0; i < shaderCount; i += 1) {
		GLuint shader = va_arg(arguments, GLuint);
		glAttachShader(program, shader);
	}
	va_end(arguments);
	glLinkProgram(program);
	char infoLog[1024];
	glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
	assertFatal(strlen(infoLog) == 0,
	"[Program Initialisation] %s\n", infoLog);
	programs.push_back(program);
	va_start(arguments, shaderCount);
	for(int i = 0; i < shaderCount; i += 1) {
		GLuint shader = va_arg(arguments, GLuint);
		glDetachShader(program, shader);
	}
	va_end(arguments);
}

void ShaderManager::deleteShaders() {
	for(int i = 0; i < shaders.size(); i += 1) {
		glDeleteShader(shaders[i]);
	}
}

ShaderManager::~ShaderManager() {
	deleteShaders();
	for(int i = 0; i < programs.size(); i += 1) {
		glDeleteProgram(programs[i]);
	}
}
