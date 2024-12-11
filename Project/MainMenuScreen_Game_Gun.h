#ifndef MAINMENUSCREEN_Game_Gun_H
#define MAINMENUSCREEN_Game_Gun_H

#include "Screen.h"
#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include "Sound.h"
#include <vector>
#include "ScreenManager.h"

namespace Engine {
	class MainMenuScreen_Game_Gun :
		public Engine::Screen
	{
	public:
		MainMenuScreen_Game_Gun();
		void Init();
		void Update();
		void Draw();
	private:
		vector<Button*> buttons;
		Sprite* backgroundSprite = NULL;
		Text* text;
		Sound* selectSound = NULL, * pickSound = NULL;
		int currentButtonIndex = 0;

	};
}

#endif