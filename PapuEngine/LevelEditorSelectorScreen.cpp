#include "LevelEditorSelectorScreen.h"
#include "ScreenIndices.h"
#include <iostream>

//enum NextScreen { LEVELEDITOR, NEWLEVEL };

LevelEditorSelectorScreen::LevelEditorSelectorScreen(Window* window) :_window(window)
{
	_screenIndex = SCREEN_INDEX_LEVEL_EDITOR_SELECTOR;
}

LevelEditorSelectorScreen::~LevelEditorSelectorScreen()
{
}

void LevelEditorSelectorScreen::build()
{
	background = new Background("Textures/Fondos/ui_bg.png",_window);
	backButton = new Button("Textures/button_32x16.png");
}

void LevelEditorSelectorScreen::destroy()
{
	backButton = nullptr;
	background = nullptr;
}

void LevelEditorSelectorScreen::onExit()
{
}

void LevelEditorSelectorScreen::onEntry()
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
	spriteFont = new SpriteFont("Fonts/VT323-Regular.ttf", 32);
}

void LevelEditorSelectorScreen::draw()
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
	backButton->draw(_spriteBatch);
	char buffer[256];
	sprintf_s(buffer, "Elige el nivel a editar");
	Color color;
	color.r = 255;
	color.g = 0;
	color.b = 0;
	color.a = 255;
	spriteFont->draw(_spriteBatch, buffer, glm::vec2(-100, 50), glm::vec2(1), 0.0f, color);

	sprintf_s(buffer, "O agrega uno nuevo");
	spriteFont->draw(_spriteBatch, buffer, glm::vec2(-200, 150), glm::vec2(1), 0.0f, color);
	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_program.unuse();
	_window->swapBuffer();
}

void LevelEditorSelectorScreen::update()
{
	draw();
	_camera.update();
	inputManager.update();
	checkInput();
}

void LevelEditorSelectorScreen::checkInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.releaseKey(event.button.button);
			break;
		case SDL_MOUSEMOTION:
			inputManager.setMouseCoords(event.motion.x, event.motion.y);
			break;
		}

		if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
			//presione click;
			glm::vec2 mouseCoords = _camera.convertScreenToWorl(inputManager.getMouseCoords());
			if (backButton->click(mouseCoords)) {
				_currentState = ScreenState::CHANGE_PREVIOUS;
			}
		}
	}
}

int LevelEditorSelectorScreen::getNextScreen() const
{
	return SCREEN_INDEX_LEVEL_EDITOR;
}

int LevelEditorSelectorScreen::getPreviousScreen() const
{
	return SCREEN_INDEX_MENU;
}
