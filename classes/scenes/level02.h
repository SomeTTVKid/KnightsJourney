#pragma once
#include "scene.h"
#include "raylib.h"
#include "classes/utilities/sceneManager.h"
#include "classes/utilities/levelLoader.h"
#include <vector>

// Negative X and Negative Z is TOP LEFT QUADRANT
// Positive X and Negative Z is TOP RIGHT QUADRANT
// Negative X and Positive Z is BOTTOM LEFT QUADRANT
// Positive X and Positive Z is BOTTOM RIGHT QUADRANT

// TODO Add in levelLoaders to all the houses
// Add one at the bottom that leads to the shore
// Make 1 npc that stays outside in the town
// Make one house a shop
// Make another house a tavern
// Level loader to the left leads to more town?
// Level loader at top lead to Woods, first real zone
// Level loader at the right leads back to first area
// Secret level loader??? perhaps inside of a house
// LEVEL LOADERS INSIDE TO LOAD TO ROOMS/LEVELS!!!!

class Level02 : public Scene{
	public:
		void Load();
		void Update(float& dT);
		void Draw();
		void Unload();

	private:
		Texture2D signTex = LoadTexture("artwork/sign.png");
		Texture2D loaderTex = LoadTexture("artwork/levelLoader.png");
		LevelLoader* toForest = nullptr;
		LevelLoader* toWoods = nullptr;
		LevelLoader* toBlackSmith = nullptr;
		size_t treeCount = 9;
		size_t grassCount = 13;
		Vector2 m_MapSize = { 10.0f, 10.0f };
		Vector3 m_ToForestPos = { m_MapSize.x / 2.0f - 1.5f, 1.0f, -1.0f };
		Vector3 m_ToForestOut = { -4.9f, 1.0f, 0.5f };
		Vector3 m_ToWoodsPos = { -0.5f, 1.0f, -m_MapSize.y / 2.0f };
		Vector3 m_ToWoodsOut = { -0.5f, 1.0f, 8.3f };
		Vector3 m_ToBlackSmithPos = { -3.2f, 1.0f, -1.1f };
		Vector3 m_ToBlackSmithOut = { -0.5f, 1.0f, 3.2f };
		std::vector<Vector3> housePositions{
			// TOP LEFT
			{ -4.5f, 1.0f, -0.8f },

			// TOP RIGHT
			{ 0.5f, 1.0f, -1.5f },

			// BOTTOM LEFT
			{ -4.5f, 1.0f, 4.0f },

			// BOTTOM RIGHT
			{ 0.5f, 1.0f, 4.0f }

		};
		std::vector<Vector3> treePositions{
			// TOP LEFT
			{ -1.3f, 1.0f, -0.1f },
			{ -4.5f, 1.0f, -2.0f },

			// TOP RIGHT
			{ 3.2, 1.0f, -2.7f },
			{ 0.5f, 1.0f, -2.0f },

			// BOTTOM LEFT
			{ -0.1f, 1.0f, 5.8f },
			{ -1.8f, 1.0f, 4.4f },
			{ -4.5f, 1.0f,  3.0f },
			{ -4.8f, 1.0f, 5.8f },

			// BOTTOM RIGHT
			{ 3.0f, 1.0f, 4.5f }
		};
		std::vector<Vector3> grassPositions{
			// TOP LEFT
			{ -1.2f, 1.0f, -3.2f },
			{ -4.8f, 1.0f, -0.2f },

			// TOP RIGHT
			{ 3.8f, 1.0f, -2.0f },
			{ 0.5f, 1.0f, -1.3f },

			// BOTTOM LEFT
			{ -1.2f, 1.0f, 0.1f },
			{ -0.2f, 1.0f, 0.7f },
			{ -0.3f, 1.0f, 5.9f },
			{ -0.8f, 1.0f, 3.9f },
			{ -2.1f, 1.0f, 4.4f },
			{ -2.3, 1.0f, 1.7f },

			// BOTTOM RIGHT
			{ 3.9f, 1.0f, 4.7f },
			{ 3.0f, 1.0f, 5.9f },
			{ 1.3f, 1.0f, 1.5f }
		};
};