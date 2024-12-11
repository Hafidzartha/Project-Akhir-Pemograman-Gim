#include "Game_Gun.h"

using namespace Engine;

Game_Gun::Game_Gun()
{
}
Game_Gun::~Game_Gun()
{
	delete dotTexture;
	delete charSprite;
	delete enemySprite;
	delete winGuiSprite;
	delete winGuiRedSprite;
	delete drawGuiSprite;
	delete pauseGuiSprite;
	delete dotSprite1;
	delete dotSprite2;
	delete dotSprite3;
	delete dotSprite4;
	delete music;
	delete text;
}

void Game_Gun::Init()
{
	// Map Selection
	Texture* textureMap = new Texture("map-btn.png");
	Texture* texture = new Texture("buttons2.png");
	Texture* time = new Texture("timebtn.png");

	Sprite* mapSprite = (new Sprite(textureMap, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(2)->SetNumYFrames(2)->AddAnimation("normal", 0, 0)->AddAnimation("hover", 0, 1)->AddAnimation("normal2", 2, 2)->AddAnimation("hover2", 2, 3)
		->SetAnimationDuration(400)->SetScale(0.95f);

	Sprite* timeSprite = (new Sprite(time, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(3)->SetNumYFrames(3)->AddAnimation("normal", 0, 0)->AddAnimation("hover", 0, 1)->AddAnimation("normal2", 2, 2)->AddAnimation("hover2", 2, 3)
		->AddAnimation("normal3", 4, 4)->AddAnimation("hover3", 4, 5)->AddAnimation("normal4", 6, 6)->AddAnimation("hover4", 6, 7)
		->SetAnimationDuration(400)->SetScale(0.6f);

	Sprite* playSprite = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(2)->SetNumYFrames(2)->AddAnimation("normal", 0, 0)->AddAnimation("hover", 0, 1)
		->AddAnimation("press", 1, 1)->SetAnimationDuration(400)->SetScale(0.55f);

	Sprite* backSprite = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(2)->SetNumYFrames(2)->AddAnimation("normal", 2, 2)->AddAnimation("hover", 2, 3)
		->AddAnimation("press", 3, 3)->SetAnimationDuration(400)->SetScale(0.55f);

	Button* mapSelectionButton = new Button(mapSprite, "map");
	mapSelectionButton->SetPosition(75,(game->GetSettings()->screenHeight / 2) - (mapSprite->GetScaleHeight() / 2)*1.25f);
	buttons.push_back(mapSelectionButton);

	Button* timeSelectButton = new Button(timeSprite, "time");
	timeSelectButton->SetPosition((game->GetSettings()->screenWidth / 2) + (playSprite->GetScaleWidth()),
		400);
	buttons.push_back(timeSelectButton);

	Button* playButton = new Button(playSprite, "play");
	playButton->SetPosition((game->GetSettings()->screenWidth / 2) + (playSprite->GetScaleWidth() + ((timeSprite->GetScaleWidth() - playSprite->GetScaleWidth()) / 2)),
		270);
	buttons.push_back(playButton);

	Button* backButton = new Button(backSprite, "back");
	backButton->SetPosition((game->GetSettings()->screenWidth / 2) + (playSprite->GetScaleWidth() + ((timeSprite->GetScaleWidth()-backSprite->GetScaleWidth())/2) ),
		170);
	buttons.push_back(backButton);

	Texture* bgMapSelect= new Texture("bgGameSetup.png");
	bgMapSelection = (new Sprite(bgMapSelect, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetSize(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight);
	// Set play button into active button
	currentButtonIndex = 0;
	buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);

	// Map 1
	int map[7][13] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 3, 1, 0, 3, 2, 1, 0, 3, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0},
		{0, 3, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0},
		{0, 0, 0, 0, 3, 2, 1, 0, 0, 0, 0, 0, 0},
		{0, 3, 2, 1, 0, 0, 0, 3, 2, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	int mapObjects[7][13] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 2, 5, 0, 0, 5, 3, 0, 2, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
		{0, 2, 2, 0, 0, 0,  0, 2, 5, 0, 0, 0, 0},
		{0, 0, 0, 0, 2, 3, 3, 0, 0, 0, 0, 0, 0},
		{0, 0, 3, 3, 0, 0, 0, 0, 3, 5, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	// Generate Platforms
	int flippedMap[7][13]; // Array untuk menyimpan hasil pembalikan
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 13; j++) {
			// Balik posisi secara horizontal dan vertikal
			flippedMap[6 - i][12 - j] = map[i][j];
		}
	}
	int flippedMapObject[7][13]; // Array untuk menyimpan hasil pembalikan
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 13; j++) {
			// Balik posisi secara horizontal dan vertikal
			flippedMapObject[6 - i][12 - j] = mapObjects[i][j];
		}
	}

	// Texture platform
	Texture* platformTexture1 = new Texture("Tile 1.png");
	Texture* platformTexture2 = new Texture("Tile 2.png");
	Texture* platformTexture3 = new Texture("Tile 3.png");

	// Texture Objects
	Texture* objectTexture1 = new Texture("object_1.png");
	Texture* objectTexture2 = new Texture("object_2.png");
	Texture* objectTexture3 = new Texture("object_3.png");
	Texture* objectTexture4 = new Texture("object_4.png");
	Texture* objectTexture5 = new Texture("object_5.png");

	for (int y = 0; y < 7; y++) {
		for (int x = 0; x < 13; x++) {
			if (flippedMap[y][x] == 1) {
				Sprite* platformSprite = new Sprite(platformTexture1, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				platformSprite->SetSize(150, 80)->SetPosition(x * 150, y * 160);
				platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (16 * platformSprite->GetScale()), platformSprite->GetScaleHeight());
				platforms.push_back(platformSprite);
			}
			if (flippedMap[y][x] == 2) {
				Sprite* platformSprite = new Sprite(platformTexture2, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				platformSprite->SetSize(150, 80)->SetPosition(x * 150, y * 160);
				platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (16 * platformSprite->GetScale()), platformSprite->GetScaleHeight());
				platforms.push_back(platformSprite);
			}
			if (flippedMap[y][x] == 3) {
				Sprite* platformSprite = new Sprite(platformTexture3, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				platformSprite->SetSize(150, 80)->SetPosition(x * 150, y * 160);
				platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (16 * platformSprite->GetScale()), platformSprite->GetScaleHeight());
				platforms.push_back(platformSprite);
			}
			if (flippedMapObject[y][x] == 1) {
				Sprite* objectSprite = new Sprite(objectTexture1, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetSize(150, 80)->SetPosition(x * 150, 80 + (y * 160))->SetScale(0.9f);
				objects.push_back(objectSprite);
			}
			if (flippedMapObject[y][x] == 2) {
				Sprite* objectSprite = new Sprite(objectTexture2, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetSize(150, 80)->SetPosition(x * 150, 80 + (y * 160))->SetScale(0.7f);
				objects.push_back(objectSprite);
			}
			if (flippedMapObject[y][x] == 3) {
				Sprite* objectSprite = new Sprite(objectTexture3, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetSize(150, 80)->SetPosition(x * 150, 80 + (y * 160))->SetScale(0.5f);
				objects.push_back(objectSprite);
			}
			if (flippedMapObject[y][x] == 4) {
				Sprite* objectSprite = new Sprite(objectTexture4, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetSize(150, 80)->SetPosition(x * 150, 80 + (y * 160))->SetScale(0.6f);
				objects.push_back(objectSprite);
			}
			if (flippedMapObject[y][x] == 5) {
				Sprite* objectSprite = new Sprite(objectTexture5, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetSize(150, 80)->SetPosition(x * 150, 80 + (y * 160))->SetScale(2.0f);
				objects.push_back(objectSprite);
			}
		}
	}
	//Create background
	Texture* bgTexture = new Texture("mapBackground1.png");
	backgroundSprite = (new Sprite(bgTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetSize(3000.0f, 1500.0f);
	// End Map 1

	// Map 2
	int map2[7][13] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 3, 2, 2, 2, 2, 1, 0, 0, 0, 0},
		{0, 3, 1, 0, 0, 3, 1, 0, 0, 3, 1, 0, 0},
		{0, 0, 0, 3, 1, 0, 0, 3, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 3, 2, 2, 1, 0, 0, 0, 0, 0},
		{0, 0, 3, 2, 1, 0, 0, 3, 2, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	int mapObjects2[7][13] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 4, 3, 0, 6, 5, 0, 0, 0, 0},
		{0, 3, 3, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0},
		{0, 0, 0, 1, 2, 0, 0, 0, 2, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	};
	// Generate Platforms
	int flippedMap2[7][13]; // Array untuk menyimpan hasil pembalikan
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 13; j++) {
			// Balik posisi secara horizontal dan vertikal
			flippedMap2[6 - i][12 - j] = map2[i][j];
		}
	}
	int flippedMapObject2[7][13]; // Array untuk menyimpan hasil pembalikan
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 13; j++) {
			// Balik posisi secara horizontal dan vertikal
			flippedMapObject2[6 - i][12 - j] = mapObjects2[i][j];
		}
	}

	// Texture platform
	Texture* platformTexture12 = new Texture("tile_desert1.png");
	Texture* platformTexture22= new Texture("tile_desert2.png");
	Texture* platformTexture32= new Texture("tile_desert3.png");

	// Texture Objects
	Texture* objectTexture12 = new Texture("obj_desert1.png");
	Texture* objectTexture22 = new Texture("obj_desert2.png");
	Texture* objectTexture32 = new Texture("obj_desert3.png");
	Texture* objectTexture42 = new Texture("obj_desert4.png");
	Texture* objectTexture52 = new Texture("obj_desert5.png");
	Texture* objectTexture62 = new Texture("obj_desert6.png");

	for (int y = 0; y < 7; y++) {
		for (int x = 0; x < 13; x++) {
			if (flippedMap2[y][x] == 1) {
				Sprite* platformSprite = new Sprite(platformTexture12, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				platformSprite->SetSize(150, 65)->SetPosition(x * 150, y * 165);
				platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (16 * platformSprite->GetScale()), platformSprite->GetScaleHeight());
				platforms2.push_back(platformSprite);
			}
			if (flippedMap2[y][x] == 2) {
				Sprite* platformSprite = new Sprite(platformTexture22, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				platformSprite->SetSize(150, 65)->SetPosition(x * 150, y * 165);
				platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (16 * platformSprite->GetScale()), platformSprite->GetScaleHeight());
				platforms2.push_back(platformSprite);
			}
			if (flippedMap2[y][x] == 3) {
				Sprite* platformSprite = new Sprite(platformTexture32, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				platformSprite->SetSize(150, 65)->SetPosition(x * 150, y * 165);
				platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (16 * platformSprite->GetScale()), platformSprite->GetScaleHeight());
				platforms2.push_back(platformSprite);
			}
			if (flippedMapObject2[y][x] == 1) {
				Sprite* objectSprite = new Sprite(objectTexture12, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetPosition(x * 150, 80 + (y * 160))->SetScale(0.6f);
				objects2.push_back(objectSprite);
			}
			if (flippedMapObject2[y][x] == 2) {
				Sprite* objectSprite = new Sprite(objectTexture22, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetPosition(x * 150, 80 + (y * 160))->SetScale(1.4f);
				objects2.push_back(objectSprite);
			}
			if (flippedMapObject2[y][x] == 3) {
				Sprite* objectSprite = new Sprite(objectTexture32, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetPosition(x * 150, 80 + (y * 160))->SetScale(1.0f);
				objects2.push_back(objectSprite);
			}
			if (flippedMapObject2[y][x] == 4) {
				Sprite* objectSprite = new Sprite(objectTexture42, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetPosition(x * 150, 80 + (y * 160))->SetScale(1.0f);
				objects2.push_back(objectSprite);
			}
			if (flippedMapObject2[y][x] == 5) {
				Sprite* objectSprite = new Sprite(objectTexture52, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetPosition(x * 150, 60 + (y * 160))->SetScale(1.0f);
				objects2.push_back(objectSprite);
			}
			if (flippedMapObject2[y][x] == 6) {
				Sprite* objectSprite = new Sprite(objectTexture62, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
				objectSprite->SetPosition(x * 150, 80 + (y * 160))->SetScale(1.0f);
				objects2.push_back(objectSprite);
			}
		}
	}
	//Create background
	Texture* bgTexture2 = new Texture("mapBackground2.png");
	backgroundSprite2 = (new Sprite(bgTexture2, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetSize(3000.0f, 1500.0f);

	// Add bullets
	bulletTexture = new Texture("small_smg.png");
	int bulletNum = 50;
	for (int i = 0; i < bulletNum; i++) {
		Sprite* bs = (new Sprite(bulletTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetNumXFrames(1)->SetNumYFrames(1)->SetScale(0.4);
		readyBullets.push_back(new Bullet(bs));
	}
	// Add bullets enemy
	for (int i = 0; i < bulletNum; i++) {
		Sprite* bs2 = (new Sprite(bulletTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetNumXFrames(1)->SetNumYFrames(1)->SetScale(0.4);
		readyBullets2.push_back(new Bullet(bs2));
	}

	// UI info time and health
	Texture* timeHealthTexture = new Texture("timehealth.png");
	timeHealth = (new Sprite(timeHealthTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetScale(0.7f);
	
	//Create win Gui green
	Texture* winGui = new Texture("win_Gui.png");
	winGuiSprite = (new Sprite(winGui, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetScale(1.0f);
	
	//Create win Gui red
	Texture* winGuiRed = new Texture("win_Gui_Red.png");
	winGuiRedSprite = (new Sprite(winGuiRed, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetScale(1.0f);

	//Create Draw
	Texture* drawGui= new Texture("draw_Gui.png");
	drawGuiSprite = (new Sprite(drawGui, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetScale(1.0f);


	//Create pause gui
	Texture* pauseGui = new Texture("pause_Gui.png");
	pauseGuiSprite = (new Sprite(pauseGui, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetScale(0.8f);

	// a char
	Texture* charTexture = new Texture("soldire-1.png");
	charSprite = new Sprite(charTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	charSprite->SetPosition(1000, 1200)->SetNumXFrames(19)->SetNumYFrames(1)->SetAnimationDuration(70)->SetScale(0.4f)->AddAnimation("attack", 0, 3)
		->AddAnimation("idle", 6, 10)->AddAnimation("run", 12, 19)->AddAnimation("down", 3, 6)->AddAnimation("jump", 10, 12);
	charSprite->SetBoundingBoxSize(charSprite->GetScaleWidth() - (32 * charSprite->GetScale()),
		charSprite->GetScaleHeight() - (8 * charSprite->GetScale()));

	// a char
	Texture* enemyTexture = new Texture("soldire-2.png");
	enemySprite = new Sprite(enemyTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	enemySprite->SetPosition(1000, 1200)->SetNumXFrames(19)->SetNumYFrames(1)->SetAnimationDuration(70)->SetScale(0.4f)
		->AddAnimation("attack2", 0, 3)->AddAnimation("idle", 6, 10)->AddAnimation("run", 12, 19)->AddAnimation("down", 3, 6)->AddAnimation("jump", 10, 12);
	enemySprite->SetBoundingBoxSize(enemySprite->GetScaleWidth() - (32 * enemySprite->GetScale()),
		enemySprite->GetScaleHeight() - (8 * enemySprite->GetScale()));

	// Add input mapping Char 1
	game->GetInputManager()->AddInputMapping("Run Right", SDLK_d)->AddInputMapping("Run Left", SDLK_a)->AddInputMapping("Jump", SDLK_w)
		->AddInputMapping("Down", SDLK_s)->AddInputMapping("Attack", SDLK_f);
	// Add Input Mapping Char 2
	game->GetInputManager()->AddInputMapping("Run-Right", SDLK_l)->AddInputMapping("Run-Left", SDLK_j)->AddInputMapping("Jump-", SDLK_i)
		->AddInputMapping("Down-", SDLK_k)->AddInputMapping("Attack-", SDLK_p);
	// Add Input Mapping other
	game->GetInputManager()->AddInputMapping("tryAgain", SDLK_SPACE)->AddInputMapping("Restart", SDLK_r)->AddInputMapping("Quit", SDLK_ESCAPE)
		->AddInputMapping("next", SDLK_DOWN)
		->AddInputMapping("prev", SDLK_UP)
		->AddInputMapping("press", SDLK_RETURN)
		->AddInputMapping("next-map", SDLK_RIGHT)
		->AddInputMapping("prev-map", SDLK_LEFT);;

	// Playing music
	music = (new Music("music_ingame.ogg"))->SetVolume(75);
	mainMenuMusic = (new Music("calm_before_a_storm-247781.ogg"))->SetVolume(75)->Play(true);

	// inisiasi SFX
	sound = (new Sound("jump.wav"))->SetVolume(80);
	gunSound = (new Sound("machine-gun.wav"))->SetVolume(100);
	gunReleaseSound = (new Sound("machine-gun-release.wav"))->SetVolume(100);
	gunSound2 = (new Sound("machine-gun.wav"))->SetVolume(100);
	gunReleaseSound2 = (new Sound("machine-gun-release.wav"))->SetVolume(100);
	deadSound = (new Sound("deadSound.wav"))->SetVolume(100);
	pickSound = (new Sound("menu_pick.wav"))->SetVolume(100);
	selectSound = (new Sound("menu_select.wav"))->SetVolume(100);
	win = (new Sound("win.wav"))->SetVolume(100);

	text = new Text("lucon.ttf", 24, game->GetDefaultTextShader());
	text->SetScale(1.0f)->SetColor(255, 255, 255)->SetPosition(0, game->GetSettings()->screenHeight - (text->GetFontSize() * text->GetScale()));

	healt = new Text("JungleAdventurer.ttf", 36, game->GetDefaultTextShader());
	healt->SetScale(1.0f)->SetColor(255, 50, 50);

	timeText = new Text("JungleAdventurer.ttf", 36, game->GetDefaultTextShader());
	text->SetScale(1.0f)->SetColor(255, 255, 255);
	//Set the background color
	game->SetBackgroundColor(102, 195, 242);

	// Set debug
	debug = true;
	gravity = 0.1f;
	worldWidth = 2000.0f;  // Ganti dengan lebar dunia sebenarnya
	worldHeight = 1000.0f; // Ganti dengan tinggi dunia sebenarnya

	nyawa = 10;
	nyawa2 = 10;
}

void Game_Gun::Update()
{
	if (state == Game_Gun::State::MAPSELECTION) {
		if (currentButtonIndex == 0) {
			if (game->GetInputManager()->IsKeyReleased("next-map")) {
				selectSound->Play(false);
				// Set previous button to normal state
				buttons[0]->SetButtonState(Engine::ButtonState::HOVER2);
				mapIndex = 1;
			}
			if (game->GetInputManager()->IsKeyReleased("prev-map")) {
				selectSound->Play(false);
				buttons[0]->SetButtonState(Engine::ButtonState::HOVER);
				mapIndex = 0;
			}
		}
		if (currentButtonIndex == 1) {
			if (game->GetInputManager()->IsKeyReleased("next-map")) {
				selectSound->Play(false);
				currentTimeButton += 1;
				currentTimeButton = (currentTimeButton > 3) ? currentTimeButton = 0 : currentTimeButton;

			}
			if (game->GetInputManager()->IsKeyReleased("prev-map")) {
				selectSound->Play(false);
				currentTimeButton -= 1;
				currentTimeButton = (currentTimeButton < 0) ? currentTimeButton = 3 : currentTimeButton;
			}
			// Set next and previous button to normal state
			if (currentTimeButton == 0) {
				buttons[1]->SetButtonState(Engine::ButtonState::HOVER);
				gameTime = 120.0f;
				timeIndex = 0;
			}
			else if (currentTimeButton == 1) {
				buttons[1]->SetButtonState(Engine::ButtonState::HOVER2);
				gameTime = 300.0f;
				timeIndex = 1;
			}
			else if (currentTimeButton == 2) {
				buttons[1]->SetButtonState(Engine::ButtonState::HOVER3);
				gameTime = 600.0f;
				timeIndex = 2;
			}
			else if (currentTimeButton == 3) {
				buttons[1]->SetButtonState(Engine::ButtonState::HOVER4);
				gameTime = -1.0f;
				timeIndex = 3;
			}
		}

		if (game->GetInputManager()->IsKeyReleased("next")) {
			selectSound->Play(false);
			// Set previous button to normal state
			if (currentButtonIndex == 0) {
				if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER2) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL2);
				}
			}			
			else if (currentButtonIndex == 1) {
				if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER2) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL2);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER3) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL3);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER4) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL4);
				}
			}
			else {
				buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
			}
			// Next Button
			currentButtonIndex = (currentButtonIndex < (int)buttons.size() - 1) ? currentButtonIndex + 1 : currentButtonIndex;
			// Set current button to hover state
			buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
		}

		if (game->GetInputManager()->IsKeyReleased("prev")) {
			selectSound->Play(false);
			// Set previous button to normal state
			if (currentButtonIndex == 1) {
				if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER2) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL2);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER3) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL3);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER4) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL4);
				}
			}
			else {
				buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
			}
			// Prev Button
			currentButtonIndex = currentButtonIndex > 0 ? currentButtonIndex - 1 : currentButtonIndex;
			// Set current button to hover state
			if (currentButtonIndex == 0) {
				if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL2) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER2);
				}
			}
			else if (currentButtonIndex == 1) {
				if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL2) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER2);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL3) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER3);
				}
				else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL4) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER4);
				}
			}
			else {
				buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
			}

		}

		if (game->GetInputManager()->IsKeyReleased("press")) {
			pickSound->Play(false);
			// Set current button to press state
			Button* b = buttons[currentButtonIndex];
			b->SetButtonState(Engine::ButtonState::PRESS);
			// If play button then go to InGame, exit button then exit
			if ("play" == b->GetButtonName()) {
				if (playMusicMainMenu == true) {
					music->Play(false);
					playMusicMainMenu = false;
				}
				state = State::PLAY;
				buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
				currentButtonIndex = 0;
				if (mapIndex==0) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
				}
				else {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER2);
				}

			}
			else if ("back" == b->GetButtonName()) {
				buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
				currentButtonIndex = 0;
				if (mapIndex == 0) {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
				}
				else {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER2);
				}
				ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
			}
			else if ("map" == b->GetButtonName()) {
				selectSound->Play(false);
				// Set previous button to normal state
				if (currentButtonIndex == 0) {
					if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER) {
						buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
					}
					else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::HOVER2) {
						buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL2);
					}
				}
				else {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
				}
				// Next Button
				currentButtonIndex = (currentButtonIndex < (int)buttons.size() - 1) ? currentButtonIndex + 1 : currentButtonIndex;
				// Set current button to hover state
				buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
			}
			else if ("time" == b->GetButtonName()) {
				selectSound->Play(false);
				// Set previous button to normal state
				if (currentButtonIndex == 1) {
					if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL) {
						buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
					}
					else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL2) {
						buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER2);
					}
					else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL3) {
						buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER3);
					}
					else if (buttons[currentButtonIndex]->GetButtonState() == Engine::ButtonState::NORMAL4) {
						buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER4);
					}
				}
				else {
					buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
				}
				// Next Button
				currentButtonIndex = (currentButtonIndex < (int)buttons.size() - 1) ? currentButtonIndex + 1 : currentButtonIndex;
				// Set current button to hover state
				buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
			}
		}

		// Update All buttons
		for (Button* b : buttons) {
			b->Update(game->GetGameTime());
		}
		if (game->GetInputManager()->IsKeyReleased("Quit"))
		{
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
		}
	}
	else if (state == Game_Gun::State::PLAY) {
		if (playMusic == false) {
			music->Play(true);
			playMusic = true;
		}
		timeInterval += game->GetGameTime();
		timeInterval2 += game->GetGameTime();

		charSprite->PlayAnim("idle");
		enemySprite->PlayAnim("idle");

		//Camera
		UpdateCamera();

		//Timer
		if (timeIndex<3 && gameTime != 0) {
			gameTime -= game->GetGameTime() / 1000.0f; // Kurangi waktu berdasarkan delta time
			if (gameTime <= 0.0f) {
				gameTime = 0.0f;
			}
		}

		//>>>>>>>Character<<<<<<<<<<<
		// Move monster sprite using keyboard
		vec2 oldMonsterPos = charSprite->GetPosition();
		float x = oldMonsterPos.x, y = oldMonsterPos.y;
		if (game->GetInputManager()->IsKeyPressed("Run Right")) {
			x += 0.2f * game->GetGameTime();
			charSprite->PlayAnim("run")->SetFlipHorizontal(false);
			toRight = true;
		}

		if (game->GetInputManager()->IsKeyPressed("Run Left")) {
			x -= 0.2f * game->GetGameTime();
			charSprite->PlayAnim("run")->SetFlipHorizontal(true);
			toRight = false;
		}

		charSprite->SetPosition(x, y);


		if (game->GetInputManager()->IsKeyPressed("Jump") && !jump) {
			//Set gravity and yVelocity
			float ratio = (game->GetGameTime() / 16.7f);
			gravity = 0.1f;
			yVelocity = 1.0f;
			jump = true;
			sound->Play(false);
		}

		if (y > 0 && game->GetInputManager()->IsKeyPressed("Jump")) {
			yVelocity -= gravity / 2.5;
		}
		else if (y > 0) {
			yVelocity -= gravity;
		}

		if (game->GetInputManager()->IsKeyPressed("Attack")) {
			charSprite->PlayAnim("attack");
			SpawnBullets();
			gunSound->Play(false);
		}
		if (game->GetInputManager()->IsKeyReleased("Attack")) {
			gunReleaseSound->Play(false);
		}
		if (yVelocity < -0.8f) {
			yVelocity = -0.8f;
		}
		y += yVelocity * game->GetGameTime();
		charSprite->SetPosition(x, y);

		//>>>>>>>End Character<<<<<<<<<<<

		//>>>>>>>Enemy<<<<<<<<<<<
		vec2 enemyPos = enemySprite->GetPosition();
		float enemyX = enemyPos.x, enemyY = enemyPos.y;
		if (game->GetInputManager()->IsKeyPressed("Run-Right")) {
			enemyX += 0.2f * game->GetGameTime();
			enemySprite->PlayAnim("run")->SetFlipHorizontal(false);
			toRight2 = true;
		}

		if (game->GetInputManager()->IsKeyPressed("Run-Left")) {
			enemyX -= 0.2f * game->GetGameTime();
			enemySprite->PlayAnim("run")->SetFlipHorizontal(true);
			toRight2 = false;
		}

		if (game->GetInputManager()->IsKeyPressed("Jump-") && !jump2) {
			//Set gravity and yVelocity
			float ratio = (game->GetGameTime() / 16.7f);
			gravity = 0.1f;
			yVelocity2 = 1.0f;
			jump2 = true;
			sound->Play(false);
		}

		if (enemyY > 0 && game->GetInputManager()->IsKeyPressed("Jump-")) {
			yVelocity2 -= gravity / 2.5;
		}
		else if (enemyY > 0) {
			yVelocity2 -= gravity;
		}

		if (game->GetInputManager()->IsKeyPressed("Attack-")) {
			enemySprite->PlayAnim("attack2");
			SpawnBullets2();
			gunSound2->Play(false);
		}
		if (game->GetInputManager()->IsKeyReleased("Attack-")) {
			gunReleaseSound2->Play(false);
		}
		if (yVelocity2 < -0.8f) {
			yVelocity2 = -0.8f;
		}
		enemyY += yVelocity2 * game->GetGameTime();
		//>>>>>>>>>>>>>>>>>>ENEMY END<<<<<<<<<<<<<<<<<<<<<<<<<<

		//Collide
		if (mapIndex==0) {
			for (Sprite* s : platforms) {
				BoundingBox* platformBox = s->GetBoundingBox();
				BoundingBox* charBox = charSprite->GetBoundingBox();
				BoundingBox* enemyBox = enemySprite->GetBoundingBox();

				// Periksa jika karakter berada di atas platform dan lompatan lebih tinggi dari platform
				if (charBox->CollideWith(platformBox)) {
					float charBottom = charBox->GetVertices()[3].y; // Bawah bounding box karakter
					float platformTop = platformBox->GetVertices()[0].y; // Atas bounding box platform

					// Jika karakter sedang turun dan berada di atas platform
					if (game->GetInputManager()->IsKeyPressed("Down")) {
						yVelocity -= 0.1f;
						charSprite->PlayAnim("down");
					}
					else if (yVelocity < 0 && charBottom >= platformTop) {
						// Set posisi karakter tepat di atas platform
						y = s->GetPosition().y + s->GetScaleHeight();
						if (game->GetInputManager()->IsKeyPressed("Run Right")) {
							x += 0.2f * game->GetGameTime();
							charSprite->PlayAnim("run")->SetFlipHorizontal(false);
							toRight = true;
						}

						if (game->GetInputManager()->IsKeyPressed("Run Left")) {
							x -= 0.2f * game->GetGameTime();
							charSprite->PlayAnim("run")->SetFlipHorizontal(true);
							toRight = false;
						}
						charSprite->SetPosition(x, y);

						// Reset status lompatan
						jump = false;
						yVelocity = 0;

					}
				}
				if (enemyBox->CollideWith(platformBox)) {
					float enemyBottom = enemyBox->GetVertices()[3].y; // Bawah bounding box karakter
					float platformTop = platformBox->GetVertices()[0].y; // Atas bounding box platform

					// Jika karakter sedang berada di atas platform
					if (game->GetInputManager()->IsKeyPressed("Down-"))
					{
						yVelocity2 -= 0.1f;
						enemySprite->PlayAnim("down");
					}
					else if (yVelocity2 < 0 && enemyBottom >= platformTop)
					{
						// Set posisi karakter tepat di atas platform
						enemyY = s->GetPosition().y + s->GetScaleHeight();
						if (game->GetInputManager()->IsKeyPressed("Run-Right")) {
							enemyX += 0.2f * game->GetGameTime();
							enemySprite->PlayAnim("run")->SetFlipHorizontal(false);
							//gunSprite->SetFlipHorizontal(false);
							toRight2 = true;
						}

						if (game->GetInputManager()->IsKeyPressed("Run-Left")) {
							enemyX -= 0.2f * game->GetGameTime();
							enemySprite->PlayAnim("run")->SetFlipHorizontal(true);
							//gunSprite->SetFlipHorizontal(true);
							toRight2 = false;
						}
						//enemySprite->SetPosition(enemyX, enemyY);
						//gunSprite->SetPosition(x, y);

						// Reset status lompatan
						jump2 = false;
						yVelocity2 = 0;

					}
				}
			}
		}
		else if (mapIndex == 1) {
			for (Sprite* s : platforms2) {
				BoundingBox* platformBox = s->GetBoundingBox();
				BoundingBox* charBox = charSprite->GetBoundingBox();
				BoundingBox* enemyBox = enemySprite->GetBoundingBox();

				// Periksa jika karakter berada di atas platform dan lompatan lebih tinggi dari platform
				if (charBox->CollideWith(platformBox)) {
					float charBottom = charBox->GetVertices()[3].y; // Bawah bounding box karakter
					float platformTop = platformBox->GetVertices()[0].y; // Atas bounding box platform

					// Jika karakter sedang turun dan berada di atas platform
					if (game->GetInputManager()->IsKeyPressed("Down")) {
						yVelocity -= 0.1f;
						charSprite->PlayAnim("down");
					}
					else if (yVelocity < 0 && charBottom >= platformTop) {
						// Set posisi karakter tepat di atas platform
						y = s->GetPosition().y + s->GetScaleHeight();
						if (game->GetInputManager()->IsKeyPressed("Run Right")) {
							x += 0.2f * game->GetGameTime();
							charSprite->PlayAnim("run")->SetFlipHorizontal(false);
							toRight = true;
						}

						if (game->GetInputManager()->IsKeyPressed("Run Left")) {
							x -= 0.2f * game->GetGameTime();
							charSprite->PlayAnim("run")->SetFlipHorizontal(true);
							toRight = false;
						}
						charSprite->SetPosition(x, y);

						// Reset status lompatan
						jump = false;
						yVelocity = 0;

					}
				}
				if (enemyBox->CollideWith(platformBox)) {
					float enemyBottom = enemyBox->GetVertices()[3].y; // Bawah bounding box karakter
					float platformTop = platformBox->GetVertices()[0].y; // Atas bounding box platform

					// Jika karakter sedang berada di atas platform
					if (game->GetInputManager()->IsKeyPressed("Down-"))
					{
						yVelocity2 -= 0.1f;
						enemySprite->PlayAnim("down");
					}
					else if (yVelocity2 < 0 && enemyBottom >= platformTop)
					{
						// Set posisi karakter tepat di atas platform
						enemyY = s->GetPosition().y + s->GetScaleHeight();
						if (game->GetInputManager()->IsKeyPressed("Run-Right")) {
							enemyX += 0.2f * game->GetGameTime();
							enemySprite->PlayAnim("run")->SetFlipHorizontal(false);
							//gunSprite->SetFlipHorizontal(false);
							toRight2 = true;
						}

						if (game->GetInputManager()->IsKeyPressed("Run-Left")) {
							enemyX -= 0.2f * game->GetGameTime();
							enemySprite->PlayAnim("run")->SetFlipHorizontal(true);
							//gunSprite->SetFlipHorizontal(true);
							toRight2 = false;
						}
						//enemySprite->SetPosition(enemyX, enemyY);
						//gunSprite->SetPosition(x, y);

						// Reset status lompatan
						jump2 = false;
						yVelocity2 = 0;

					}
				}
			}
		}

		//knockback bullet char
		for (Bullet* b : inUseBullets) {
			BoundingBox* bulletBox = b->GetSprite()->GetBoundingBox();
			BoundingBox* enemyBox = enemySprite->GetBoundingBox();
			BoundingBox* charBox = charSprite->GetBoundingBox();

			if (enemyBox->CollideWith(bulletBox)) {
				// Tentukan arah knockback berdasarkan posisi relative
				if (enemySprite->GetPosition().x > charSprite->GetPosition().x) {
					knockbackVelocity = 8.0f; // Knockback ke kanan
				}
				else {
					knockbackVelocity = -8.0f; // Knockback ke kiri
				}

				knockbackDuration = 1.5f; // Knockback berlangsung selama 0.3 detik

				// Kembalikan peluru ke pool
				readyBullets.push_back(b);
				inUseBullets.erase(remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
			}

			// If bullet off screen then remove a bullet from in-use container, and insert into ready-to-use container
			if (b->GetPosition().x < -b->sprite->GetScaleWidth() || b->GetPosition().x > worldWidth) {
				readyBullets.push_back(b);
				inUseBullets.erase(remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
			}

			b->Update(game->GetGameTime());
		}
		if (knockbackDuration > 0) {
			// Update posisi enemy dengan knockback
			enemyX += knockbackVelocity * game->GetGameTime();
			knockbackDuration -= game->GetGameTime(); // Kurangi durasi knockback

			// Hentikan knockback jika waktunya habis
			if (knockbackDuration <= 0) {
				knockbackVelocity = 0.0f;
			}
		}

		//knock back bullet enemy
		for (Bullet* b : inUseBullets2) {
			BoundingBox* bulletBox = b->GetSprite()->GetBoundingBox();
			BoundingBox* enemyBox = enemySprite->GetBoundingBox();
			BoundingBox* charBox = charSprite->GetBoundingBox();

			if (charBox->CollideWith(bulletBox)) {
				// Tentukan arah knockback berdasarkan posisi relative
				if (charSprite->GetPosition().x > enemySprite->GetPosition().x) {
					knockbackVelocity2 = 8.0f; // Knockback ke kanan
				}
				else {
					knockbackVelocity2 = -8.0f; // Knockback ke kiri
				}

				knockbackDuration2 = 1.5f; // Knockback berlangsung selama 0.3 detik

				// Kembalikan peluru ke pool
				readyBullets2.push_back(b);
				inUseBullets2.erase(remove(inUseBullets2.begin(), inUseBullets2.end(), b), inUseBullets2.end());
			}

			// If bullet off screen then remove a bullet from in-use container, and insert into ready-to-use container
			if (b->GetPosition().x < -b->sprite->GetScaleWidth() || b->GetPosition().x > worldWidth) {
				readyBullets2.push_back(b);
				inUseBullets2.erase(remove(inUseBullets2.begin(), inUseBullets2.end(), b), inUseBullets2.end());
			}

			b->Update(game->GetGameTime());
		}
		if (knockbackDuration2 > 0) {
			// Update posisi enemy dengan knockback
			x += knockbackVelocity2 * game->GetGameTime();
			knockbackDuration2 -= game->GetGameTime(); // Kurangi durasi knockback

			// Hentikan knockback jika waktunya habis
			if (knockbackDuration2 <= 0) {
				knockbackVelocity2 = 0.0f;
			}
		}
		if (y < 0) {
			jump = false;
			yVelocity = 0;
			y = 1000;
			x = 1000;
			nyawa--;
			nyawa2++;
			deadSound->Play(false);
		}
		if (enemyY < 0) {
			jump2 = false;
			yVelocity2 = 0;
			enemyY = 1000;
			enemyX = 1000;
			nyawa2--;
			nyawa++;
			deadSound->Play(false);
		}
		// Logika End Game
		if (nyawa < 1) {
			winner = 1;
			state = State::GAME_OVER;
			win->Play(false);
		}
		else if(nyawa2 < 1)
		{
			winner = 0;
			state = State::GAME_OVER;
			win->Play(false);
		}

		if (gameTime == 0 && nyawa < nyawa2) {
			winner = 1;
			state = State::GAME_OVER;
			win->Play(false);
		}
		else if (gameTime == 0 && nyawa2 < nyawa) {
			winner = 0;
			state = State::GAME_OVER;
			win->Play(false);
		}
		else if (gameTime == 0 && nyawa == nyawa2) {
			winner = 2;
			state = State::GAME_OVER;
			win->Play(false);
		}

		// Update monster sprite animation
		enemySprite->SetPosition(enemyX, enemyY);
		charSprite->SetPosition(x, y);

		charSprite->Update(game->GetGameTime());
		enemySprite->Update(game->GetGameTime());

		if (game->GetInputManager()->IsKeyReleased("Quit"))
		{
			state = State::PAUSE;
			return;
		}
	}
	else if (state == Game_Gun::State::GAME_OVER)
	{
		if (playMusic == true) {
			playMusic = false;
			music->Stop();
		}
		charSprite->SetFlipHorizontal(false)->PlayAnim("idle");
		enemySprite->SetFlipHorizontal(false)->PlayAnim("idle");
		charSprite->Update(game->GetGameTime());
		enemySprite->Update(game->GetGameTime());
		if (game->GetInputManager()->IsKeyPressed("tryAgain"))
		{
			pickSound->Play(false);
			RestartGame();  // Fungsi untuk mereset permainan
		}
		else if (game->GetInputManager()->IsKeyReleased("Quit"))
		{
			pickSound->Play(false);
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
			RestartGame();
			return;
		}
		return;
	}else if (state == Game_Gun::State::PAUSE)
	{
		music->Pause();
		if (game->GetInputManager()->IsKeyPressed("tryAgain"))
		{
			music->Resume();
			pickSound->Play(false);
			state = State::PLAY;;  // Melanjutkan permainan
		}
		else if (game->GetInputManager()->IsKeyReleased("Quit"))
		{
			pickSound->Play(false);
			if (playMusic == true) {
				playMusic = false;
				music->Stop();
			}
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
			RestartGame();
			return;
		}
		else if (game->GetInputManager()->IsKeyReleased("Restart")) 
		{
			pickSound->Play(false);
			playMusic = false;
			music->Stop();
			RestartGame();
			return;
		}
		return;
	}
}

void Game_Gun::Draw()
{
	if (state == State::MAPSELECTION) {
		bgMapSelection->Draw();
		// Render all buttons
		for (Button* b : buttons) {
			b->Draw();
		}
	}
	else if (state == State::PLAY) {
		if (mapIndex == 0) {
			// Draw background
			backgroundSprite->SetPosition(-cameraOffset.x, -cameraOffset.y);
			backgroundSprite->Draw();
			text->Draw();
			
			// Draw Object
			for (Sprite* o : objects) {
				vec2 oPos = vec2(
					o->GetPosition().x - cameraOffset.x,
					o->GetPosition().y - cameraOffset.y
				);
				o->SetPosition(oPos.x, oPos.y)->Draw();
			}
			for (Sprite* o : objects) {
				o->SetPosition(o->GetPosition().x + cameraOffset.x, o->GetPosition().y + cameraOffset.y);
			}
			// Draw platforms
			for (Sprite* s : platforms) {
				vec2 adjustedPos = vec2(
					s->GetPosition().x - cameraOffset.x,
					s->GetPosition().y - cameraOffset.y
				);
				s->SetPosition(adjustedPos.x, adjustedPos.y)->Draw();
			}
			for (Sprite* s : platforms) {
				s->SetPosition(s->GetPosition().x + cameraOffset.x, s->GetPosition().y + cameraOffset.y);
			}
		}
		else if (mapIndex == 1) {
			// Draw background
			backgroundSprite2->SetPosition(-cameraOffset.x, -cameraOffset.y);
			backgroundSprite2->Draw();
			text->Draw();
			// Draw Object
			for (Sprite* o : objects2) {
				vec2 oPos = vec2(
					o->GetPosition().x - cameraOffset.x,
					o->GetPosition().y - cameraOffset.y
				);
				o->SetPosition(oPos.x, oPos.y)->Draw();
			}
			for (Sprite* o : objects2) {
				o->SetPosition(o->GetPosition().x + cameraOffset.x, o->GetPosition().y + cameraOffset.y);
			}
			// Draw platforms
			for (Sprite* s : platforms2) {
				vec2 adjustedPos = vec2(
					s->GetPosition().x - cameraOffset.x,
					s->GetPosition().y - cameraOffset.y
				);
				s->SetPosition(adjustedPos.x, adjustedPos.y)->Draw();
			}
			for (Sprite* s : platforms2) {
				s->SetPosition(s->GetPosition().x + cameraOffset.x, s->GetPosition().y + cameraOffset.y);
			}
		}

		for (Bullet* b : inUseBullets) {
			b->Draw(cameraOffset); // Kirim offset kamera ke fungsi Draw peluru
		}
		for (Bullet* b2 : inUseBullets2) {
			b2->Draw(cameraOffset); // Kirim offset kamera ke fungsi Draw peluru
		}

		// time and healt info
		timeHealth->SetPosition((game->GetSettings()->screenWidth/2)-timeHealth->GetScaleWidth()/2, 
			(game->GetSettings()->screenHeight * 0.9) - ((timeHealth->GetScaleHeight() - timeText->GetTextHeight())/2));
		timeHealth->Draw();

		// Time render
		if (timeIndex==3) {
			timeText->SetText("Unlimited");
		}else {
			int minutes = static_cast<int>(gameTime) / 60;
			int seconds = static_cast<int>(gameTime) % 60;
			std::string timeTextUpdate = std::to_string(minutes) + ":" +
				(seconds < 10 ? "0" : "") + std::to_string(seconds);
			timeText->SetText(timeTextUpdate); // Mengubah teks yang ditampilkan
		}
		timeText->SetPosition((game->GetSettings()->screenWidth / 2) - (timeText->GetTextWidth() / 2), game->GetSettings()->screenHeight * 0.9); // Posisi timer di layar
		timeText->Draw();

		// Score render
		healt->SetText(to_string(nyawa));
		healt->SetColor(255, 50, 50);
		healt->SetPosition((game->GetSettings()->screenWidth / 2) + timeHealth->GetScaleWidth()/5,
			game->GetSettings()->screenHeight * 0.9);
		healt->Draw();
		healt->SetText(to_string(nyawa2));
		healt->SetColor(50, 255, 50);
		healt->SetPosition((game->GetSettings()->screenWidth / 2) - timeHealth->GetScaleWidth()/5 - healt->GetTextWidth(),
			game->GetSettings()->screenHeight * 0.9);
		healt->Draw();

		// Karakter 1
		vec2 charPos = vec2(charSprite->GetPosition().x - cameraOffset.x, charSprite->GetPosition().y - cameraOffset.y);
		charSprite->SetPosition(charPos.x, charPos.y);
		charSprite->Draw();
		charSprite->SetPosition(charPos.x + cameraOffset.x, charPos.y + cameraOffset.y); // Kembalikan posisi asli

		// Karakter 2
		vec2 enemyPos = vec2(enemySprite->GetPosition().x - cameraOffset.x, enemySprite->GetPosition().y - cameraOffset.y);
		enemySprite->SetPosition(enemyPos.x, enemyPos.y);
		enemySprite->Draw();
		enemySprite->SetPosition(enemyPos.x + cameraOffset.x, enemyPos.y + cameraOffset.y); // Kembalikan posisi asli
	}
	else if (state == State::GAME_OVER) {
		if (mapIndex == 0) {
			// Draw background
			backgroundSprite->SetPosition(-cameraOffset.x, -cameraOffset.y);
			backgroundSprite->Draw();
			text->Draw();

			// Draw Object
			for (Sprite* o : objects) {
				vec2 oPos = vec2(
					o->GetPosition().x - cameraOffset.x,
					o->GetPosition().y - cameraOffset.y
				);
				o->SetPosition(oPos.x, oPos.y)->Draw();
			}
			for (Sprite* o : objects) {
				o->SetPosition(o->GetPosition().x + cameraOffset.x, o->GetPosition().y + cameraOffset.y);
			}
			// Draw platforms
			for (Sprite* s : platforms) {
				vec2 adjustedPos = vec2(
					s->GetPosition().x - cameraOffset.x,
					s->GetPosition().y - cameraOffset.y
				);
				s->SetPosition(adjustedPos.x, adjustedPos.y)->Draw();
			}
			for (Sprite* s : platforms) {
				s->SetPosition(s->GetPosition().x + cameraOffset.x, s->GetPosition().y + cameraOffset.y);
			}
		}
		else if (mapIndex == 1) {
			// Draw background
			backgroundSprite2->SetPosition(-cameraOffset.x, -cameraOffset.y);
			backgroundSprite2->Draw();
			text->Draw();
			// Draw Object
			for (Sprite* o : objects2) {
				vec2 oPos = vec2(
					o->GetPosition().x - cameraOffset.x,
					o->GetPosition().y - cameraOffset.y
				);
				o->SetPosition(oPos.x, oPos.y)->Draw();
			}
			for (Sprite* o : objects2) {
				o->SetPosition(o->GetPosition().x + cameraOffset.x, o->GetPosition().y + cameraOffset.y);
			}
			// Draw platforms
			for (Sprite* s : platforms2) {
				vec2 adjustedPos = vec2(
					s->GetPosition().x - cameraOffset.x,
					s->GetPosition().y - cameraOffset.y
				);
				s->SetPosition(adjustedPos.x, adjustedPos.y)->Draw();
			}
			for (Sprite* s : platforms2) {
				s->SetPosition(s->GetPosition().x + cameraOffset.x, s->GetPosition().y + cameraOffset.y);
			}
		}

		for (Bullet* b : inUseBullets) {
			b->Draw(cameraOffset); // Kirim offset kamera ke fungsi Draw peluru
		}
		for (Bullet* b2 : inUseBullets2) {
			b2->Draw(cameraOffset); // Kirim offset kamera ke fungsi Draw peluru
		}

		// time and healt info
		timeHealth->SetPosition((game->GetSettings()->screenWidth / 2) - timeHealth->GetScaleWidth() / 2,
			(game->GetSettings()->screenHeight * 0.9) - ((timeHealth->GetScaleHeight() - timeText->GetTextHeight()) / 2));
		timeHealth->Draw();

		// Time render
		if (timeIndex == 3) {
			timeText->SetText("Unlimited");
		}
		else {
			int minutes = static_cast<int>(gameTime) / 60;
			int seconds = static_cast<int>(gameTime) % 60;
			std::string timeTextUpdate = std::to_string(minutes) + ":" +
				(seconds < 10 ? "0" : "") + std::to_string(seconds);
			timeText->SetText(timeTextUpdate); // Mengubah teks yang ditampilkan
		}
		timeText->SetPosition((game->GetSettings()->screenWidth / 2) - (timeText->GetTextWidth() / 2), game->GetSettings()->screenHeight * 0.9); // Posisi timer di layar
		timeText->Draw();

		// Score render
		healt->SetText(to_string(nyawa));
		healt->SetColor(255, 50, 50);
		healt->SetPosition((game->GetSettings()->screenWidth / 2) + timeHealth->GetScaleWidth() / 5,
			game->GetSettings()->screenHeight * 0.9);
		healt->Draw();
		healt->SetText(to_string(nyawa2));
		healt->SetColor(50, 255, 50);
		healt->SetPosition((game->GetSettings()->screenWidth / 2) - timeHealth->GetScaleWidth() / 5 - healt->GetTextWidth(),
			game->GetSettings()->screenHeight * 0.9);
		healt->Draw();

		if (winner == 0) {
			winGuiRedSprite->SetPosition((game->GetSettings()->screenWidth * 0.5) - (winGuiRedSprite->GetScaleWidth()) / 2,
				(game->GetSettings()->screenHeight) / 2 - winGuiRedSprite->GetScaleHeight() / 2);
			winGuiRedSprite->Draw();
			charSprite->SetScale(1.0f);
			charSprite->SetPosition((game->GetSettings()->screenWidth / 2)-(charSprite->GetScaleWidth()*0.43),
				(game->GetSettings()->screenHeight / 2)-(charSprite->GetScaleHeight())+10);
			charSprite->Draw();
			charSprite->SetScale(0.4f);
		}
		else if (winner == 1) {
			winGuiSprite->SetPosition((game->GetSettings()->screenWidth * 0.5) - (winGuiSprite->GetScaleWidth()) / 2,
				(game->GetSettings()->screenHeight) / 2 - winGuiSprite->GetScaleHeight() / 2);
			winGuiSprite->Draw();
			enemySprite->SetScale(1.0f);
			enemySprite->SetPosition((game->GetSettings()->screenWidth / 2) - (enemySprite->GetScaleWidth()*0.43),
				(game->GetSettings()->screenHeight / 2)-(enemySprite->GetScaleHeight())+10);
			enemySprite->Draw();
			enemySprite->SetScale(0.4f);
		}
		else if (winner == 3) {
			drawGuiSprite->SetPosition((game->GetSettings()->screenWidth * 0.5) - (drawGuiSprite->GetScaleWidth()) / 2,
				(game->GetSettings()->screenHeight) / 2 - drawGuiSprite->GetScaleHeight() / 2);
			drawGuiSprite->Draw();
		}

	}
	else if (state == State::PAUSE) {
		if (mapIndex == 0) {
			// Draw background
			backgroundSprite->SetPosition(-cameraOffset.x, -cameraOffset.y);
			backgroundSprite->Draw();
			text->Draw();

			// Draw Object
			for (Sprite* o : objects) {
				vec2 oPos = vec2(
					o->GetPosition().x - cameraOffset.x,
					o->GetPosition().y - cameraOffset.y
				);
				o->SetPosition(oPos.x, oPos.y)->Draw();
			}
			for (Sprite* o : objects) {
				o->SetPosition(o->GetPosition().x + cameraOffset.x, o->GetPosition().y + cameraOffset.y);
			}
			// Draw platforms
			for (Sprite* s : platforms) {
				vec2 adjustedPos = vec2(
					s->GetPosition().x - cameraOffset.x,
					s->GetPosition().y - cameraOffset.y
				);
				s->SetPosition(adjustedPos.x, adjustedPos.y)->Draw();
			}
			for (Sprite* s : platforms) {
				s->SetPosition(s->GetPosition().x + cameraOffset.x, s->GetPosition().y + cameraOffset.y);
			}
		}
		else if (mapIndex == 1) {
			// Draw background
			backgroundSprite2->SetPosition(-cameraOffset.x, -cameraOffset.y);
			backgroundSprite2->Draw();
			text->Draw();
			// Draw Object
			for (Sprite* o : objects2) {
				vec2 oPos = vec2(
					o->GetPosition().x - cameraOffset.x,
					o->GetPosition().y - cameraOffset.y
				);
				o->SetPosition(oPos.x, oPos.y)->Draw();
			}
			for (Sprite* o : objects2) {
				o->SetPosition(o->GetPosition().x + cameraOffset.x, o->GetPosition().y + cameraOffset.y);
			}
			// Draw platforms
			for (Sprite* s : platforms2) {
				vec2 adjustedPos = vec2(
					s->GetPosition().x - cameraOffset.x,
					s->GetPosition().y - cameraOffset.y
				);
				s->SetPosition(adjustedPos.x, adjustedPos.y)->Draw();
			}
			for (Sprite* s : platforms2) {
				s->SetPosition(s->GetPosition().x + cameraOffset.x, s->GetPosition().y + cameraOffset.y);
			}
		}

		for (Bullet* b : inUseBullets) {
			b->Draw(cameraOffset); // Kirim offset kamera ke fungsi Draw peluru
		}
		for (Bullet* b2 : inUseBullets2) {
			b2->Draw(cameraOffset); // Kirim offset kamera ke fungsi Draw peluru
		}

		// time and healt info
		timeHealth->SetPosition((game->GetSettings()->screenWidth / 2) - timeHealth->GetScaleWidth() / 2,
			(game->GetSettings()->screenHeight * 0.9) - ((timeHealth->GetScaleHeight() - timeText->GetTextHeight()) / 2));
		timeHealth->Draw();

		// Time render
		if (timeIndex == 3) {
			timeText->SetText("Unlimited");
		}
		else {
			int minutes = static_cast<int>(gameTime) / 60;
			int seconds = static_cast<int>(gameTime) % 60;
			std::string timeTextUpdate = std::to_string(minutes) + ":" +
				(seconds < 10 ? "0" : "") + std::to_string(seconds);
			timeText->SetText(timeTextUpdate); // Mengubah teks yang ditampilkan
		}
		timeText->SetPosition((game->GetSettings()->screenWidth / 2) - (timeText->GetTextWidth() / 2), game->GetSettings()->screenHeight * 0.9); // Posisi timer di layar
		timeText->Draw();

		// Score render
		healt->SetText(to_string(nyawa));
		healt->SetColor(255, 50, 50);
		healt->SetPosition((game->GetSettings()->screenWidth / 2) + timeHealth->GetScaleWidth() / 5,
			game->GetSettings()->screenHeight * 0.9);
		healt->Draw();
		healt->SetText(to_string(nyawa2));
		healt->SetColor(50, 255, 50);
		healt->SetPosition((game->GetSettings()->screenWidth / 2) - timeHealth->GetScaleWidth() / 5 - healt->GetTextWidth(),
			game->GetSettings()->screenHeight * 0.9);
		healt->Draw();

		// Karakter 1
		vec2 charPos = vec2(charSprite->GetPosition().x - cameraOffset.x, charSprite->GetPosition().y - cameraOffset.y);
		charSprite->SetPosition(charPos.x, charPos.y);
		charSprite->Draw();
		charSprite->SetPosition(charPos.x + cameraOffset.x, charPos.y + cameraOffset.y); // Kembalikan posisi asli

		// Karakter 2
		vec2 enemyPos = vec2(enemySprite->GetPosition().x - cameraOffset.x, enemySprite->GetPosition().y - cameraOffset.y);
		enemySprite->SetPosition(enemyPos.x, enemyPos.y);
		enemySprite->Draw();
		enemySprite->SetPosition(enemyPos.x + cameraOffset.x, enemyPos.y + cameraOffset.y); // Kembalikan posisi asli

		pauseGuiSprite->SetPosition((game->GetSettings()->screenWidth * 0.5) - (pauseGuiSprite->GetScaleWidth()) / 2, (game->GetSettings()->screenHeight) / 2 - pauseGuiSprite->GetScaleHeight() / 2);
		pauseGuiSprite->Draw();
	}

}
void Game_Gun::RestartGame() {
	// Reset skor & Spawntime
	nyawa = 10;
	nyawa2 = 10;

	// Reset Waktu
	if (timeIndex==0) {
		gameTime = 120.f;
	}
	else if (timeIndex == 1) {
		gameTime = 300.f;
	} 
	else if (timeIndex == 2) {
		gameTime = 600.f;
	}
	if (playMusicMainMenu == false) {
		mainMenuMusic->Play(true);
		playMusicMainMenu = true;
	}
	// Reset Posisi Character
	charSprite->SetPosition(1000, 1250);
	enemySprite->SetPosition(1000, 1250);

	// Kembali ke state MAPSELECTION
	state = State::MAPSELECTION;
}
void Game_Gun::SpawnBullets()
{
	if (timeInterval >= 150) {
		if (readyBullets.empty()) {
			return;
		}
		Bullet* b = readyBullets.back();
		readyBullets.pop_back();

		// Sesuaikan posisi peluru relatif terhadap karakter dengan mengabaikan offset kamera
		vec2 charPos = charSprite->GetPosition();
		b->SetPosition(
			charPos.x + (toRight ? (charSprite->GetScaleWidth() / 2)+20 : (- charSprite->GetScaleWidth() / 2) + 40),
			charPos.y + charSprite->GetScaleHeight() / 2
		);

		// Set arah peluru
		b->xVelocity = toRight ? 1.0f : -1.0f;
		if (toRight==true) {
			b->GetSprite()->SetFlipHorizontal(false);
		}
		else {
			b->GetSprite()->SetFlipHorizontal(true);
		}
		inUseBullets.push_back(b);
		timeInterval = 0;
	}
}
void Game_Gun::SpawnBullets2()
{
	if (timeInterval2 >= 150) {
		if (readyBullets2.empty()) {
			return;
		}
		Bullet* b = readyBullets2.back();
		readyBullets2.pop_back();

		// Sesuaikan posisi peluru relatif terhadap karakter dengan mengabaikan offset kamera
		vec2 enemyPos = enemySprite->GetPosition();
		b->SetPosition(
			enemyPos.x + (toRight2 ? (enemySprite->GetScaleWidth() / 2)+20 : (- enemySprite->GetScaleWidth() / 2)+40),
			enemyPos.y + enemySprite->GetScaleHeight() / 2 - 5
		);

		// Set arah peluru
		b->xVelocity = toRight2 ? 1.0f : -1.0f;
		if (toRight2 == true) {
			b->GetSprite()->SetFlipHorizontal(false);
		}
		else {
			b->GetSprite()->SetFlipHorizontal(true);
		}
		inUseBullets2.push_back(b);
		timeInterval2 = 0;
	}
}
void Game_Gun::UpdateCamera() {
	// Posisi kedua karakter
	vec2 charPos = charSprite->GetPosition();
	vec2 enemyPos = enemySprite->GetPosition();

	// Hitung midpoint antara kedua karakter
	vec2 midpoint = vec2(
		(charPos.x + enemyPos.x) / 2,
		(charPos.y + enemyPos.y) / 2
	);

	// Hitung jarak horizontal dan vertikal antara kedua karakter
	float distanceX = fabs(charPos.x - enemyPos.x);
	float distanceY = fabs(charPos.y - enemyPos.y);

	// Hitung jarak maksimum antara kedua karakter
	float maxDistanceX = game->GetSettings()->screenWidth * 0.8f;
	float maxDistanceY = game->GetSettings()->screenHeight * 0.8f;

	// Tentukan zoom kamera berdasarkan jarak karakter
	float zoomX = 1.0f + (distanceX / maxDistanceX);
	float zoomY = 1.0f + (distanceY / maxDistanceY);
	float targetZoom = std::max(zoomX, zoomY);

	// Batasi zoom ke dalam rentang tertentu
	float minZoom = 1.0f;
	float maxZoom = 2.0f;
	targetZoom = clamp(targetZoom, minZoom, maxZoom);

	// Smooth zoom menggunakan interpolasi
	float zoomLerpFactor = 0.1f;
	currentZoom += (targetZoom - currentZoom) * zoomLerpFactor;

	// Hitung target offset kamera mengikuti midpoint
	vec2 targetOffset = vec2(
		midpoint.x - (game->GetSettings()->screenWidth / 2) / currentZoom,
		midpoint.y - (game->GetSettings()->screenHeight / 2) / currentZoom
	);

	// Batasi offset kamera agar midpoint tidak keluar dari batas dunia
	float worldWidth = 2000.0f;  // Lebar dunia
	float worldHeight = 1000.0f; // Tinggi dunia
	targetOffset.x = clamp(targetOffset.x, 0.0f, worldWidth - game->GetSettings()->screenWidth / currentZoom);
	targetOffset.y = clamp(targetOffset.y, 0.0f, worldHeight - game->GetSettings()->screenHeight / currentZoom);

	// Smooth movement kamera menggunakan interpolasi
	float cameraLerpFactor = 0.1f;
	cameraOffset.x += (targetOffset.x - cameraOffset.x) * cameraLerpFactor;
	cameraOffset.y += (targetOffset.y - cameraOffset.y) * cameraLerpFactor;
}