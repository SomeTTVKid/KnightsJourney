#include "scene.h"
#include "raylib.h"
#include "classes/entities/entity.h"
#include "classes/player/player.h"
#include "classes/utilities/sceneManager.h"
#include "classes/utilities/globalVariables.h"
#include "classes/utilities/levelLoader.h"
#include <algorithm>

void Scene::SetInteractText(){
	G_VARS.DISPLAY_TEXT = true;
	worldSpace = GetWorldToScreen(Scene::m_Player->GetPos(), Scene::m_Camera);
	worldSpace.x = worldSpace.x - 75.0f;
	worldSpace.y = worldSpace.y - 85.0f;
}

void Scene::DisplayInteractText(){
	DrawTextEx(G_VARS.FONT, "Press 'E' to interact.", worldSpace, 28, 1.0f, ORANGE);
}

// Build our frame 
void Scene::BuildFrame(){
	m_DrawList.clear();

	// Entities
	for(auto& entity : m_Entities){
		if(entity->GetState()){
			m_DrawList.push_back((DrawableVariant)entity);
		} 	
	}

	// Structures
	if(!m_Structures.empty()){
		for( auto& structure : m_Structures){
			if(structure->GetState()){
				m_DrawList.push_back(structure.get());
			}
		}
	}

	// Level Loaders
	for( auto& loader : m_LevelLoaders){
		m_DrawList.push_back((DrawableVariant)loader);
	}
	
	// Npcs
	if(!m_Npcs.empty()){
		for( auto& npc : m_Npcs){
			if(npc && npc->GetState()){
				m_DrawList.push_back(npc.get());
			}
		}
	}

	// Enemies
	if(!m_Enemies.empty()){
		for( auto& enemy : m_Enemies){
			if(enemy && enemy->GetState()){
				m_DrawList.push_back(enemy.get());
			}	
		}
	}

	// Items
	if(!m_Items.empty()){
		for( auto& item : m_Items){
			if(item && item->GetState()){
				m_DrawList.push_back(item.get());
			}
		}
	}

	// Player
	// Checking if current scene isnt Main Menu
	if(SceneManager::GetInstance().GetSceneID() != 0){
		m_DrawList.push_back((DrawableVariant)m_Player);
	} 

	// Projectiles
	if(!m_Projectiles.empty()){ 
		for(const auto& projectile : m_Projectiles){
			if(projectile && projectile->GetState()){
				m_DrawList.push_back(projectile.get());
			} 
		}
	} 

	// Sort
	SceneManager::DepthBuffer(m_DrawList);
}

void Scene::SetPlayer(Player* Player){
	m_Player = Player;
}

void Scene::Load(){
	if(IsMusicValid(m_LevelMusic)){
		SetMusicVolume(m_LevelMusic, G_VARS.VOLUME);
	}
}

