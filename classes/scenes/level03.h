#pragma once
#include "scene.h"
#include "raylib.h"
#include <vector>

// TODO add more than ONE level loader into this scene
// Flesh out the scene

class Entity;
class LevelLoader;

class Level03 : public Scene{
	public:
		void Update(float& dT);
		void Draw();
		void Load();
		void Unload();

	private:
		Texture2D loaderTex = LoadTexture("classes/utilities/artwork/levelLoaders/levelLoader.png");
		LevelLoader* toTown = nullptr;
		size_t rockCount = 3;
		size_t smallRockCount = 2;
		size_t treeCount = 1;
		Vector2 m_MapSize = { 5.0f, 15.0f };
		Vector3 m_ToTownPos = { -0.8f, 1.0f, m_MapSize.y / 2.0f + 0.5f};
		Vector3 m_ToTownOut = { -0.5f, 1.0f, -3.9f };
		std::vector<Vector3> rockPositions{
			{ 1.1f, 1.0f, 6.5f },
			{ -1.5f, 1.0f, 4.0f },
			{ -0.3f, 1.0f, -0.1f }
		};
		std::vector<Vector3> smallRockPositions{
			{ 0.0f, 1.0f, 1.0f },
			{ 1.5f, 1.0f, 2.5f }
		};
		std::vector<Vector3> treePositions{
			{ -2.3f, 1.0f, 6.1f }
		};

};