#pragma once
#include <SDL.h>
class Application {
public:
	double const UPDATE_STEP = 1.0 / 20.0;
	int const WIDTH = 640, HEIGHT = 480;
	double accumulator = 0.0;
	Uint64 currentTick = 0.0;
	double currentTime = 0.0;
	SDL_GLContext glContext;
	bool running = true;
	SDL_Window *window = NULL;
	~Application();
	void assertFatal(bool condition, char const *format, ...);
	void handleKeydownEvent(SDL_Keycode sym);
	void handleKeyupEvent(SDL_Keycode sym);
	void initialise();
	void pollEvents();
	void render();
	void update();
};
