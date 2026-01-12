#pragma once
#include "raylib.h"
#include "scene.h"
#include "classes/utilities/sceneManager.h"
#include "level01.h"

// Add in fade in when launching into main menu
// We should fade in just like the second half of transition
// Once fade in is done we should start playing music 
// We need some buttons:
//	1.) Start Game				|| DONE
//	2.) Load Game(WIP Feature)	|| DONE
//	3.) Settings(WIP Feature)	|| DONE
//	4.) Quit					|| DONE
//	5.) Maybe a secret that when you click unlocks the ability to play as a chicken?


class MainMenu : public Scene{
	public:
		void Update(float& dT);
		void Draw();
		void Load();
		void Unload();

	private:
		// Textures
		Texture2D m_BackgroundTex;
		Texture2D m_TitleTex;
		Texture2D m_NewGameButtonTex;
		Texture2D m_LoadGameButtonTex;
		Texture2D m_SettingsButtonTex;
		Texture2D m_QuitGameButtonTex;

		// Mouse
		Rectangle m_MouseCollider{
			G_VARS.WIDTH / 2.0f,
			G_VARS.HEIGHT / 2.0f,
			5.0f,
			5.0f
		};

		// Button Dimesions
		float m_ButtonWidth = 175 * G_VARS.WIDTH_SCALE;
		float m_ButtonHeight = 75 * G_VARS.HEIGHT_SCALE;

		// Buttons
		Rectangle m_NewGameButton;
		Rectangle m_LoadGameButton;
		Rectangle m_SettingsButton;
		Rectangle m_QuitGameButton;

		// Scaling Title Text
		Vector2 titleTextSize = { 300.0f * G_VARS.WIDTH_SCALE, 250.0f * G_VARS.HEIGHT_SCALE };
		bool m_Increase = true;
		bool m_RotateRight = true;
		float m_SizeIncreaseAmount = { 30.0f };

};
