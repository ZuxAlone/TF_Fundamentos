#pragma once
#include <glm\glm.hpp>
#include <string>
#include "SpriteBacth.h"
#include "GLTexture.h"

const float SPRITE_WIDTH = 64.0f;
const float SPRITE_RADIUS = SPRITE_WIDTH / 2.0f;

class Sprite
{
protected:
	Color color;
	glm::vec2 _position;
	std::string _texturePath;

public:
	Sprite();
	virtual ~Sprite();

	glm::vec2 getPosition() const { return _position; };

	void draw(SpriteBacth& spritebatch);
};

