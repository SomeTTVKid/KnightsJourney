#include "level01.h"
#include "levelStates.h"
#include "classes/enemies/enemy.h"
#include "classes/entities/entity.h"
#include "classes/player/player.h"
#include "classes/npcs/npc.h"
#include "classes/items/healthPotion.h"
#include "classes/items/manaPotion.h"
#include "classes/utilities/sceneManager.h"
#include "classes/utilities/globalVariables.h"
#include "classes/structures/trees.h"
#include "scene.h"

void Level01::Load(){
	OakTree::m_OakTreeTex = LoadTexture("classes/structures/artwork/trees/oakTree.png");
	
	Scene::m_LevelEnterAudio = LoadSound("audio/levelExit.wav");

	Scene::Load();

	// Player Setup
	Scene::m_Player->GetWorldSize() = m_MapSize;
	Scene::m_Player->GetPos() = Scene::m_Player->GetSpawnPoint();

	// Enemy Setup
	Enemy::SetTarget(m_Player);

	// BLACKSMITH
	if(levelStates.SpokeToBSInForest == false){
		Vector3 BlackSmithPos = { 3.0f, 1.0f, -1.0f };
		std::vector<int> m_Dialogue = {0, 1, 2, 3};
		auto BlackSmith = std::make_unique<Npc>(npc01Tex, npc01Dialogue, G_VARS.NPC_SIZE, BlackSmithPos, G_VARS.LEFT, true, G_VARS.NPC_ID, m_Dialogue);
		Scene::m_Npcs.push_back(std::move(BlackSmith));
		levelStates.BSInForest = true;
	}else if(levelStates.BSInForest){
		levelStates.BSInForest = false;
	}

	// TEST ITEMS
	Vector3 healthPotPos = {-0.2f, 1.0f, 4.2f};
	Vector3 manaPotPos = {-2.5f, 1.0f, 0.0f};
	Vector3 woodenSwordPos = {-0.2f, 1.0f, 4.5f};

	auto healthPot = std::make_shared<HealthPotion>(true, healthPotPos);
	Scene::m_Items.push_back(healthPot);

	auto manaPot = std::make_shared<ManaPotion>(true, manaPotPos);
	Scene::m_Items.push_back(manaPot);

	auto woodSword = std::make_shared<WoodenSword>(woodenSwordPos);
	Scene::m_Items.push_back(woodSword);
	
	// Level Loaders
	toTown = new LevelLoader(loaderTex, G_VARS.LEVEL_02, m_ToTownPos, m_ToForestPos, G_VARS.LOADER_SIZE);
	Scene::m_LevelLoaders.push_back(toTown);

	// Level Loader Signs
	Entity* toTownSign = new Entity(signTex, G_VARS.SIGN_SIZE, m_ToTownPos, G_VARS.LEFT, true, G_VARS.STRUCTURE_ID, 1);
	Scene::m_Entities.push_back(toTownSign);

	// Campfire
	Entity* campfire = new Entity(campfireTex, G_VARS.CAMPFIRE_SIZE, {2.4f, 1.0f, 0.4f}, G_VARS.RIGHT, true, G_VARS.STRUCTURE_ID);
	Scene::m_Entities.push_back(campfire);

	// Trees
	for( size_t t = 0; t < treeCount; ++t){
		auto tree = std::make_unique<OakTree>(treePositions[t], G_VARS.TREE_SIZE);
		Scene::m_Structures.push_back(std::move(tree));
	};

	// Goblins
	for( size_t g = 0; g < goblinCount; ++g){
		auto goblin = std::make_unique<Enemy>(goblinTex, G_VARS.GOBLIN_SIZE, goblinPositions[g], G_VARS.LEFT, true, G_VARS.ENEMY_ID, 5.0f, 1.5f, 1.9f, 0.5f);
		Scene::m_Enemies.push_back(std::move(goblin));
	};

	PlaySound(m_LevelEnterAudio);
}

void Level01::Update(float& dT){

	// UPDATE ACTIVE QUEST
	if(G_VARS.IN_DIALOGUE && levelStates.SpokeToBSInForest != true){
		if(Scene::npcInDialogue->ReturnDialogueCount() == Scene::npcInDialogue->ReturnDialogueSize() - 1){
			// TODO Change this down the line to a state or variable instead of hard coding?
			// If we wanted we could just create a vector/array of strings and change the index?
			SceneManager::GetInstance().SetActiveQuest() = "Talk to the BlackSmith in Town!";
			levelStates.SpokeToBSInForest = true;

		}
	}

	Scene::Update(dT);

}

void Level01::Draw(){
	BeginMode3D(Scene::m_Camera);
		DrawPlane({}, m_MapSize + Scene::m_MapOffset, DARKGREEN);

		Scene::Draw();

	EndMode3D();
}

void Level01::Unload(){
	if(BlackSmith != nullptr){
		BlackSmith = nullptr;
	}

	Scene::Unload();

	UnloadTexture(goblinTex);
	UnloadTexture(eKnightTex);
	UnloadTexture(OakTree::m_OakTreeTex);
	UnloadTexture(signTex);
	UnloadTexture(loaderTex);
	UnloadTexture(campfireTex);
	UnloadTexture(npc01Tex);
	UnloadTexture(npc01Dialogue);

	for(size_t i = 0; i < 20; ++i){
		std::cout << "\n" << std::endl;
	}

	std::cout << "Unloaded Level01 Enter Sound" << std::endl;
	std::cout << "Unloaded Level01 Textures" << std::endl;

}