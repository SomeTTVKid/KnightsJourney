#pragma once
#include "scene.h"
#include "raylib.h"
#include <vector>

class Entity;
class LevelLoader;
class Npc;

class BSInterior : public Scene{
	public:
		void Load();
		void Update(float& dT);
		void Draw();
		void Unload();

	private:
		Texture2D loaderTex = LoadTexture("artwork/levelLoader.png");
		Texture2D npcTex = LoadTexture("classes/npcs/artwork/npc01_idle.png");
		Texture2D npcDialogueTex = LoadTexture("classes/npcs/artwork/npc01_talkingTest.png");
		LevelLoader* toTown = nullptr;
		Npc* BlackSmith = nullptr;
		Vector2 m_MapSize = { 5.0f, 4.5f };
		Vector2 m_MapOffset = { 1.5, 1.5f };
		Vector3 m_ToTownPos = { -0.5f, 1.0f, 2.6f };
		Vector3 m_ToTownOut = { -3.2f, 1.0f, -0.4f };

		float m_SoundTimer{};
		float m_UnloadTimeLimit = 1.5f;
};