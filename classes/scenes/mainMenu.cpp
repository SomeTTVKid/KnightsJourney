#include "mainMenu.h"
#include "classes/entities/entity.h"
#include "classes/utilities/globalVariables.h"

void MainMenu::Load(){
	Scene::m_LevelMusic = LoadMusicStream("audio/music/mainMenu.mp3");
	m_BackgroundTex = LoadTexture("classes/utilities/artwork/mainMenuBackground.png");
	m_TitleTex = LoadTexture("classes/utilities/artwork/TitleText.png");
	m_NewGameButtonTex = LoadTexture("classes/utilities/artwork/buttons/newGameButton.png");
	m_LoadGameButtonTex = LoadTexture("classes/utilities/artwork/buttons/loadGameButton.png");
	m_SettingsButtonTex = LoadTexture("classes/utilities/artwork/buttons/settingsButton.png");
	m_QuitGameButtonTex = LoadTexture("classes/utilities/artwork/buttons/quitGameButton.png");

	Scene::Load();

	SceneManager::GetInstance().m_Transitioning = true;
	SceneManager::GetInstance().m_TransitionPhase = 1;

	PlayMusicStream(Scene::m_LevelMusic);

}

void MainMenu::Update(float& dT){
	m_MouseCollider.x = GetMousePosition().x - m_MouseCollider.width / 2.0f;
	m_MouseCollider.y = GetMousePosition().y - m_MouseCollider.height /2.0f;

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		if(CheckCollisionRecs(m_MouseCollider, m_NewGameButton)){
			SceneManager::GetInstance().ChangeScene(G_VARS.LEVEL_01);
			return;
		}
		// Later when we have some sort of save file, need to check beforehand that it exists then load data
		if(CheckCollisionRecs(m_MouseCollider, m_LoadGameButton)){
			std::cout << "Loading Save Data!" << std::endl;
			return;
		}
		if(CheckCollisionRecs(m_MouseCollider, m_SettingsButton)){
			std::cout << "Opening Settings Menu" << std::endl;
			return;
		}
		if(CheckCollisionRecs(m_MouseCollider, m_QuitGameButton)){
			G_VARS.RUNNING = false;
			return;
		}
	}

	// Scaling Title Text
	if(m_Increase){
		titleTextSize.x += m_SizeIncreaseAmount * dT;
		titleTextSize.y += m_SizeIncreaseAmount * dT;

		if(titleTextSize.x >= 240 && titleTextSize.y >= 210){
			m_Increase = false;
		}
	}else{
		titleTextSize.x -= m_SizeIncreaseAmount * dT;
		titleTextSize.y -= m_SizeIncreaseAmount * dT;

		if(titleTextSize.x <= 230 && titleTextSize.y <= 180){
			m_Increase = true;
		}
	}

	Scene::Update(dT);
}

void MainMenu::Draw(){
	// Background
	DrawTexturePro(
		m_BackgroundTex,
		{0.0f, 0.0f, (float)m_BackgroundTex.width, (float)m_BackgroundTex.height},
		{0, 0, (float)G_VARS.WIDTH, (float)G_VARS.HEIGHT},
		{}, 0.0f, WHITE);

	DrawTexturePro(
		m_TitleTex,
		{0.0f, 0.0f, (float)m_TitleTex.width, (float)m_TitleTex.height},
		{(float)G_VARS.WIDTH / 2.0f - titleTextSize.x / 2.0f, 100 * G_VARS.HEIGHT_SCALE, titleTextSize.x, titleTextSize.y},
		{}, 0.0f, WHITE);

	// New Game
	m_NewGameButton = { 
		G_VARS.WIDTH / 2.0f - m_ButtonWidth / 2.0f,
		G_VARS.HEIGHT / 2.5f,
		m_ButtonWidth,
		m_ButtonHeight
	};

	DrawTexturePro(
		m_NewGameButtonTex,
		{0.0f, 0.0f, (float)m_NewGameButtonTex.width, (float)m_NewGameButtonTex.height},
		m_NewGameButton,
		{}, 0.0f, WHITE
	);

	// Load Game
	m_LoadGameButton = {
		G_VARS.WIDTH / 2.0f - m_ButtonWidth / 2.0f,
		m_NewGameButton.y + m_ButtonHeight * 1.5f,
		m_ButtonWidth,
		m_ButtonHeight
	};

	DrawTexturePro(
		m_LoadGameButtonTex,
		{0.0f, 0.0f, (float)m_LoadGameButtonTex.width, (float)m_LoadGameButtonTex.height},
		m_LoadGameButton,
		{}, 0.0f, WHITE
	);

	// Settings 
	m_SettingsButton = {
		G_VARS.WIDTH / 2.0f - m_ButtonWidth / 2.0f,
		m_LoadGameButton.y + m_ButtonHeight * 1.5f,
		m_ButtonWidth,
		m_ButtonHeight
	};

	DrawTexturePro(
		m_SettingsButtonTex,
		{0.0f, 0.0f, (float)m_SettingsButtonTex.width, (float)m_SettingsButtonTex.height},
		m_SettingsButton,
		{}, 0.0f, WHITE
	);

	// Quit Game
	m_QuitGameButton = {
		G_VARS.WIDTH / 2.0f - m_ButtonWidth / 2.0f,
		m_SettingsButton.y + m_ButtonHeight * 1.5f,
		m_ButtonWidth,
		m_ButtonHeight
	};

	DrawTexturePro(
		m_QuitGameButtonTex,
		{0.0f, 0.0f, (float)m_QuitGameButtonTex.width, (float)m_QuitGameButtonTex.height},
		m_QuitGameButton,
		{}, 0.0f, WHITE
	);

	BeginMode3D(Scene::m_Camera);
		Scene::Draw();

	EndMode3D();

}

void MainMenu::Unload(){
	Scene::Unload();
	
	UnloadTexture(m_BackgroundTex);
	UnloadTexture(m_TitleTex);
	UnloadTexture(m_NewGameButtonTex); 
	UnloadTexture(m_LoadGameButtonTex);
	UnloadTexture(m_SettingsButtonTex);
	UnloadTexture(m_QuitGameButtonTex);
}