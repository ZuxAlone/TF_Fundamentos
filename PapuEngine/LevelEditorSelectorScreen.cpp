#include "LevelEditorSelectorScreen.h"
#include "ScreenIndices.h"
#include <iostream>
#include <fstream>
#include "ResourceManager.h"
#include "Error.h"


LevelEditorSelectorScreen::LevelEditorSelectorScreen(Window* window) :_window(window)
{
	_screenIndex = SCREEN_INDEX_LEVEL_EDITOR_SELECTOR;
	nextScreen = SCREEN_INDEX_LEVEL_EDITOR;

	std::ifstream file;
	std::string fileName = "Levels/levelsFiles.txt";
	file.open(fileName);
	if (file.fail()) {
		fatalError("failed to open " + fileName);
	}
	std::string tmp;
	size_t count = 1;
	while (std::getline(file, tmp)) {
		_levelFiles.push_back(tmp);
		_levelNames.push_back("Nivel " + std::to_string(count));
		count++;
	}
}

LevelEditorSelectorScreen::~LevelEditorSelectorScreen()
{
}

void LevelEditorSelectorScreen::build()
{
	background = new Background("Textures/Fondos/ui_bg.png",_window);
	backButton = new Button("Textures/UI/back.png", 
		int(_window->getScreenWidth() / 2) - 64, int(_window->getScreenHeight() / 2) - 64, 32, 32);
	newLevelButton = new ButtonText("Textures/UI/button_64x16.png", "Agregar nuevo nivel",
		-int(_window->getScreenHeight() / 2) + 64, - 128, 256, 32); 

	size_t max_r = floor(_window->getScreenHeight() * 0.6 / 64);
	//size_t max_c = floor(_window->getScreenWidth() - 128 / 4);
	size_t nrows = _levelFiles.size() > max_r ? max_r : _levelFiles.size();
	size_t ncols = floor(nrows / 4) + 1;
	size_t i = 0;
	for (size_t i = 0; i < _levelFiles.size(); i++)
	{
		levelButtons.push_back(
			new ButtonText("Textures/UI/button_32x16.png", _levelNames[i].c_str(), //buffer
				-_window->getScreenWidth() / 2 + 64 + 20 * 1, -72 * i + 20,
				128, 64)
		);
	}
}

void LevelEditorSelectorScreen::destroy()
{
	backButton = nullptr;
	background = nullptr; 
	/*for (size_t i = _levelFiles.size() - 1; i > 0; --i)
	{
		_levelFiles[i] = nullptr;
	}*/
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
	newLevelButton->draw(_spriteBatch); // TO DO: FIX SHOW IMG
	for ( ButtonText* b : levelButtons) b->draw(_spriteBatch);

	char buffer[256];
	sprintf_s(buffer, "Elige el nivel a editar"); //23 / 2 = 12.5
	Color color{ 255, 255, 255, 255 };
	spriteFont->draw(_spriteBatch, buffer, glm::vec2(
		_window->getScreenWidth() / 2 - 13 * 64, 200), glm::vec2(1), 0.0f, color);

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
				//_currentState = ScreenState::CHANGE_PREVIOUS;
				nextScreen = SCREEN_INDEX_MENU;
				_currentState = ScreenState::CHANGE_NEXT;
			}
			if (newLevelButton->click(mouseCoords)) {
				nextScreen = SCREEN_INDEX_LEVEL_EDITOR_NEW;
				_currentState = ScreenState::CHANGE_NEXT;
			}
		}
	}
}

int LevelEditorSelectorScreen::getNextScreen() const
{
	return nextScreen;
}

int LevelEditorSelectorScreen::getPreviousScreen() const
{
	return SCREEN_INDEX_MENU;
}
