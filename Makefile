PHONY: compile
compile:
	clang++ $(wildcard *.cpp) \
	$(shell pkg-config --cflags --static --libs sdl2) \
	-lGL -o out -Wall
