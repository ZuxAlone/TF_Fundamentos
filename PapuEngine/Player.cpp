#include "Player.h"
#include <SDL\SDL.h>


void Player::init(float speed, glm::vec2 position, InputManager* inputManager, Camera2D* camera) {
	_speed = speed;
	_position = position;
	_camera = camera;
	_inputManager = inputManager;
	color.set(0, 0, 185, 255);
}
void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies
	) {
	if (_inputManager->isKeyDown(SDLK_w)) {
		_position.y += _speed;
	}
	if (_inputManager->isKeyDown(SDLK_s)) {
		_position.y -= _speed;
	}
	if (_inputManager->isKeyDown(SDLK_a)) {
		_position.x -= _speed;
	}
	if (_inputManager->isKeyDown(SDLK_d)) {
		_position.x += _speed;
	}
	collideWithLevel(levelData);
}


Player::Player():_currentGun(-1)
{
}


Player::~Player()
{
}
