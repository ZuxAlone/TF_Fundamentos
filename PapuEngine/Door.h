#pragma once
#include "Sprite.h"
class Door : public Sprite
{
private:
	bool _isOpen;

public:
	Door();
	~Door();

	bool isOpen() const { return _isOpen; }

	void init(glm::vec2 position, std::string texturePath);
};

