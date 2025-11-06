#pragma once
#include "raylib.h"
#include "scene.h"
#include "classes/utilities/sceneManager.h"
#include "level01.h"

// Add in some 'buttons'
// Add in fade in when launching into main menu
// We should fade in just like the second half of transition
// Once fade in is done we should start playing music 
// We need some buttons:
//	1.) Start Game
//	2.) Load Game(WIP Feature)
//	3.) Settings(WIP Feature)
//	4.) Quit
//	5.) Maybe a secret that when you click unlocks the ability to play as a chicken?

class MainMenu : public Scene{
	public:
		void Update(float& dT);
		void Draw();
		void Load();
		void Unload();

	private:
		Texture2D backgroundTex = LoadTexture("artwork/mainMenuBackground.png");
		Texture2D titleTex = LoadTexture("artwork/TitleText.png");
		Rectangle START;
		Rectangle LOAD;
		Vector2 titleTextSize = { 300.0f * G_VARS.WIDTH_SCALE, 250.0f * G_VARS.HEIGHT_SCALE };
		bool m_Increase = true;
		bool m_RotateRight = true;
		float m_SizeIncreaseAmount = { 30.0f };
};
