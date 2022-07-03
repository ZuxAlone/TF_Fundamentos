#include "ScreenCredits.h"
#include "ScreenIndices.h"
#include <iostream>


ScreenCredits::ScreenCredits(Window* window) :_window(window)
{
	_screenIndex = SCREEN_INDEX_CREDITS;
	nextScreen = SCREEN_INDEX_MENU;
}

ScreenCredits::~ScreenCredits()
{
}

void ScreenCredits::build()
{
	background = new Background("Textures/Fondos/ui_bg.png", _window);
	// -128, -32, 256,64
	buttonLevel = new ButtonText("Textures/UI/button_64x16.png", "Creditos", -128, 128, 256, 64);
}

void ScreenCredits::destroy()
{
	//buttonLevel = nullptr;
	background = nullptr;
}

void ScreenCredits::onExit()
{
}

void ScreenCredits::onEntry()
{
	_program.compileShaders("Shaders/colorShaderVert.txt",
		"Shaders/colorShaderFrag.txt");
	_program.addAtribute("vertexPosition");
	_program.addAtribute("vertexColor");
	_program.addAtribute("vertexUV");
	_program.linkShader();
	_spriteBatch.init();
	_camera.init(_window->getScreenWidth(),
		_window->getScreenHeight());
	//spriteFont = new SpriteFont("Fonts/Fuente2.ttf", 40);
	spriteFont = new SpriteFont("Fonts/VT323-Regular.ttf", 32);
}

void ScreenCredits::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_program.use();

	glActiveTexture(GL_TEXTURE0);

	GLuint pLocation =
		_program.getUniformLocation("P");

	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	GLuint imageLocation = _program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);

	_spriteBatch.begin();

	background->draw(_spriteBatch);
	buttonLevel->draw(_spriteBatch);
	
	char buffer[256];
	sprintf_s(buffer, "Dante Moreno\nRichard Maguina\nJunior Huerta\nCoralain Anto\nRealizado por:");
	Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	spriteFont->draw(_spriteBatch, buffer, glm::vec2(-180, -130), glm::vec2(1), 0.0f, color);

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_program.unuse();
	_window->swapBuffer();
}

void ScreenCredits::update()
{
	draw();
	_camera.update();
	_inputManager.update();
	checkInput();
}

void ScreenCredits::checkInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			_currentState = ScreenState::CHANGE_NEXT;
			break;
		case  SDL_KEYUP:
			_inputManager.releaseKey(event.key.keysym.sym);
			break;
		case  SDL_KEYDOWN:
			_inputManager.pressKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(event.button.button);
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(event.motion.x, event.motion.y);
			break;
		}

		if (_inputManager.isKeyDown(SDLK_SPACE)) {
			_currentState = ScreenState::CHANGE_NEXT;
		}
		if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
			//presione click;
			glm::vec2 mouseCoords = _camera.convertScreenToWorl(_inputManager.getMouseCoords());
			std::cout << "x" << mouseCoords.x << " | y " << mouseCoords.y << endl;
			if (buttonLevel->click(mouseCoords)) {
				nextScreen = SCREEN_INDEX_MENU;
				_currentState = ScreenState::CHANGE_PREVIOUS;
			}
		}
	}
}

int ScreenCredits::getNextScreen() const
{
	return nextScreen;
}

int ScreenCredits::getPreviousScreen() const
{
	return SCREEN_INDEX_NO_INDEX;
}