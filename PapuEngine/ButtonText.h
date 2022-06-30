#pragma once
#include "Button.h"
#include "SpriteFont.h"

class ButtonText : public Button
{
private:
	std::string path;
	int _textureID;
	glm::vec4 uvRect;
	SpriteFont* spriteFont;
	const char* text;
	Color textColor;
	glm::vec2 textPosition;
	void setTextPosition();
public:
	ButtonText(std::string _path, const char* text);
	~ButtonText();
	void draw(SpriteBacth& spriteBatch);
	//bool click(glm::vec2 position);
	void setTextColor(GLubyte r, GLubyte g, GLubyte b);
	void setSize(int width, int height);
	void setPosition(int x, int y);
};