#include "player.h"
#include "classes/entities/entity.h"
#include "classes/npcs/npc.h"
#include "classes/scenes/scene.h"
#include "classes/scenes/levelStates.h"
#include "classes/utilities/globalVariables.h"
#include "classes/utilities/levelLoader.h"
#include "classes/projectiles/projectile.h"
#include <memory>
#include <format>

void Player::Update(float& dT){
	// KEYBINDS
		// Change Camera
	if(SceneManager::GetInstance().GetSceneID() != 0){
		if(GetMouseWheelMove()){
			if(Scene::m_Camera.projection == CAMERA_PERSPECTIVE){
				Scene::m_Camera.fovy = 15.0f;   
				Scene::m_Camera.projection = CAMERA_ORTHOGRAPHIC;
			}else{
				Scene::m_Camera.fovy = 35.0f;   
				Scene::m_Camera.projection = CAMERA_PERSPECTIVE;
			}
		}

		// INVENTORY
		if(IsKeyPressed(KEY_TAB)){
			if(!G_VARS.IN_DIALOGUE){
				G_VARS.INVENTORY_OPEN = !G_VARS.INVENTORY_OPEN;
				G_VARS.ITEM_SELECTED = false;
			}

			if(G_VARS.INVENTORY_OPEN){
				PlaySound(m_InventoryOpen);
			}else{
				PlaySound(m_InventoryClose);
			}
		}

		// CHARACTER PANEL
		if(IsKeyPressed(KEY_C)){
			if(!G_VARS.IN_DIALOGUE){
				G_VARS.CHARACTER_PANEL = !G_VARS.CHARACTER_PANEL;
			}
		}
	
		// SWAP SPELLS
		if(IsKeyPressed(KEY_ONE)){
			if(m_SelectedSpell >= 2){
				m_SelectedSpell = 0;
			}else{
				++m_SelectedSpell;
			}
			
			UnloadTexture(m_SpellTexture);

			switch(m_SelectedSpell){
				case 0: m_SpellTexture = LoadTexture("classes/projectiles/artwork/fireball_image.png"); m_SpellDamage = 2.0; break;
				case 1: m_SpellTexture = LoadTexture("classes/projectiles/artwork/frostorb_image.png"); m_SpellDamage = 2.5; break;
				case 2: m_SpellTexture = LoadTexture("classes/projectiles/artwork/lightningbolt_image.png"); m_SpellDamage = 1.5; break;
			}
		}

		// VECTOR SIZE INFORMATION
		if(IsKeyPressed(KEY_U)){
			std::cout << "Entities Size: " << Scene::m_Entities.size() << std::endl;
			std::cout << "Enemies Size: " << Scene::m_Enemies.size() << std::endl;
			std::cout << "LevelLoaders Size: " << Scene::m_LevelLoaders.size() << std::endl;
			std::cout << "Npcs Size: " << Scene::m_Npcs.size() << std::endl;
			std::cout << "Projectiles Size: " << Scene::m_Projectiles.size() << std::endl;
			std::cout << "Items Size: " << Scene::m_Items.size() << std::endl;
			std::cout << "Equipment Size " << m_Equipment.size() << std::endl;

		}

		// MELEE ATTACK
		if((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)) && !m_ToolSwung){
			// Just reuse our tool swung state cause why not :D
			for( auto& enemy : Scene::m_Enemies){
				if(CheckCollisionBoxes(Scene::m_Player->GetCollider(), enemy->GetCollider())){
					if(CheckCollisionBoxes(GetCollider(), enemy->GetCollider())){
						enemy->TakeDamage(m_Damage);
						// Update text AND position
						if(G_VARS.POPUP_TEXT){
							SceneManager::GetInstance().GetCurrentScene()->ResetPopupText();
						}
						std::string m_Text = std::format("{:.2f}", m_Damage);
						SceneManager::GetInstance().GetCurrentScene()->SetPopupInfo(m_Text, enemy->GetPos());
						G_VARS.POPUP_TEXT = true;
						
					}
				}
			}
			m_ToolSwung = true;
		}

		// WIZARD TIME!!!
		if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !G_VARS.IN_DIALOGUE && m_Casted != true){
			Vector2 mousePos = GetMousePosition();

			// Add future spells to here for mana checks
			switch(m_SelectedSpell){
				case 0: m_SpellCost = Fireball::m_SpellCost; break;
				case 1: m_SpellCost = FrostOrb::m_SpellCost; break;
				case 2: m_SpellCost = LightningBolt::m_SpellCost; break;
			}

			if(m_Mana - m_SpellCost >= 0.00f){
				std::unique_ptr<Projectile> m_CurrentSpell;

				// Add future spells to here for creating them
				switch(m_SelectedSpell){
					case 0: m_CurrentSpell = std::make_unique<Fireball>(GetPos(), mousePos); break;
					case 1: m_CurrentSpell = std::make_unique<FrostOrb>(GetPos(), mousePos); break;
					case 2: m_CurrentSpell = std::make_unique<LightningBolt>(GetPos(), mousePos); break;
				}

				m_Mana -= m_CurrentSpell->ManaCost();
				m_SpellCooldown = m_CurrentSpell->SpellCooldown();
				PlaySound(m_CurrentSpell->m_CastingSound);
				Scene::m_Projectiles.push_back(std::move(m_CurrentSpell));
				m_Casted = true;

			}
		}

		// TEMP stat changes
		// LMAO might need to rebind tick_speed to another key
		// Also cap player movement speed unless we truly dont care about it?
		if(IsKeyPressed(KEY_UP)){
			m_ManaRegen += 0.1f;
			m_HealthRegen += 0.1f;
			m_Defense += 0.1f;
			m_SprintSpeed += 0.1f;
		}

		// Messing with tick speed 
		if(IsKeyPressed(KEY_LEFT)){
			G_VARS.TICK_SPEED += 0.1f;
		}

		// TEMP skin change
		if(IsKeyPressed(KEY_DOWN)){
			G_VARS.UPDATE_SKIN = true;
			G_VARS.IS_CHICKEN = !G_VARS.IS_CHICKEN;
			G_VARS.IS_KNIGHT = !G_VARS.IS_KNIGHT;
		}

		// Axe Testing
		if(IsKeyPressed(KEY_TWO)){
			m_AxeEquipped = !m_AxeEquipped;
			std::cout << m_AxeEquipped << std::endl;
		}

		// Axe Tier Testing
		if(IsKeyPressed(KEY_THREE)){
			++m_AxeTier;
			std::cout << m_AxeTier << std::endl;
		}

		// Dyslexic Setting lol
		if(IsKeyPressed(KEY_FOUR)){
			G_VARS.DYSLEXIC = !G_VARS.DYSLEXIC;
			if(G_VARS.DYSLEXIC){
				G_VARS.FONT_SPACING = G_VARS.FONT_DYSLEXIC_SPACING;
			}else{
				G_VARS.FONT_SPACING = G_VARS.DEFAULT_FONT_SPACING;
			}
		}

		Entity::Update(dT);

		// Temp updating of skin
		if(G_VARS.UPDATE_SKIN){
			UnloadTexture(m_Idle);
			UnloadTexture(m_Running);
			if(G_VARS.IS_CHICKEN){
				m_Idle = LoadTexture("artwork/chimken_idle.png");
				m_Running = LoadTexture("artwork/chimken_walking.png");
				m_Size = G_VARS.CHICKEN_SIZE;
			}
			if(G_VARS.IS_KNIGHT){
				m_Idle = LoadTexture("classes/player/artwork/player_idle.png");
				m_Running = LoadTexture("classes/player/artwork/player_run.png");
				m_Size = { 1.0f, 1.0f };
			}
			G_VARS.UPDATE_SKIN = false;
		}

		// SPELL COOLDOWN
		if(m_Casted){
			m_CastTimer += dT;
			if(m_CastTimer >= m_SpellCooldown){
				m_Casted = false;
				m_CastTimer = 0.0f;
			}
		}

		// TOOL COOLDOWN
		if(m_ToolSwung){
			m_SwingCooldown += dT;
			if(m_SwingCooldown >= m_MaxSwingCooldown){
				m_ToolSwung = false;
				m_SwingCooldown = 0.0f;
			}
		}

		// DAMAGE VISUALS
		if(m_TookDamage){
			m_FlashTimer += dT;
			if(m_FlashTimer >= m_FlashMaxTime){
				m_Color = WHITE;
				m_TookDamage = false;
				m_FlashTimer = 0.0f;
			}else{
				m_Color = RED;
			}
		}

		checkWorldBounds();
		m_LastPos = m_Pos;

		if(m_Walking){
			m_Walking = false;
		}

		// MOVEMENT
		if(G_VARS.IN_DIALOGUE == false){
			// Sprinting
			if(IsKeyDown(KEY_LEFT_SHIFT)){
				m_CurrentSpeed = m_BaseSpeed * m_SprintSpeed;
				m_Texture = m_Running;
				m_Sprinting = true;
			}else{
				m_CurrentSpeed = m_BaseSpeed;
				m_Texture = m_Idle;
				m_Sprinting = false;
			}

			// Movement
			if(IsKeyDown(KEY_W)){
				m_Pos.z -= m_CurrentSpeed * dT;
				m_Walking = true;
			}
			if(IsKeyDown(KEY_A)){
				m_Pos.x -= m_CurrentSpeed * dT;
				m_Facing = -1.0f;
				m_Walking = true;
			}
			if(IsKeyDown(KEY_S)){
				m_Pos.z += m_CurrentSpeed * dT;
				m_Walking = true;
			}
			if(IsKeyDown(KEY_D)){
				m_Pos.x += m_CurrentSpeed * dT;
				m_Facing = 1.0f;
				m_Walking = true;
			}

			// Moving Audio Section
			if(m_Sprinting){
				m_Walking = false;
				if(!IsSoundPlaying(m_RunningSound)){
					PlaySound(m_RunningSound);
				}
			}else{
				StopSound(m_RunningSound);
			}
			
			if(m_Walking){
				if(!IsSoundPlaying(m_WalkingSound)){
					PlaySound(m_WalkingSound);
				}
			}else{
				StopSound(m_WalkingSound);
			}
		}
	}
}

