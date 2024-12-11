#ifndef GUI_GAME_GUN_H
#define GUI_GAME_GUN_H

#include "Game.h"
#include "Setting.h"
#include "Game_Gun.h"
#include "MainMenuScreen_Game_Gun.h"

namespace Engine {
	class GUI_Game_Gun :public Engine::Game
	{
	public:
		GUI_Game_Gun(Setting* setting);
		~GUI_Game_Gun();
		virtual void Init();
		virtual void Update();
		virtual void Render();
	};
}

#endif //GUI_GAME_GUN_H
