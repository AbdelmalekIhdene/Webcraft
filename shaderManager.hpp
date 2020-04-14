#pragma once
#include <SDL_opengles2.h>
#include <vector>
using namespace std;

class ShaderManager {
public:
	vector<GLuint> programs;
	vector<GLuint> shaders;
	void appendShader(GLenum type, char const *path);
	void appendProgram(int shaderCount, ...);
	void deleteShaders();
	~ShaderManager();
};