void::Player::Draw(Camera3D& camera){
	Entity::Draw(camera);
}

float& Player::GetDamage(){
	return m_Damage;
}

const std::map<std::unique_ptr<Item>, int>& Player::GetInventory() const{
	return m_Inventory;
}

void Player::AddToInventory(std::unique_ptr<Item> item){
    bool found = false;
    for(auto& items : m_Inventory){
        auto invItem = items.first.get();
        int& itemCount = items.second;

		// Later on when we add stack sizes
		// If we find the item, check the stack size
		// If count >= invItem->GetStackSize()
		// Then we want to do another search to find the item
		// Check the count all over again
		// Probably not too good but i think it will work
        if(item->GetID() == invItem->GetID()){
            ++itemCount; 

            found = true;
			item = nullptr;
            break; 
        }
    }

    if(!found){
        m_Inventory.insert(std::make_pair(std::move(item), 1));
   
    }
}

void Player::RemoveFromInventory(Item* item){
	for(auto currentItem = m_Inventory.begin(); currentItem != m_Inventory.end(); ++currentItem){
		if(currentItem->first.get() == item){
			if(currentItem->second > 1){
				currentItem->second -= 1;
			}else{
				Scene::m_SelectedItem = nullptr;
				m_Inventory.erase(currentItem);
				G_VARS.ITEM_SELECTED = false;
				break;
			}

		}
	}
	
}

