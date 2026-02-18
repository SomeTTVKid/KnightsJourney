#include "blacksmithInterior.h"
#include "levelStates.h"
#include "classes/entities/entity.h"
#include "classes/player/player.h"
#include "classes/npcs/npc.h"
#include "classes/utilities/sceneManager.h"
#include "classes/utilities/globalVariables.h"
#include "classes/utilities/levelLoader.h"

void BSInterior::Load(){
	Scene::m_LevelEnterAudio = LoadSound("audio/levelChange/door_opening.wav");
	Scene::m_LevelExitAudio = LoadSound("audio/levelChange/door_closing.wav");

	Scene::Load();

	// Player Setup
	Scene::m_Player->GetWorldSize() = m_MapSize;
	Scene::m_Player->GetPos() = Scene::m_Player->GetSpawnPoint();

	// BLACKSMITH
	if(levelStates.SpokeToBSInForest){
		Vector3 BlackSmithPos = { -0.5f, 1.0f, -1.0f };
		std::vector<int> m_Dialogue = {4, 5, 6};
		auto BlackSmith = std::make_unique<Npc>(npcTex, npcDialogueTex, G_VARS.NPC_SIZE, BlackSmithPos, G_VARS.LEFT, true, G_VARS.NPC_ID, m_Dialogue);
		Scene::m_Npcs.push_back(std::move(BlackSmith));
	}

	// Level Loaders
	toTown = new LevelLoader(loaderTex, G_VARS.LEVEL_02, m_ToTownPos, m_ToTownOut, G_VARS.LOADER_SIZE);
	Scene::m_LevelLoaders.push_back(toTown);

	PlaySound(Scene::m_LevelEnterAudio);

}

void BSInterior::Update(float& dT){
	if(IsKeyPressed(KEY_E) && !Scene::m_Npcs.empty()){
		for( auto& npc : Scene::m_Npcs){
			if(CheckCollisionBoxes(Scene::m_Player->GetCollider(), npc->GetInteractCollider())){
				G_VARS.IN_DIALOGUE = true;
				Scene::npcInDialogue = npc.get();
			}
		}
	}

	Scene::Update(dT);

}

void BSInterior::Draw(){
	BeginMode3D(Scene::m_Camera);
		DrawPlane({}, m_MapSize + m_MapOffset, DARKGRAY);

		Scene::Draw();

	EndMode3D();
}

void BSInterior::Unload(){
	Scene::Unload();

	if(BlackSmith != nullptr){
		BlackSmith = nullptr;
	}

	UnloadTexture(loaderTex);
	UnloadTexture(npcTex);
	UnloadTexture(npcDialogueTex);

	for(size_t i = 0; i < 20; ++i){
		std::cout << "\n" << std::endl;
	}
	std::cout << "Unloaded BlackSmith Textures" << std::endl;

}