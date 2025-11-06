#pragma once
#include "scene.h"
#include "raylib.h"
#include "classes/utilities/levelLoader.h"
#include <vector>
#include <any>

class Entity;
class Npc;
class Enemy;
class Projectile;

// TODO flesh out the scene more
// Add in 3 more level loaders + 1 secret one ;)
// Make the level a bit bigger
// Spawn the player at the BOTTOM of the level
// Move toTown to the top of the level
// Place our blacksmith npc 
// Block the level loader with enemy npcs
// 

class Level01 : public Scene{
	public:
		void Load();
		void Update(float& dT);
		void Draw();
		void Unload();

	private:
		Texture2D eKnightTex = LoadTexture("classes/enemies/artwork/enemy_idle.png");
		Texture2D goblinTex = LoadTexture("classes/enemies/artwork/goblin_idle.png");
		Texture2D signTex = LoadTexture("artwork/sign.png");
		Texture2D loaderTex = LoadTexture("artwork/levelLoader.png");
		Texture2D campfireTex = LoadTexture("artwork/campfire.png");
		Texture2D npc01Tex = LoadTexture("classes/npcs/artwork/npc01_idle.png");
		Texture2D npc01Dialogue = LoadTexture("classes/npcs/artwork/npc01_talkingTest.png");
		LevelLoader* toTown = nullptr;
		Npc* BlackSmith = nullptr;
		size_t treeCount = 17;
		size_t goblinCount = 3;
		Vector2 m_MapSize = { 10.0f, 8.0f };
		Vector2 m_MapOffset = { 2.0f, 2.0f };
		Vector3 m_ToTownPos = { -m_MapSize.x / 2.0f, 1.0f, 0.0f };
		Vector3 m_ToTownOut = { 3.9f, 1.0f, -0.5f };
	// Negative X and Negative Z is TOP LEFT QUADRANT
	// Positive X and Negative Z is TOP RIGHT QUADRANT
	// Negative X and Positive Z is BOTTOM LEFT QUADRANT
	// Positive X and Positive Z is BOTTOM RIGHT QUADRANT
		std::vector<Vector3> treePositions = {
			// TOP LEFT QUADRANT
			{ -4.9f, 1.0f, -2.9f },
			{ -2.5f, 1.0f, -2.5f },
			{ -3.4f, 1.0f, -0.8f },

			// TOP RIGHT QUADRANT
			{ 0.6f, 1.0f, -2.8f },
			{ 2.3f, 1.0f, -2.5f },

			// BOTTOM LEFT QUADRANT
			{ -1.0f, 1.0f, 0.0f },
			{ -1.0f, 1.0f, 2.0f },
			{ -2.0f, 1.0f, 3.8f },
			{ -3.0f, 1.0f, 4.9f },
			{ -5.6f, 1.0f, 4.4f },
			{ -5.0f, 1.0f, 5.5f },

			// BOTTOM RIGHT QUADRANT
			{ 0.7f, 1.0f, 5.5f },
			{ 2.6f, 1.0f, 5.0f },
			{ 3.8f, 1.0f, 4.7f },
			{ 3.9f, 1.0f, 4.5f },
			{ 2.8f, 1.0f, 3.5f },
			{ 3.5f, 1.0f, 1.8f }

		};
		std::vector<Vector3> goblinPositions = {
			{ 2.5f, 1.0f, -0.5f },
			{ 2.0f, 1.0f, 1.0f },
			{ 3.0f, 1.0f, 1.0f }
		};
};