#pragma once
#include "ShaderManager.hpp"
#include <SDL.h>

class Application {
public:
	double accumulator = 0.0;
	Uint64 currentTick = 0.0;
	double currentTime = 0.0;
	SDL_GLContext glContext;
	bool running = true;
	ShaderManager shaderMan;
	SDL_Window *window = NULL;
	~Application();
	void handleKeydownEvent(SDL_Keycode sym);
	void handleKeyupEvent(SDL_Keycode sym);
	void initialise(int windowWidth, int windowHeight);
	void initialiseShaders();
	void pollEvents();
	void render();
	void update();
};
