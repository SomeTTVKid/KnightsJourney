#include "mainMenu.h"
#include "classes/entities/entity.h"
#include "classes/utilities/globalVariables.h"

void MainMenu::Load(){
	Scene::m_LevelMusic = LoadMusicStream("audio/mainMenu.mp3");

	Scene::Load();

	PlayMusicStream(Scene::m_LevelMusic);

}

void MainMenu::Update(float& dT){
	if(IsKeyPressed(KEY_ENTER)){
		SceneManager::GetInstance().ChangeScene(G_VARS.LEVEL_01);
		return;
	}

	if(m_Increase){
		titleTextSize.x += m_SizeIncreaseAmount * dT;
		titleTextSize.y += m_SizeIncreaseAmount * dT;

		if(titleTextSize.x >= 350 * G_VARS.WIDTH_SCALE && titleTextSize.y >= 300 * G_VARS.HEIGHT_SCALE){
			m_Increase = false;
		}
	}else{
		titleTextSize.x -= m_SizeIncreaseAmount * dT;
		titleTextSize.y -= m_SizeIncreaseAmount * dT;

		if(titleTextSize.x <= 250 * G_VARS.WIDTH_SCALE && titleTextSize.y <= 200 * G_VARS.HEIGHT_SCALE){
			m_Increase = true;
		}
	}

	Scene::Update(dT);
}

void MainMenu::Draw(){
	DrawTexturePro(
		backgroundTex,
		{0.0f, 0.0f, (float)backgroundTex.width, (float)backgroundTex.height},
		{0, 0, (float)G_VARS.WIDTH, (float)G_VARS.HEIGHT},
		{}, 0.0f, WHITE);

	DrawTexturePro(
		titleTex,
		{0.0f, 0.0f, (float)titleTex.width, (float)titleTex.height},
		{(float)G_VARS.WIDTH / 2.0f - titleTextSize.x / 2.0f, 100 * G_VARS.HEIGHT_SCALE, titleTextSize.x, titleTextSize.y},
		{}, 0.0f, WHITE);

	BeginMode3D(Scene::m_Camera);
		Scene::Draw();

	EndMode3D();

}

void MainMenu::Unload(){
	Scene::Unload();
	
	UnloadTexture(backgroundTex);
	UnloadTexture(titleTex);

}