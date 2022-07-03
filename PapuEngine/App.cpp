#include "App.h"
#include "ScreenList.h"


App::App()
{
}


void App::onInit() {

}
void App::addScreens() {
	_gamePlayScreen =
		std::make_unique<GamePlayScreen>(&_window);
	_menuScreen =
		std::make_unique<MenuScreen>(&_window);
	_levelSelectorEditorScreen =
		std::make_unique<LevelEditorSelectorScreen>(&_window);
	_levelNewEditorScreen =
		std::make_unique<LevelEditorNewScreen>(&_window);
	_levelEditorScreen =
		std::make_unique<LevelEditorScreen>(&_window);
	// credits:
	_screenCredits =
		std::make_unique<ScreenCredits>(&_window);

	_screenList->addScreen(
		_menuScreen.get());
	_screenList->addScreen(
		_gamePlayScreen.get());
	_screenList->addScreen(
		_levelSelectorEditorScreen.get());
	_screenList->addScreen(
		_levelNewEditorScreen.get());
	_screenList->addScreen(
		_levelEditorScreen.get());
	_screenList->addScreen(
		_screenCredits.get());

	_screenList->setScreen(
		_menuScreen->getIndex());
	
	//_screenList->setScreen(
	//	_screenCredits->getIndex());
}

void App::onExit() {
}


App::~App()
{
}
