#include "scene.h"
#include "raylib.h"
#include "classes/entities/entity.h"
#include "classes/player/player.h"
#include "classes/utilities/sceneManager.h"
#include "classes/utilities/globalVariables.h"
#include "classes/utilities/levelLoader.h"
#include "classes/scenes/levelStates.h"
#include <algorithm>
#include <format>

void Scene::SetInteractText(){
	G_VARS.DISPLAY_TEXT = true;
	Vector2 interactTextSize = MeasureTextEx(G_VARS.FONT, G_VARS.INTERACT_TEXT.c_str(), 28 * G_VARS.WIDTH_SCALE, G_VARS.FONT_SPACING);
	m_WorldSpace = GetWorldToScreen(Scene::m_Player->GetPos(), Scene::m_Camera);
	m_WorldSpace.x = m_WorldSpace.x - interactTextSize.x / 3.0f;
	m_WorldSpace.y = m_WorldSpace.y - 130.0f * G_VARS.HEIGHT_SCALE;
}

void Scene::Popup_text(float& dT){
	m_TextPosition = GetWorldToScreen(m_EntityPosition, Scene::m_Camera);
	m_CurrentTime += dT;
	m_TextOpacity -= 119.5f * dT; 
	m_TextOpacity = std::clamp(static_cast<float>(m_TextOpacity), 0.0f, 255.0f);
	if(m_CurrentTime < m_PopupTime){
		DrawTextEx(G_VARS.FONT, m_Text.c_str(), m_TextPosition, 38 * G_VARS.WIDTH_SCALE, G_VARS.FONT_SPACING, {255, 255, 255, m_TextOpacity});
	}else{
		m_CurrentTime = 0.0f;
		G_VARS.POPUP_TEXT = false;
		m_TextOpacity = 255;
	}
}

void Scene::ResetPopupText(){
	m_CurrentTime = 0.0f;
	m_TextOpacity = 255;
	G_VARS.POPUP_TEXT = false;
}

void Scene::SetPopupInfo(std::string text, Vector3 enemyPos){
	m_Text = text,
	m_EntityPosition = enemyPos;
	
}