void Scene::Update(float& dT){
	if(IsMusicValid(m_LevelMusic)){
		UpdateMusicStream(m_LevelMusic);
	}

	// Setting Camera Position Out of Main Menu
	if(SceneManager::GetInstance().GetSceneID() != 0){
		Vector3 cameraOffset = { 0.0f, 10.0f, 10.0f }; 
		m_Camera.position = Vector3Add(m_Player->GetPos(), cameraOffset);
		m_Camera.target = m_Player->GetPos();
		m_OpacityRay.position = m_Camera.position;
		Vector3 m_RayTarget = Vector3Subtract(m_Camera.target, m_Camera.position);
		m_RayTarget.x += 0.5;
		m_OpacityRay.direction = Vector3Normalize(m_RayTarget);
	}else{
		m_Camera.target = {0.0f, 0.0f, 0.0f};
		m_Camera.position = { 0.0f, 10.0f, 10.0f };
		m_Camera.up = { 0.0f, 1.0f, 0.0f };         
		m_Camera.fovy = 35.0f;                         
		m_Camera.projection = CAMERA_PERSPECTIVE;    
	}

// Updating Vectors & Collision Checks
	// Entities
	// TODO Make sure to update this when done with refactor
	if(!m_Entities.empty()){
		std::erase_if(m_Entities, [](const auto& entity) {
			return !entity->GetState();
		});

		for( auto& entity : m_Entities){
			if(entity && entity->GetState()){
				entity->Update(dT);
				// Turn objects transparent
				m_RayCollision = GetRayCollisionBox(m_OpacityRay, entity->GetCollider());
				if(m_RayCollision.hit){
					entity->m_Color.a = 100;
				}else{
					if(entity->m_Color.a != 255){
						entity->m_Color.a = 255;
					}
				}
				// Collision Checks
				if(entity->HasCollider()){
					if(CheckCollisionBoxes(m_Player->GetCollider(), entity->GetCollider())){
						m_Player->GetPos() = m_Player->GetLastPos();
						break;
					}
				}
			}
		}
	}

	if(!m_Structures.empty()){
		std::erase_if(m_Structures, [](const auto& structure) {
			return !structure->GetState();
		});

		for( auto& structure : m_Structures){
			if(structure && structure->GetState()){
				structure->Update(dT);
				// Turn objects transparent
				if(structure->HasCollider()){
					m_RayCollision = GetRayCollisionBox(m_OpacityRay, structure->GetCollider());
					if(m_RayCollision.hit){
						structure->m_Color.a = 100;
					}else{
						if(structure->m_Color.a != 255){
							structure->m_Color.a = 255;
						}
					}

					// Collision Checks
					if(CheckCollisionBoxes(m_Player->GetCollider(), structure->GetCollider())){
						m_Player->GetPos() = m_Player->GetLastPos();
						// WoodCutting
						// TODO
						// Change this once we add in mining
						// Whole function overhaul as well :D
						// Mess with resource health in regards to scaling with axeTier
						// Might check for id before mouseclick? 
						// We just need some way to check if we need to woodcut or mine 
						// Make sure to add a cooldown to tool swinging so we cant spam it
						if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_Player->IsAxeEquipped() && static_cast<int>(structure->GetID()) == 0){
							// 10 is the max axeTier achievable...maybe a secret later could increase it or a quest?
							structure->TakeDamage(m_Player->GetAxeTier() + 10.0f / 10.0f, m_Player->GetAxeTier());
							break;
						} 
						break;
					}
				}
				continue;
			}
		}
	}

	// Level Loaders
	for( auto& loader : m_LevelLoaders){
		loader->Update(dT);

		if(CheckCollisionBoxes(Scene::m_Player->GetCollider(), loader->GetInteractCollider())){
			SetInteractText();
			break;
		}else{
			G_VARS.DISPLAY_TEXT = false;
		}

	}

	// Enemies
	if(!m_Enemies.empty()){
		std::erase_if(m_Enemies, [](const auto& enemy) {
			return !enemy->GetState();
		});

		for( auto& enemy : m_Enemies){
			if(enemy && enemy->GetState()){
				enemy->Update(dT);
			}
		}
	}

	// Npcs
	if(!m_Npcs.empty()){
		std::erase_if(m_Npcs, [](const auto& npc) {
			return !npc->GetState();
		});

		for( auto& npc : m_Npcs){
			if(npc && npc.get()->GetState()){
				npc->Update(dT);
				if(CheckCollisionBoxes(m_Player->GetCollider(), npc->GetCollider())){
					m_Player->GetPos() = m_Player->GetLastPos();
					break;	
				}
			}
		}
	}

	// Projectiles
	if(!m_Projectiles.empty()){
		std::erase_if(m_Projectiles, [](const auto& projectile) {
			return !projectile->GetState();
		});
		
		for (auto& projectile : m_Projectiles){
			if(projectile->GetState()){
				projectile->Update(dT);

				// Collision with enemies
				for( auto& enemy : m_Enemies){
					if(CheckCollisionBoxes(projectile->GetCollider(), enemy->GetCollider())){
						projectile->GetState() = false;
						enemy->TakeDamage(projectile->GetDamage());
						enemy->GetPos().x += projectile->GetVector().x * 10.0f * dT;
						enemy->GetPos().z += projectile->GetVector().y * 10.0f * dT;
						break;
					}
				}

				// Collision with structures
				for( auto& entity : m_Entities){
					if(CheckCollisionBoxes(projectile->GetCollider(), entity->GetCollider())){
						projectile->GetState() = false;
						break;
					}
				}

				// Collision with npcs
				for( auto& npc : m_Npcs){
					if(CheckCollisionBoxes(projectile->GetCollider(), npc->GetCollider())){
						projectile->GetState() = false;
						break;
					}
				}
			}

		}
	}

	// Items
	if(!m_Items.empty()){
		for( auto& item : m_Items){
			if(item && item.get()->GetState()){
				if(CheckCollisionBoxes(m_Player->GetCollider(), item->GetCollider())){
					SetInteractText();
					if(IsKeyPressed(KEY_E) && m_Player->GetInventory().size() < m_Player->GetInventoryMaxSize()){
						item->GetState() = false;
						m_Player->AddToInventory(std::move(item));
						auto itemToRemove = std::find(m_Items.begin(), m_Items.end(), item);
						m_Items.erase(itemToRemove);
						PlaySound(m_PickUpAudio);
						break;
					}else{
						// Add a drawtext function call here with a timer 
						// Will say 'Inventory Full'
						break;
					} 
				}
			}
		}
	}

	// Inventory Interaction
	if(G_VARS.INVENTORY_OPEN && !m_Player->GetInventory().empty()){
		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			// Check for collisionPointRec here
			Vector2 mousePos = GetMousePosition();
			for( auto& [item, itemCount] : m_Player->GetInventory()){
				// TODO Make sure to check for buttons later before breaking
				// So that we dont clear selectedItem pointer before interacting!
				if (CheckCollisionPointRec(mousePos, item.get()->GetRect())) {
					G_VARS.ITEM_SELECTED = true;
					m_SelectedItem = item.get(); 
					break;
				}
				m_SelectedItem = nullptr;
			}
		}
	}

	// Regeneration
	if(Scene::m_Player->GetHealth() < Scene::m_Player->GetMaxHealth()){
		Scene::m_Player->GetHealth() = std::clamp(Scene::m_Player->GetHealth() += Scene::m_Player->GetHealthRegen() * dT, 0.0f, Scene::m_Player->GetMaxHealth());
	}

	if(Scene::m_Player->GetMana() < Scene::m_Player->GetMaxMana()){
		Scene::m_Player->GetMana() = std::clamp(Scene::m_Player->GetMana() += Scene::m_Player->GetManaRegen() * dT, 0.0f, Scene::m_Player->GetMaxMana());
	}

	Scene::m_Player->Update(dT);

}

