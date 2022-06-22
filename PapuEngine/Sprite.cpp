#include "Sprite.h"
#include "ResourceManager.h"

Sprite::Sprite() { }

Sprite::~Sprite() { }

void Sprite::draw(SpriteBacth& spritebatch) {
	int textureID = ResourceManager::getTexture(_texturePath).id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(_position.x, _position.y, SPRITE_WIDTH, SPRITE_WIDTH);
	spritebatch.draw(destRect, uvRect, textureID, 0.0f, color);
}