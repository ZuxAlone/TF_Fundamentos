#include "ButtonText.h"

ButtonText::ButtonText(std::string _path, const char* text)
	: Button(_path), text(text)
{
	textColor.r = 255;
	textColor.g = 255;
	textColor.b = 255;
	textColor.a = 255;
	spriteFont = new SpriteFont("Fonts/VT323-Regular.ttf", 28);
	this->setTextPosition();
}

ButtonText::~ButtonText()
{
}

void ButtonText::draw(SpriteBacth& spriteBatch)
{
	Button::draw(spriteBatch);
	char buffer[256];
	sprintf_s(buffer, this->text, 100);
	//2nd es posicion
	spriteFont->draw(spriteBatch, buffer, textPosition, glm::vec2(1), 0.0f, textColor);
}

void ButtonText::setTextColor(GLubyte r, GLubyte g, GLubyte b)
{
	textColor.r = r;
	textColor.g = g;
	textColor.b = b;
}

void ButtonText::setTextPosition()
{
	int textSize = strlen(text) * 28;
	this->textPosition = glm::vec2(x + width - (textSize / 2), int(y + height / 3.5));
}
void ButtonText::setSize(int width, int height)
{
	Button::setSize(width, height);
	this->setTextPosition();
}

void ButtonText::setPosition(int x, int y)
{
	Button::setPosition(x, y);
	this->setTextPosition();
}
