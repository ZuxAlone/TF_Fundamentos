#pragma once
#include <SDL\SDL.h>
#include <GL\glew.h>
#include <string>

using namespace std;
//1
//10
//100
enum WindowFlags {
	INVISBLE = 0x1,FULLSCREEN=0x2, BORDERLESS=0x4
};

class Window
{
private:
	SDL_Window *_window;
	int _screenWidth, _screenHeight;
public:
	Window();
	~Window();
	void swapBuffer();
	void setGLColor(GLfloat r, GLfloat g, GLfloat b);
	int getScreenWidth() { return _screenWidth; };
	int getScreenHeight() { return _screenHeight; };
	int create(string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);
};

