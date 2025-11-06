#include "level03.h"
#include "raylib.h"
#include "classes/entities/entity.h"
#include "classes/player/player.h"
#include "classes/utilities/sceneManager.h"
#include "classes/utilities/globalVariables.h"
#include "classes/utilities/levelLoader.h"
#include "classes/structures/trees.h"

void Level03::Load(){
	SpruceTree::m_SpruceTreeTex = LoadTexture("classes/structures/artwork/trees/spruceTree.png");

	Scene::Load();

	Scene::m_Player->GetWorldSize() = m_MapSize;
	Scene::m_Player->GetPos() = Scene::m_Player->GetSpawnPoint();

	toTown = new LevelLoader(loaderTex, G_VARS.LEVEL_02, m_ToTownPos, m_ToTownOut, G_VARS.LOADER_SIZE);
	Scene::m_LevelLoaders.push_back(toTown);

	for(size_t r = 0; r < rockCount; ++r){
		Entity* rock = new Entity(mossyRockTex, G_VARS.ROCK_SIZE, rockPositions[r], G_VARS.RIGHT, true, G_VARS.STRUCTURE_ID, 1);
		Scene::m_Entities.push_back(rock);
	}

	for(size_t t = 0; t < treeCount; ++t){
		auto tree = std::make_unique<SpruceTree>(treePositions[t], G_VARS.TREE_SIZE);
		Scene::m_Structures.push_back(std::move(tree));
	}

}

void Level03::Update(float& dT){
	Scene::Update(dT);
}

void Level03::Draw(){
	BeginMode3D(Scene::m_Camera);
		DrawPlane({}, m_MapSize + m_MapOffset, DARKGREEN);

		// Use shared Scene draw logic which builds the draw list from Scene::m_Entities
		Scene::Draw();

	EndMode3D();
}

void Level03::Unload(){
	Scene::Unload();

	UnloadTexture(loaderTex);
	UnloadTexture(mossyRockTex);
	UnloadTexture(SpruceTree::m_SpruceTreeTex);

	for(size_t i = 0; i < 20; ++i){
		std::cout << "\n" << std::endl;
	}
	std::cout << "Unloaded Level03 Textures" << std::endl;

}