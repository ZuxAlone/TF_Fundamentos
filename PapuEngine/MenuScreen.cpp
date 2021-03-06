#include "MenuScreen.h"
#include "ScreenIndices.h"
#include <iostream>


MenuScreen::MenuScreen(Window* window):_window(window)
{
	_screenIndex = SCREEN_INDEX_MENU;
	nextScreen = SCREEN_INDEX_GAMEPLAY;
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::build()
{
	background = new Background("Textures/Fondos/splash.png", _window);
	//buttonLevel = new ButtonText("Textures/UI/button_64x16.png", "Editar Niveles", -128, -32, 256, 64);
	buttonCredits = new ButtonText("Textures/UI/button_64x16.png", "Ver Creditos", -128, 10, 256, 64);
}

void MenuScreen::destroy()
{
	//buttonLevel = nullptr;
	buttonCredits = nullptr;
	background = nullptr;
}

void MenuScreen::onExit()
{
}

void MenuScreen::onEntry()
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

void MenuScreen::draw()
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
	//buttonLevel->draw(_spriteBatch);
	buttonCredits->draw(_spriteBatch);

	char buffer[256];
	sprintf_s(buffer, "Presiona espacio para continuar...");
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

void MenuScreen::update()
{
	draw();
	_camera.update();
	_inputManager.update();
	checkInput();
}

void MenuScreen::checkInput()
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
			/*if (buttonLevel->click(mouseCoords)) {
				nextScreen = SCREEN_INDEX_LEVEL_EDITOR_SELECTOR;
				_currentState = ScreenState::CHANGE_NEXT;
			}*/
			if (buttonCredits->click(mouseCoords)) {
				nextScreen = SCREEN_INDEX_CREDITS;
				_currentState = ScreenState::CHANGE_NEXT;
			}
		}
	}
}

int MenuScreen::getNextScreen() const
{
	return nextScreen;
}

int MenuScreen::getPreviousScreen() const
{
	return SCREEN_INDEX_NO_INDEX;
}