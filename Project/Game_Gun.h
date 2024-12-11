#ifndef GAME_GUN_H
#define GAME_GUN_H

#include "Screen.h"
#include "ScreenManager.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Music.h"
#include "Sound.h"
#include "Text.h"
#include "Bullet.h"
#include "Button.h"
#include <vector>
#include <algorithm>
#include <cmath>

namespace Engine {
	class Game_Gun :
		public Engine::Screen
	{
	public:
		Game_Gun();
		~Game_Gun();
		virtual void Init();
		virtual void Update();
		virtual void Draw();
	private:
		enum class State {
			PLAY,
			GAME_OVER,
			PAUSE,
			MAPSELECTION
		};
		State state = State::MAPSELECTION;

		Texture* dotTexture = NULL;
		vector<Sprite*> platforms;
		vector<Sprite*> objects;

		vector<Sprite*> platforms2;
		vector<Sprite*> objects2;

		vector<Bullet*> inUseBullets;
		vector<Bullet*> readyBullets;

		vector<Bullet*> inUseBullets2;
		vector<Bullet*> readyBullets2;

		vector<Button*> buttons;

		Texture* texture = NULL, * bulletTexture = NULL, * bulletTexture2 = NULL;
		Sprite* charSprite = NULL;
		Sprite* enemySprite = NULL;
		Sprite* backgroundSprite = NULL, * backgroundSprite2 = NULL, * winGuiSprite = NULL, * winGuiRedSprite = NULL, * drawGuiSprite = NULL, * pauseGuiSprite = NULL,
			* bgMapSelection = NULL, * timeHealth = NULL;
		Sprite* dotSprite1 = NULL;
		Sprite* dotSprite2 = NULL;
		Sprite* dotSprite3 = NULL;
		Sprite* dotSprite4 = NULL;
		Music* music = NULL,* mainMenuMusic = NULL;
		Sound* sound = NULL, * gunSound = NULL,* gunReleaseSound = NULL, * gunSound2 = NULL, * gunReleaseSound2 = NULL, * deadSound = NULL, * pickSound = NULL,* selectSound = NULL, * win = NULL;
		Text* text = NULL, * healt = NULL,* timeText = NULL;
		float yVelocity = 0, gravity = 0, timeInterval = 0, xBulletVel = 0, jumpSpeed = 0, fallSpeed = 0, timeInterval2 = 0, xBulletVel2 = 0;
		float yVelocity2 = 0, worldWidth = 0, worldHeight = 0, currentZoom = 1.0f, gameTime = 0.0f;
		int currentButtonIndex = 0, currentTimeButton = 0,mapIndex = 0, timeIndex  = 0;

		float knockbackVelocity = 0.0f, knockbackDuration = 0.0f; // char
		float knockbackVelocity2 = 0.0f, knockbackDuration2 = 0.0f; // enemy
		int nyawa = 10, nyawa2 = 10, winner = 0;
		bool jump = false, jump2 = false, debug = false, toRight = true, toRight2 = true, playMusic = false, playMusicMainMenu = true;
		vec2 cameraOffset = vec2(0, 0);
		float cameraSpeed = 0.2f;

		template <typename T>
		T clamp(T value, T min, T max) {
			if (value < min) return min;
			if (value > max) return max;
			return value;
		}

		void SpawnBullets();
		void SpawnBullets2();
		void RestartGame();
		void Game_Gun::UpdateCamera();
	};
}
#endif

