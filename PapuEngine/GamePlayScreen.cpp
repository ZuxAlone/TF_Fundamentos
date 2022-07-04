#include "GamePlayScreen.h"
#include "Game.h"

#include "ImageLoader.h"
#include <iostream>
#include "ResourceManager.h"
#include "PapuEngine.h"
#include <random>
#include <ctime>
#include "ScreenIndices.h"



GamePlayScreen::GamePlayScreen(Window* window):
	_window(window)
{
	_currenLevel = 0;
	_screenIndex = SCREEN_INDEX_GAMEPLAY;
	levelState = LevelState::PLAYING;
}


GamePlayScreen::~GamePlayScreen()
{
}


void GamePlayScreen::build() {
	_player = new Player();
	switch (_currenLevel) {
	case 0:
		_levels.push_back(new Level("Levels/level1.txt"));
	case 1:
		_levels.push_back(new Level("Levels/level2.txt"));
	case 2:
		_levels.push_back(new Level("Levels/level3.txt"));
	case 3:
		_levels.push_back(new Level("Levels/level4.txt"));
	}
	backButton = new Button("Textures/UI/back.png",
		int(_window->getScreenWidth() / 2) - 64, int(_window->getScreenHeight() / 2) - 64, 32, 32);
	_key = new Key();
	_door = new Door();
	_portal = new Portal();
	std::string tilemapChars = "Textures/GamePlay/tilemap_characters.png";
	std::string portal = "Textures/GamePlay/tilemap_scenary.png";

	_player->init(0.5f, _levels[_currenLevel]->getPlayerPosition(), &_inputManager, &_camera, tilemapChars);
	_player->setUvRect(0.0f, 0.25f, 1.0f, 0.25f);
	_key->init(_levels[_currenLevel]->getKeyPosition(), tilemapChars);
	_key->setUvRect(0.0f, 0.0f, 1.0f, 0.25f);
	_door->init(_levels[_currenLevel]->getDoorPosition(), "Textures/GamePlay/tilemap_door.png");
	_door->setUvRect(0.0f, 0.5f, 1.0f, 0.5f); // isClosed
	_portal->init(_levels[_currenLevel]->getPortalPosition(), portal);
	_portal->setUvRect(0.0f, 0.0f, 1.0f, 0.25f);
	_humans.push_back(_player);
	_spriteBatch.init();

	_backgroundWin = new Background("Textures/Fondos/screen_win.png", _window);
	_backgroundLost = new Background("Textures/Fondos/screen_game-over.png", _window);

	std::mt19937 randomEngine(time(nullptr));
	std::uniform_int_distribution<int>randPosX(
		2, _levels[_currenLevel]->getWidth() - 3);
	std::uniform_int_distribution<int>randPosY(
		2, _levels[_currenLevel]->getHeight() - 3);

	for (int i = 0; i < _levels[_currenLevel]->getNumHumans(); i++)
	{
		_humans.push_back(new Human());
		glm::vec2 pos(randPosX(randomEngine) * TILE_WIDTH,
			randPosY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(0.4f, pos, tilemapChars);
		_humans.back()->setUvRect(0.0f, 0.5f, 1.0f, 0.25f);
	}

	const std::vector<glm::vec2>& zombiePosition =
		_levels[_currenLevel]->getZombiesPosition();

	for (size_t i = 0; i < zombiePosition.size(); i++)
	{
		_zombies.push_back(new Zombie());
		_zombies.back()->init(0.4f, zombiePosition[i], tilemapChars);
		_zombies.back()->setUvRect(0.0f, 0.75f, 1.0f, 0.25f);
	}
}
void GamePlayScreen::destroy() {
}
void GamePlayScreen::onExit() {
}
void GamePlayScreen::onEntry() {
	_program.compileShaders("Shaders/colorShaderVert.txt",
		"Shaders/colorShaderFrag.txt");
	_program.addAtribute("vertexPosition");
	_program.addAtribute("vertexColor");
	_program.addAtribute("vertexUV");
	_program.linkShader();

	_camera.init(_window->getScreenWidth(), _window->getScreenHeight());
	_window->setGLColor(0.195f, 0.273, 0.215); //50, 70, 55  1.0f, 1.0f, 0.0f
	_spriteFont = new SpriteFont("Fonts/Fuente2.ttf", 35);
}
void GamePlayScreen::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_program.use();

	glActiveTexture(GL_TEXTURE0);

	GLuint pLocation = _program.getUniformLocation("P");

	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	GLuint imageLocation = _program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);

	_spriteBatch.begin();

	if (levelState == LevelState::PLAYING) {
		drawAgents();
		drawUI();
	}
	else drawBackground();

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_program.unuse();
	_window->swapBuffer();
}