void Scene::Draw() {
	BuildFrame();

	// Draw
	for(const auto& drawable : m_DrawList) {
		std::visit([&](auto* obj) {
			obj->Draw(Scene::m_Camera);
		}, drawable);
	}
}

void Scene::Unload(){
    for( auto& entity : m_Entities){
        delete entity;
    }

	if(npcInDialogue != nullptr){
		npcInDialogue = nullptr;
	}

	if(m_SelectedItem != nullptr){
		m_SelectedItem = nullptr;
	}

	for( auto& loader : m_LevelLoaders){
		delete loader;
	}

	if(IsSoundValid(m_LevelEnterAudio)){
		UnloadSound(m_LevelEnterAudio);
		m_LevelEnterAudio = {};
	}
	if(IsSoundValid(m_LevelExitAudio)){
		UnloadSound(m_LevelExitAudio);
		m_LevelExitAudio = {};
	}

	if(IsMusicValid(m_LevelMusic)){
		UnloadMusicStream(m_LevelMusic);
		m_LevelMusic = {};
	}

	worldSpace = {};
	G_VARS.INVENTORY_OPEN = false;
	G_VARS.IN_DIALOGUE = false;
	G_VARS.INTERACT_TEXT = false;
	G_VARS.ITEM_SELECTED = false;
	G_VARS.CHARACTER_PANEL = false;

    m_Entities.clear(); 
	m_DrawList.clear();
	m_LevelLoaders.clear();
	m_Projectiles.clear();
	m_Enemies.clear();
	m_Npcs.clear();
	m_Items.clear();
	m_Structures.clear();
}