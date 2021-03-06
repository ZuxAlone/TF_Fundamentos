#pragma once
#include "IGameScreen.h"
#include "GLS_Program.h"
#include "Window.h"
#include "SpriteBacth.h"
#include "Background.h"
#include "SpriteFont.h"
#include "Camera2D.h"
#include "Button.h"
#include "ButtonText.h"
#include "InputManager.h"
#include <vector>

//Pantalla para seleccionar el nivel a editar o crear nuevo nivel
class LevelEditorSelectorScreen : public IGameScreen
{
protected:
	int _screenIndex = 0;
private:
	GLS_Program _program;
	Window* _window = nullptr;
	SpriteBacth _spriteBatch;
	Background* background;
	Button* backButton;
	ButtonText* newLevelButton;
	vector<ButtonText*> levelButtons;
	vector<std::string> _levelFiles;
	vector<std::string> _levelNames;
	InputManager inputManager;
	Camera2D _camera;
	void checkInput();
	SpriteFont* spriteFont;
	int nextScreen;
public:
	LevelEditorSelectorScreen(Window* window);
	~LevelEditorSelectorScreen();
	virtual void build() override;
	virtual void destroy() override;
	virtual void onExit() override;
	virtual void onEntry() override;
	virtual void draw()override;
	virtual void update()override;
	virtual int getNextScreen() const override;
	virtual int getPreviousScreen() const override;
};

