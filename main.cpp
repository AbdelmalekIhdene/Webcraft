#include "main.hpp"
#include "ShaderManager.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <SDL.h>
#include <SDL_opengles2.h>
#include <string>
#include <vector>

void assertFatal(bool condition, char const *format, ...) {
	if(condition == true) return;
	va_list arguments;
	va_start(arguments, format);
	vprintf(format, arguments);
	va_end(arguments);
	exit(EXIT_FAILURE);
}

// char *readFile(char const *path);

Application::~Application() {
	SDL_GL_DeleteContext(glContext);
	if(window != NULL) SDL_DestroyWindow(window);
	SDL_Quit();
}

void Application::handleKeydownEvent(SDL_Keycode sym) {
	switch(sym) {
		case SDLK_ESCAPE:
			running = false;
			break;
	}
}

void Application::handleKeyupEvent(SDL_Keycode sym) {
	return;
}

void Application::initialise(int windowWidth, int windowHeight) {
	assertFatal(SDL_Init(SDL_INIT_VIDEO) == 0,
	"[SDL Initialisation] %s\n", SDL_GetError());
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	window = SDL_CreateWindow("Webcraft", SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight,
	SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	assertFatal(window != NULL,
	"[SDL Window Initialisation] %s\n", SDL_GetError());
	glContext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(0);
	glClearColor(0, 0, 0, 1);
	currentTick = SDL_GetPerformanceCounter();
	initialiseShaders();
}

void Application::initialiseShaders() {
	std::vector<Shader> const shaders = {
		ShaderManager::createShader("basicVert", GL_VERTEX_SHADER, "shaders/basic.vert"),
		ShaderManager::createShader("basicFrag", GL_FRAGMENT_SHADER, "shaders/basic.frag")
	};
	shaderMan.appendProgram(
	"basicProgram", {shaders[0], shaders[1]}, {{0, "position"}});
	for(auto s : shaders) s.deleteShader();
}

void Application::pollEvents() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				handleKeydownEvent(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				handleKeyupEvent(event.key.keysym.sym);
				break;
		}
	}
}

void Application::render() {
	GLfloat const vertices[] = {
		0.0, 0.5, 0.0,
		-0.5, -0.5, 0.0,
		0.5, -0.5, 0.0
	};
	shaderMan.programs[0].useProgram();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	SDL_GL_SwapWindow(window);
}

void Application::update() {
	// printf("%lf\n", currentTime);
}

int main(int argc, char const *argv[]) {
	Application application;
	application.initialise(640, 480);
	double const tickFrequency = SDL_GetPerformanceFrequency();
	double const updateStep = 1.0 / 20.0;
	while(application.running) {
		application.pollEvents();
		Uint64 const newTick = SDL_GetPerformanceCounter();
		double const deltaTime =
		(double) (newTick - application.currentTick) / tickFrequency;
		application.accumulator += deltaTime;
		application.currentTick = newTick;
		while(application.accumulator > updateStep) {
			application.accumulator -= updateStep;
			application.currentTime += updateStep;
			application.update();
		}
		glClear(GL_COLOR_BUFFER_BIT);
		application.render();
	}
}