void GamePlayScreen::drawUI() {
	char buffer[256];
	if (!_player->hasKey()) sprintf_s(buffer, "No tienes la llave!");
	else sprintf_s(buffer, "Busca la salida!");
	Color color;
	color.set(255, 252, 161, 255);
	_spriteFont->draw(_spriteBatch, buffer, _camera.getPosition() + glm::vec2(- _window->getScreenWidth() / 2.1, _window->getScreenHeight() / 2.5), glm::vec2(1), 0.0f, color);
	/*backButton->setPosition( _camera.getPosition().x + _window->getScreenWidth() / 2 - 64, - _camera.getPosition().y - _window->getScreenHeight() / 2 + 64);
	backButton->draw(_spriteBatch);*/
}

void GamePlayScreen::drawAgents() {
	_levels[_currenLevel]->draw();

	if (!_player->hasKey()) _key->draw(_spriteBatch);
	_door->draw(_spriteBatch);//if (!_door->isOpen()) 

	for (size_t i = 0; i < _humans.size(); i++)
	{
		_humans[i]->draw(_spriteBatch);

	}

	for (size_t i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->draw(_spriteBatch);
	}
}

void GamePlayScreen::drawBackground() {
	if (levelState == LevelState::WON)
		_backgroundWin->draw(_spriteBatch);
	if (levelState == LevelState::LOST)
		_backgroundLost->draw(_spriteBatch);
}

void GamePlayScreen::update() {
	checkInput();
	draw();
	_camera.update();
	updateAgents();
	_inputManager.update();
	if (levelState == LevelState::PLAYING) _camera.setPosition(_player->getPosition());
	else _camera.setPosition(glm::vec2(0.0f, 0.0f));
}

void GamePlayScreen::updateAgents() {

	if (!_player->hasKey()) {
		if (_player->collideWithKey(_key)) {
			_player->getKey();
			_door->openDoor();
			_door->setUvRect(0.0f, 0.0f, 1.0f, 0.5f); // isOpen
			glm::vec2 doorPos = _door->getPosition();
			_levels[_currenLevel]->setDot(doorPos);
		}
	}
	if (_player->collideWithPortal(_portal) && _currenLevel < 3) {
		
			_currenLevel += 1;	
			for (size_t i = 0; i < _humans.size(); i++)
			{
				delete _humans[i];
				_humans[i] = _humans.back();
				_humans.pop_back();
			}
			for (size_t i = 0; i < _zombies.size(); i++)
			{
				delete _zombies[i];
				_zombies[i] = _zombies.back();
				_zombies.pop_back();
			}
			delete _key;
			delete _door;
			delete _portal;
			delete _backgroundWin;
			delete _backgroundLost;
			build();
	}

	if (_player->collideWithPortal(_portal) && _currenLevel == 3 ) levelState = LevelState::WON;
	

	for (size_t i = 0; i < _humans.size(); i++)
	{

		_humans[i]->update(_levels[_currenLevel]->getLevelData(),
			_humans, _zombies);
	}

	for (size_t i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currenLevel]->getLevelData(),
			_humans, _zombies);

		if (_zombies[i]->collideWithAgent(_player)) levelState = LevelState::LOST;

		for (size_t j = 1; j < _humans.size(); j++)
		{
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				_zombies.push_back(new Zombie);
				_zombies.back()->init(0.4f, _humans[j]->getPosition(), "Textures/GamePlay/tilemap_characters.png");
				_zombies.back()->setUvRect(0.0f, 0.75f, 1.0f, 0.25f);

				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}
	}
}

void GamePlayScreen::checkInput() {
	SDL_Event event;
	const float CAMERA_SPEED = 20.0f;
	const float SCALE_SPEED = 0.1f;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			_currentState = ScreenState::EXIT_APPLICATION;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(event.motion.x, event.motion.y);
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
		}

		//if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		//	//presione click;
		//	glm::vec2 mouseCoords = _camera.convertScreenToWorl(_inputManager.getMouseCoords());
		//	if (backButton->click(mouseCoords)) {
		//		_currentState = ScreenState::CHANGE_NEXT;
		//	}
		//}
	}
}

int GamePlayScreen::getNextScreen() const{ 
	return SCREEN_INDEX_MENU;

};

int GamePlayScreen::getPreviousScreen() const {
	return SCREEN_INDEX_MENU;
}
