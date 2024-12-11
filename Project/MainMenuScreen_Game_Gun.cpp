#include "MainMenuScreen_Game_Gun.h"

Engine::MainMenuScreen_Game_Gun::MainMenuScreen_Game_Gun()
{
	text = NULL;
}

void Engine::MainMenuScreen_Game_Gun::Init()
{
	// Create a Texture
	Texture* texture = new Texture("buttons1.png");

	// Create Sprites
	Sprite* playSprite = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(2)->SetNumYFrames(2)->AddAnimation("normal", 0, 0)->AddAnimation("hover", 0, 1)
		->AddAnimation("press", 1, 1)->SetAnimationDuration(400)->SetScale(0.7f);

	Sprite* exitSprite = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(2)->SetNumYFrames(2)->AddAnimation("normal", 2, 2)->AddAnimation("hover", 2, 3)
		->AddAnimation("press", 3, 3)->SetAnimationDuration(400)->SetScale(0.7f);

	//Create Buttons
	Button* playButton = new Button(playSprite, "play");
	playButton->SetPosition((game->GetSettings()->screenWidth / 2) - (playSprite->GetScaleWidth() / 2),
		320);
	buttons.push_back(playButton);

	Button* exitButton = new Button(exitSprite, "exit");
	exitButton->SetPosition((game->GetSettings()->screenWidth / 2) - (exitSprite->GetScaleWidth() / 2),
		180);
	buttons.push_back(exitButton);

	Texture* bgTexture = new Texture("bg_MainMenu.png");
	backgroundSprite = (new Sprite(bgTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()));
	backgroundSprite->SetSize(game->GetSettings()->screenWidth,game->GetSettings()->screenHeight);

	// Set play button into active button
	currentButtonIndex = 0;
	buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);

	// Create Text
	text = (new Text("8-bit Arcade In.ttf", 100, game->GetDefaultTextShader()))
		->SetText("War Gun")->SetPosition(game->GetSettings()->screenWidth * 0.5f - 500, game->GetSettings()->screenHeight - 100.0f)->SetColor(235, 229, 52);
	

	// Sound SFX
	selectSound = (new Sound("menu_select.wav"))->SetVolume(100);
	pickSound = (new Sound("menu_pick.wav"))->SetVolume(100);
	
	// Add input mappings
	game->GetInputManager()->AddInputMapping("next", SDLK_DOWN)
		->AddInputMapping("prev", SDLK_UP)
		->AddInputMapping("press", SDLK_RETURN)
		->AddInputMapping("next-map", SDLK_RIGHT)
		->AddInputMapping("prev-map", SDLK_LEFT);

}

void Engine::MainMenuScreen_Game_Gun::Update()
{
	if (game->GetInputManager()->IsKeyReleased("next")) {
		selectSound->Play(false);
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Next Button
		currentButtonIndex = (currentButtonIndex < (int)buttons.size() - 1) ? currentButtonIndex + 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}

	if (game->GetInputManager()->IsKeyReleased("prev")) {
		selectSound->Play(false);
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Prev Button
		currentButtonIndex = currentButtonIndex > 0 ? currentButtonIndex - 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}

	if (game->GetInputManager()->IsKeyReleased("press")) {
		pickSound->Play(false);
		// Set current button to press state
		Button* b = buttons[currentButtonIndex];
		b->SetButtonState(Engine::ButtonState::PRESS);
		// If play button then go to InGame, exit button then exit
		if ("play" == b->GetButtonName()) {
			ScreenManager::GetInstance(game)->SetCurrentScreen("ingame");
		}
		else if ("exit" == b->GetButtonName()) {
			game->SetState(Engine::State::EXIT);
		}
	}
	buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	// Update All buttons
	for (Button* b : buttons) {
		b->Update(game->GetGameTime());
	}
}

void Engine::MainMenuScreen_Game_Gun::Draw()
{
	backgroundSprite->Draw();
	// Render all buttons
	for (Button* b : buttons) {
		b->Draw();
	}
}
