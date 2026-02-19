#include "level02.h"
#include "raylib.h"
#include "classes/player/player.h"
#include "classes/npcs/npc.h"
#include "classes/utilities/globalVariables.h"
#include "classes/structures/trees.h"
#include "classes/structures/foliage.h"
#include "classes/structures/houses.h"

void Level02::Load(){
	BirchTree::m_BirchTreeTex = LoadTexture("classes/structures/artwork/trees/birchTree.png");
	Grass01::m_Grass01Tex = LoadTexture("classes/structures/artwork/grass/grass01.png");
	Sign01::m_Sign01Tex = LoadTexture("classes/structures/artwork/signs/sign.png");

	House01::m_House01Tex = LoadTexture("classes/structures/artwork/houses/house01.png");
	House02::m_House02Tex = LoadTexture("classes/structures/artwork/houses/house02.png");
	Shed::m_ShedTex = LoadTexture("classes/structures/artwork/houses/shed.png");
	Inn::m_InnTex = LoadTexture("classes/structures/artwork/houses/inn.png");

	Scene::m_LevelEnterAudio = LoadSound("audio/levelChange/levelExit.wav");

	Scene::Load();

	// Player Setup
	Scene::m_Player->GetWorldSize() = m_MapSize;
	Scene::m_Player->GetPos() = Scene::m_Player->GetSpawnPoint();

	// Level Loaders
	toForest = new LevelLoader(loaderTex, G_VARS.LEVEL_01, m_ToForestPos, m_ToForestOut, G_VARS.LOADER_SIZE);
	toWoods = new LevelLoader(loaderTex, G_VARS.LEVEL_03, m_ToWoodsPos, m_ToWoodsOut, G_VARS.LOADER_SIZE);
	toBlackSmith = new LevelLoader(loaderTex, G_VARS.BSINTERIOR, m_ToBlackSmithPos, m_ToBlackSmithOut, G_VARS.LOADER_SIZE);

	// Register loaders in shared scene lists
	Scene::m_LevelLoaders.push_back(toForest);
	Scene::m_LevelLoaders.push_back(toWoods);
	Scene::m_LevelLoaders.push_back(toBlackSmith);

	// Level Loader Signs
	auto toForestSign = std::make_unique<Sign01>(m_ToForestPos);
	Scene::m_Structures.push_back(std::move(toForestSign));

	// Houses
	auto house02 = std::make_unique<House02>(housePositions[0]);
	Scene::m_Structures.push_back(std::move(house02));

	auto house01 = std::make_unique<House01>(housePositions[1]);
	Scene::m_Structures.push_back(std::move(house01));

	auto shed = std::make_unique<Shed>(housePositions[2]);
	Scene::m_Structures.push_back(std::move(shed));

	auto inn = std::make_unique<Inn>(housePositions[3]);
	Scene::m_Structures.push_back(std::move(inn));

	// Trees
	for(size_t t = 0; t < treeCount; ++t){
		auto tree = std::make_unique<BirchTree>(treePositions[t]);
		Scene::m_Structures.push_back(std::move(tree));
	}

	// Grass
	for(size_t g = 0; g < grassCount; ++g){
		auto grass = std::make_unique<Grass01>(grassPositions[g]);
		Scene::m_Structures.push_back(std::move(grass));
	}

	PlaySound(m_LevelEnterAudio);
}

void Level02::Update(float& dT){
	Scene::Update(dT);
}

void Level02::Draw(){
	BeginMode3D(Scene::m_Camera);
		DrawPlane({}, m_MapSize + Scene::m_MapOffset, DARKGREEN);

		Scene::Draw();

	EndMode3D();
}

void Level02::Unload(){
	Scene::Unload();

	// UnloadTexture(tree01Tex);
	UnloadTexture(BirchTree::m_BirchTreeTex);
	UnloadTexture(Grass01::m_Grass01Tex);
	UnloadTexture(House01::m_House01Tex);
	UnloadTexture(House02::m_House02Tex);
	UnloadTexture(Inn::m_InnTex);
	UnloadTexture(Shed::m_ShedTex);
	UnloadTexture(Sign01::m_Sign01Tex);
	UnloadTexture(loaderTex);

	for(size_t i = 0; i < 20; ++i){
		std::cout << "\n" << std::endl;
	}
	std::cout << "Unloaded Level02 Textures" << std::endl;
	
}