void Player::UseItem(Item* item){
	if(!item) return;
	switch(item->GetID()){
		case Item::ItemID::HEALTH_POTION: 
			if(m_Health + item->GetRestoreAmount() <= m_MaxHealth){
				std::cout << m_Health << "/" << m_MaxHealth << std::endl;
				m_Health += item->GetRestoreAmount();
				RemoveFromInventory(item);
			}else if(m_Health + item->GetRestoreAmount() >= m_MaxHealth && m_Health < m_MaxHealth){
				std::cout << m_Health << "/" << m_MaxHealth << std::endl;
				m_Health = m_MaxHealth;
				RemoveFromInventory(item);
			}
			break;
		case Item::ItemID::MANA_POTION: 
			if(m_Mana + item->GetRestoreAmount() < m_MaxMana){
				m_Mana += item->GetRestoreAmount();
				RemoveFromInventory(item);
			}else if(m_Mana + item->GetRestoreAmount() >= m_MaxMana && m_Mana < m_MaxMana){
				m_Mana = m_MaxMana;
				RemoveFromInventory(item);
			}
			break;
		// TODO Make sure to move the item if its not a consumable to our equipment vector 
		case Item::ItemID::WEAPON: 
			// TODO Fix this so that we can push item to our equipment slot AND still remove items from the inventory
			//m_Equipment.push_back(item); break;
		case Item::ItemID::HELMET: break;
		case Item::ItemID::CHEST: break;
		case Item::ItemID::LEGS: break;
		default: std::cout << "Defaulted" << std::endl; break;
	}
	
}


void Player::TakeDamage(float damage){
	if(m_Health - damage <= 0.0f){
		m_Alive = false;
	}else{
		m_Health -= damage;
		m_TookDamage = true;
	}
}

Vector3& Player::GetSpawnPoint(){
	return m_SpawnPoint;
}

Vector2& Player::GetWorldSize(){
	return worldSize;
}

Vector3& Player::GetLastPos(){
	return m_LastPos;
}

float& Player::GetHealth(){
	return m_Health;
}

float& Player::GetFacing(){
	return m_Facing;
}

float& Player::GetMaxHealth(){
	return m_MaxHealth;
}

float& Player::GetMaxMana(){
	return m_MaxMana;
}

float& Player::GetMana(){
	return m_Mana;
}

float& Player::GetHealthRegen(){
	return m_ManaRegen;
}

float& Player::GetManaRegen(){
	return m_ManaRegen;
}

float& Player::GetDefense(){
	return m_Defense;
}

float& Player::GetSpeed(){
	return m_SprintSpeed;
}

float& Player::GetSpellDamage(){
	return m_SpellDamage;
}

size_t& Player::GetInventoryMaxSize(){
	return m_MaxInventorySize;
}

void Player::checkWorldBounds(){
	if(m_Pos.x >= worldSize.x / 2.0f - m_Size.x ||
		m_Pos.x <= (worldSize.x / 2.0f) * -1.0f  ||
		m_Pos.z >= worldSize.y / 2.0f	+ m_Size.x||
		m_Pos.z <= (worldSize.y / 2.0f) * -1.0f + m_Size.x){
		m_Pos = m_LastPos;
	}else{
		m_LastPos = m_Pos;
	}
}
