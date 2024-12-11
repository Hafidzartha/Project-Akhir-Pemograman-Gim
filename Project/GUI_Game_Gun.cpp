#include "GUI_Game_Gun.h"

Engine::GUI_Game_Gun::GUI_Game_Gun(Setting* setting) :Engine::Game(setting) 
{
	setting->windowTitle = "War AE";
}
Engine::GUI_Game_Gun::~GUI_Game_Gun()
{

}

void Engine::GUI_Game_Gun::Init()
{
	Engine::ScreenManager::GetInstance(this)->AddScreen("ingame", new Game_Gun())
		->AddScreen("mainmenu", new MainMenuScreen_Game_Gun())->SetCurrentScreen("mainmenu");
}

void Engine::GUI_Game_Gun::Update()
{
	Engine::ScreenManager::GetInstance(this)->Update();

}

void Engine::GUI_Game_Gun::Render()
{
	Engine::ScreenManager::GetInstance(this)->Draw();
}