void Scene::DisplayInteractText(){
	DrawTextEx(G_VARS.FONT, G_VARS.INTERACT_TEXT.c_str(), m_WorldSpace, 28 * G_VARS.WIDTH_SCALE, G_VARS.FONT_SPACING, ORANGE);
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

	if(IsKeyPressed(KEY_E)){
		for( auto& loader : Scene::m_LevelLoaders){
			if(CheckCollisionBoxes(Scene::m_Player->GetCollider(), loader->GetInteractCollider())){
				m_Player->GetSpawnPoint() = loader->GetOutPos();
				loader->MoveScene();
				return;
			}
		}

		for( auto& npc : Scene::m_Npcs){
			if(CheckCollisionBoxes(Scene::m_Player->GetCollider(), npc->GetInteractCollider())){
				G_VARS.IN_DIALOGUE = true;
				Scene::npcInDialogue = npc.get();
				return;
			}
		}
	}

	// Setting Camera Position Out of Main Menu
	if(SceneManager::GetInstance().GetSceneID() != 0){
		Vector3 cameraOffset = { 0.0f, 10.0f, 10.0f }; 
		m_Camera.position = Vector3Add(m_Player->GetPos(), cameraOffset);
		m_Camera.target = m_Player->GetPos();
		// Opacity RayCast
		m_OpacityRay.position = m_Camera.position;
		Vector3 m_RayTarget = Vector3Subtract(m_Camera.target, m_Camera.position);
		m_RayTarget.x += 0.5f;
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
				m_StructureCollision = GetRayCollisionBox(m_OpacityRay, entity->GetCollider());
				if(m_StructureCollision.hit){
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
					// Collision Checks
					if(CheckCollisionBoxes(m_Player->GetCollider(), structure->GetCollider())){
						m_Player->GetPos() = m_Player->GetLastPos();
					}

					// Ray collision detection
					m_StructureCollision = GetRayCollisionBox(m_OpacityRay, structure->GetCollider());
					m_LeavesCollision = GetRayCollisionBox(m_OpacityRay, structure->GetCurrentLeavesCollider());
					if(m_StructureCollision.hit || m_LeavesCollision.hit){
						// Opacity level
						structure->m_Color.a = 80;
					}else{
						if(structure->m_Color.a != 255){
							structure->m_Color.a = 255;
						}
					}

					if(CheckCollisionBoxes(m_Player->GetCollider(), structure->GetInteractCollider()) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT))){
						if(m_Player->m_ToolSwung != true){
							// Woodcutting Section
							if(m_Player->m_AxeEquipped && static_cast<int>(structure->GetID()) == 0){
								if(structure->IsDepleted() != true && structure->GetHealth() > 0 && m_Player->m_AxeTier >= structure->GetTier()){
									// 10 is the max axeTier achievable...maybe a secret later could increase it or a quest
									// TODO combine all of this into a function perhaps?
									// Would look a lot cleaner than what we have right now
									structure->TakeDamage(m_Player->m_AxeTier + 10.0f / 10.0f);
									m_Player->m_ToolSwung = true;
									// Need to get around only being able to display one thing at a time
									// Need to somehow add this function call inside of each entity/structure's draw call
									// Somehow draw text in the 3d space
									// Still need to find a way to display multiple text on the screen using this system!
									// What if the entity subclass had a text variable, and instead of global flag for showing text, it was on taking damage
									// Since even skill is just damaging the strucure?
									if(G_VARS.POPUP_TEXT){
										ResetPopupText();
									}

									m_Text = std::format("{:.2f}/{:.2f}", structure->GetHealth(), structure->GetMaxHealth());
								}
								else if(m_Player->m_AxeTier < structure->GetTier()){
									m_Text = "Axe tier is not high enough.";
								}
								else{
									m_Text = "This resource is depleted!";
								}
								m_EntityPosition = structure->GetPos();
								G_VARS.POPUP_TEXT = true;
								break;
							}
							// TODO Mining Section 
						} 
					}
				}
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
				if(CheckCollisionBoxes(m_Player->GetCollider(), npc->GetInteractCollider())){
					SetInteractText();
					G_VARS.DISPLAY_TEXT = true;
				}

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
						if(G_VARS.POPUP_TEXT){
							ResetPopupText();
						}
						m_Text = std::format("{:.2f}", projectile->GetDamage());
						m_EntityPosition = enemy->GetPos();
						G_VARS.POPUP_TEXT = true;
						break;
					}
				}

				// Collision with structures
				// TODO Remove this later :D once we get signs into structure class
				for( auto& entity : m_Entities){
					if(CheckCollisionBoxes(projectile->GetCollider(), entity->GetCollider())){
						projectile->GetState() = false;
						break;
					}
				}

				for( auto& structure : m_Structures){
					if(CheckCollisionBoxes(projectile->GetCollider(), structure->GetCollider())){
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
			if(item && item->GetState()){
					if(CheckCollisionBoxes(m_Player->GetCollider(), item->GetCollider())){
						SetInteractText();
						if(IsKeyPressed(KEY_E) && m_Player->GetInventory().size() < m_Player->GetInventoryMaxSize()){
							item->GetState() = false;
							m_Player->AddToInventory(item);
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
			// TEMP
			Rectangle UseBtn = {
				G_VARS.WIDTH / 2.0f - ((350 * G_VARS.WIDTH_SCALE) / 2.0f) + 30 * G_VARS.WIDTH_SCALE, 
				G_VARS.HEIGHT / 1.8f + 250 * (G_VARS.HEIGHT_SCALE / 2.0f) - 60 * G_VARS.HEIGHT_SCALE, 
				75 * G_VARS.WIDTH_SCALE, 
				60 * G_VARS.HEIGHT_SCALE
			};

			Rectangle DltBtn = {
				G_VARS.WIDTH / 2.0f - ((350 * G_VARS.WIDTH_SCALE) / 2.0f) + 350 * G_VARS.WIDTH_SCALE - 120 * G_VARS.WIDTH_SCALE, 
				G_VARS.HEIGHT / 1.8f + 250 * G_VARS.HEIGHT_SCALE / 2.0f - 60 * G_VARS.HEIGHT_SCALE,
				75 * G_VARS.WIDTH_SCALE, 
				60 * G_VARS.HEIGHT_SCALE
			};

			// Check for collisionPointRec here
			Vector2 mousePos = GetMousePosition();
			for( auto& [item, itemCount] : m_Player->GetInventory()){
				// TODO Make sure to check for buttons later before breaking
				// So that we dont clear selectedItem pointer before interacting!
				if(CheckCollisionPointRec(mousePos, item->GetRect())) {
					G_VARS.ITEM_SELECTED = true;
					m_SelectedItem = item; 
					// TODO Maybe remove this break call and seperate the checks for the buttons?
					break;
				}else if(CheckCollisionPointRec(mousePos, UseBtn)){
					// Use the currently selected item (may be null)
					m_Player->UseItem(m_SelectedItem);
					break;
				}else if(CheckCollisionPointRec(mousePos, DltBtn)){
					if(m_SelectedItem){
						m_Player->RemoveFromInventory(m_SelectedItem);
					}
					break;
				}else{
					m_SelectedItem = nullptr;
				}
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

	m_WorldSpace = {};
	G_VARS.INVENTORY_OPEN = false;
	G_VARS.IN_DIALOGUE = false;
	G_VARS.DISPLAY_TEXT = false;
	G_VARS.ITEM_SELECTED = false;
	G_VARS.CHARACTER_PANEL = false;
	m_Player->m_AxeEquipped = false;

    m_Entities.clear(); 
	m_DrawList.clear();
	m_LevelLoaders.clear();
	m_Projectiles.clear();
	m_Enemies.clear();
	m_Npcs.clear();
	m_Items.clear();
	m_Structures.clear();
}