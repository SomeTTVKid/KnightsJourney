#include "sceneManager.h"
#include "globalVariables.h"
#include "classes/player/player.h"
#include "classes/projectiles/projectile.h"
#include "classes/items/item.h"
#include "classes/enemies/enemy.h"
#include "classes/npcs/npc.h"
#include "classes/scenes/scene.h"
#include "classes/scenes/mainMenu.h"
#include "classes/scenes/level01.h"
#include "classes/scenes/level02.h"
#include "classes/scenes/level03.h"
#include "classes/scenes/blacksmithInterior.h"
#include <iomanip> 
#include <string>
#include <cmath>
#include <format>

SceneManager& SceneManager::GetInstance(){
    static SceneManager instance;
    return instance;
}

int& SceneManager::GetSceneID(){
	return m_CurrentSceneID;
}

std::string& SceneManager::SetActiveQuest(){
	return m_ActiveQuest;
}

// Sorting and being effecient is hard but this works
void SceneManager::DepthBuffer(std::vector<DrawableVariant>& drawList) {
    // Create a temporary vector to store z-positions and indices.
    std::vector<std::pair<float, DrawableVariant*>> temp_drawables;
    temp_drawables.reserve(drawList.size());

    for (auto& drawable : drawList) {
        std::visit([&](auto* obj) {
            temp_drawables.emplace_back(obj->GetPos().z, &drawable);
        }, drawable);
    }

    // Sort the temporary vector based on z-position.
    std::sort(temp_drawables.begin(), temp_drawables.end(),
              [](const auto& a, const auto& b) {
                  return a.first < b.first;
              });

    // Reorder the original drawList based on the sorted temporary vector.
    std::vector<DrawableVariant> new_drawList;
    new_drawList.reserve(drawList.size());
    for (const auto& pair : temp_drawables) {
        new_drawList.push_back(std::move(*pair.second));
    }
    
    drawList = std::move(new_drawList);
}

void SceneManager::ChangeScene(int sceneID){
	if (m_CurrentScene) {
		m_CurrentScene->Unload();
		delete m_CurrentScene;
		m_CurrentScene = nullptr;
	}

	switch(sceneID){
		case G_VARS.SCENEIDS::MAIN_MENU: m_CurrentScene = new MainMenu(); m_CurrentSceneID = 0; break; 
		case G_VARS.SCENEIDS::LEVEL_01: m_CurrentScene = new Level01(); m_CurrentSceneID = 1; break; 
		case G_VARS.SCENEIDS::LEVEL_02: m_CurrentScene = new Level02(); m_CurrentSceneID = 2; break; 
		case G_VARS.SCENEIDS::LEVEL_03: m_CurrentScene = new Level03(); m_CurrentSceneID = 3; break; 
		case G_VARS.SCENEIDS::BSINTERIOR: m_CurrentScene = new BSInterior(); m_CurrentSceneID = 4; break; 
		default: m_CurrentScene = new MainMenu(); std::cout << "Error in scene change function" << std::endl; break;
	}

	if(m_CurrentScene){
		m_CurrentScene->Load();
	}

	if (Scene::m_Player && m_CurrentSceneID != 0) {
		Vector3 cameraOffset = { 0.0f, 10.0f, 10.0f };
		Scene::m_Camera.position = Vector3Add(Scene::m_Player->GetPos(), cameraOffset);
		Scene::m_Camera.target = Scene::m_Player->GetPos();
	}
    
}

void SceneManager::RequestSceneChange(int sceneID){
	// We are already in a scene transition
	if(m_Transitioning){
		return; 
	} 

	// Play Exit Audio for specific scenes
	if(IsSoundValid(Scene::m_LevelExitAudio)){
		PlaySound(Scene::m_LevelExitAudio);
	}

	m_PendingScene = sceneID;
	G_VARS.DISPLAY_TEXT = false;
	m_Transitioning = true;
	// Reset Phase, Timer, & Opacity
	m_TransitionPhase = 0; 
	m_TransitionTimer = 0.0f;
	m_Opacity = 0.0f;
}

void SceneManager::Update(float& dT){
	// We have this pending pause and timer 
	// So that we can close inventory 
	// And remove selected item pointer
	if(m_PendingPause){
		m_PauseTimer += dT;
		if(m_PauseTimer >= m_PauseTimerMax && Scene::m_Player->GetState()){
			m_PendingPause = false;
			G_VARS.IS_PAUSED = true;
		}
	}

	// Transition 
	if (m_Transitioning){
		m_TransitionTimer += dT;
		// fading out
		if (m_TransitionPhase == 0){
			m_Opacity = std::min(1.0f, m_TransitionTimer / m_TransitionMaxTime);
			if (m_TransitionTimer >= m_TransitionMaxTime){
				// swap scenes now that screen is fully covered
				if (m_PendingScene != -1){
					ChangeScene(m_PendingScene);
					m_PendingScene = -1;
				}
				// set phast to fade in
				// reset timer
				m_TransitionPhase = 1;
				m_TransitionTimer = 0.0f;
			}
		}
		// fading in
		else if (m_TransitionPhase == 1){
			m_Opacity = 1.0f - std::min(1.0f, m_TransitionTimer / m_TransitionMaxTime);
			if (m_TransitionTimer >= m_TransitionMaxTime){
				// done transitioning
				m_Transitioning = false;
				m_TransitionPhase = 0;
				m_TransitionTimer = 0.0f;
				m_Opacity = 0.0f;
			}
		}
	}

	// Keybind Section
		// FULLSCREEN MODE
	if(IsKeyPressed(KEY_F)){
		if(G_VARS.IS_PAUSED != true){
			G_VARS.FULLSCREEN = !G_VARS.FULLSCREEN;
			m_UpdateUI = true;
			if(G_VARS.FULLSCREEN){
				G_VARS.WIDTH = G_VARS.FULLSCREEN_WIDTH;
				G_VARS.HEIGHT = G_VARS.FULLSCREEN_HEIGHT;
				SetWindowSize(G_VARS.WIDTH, G_VARS.HEIGHT);
				ToggleFullscreen();

			}else{
				ToggleFullscreen();
				G_VARS.WIDTH = G_VARS.BASE_WIDTH;
				G_VARS.HEIGHT = G_VARS.BASE_HEIGHT;
				SetWindowSize(G_VARS.WIDTH, G_VARS.HEIGHT);
			}
			G_VARS.WIDTH_SCALE = G_VARS.WIDTH / 1000.0f;
			G_VARS.HEIGHT_SCALE = G_VARS.HEIGHT / 1000.0f;

			G_VARS.WIDTH_SCALE = std::clamp(G_VARS.WIDTH_SCALE, G_VARS.BASE_WIDTH / 1000.0f, 1.5f);
			G_VARS.HEIGHT_SCALE = std::clamp(G_VARS.HEIGHT_SCALE, G_VARS.BASE_HEIGHT / 1000.0f, 1.5f);
			
		}
	}
	 
	if(G_VARS.IN_DIALOGUE){
		if(IsKeyPressed(KEY_ESCAPE)){
			G_VARS.IN_DIALOGUE = false;
		}
	}else if(m_CurrentSceneID != 0){
		if(IsKeyPressed(KEY_ESCAPE)){
			G_VARS.INVENTORY_OPEN = false;
			G_VARS.ITEM_SELECTED = false;
			G_VARS.CHARACTER_PANEL = false;
			Scene::m_SelectedItem = nullptr;
			if(G_VARS.IS_PAUSED != true){
				m_PendingPause = true;
				m_PauseTimer = 0.0f;
			}else{
				G_VARS.IS_PAUSED = false;
			}
		}
	}
	// TODO Either remove this entirely or add it back in at a later date
	// }else{
	// 	if(IsKeyPressed(KEY_ESCAPE)){
	// 		G_VARS.RUNNING = false;
	// 	}
	// }

	// DEBUG MODE
	if(IsKeyPressed(KEY_GRAVE)){
		G_VARS.DEBUG_MODE = !G_VARS.DEBUG_MODE;
	}

	// TODO Remove this at some point
	if(IsKeyPressed(KEY_UP)){
		G_VARS.VOLUME += 0.1f;
		SetMusicVolume(Scene::m_LevelMusic, G_VARS.VOLUME);
	}

	// PRINT POSITION
	if(IsKeyPressed(KEY_M)){
		std::cout << "Player X Coord " << Scene::m_Player->GetPos().x << "\n"
					<< "Player Z Coord " << Scene::m_Player->GetPos().z << std::endl;
	}

	// Only update when not transitioning, unpaused, and player alive
	if(m_Transitioning || G_VARS.IS_PAUSED || Scene::m_Player->GetState() != true){
		return;
	}else{
		if (m_CurrentScene) {
			m_CurrentScene->Update(dT);
		}

		fullHearts = static_cast<int>(Scene::m_Player->GetHealth() / Scene::m_Player->m_HealthPerHeart);
		maxHearts = static_cast<int>(Scene::m_Player->GetMaxHealth() / Scene::m_Player->m_HealthPerHeart);

		fullStars = static_cast<int>(Scene::m_Player->GetMana() / Scene::m_Player->m_ManaPerStar);
		maxStars = static_cast<int>(Scene::m_Player->GetMaxMana() / Scene::m_Player->m_ManaPerStar);

		Scene::m_Player->m_OnHalfHeart = (Scene::m_Player->GetHealth() / Scene::m_Player->m_HealthPerHeart) - fullHearts >= 0.5f;
		Scene::m_Player->m_OnHalfStar = (Scene::m_Player->GetMana() / Scene::m_Player->m_ManaPerStar) - fullStars >= 0.5f;
	}
}

void SceneManager::Draw(){

	BeginDrawing();

	// Alive and Un-Paused
	if(G_VARS.IS_PAUSED != true && Scene::m_Player->GetState()){
		ClearBackground(BLACK); 

		// Draw Scene
		if (m_CurrentScene) {
			m_CurrentScene->Draw();
		}

		// Debug Mode
		if(G_VARS.DEBUG_MODE){
			// Draw Frame Time
			frametime = std::to_string(GetFrameTime());
			DrawText(frametime.c_str(), 0, 0, 15, WHITE);

			// FPS
			DrawFPS(G_VARS.WIDTH / 2.0f, 0);
		}

		// Draw Interact Text
		if(G_VARS.DISPLAY_TEXT){
			m_CurrentScene->DisplayInteractText();
		}

		if(G_VARS.POPUP_TEXT){
			float dT = GetFrameTime();
			m_CurrentScene->Popup_text(dT);
		}

		// Draw Everything NOT on MAIN MENU
		if(m_CurrentSceneID != 0){
			DrawUI();
	
		}

	// Alive and Paused
	}else if(G_VARS.IS_PAUSED){
		G_VARS.INVENTORY_OPEN = false;
		DrawPauseScreen();
	// Dead
	}else{
		G_VARS.INVENTORY_OPEN = false;
		DrawDeadScreen();
	}

	// Draw transition if active
	if (m_Transitioning){
		// m_Opacity is 0.0 - 1.0; color alpha is 0-255
		unsigned char alpha = static_cast<unsigned char>(m_Opacity * 255.0f);
		DrawRectangle(0, 0, G_VARS.WIDTH, G_VARS.HEIGHT, {0, 0, 0, alpha});

	}

	EndDrawing();
}

void SceneManager::DrawUI(){
	// Draw Dialogue Menu
	if(G_VARS.IN_DIALOGUE){
		G_VARS.INVENTORY_OPEN = false;
		G_VARS.CHARACTER_PANEL = false;
		DrawDialogue();
	}

	// Draw Inventory
	if(G_VARS.INVENTORY_OPEN){
		DrawInventory();
		if(G_VARS.ITEM_SELECTED && Scene::m_SelectedItem != nullptr){
			DrawSelectedItem();
		}
	}

	if(Player::m_AxeEquipped){
		std::string AxeText = "Tier:" + std::to_string(Player::m_AxeTier) + " Axe Equipped";
		Vector2 AxeTextSize = MeasureTextEx(G_VARS.FONT, AxeText.c_str(), 32 * G_VARS.WIDTH_SCALE, G_VARS.FONT_SPACING);
		DrawTextEx(
			G_VARS.FONT,
			AxeText.c_str(),
			{G_VARS.WIDTH / 2.0f - AxeTextSize.x / 2.0f, 75.0f * G_VARS.HEIGHT_SCALE},
			32 * G_VARS.WIDTH_SCALE, G_VARS.FONT_SPACING, WHITE);
	}

// Player Resources
	// Heart Positions
	Vector2 heartPosition = {10 * G_VARS.WIDTH_SCALE, 15 * G_VARS.HEIGHT_SCALE};
	Vector2 maxHeartPosition = {10 * G_VARS.WIDTH_SCALE, 15 * G_VARS.HEIGHT_SCALE};

	// Star Positions
	Vector2 manaPosition = {25 * G_VARS.WIDTH_SCALE, 75 * G_VARS.HEIGHT_SCALE};
	Vector2 maxManaPosition = {25 * G_VARS.WIDTH_SCALE, 75 * G_VARS.HEIGHT_SCALE};

	// Draw empty hearts
	for(int H = 0; H < maxHearts; ++H){
		DrawTextureEx(
			m_EmptyHeart, 
			maxHeartPosition, 
			0.0f, 
			G_VARS.WIDTH_SCALE + G_VARS.HEIGHT_SCALE, 
			WHITE);
		maxHeartPosition.x += m_Heart.width * G_VARS.WIDTH_SCALE + 15.0f;
	}

	// Draw all the full hearts
	for(int h = 0; h < fullHearts; ++h){
		DrawTextureEx(
			m_Heart, 
			heartPosition,
			0.0f,
			G_VARS.WIDTH_SCALE + G_VARS.HEIGHT_SCALE,
			WHITE);
		heartPosition.x += m_Heart.width * G_VARS.WIDTH_SCALE + 15.0f;
	}

	// Draw half heart
	if(Scene::m_Player->m_OnHalfHeart){
		DrawTextureEx(
			m_HalfHeart,
			heartPosition,
			0.0f,
			G_VARS.WIDTH_SCALE + G_VARS.HEIGHT_SCALE,
			WHITE);
	}

// Player Mana Stars
	// Draw empty stars
	for(int M = 0; M < maxStars; ++M){
		DrawTextureEx(
			m_EmptyManaStar, 
			maxManaPosition, 
			0.0f,
			1.5 * G_VARS.WIDTH_SCALE,
			WHITE);
		maxManaPosition.x += m_ManaStar.width * G_VARS.WIDTH_SCALE + 15.0f;	
	}

	// Draw all the full stars
	for(int m = 0; m < fullStars; ++m){
		DrawTextureEx(
			m_ManaStar, 
			manaPosition,
			0.0f, 
			1.5 * G_VARS.WIDTH_SCALE,
			WHITE);
		manaPosition.x += m_ManaStar.width * G_VARS.WIDTH_SCALE + 15.0f;
	}

	// Draw half star
	if(Scene::m_Player->m_OnHalfStar){
		DrawTextureEx(
			m_HalfManaStar, 
			manaPosition, 
			0.0f,
			1.5 * G_VARS.WIDTH_SCALE,
			WHITE);
	}

// MiniMap
	DrawRectangle(G_VARS.WIDTH - 200 * G_VARS.WIDTH_SCALE, 0, 200 * G_VARS.WIDTH_SCALE, 150 * G_VARS.HEIGHT_SCALE, YELLOW);

// Current Active Quest
	Rectangle ActiveQuest = {
		0.0f,
		175 * G_VARS.HEIGHT_SCALE,
		200 * G_VARS.WIDTH_SCALE,
		85 * G_VARS.HEIGHT_SCALE
	};

	// TODO Make this a variable that we can acccess and change to update current quest objective
	DrawTextEx(
		G_VARS.FONT, 
		m_ActiveQuest.c_str(), 
		{ActiveQuest.x + 10 * G_VARS.WIDTH_SCALE, ActiveQuest.y + 5 * G_VARS.HEIGHT_SCALE},
		25 * G_VARS.WIDTH_SCALE, G_VARS.FONT_SPACING, WHITE);

// Action Bar 
	if(!G_VARS.IN_DIALOGUE){
		Rectangle ActionBar = {
			G_VARS.WIDTH / 2.0f - 80 * G_VARS.WIDTH_SCALE / 2.0f, 
			G_VARS.HEIGHT - 150 * G_VARS.HEIGHT_SCALE, 
			80 * G_VARS.WIDTH_SCALE, 
			95 * G_VARS.HEIGHT_SCALE
		};

		DrawTexturePro(
			m_ActionBarTex,
			{0.0f, 0.0f, (float)m_ActionBarTex.width, (float)m_ActionBarTex.height},
			ActionBar,
			{}, 0.0f, WHITE);

		Vector2 spellTexDimension = {50 * G_VARS.WIDTH_SCALE, 75 * G_VARS.HEIGHT_SCALE};

		DrawTexturePro(
			Scene::m_Player->m_SpellTexture,
			{0.0f, 0.0f, (float)Scene::m_Player->m_SpellTexture.width, (float)Scene::m_Player->m_SpellTexture.height},
			{ActionBar.x + ActionBar.width / 2.0f - spellTexDimension.x / 2.0f, 
			ActionBar.y + ActionBar.height / 2.0f - spellTexDimension.y / 2.0f, 
			spellTexDimension.x, 
			spellTexDimension.y},
			{}, 0.0f, WHITE);
	}

// Character Panel
	if(G_VARS.CHARACTER_PANEL && !G_VARS.IN_DIALOGUE){
		// Character Panel
		// This works for now on the laptop, check on desktop and change accordingly
		if(m_UpdateUI){
			if(G_VARS.FULLSCREEN){
				m_CharacterPanelX = G_VARS.WIDTH / 7.0f * G_VARS.WIDTH_SCALE - 200 * G_VARS.WIDTH_SCALE / 2.3f;		
			}else{
				m_CharacterPanelX = G_VARS.WIDTH / 7.0f * G_VARS.WIDTH_SCALE;
			}
			m_UpdateUI = false;
		}

		Rectangle CharacterPanel = {
			m_CharacterPanelX,
			G_VARS.HEIGHT - 300 * G_VARS.HEIGHT_SCALE, 
			200 * G_VARS.WIDTH_SCALE, 
			300 * G_VARS.HEIGHT_SCALE
		};

		DrawRectangleRec(CharacterPanel, GRAY);

		//Melee Weapon Slot
		Rectangle MeleeSlot = {
			CharacterPanel.x + 11 * G_VARS.WIDTH_SCALE,
			CharacterPanel.y + 5,
			CharacterPanel.width / 5,
			135 * G_VARS.HEIGHT_SCALE
		};

		DrawRectangleRec(MeleeSlot, DARKGRAY);

		// Wand Weapon Slot
		Rectangle WandSlot = {
			MeleeSlot.x + MeleeSlot.width + 2,
			CharacterPanel.y + 5,
			CharacterPanel.width / 5,
			135 * G_VARS.HEIGHT_SCALE
		};

		DrawRectangleRec(WandSlot, DARKGRAY);

		// Necklace Slot
		Rectangle NecklaceSlot = {
			WandSlot.x + WandSlot.width + 2,
			CharacterPanel.y + 5,
			CharacterPanel.width / 5,
			135 * G_VARS.HEIGHT_SCALE
		};

		DrawRectangleRec(NecklaceSlot, DARKGRAY);

		// Ring Slots
		Rectangle RingSlot1 = {
			NecklaceSlot.x + NecklaceSlot.width + 2,
			CharacterPanel.y + 5,
			CharacterPanel.width / 4,
			66 * G_VARS.HEIGHT_SCALE
		};

		Rectangle RingSlot2 = {
			NecklaceSlot.x + NecklaceSlot.width + 2,
			RingSlot1.y + RingSlot1.height + 2,
			CharacterPanel.width / 4,
			66 * G_VARS.HEIGHT_SCALE
		};

		DrawRectangleRec(RingSlot1, DARKGRAY);
		DrawRectangleRec(RingSlot2, DARKGRAY);

		float CharacterPanelStatY = CharacterPanel.y + CharacterPanel.height / 2.0f;
		float CharacterPanelStatX = CharacterPanel.x + 2;

		// Player Health 
		std::string playerHealth = std::format("Health: {:.2f}/{:.2f} Regen: {:.2f}",
		                                        Scene::m_Player->GetHealth(),
		                                        Scene::m_Player->GetMaxHealth(),
												Scene::m_Player->GetHealthRegen());
		DrawTextEx(G_VARS.FONT, playerHealth.c_str(), {CharacterPanelStatX, CharacterPanelStatY}, 18 * G_VARS.WIDTH_SCALE, 1.0f, WHITE);

		// Player Mana
		std::string playerMana = std::format("Mana: {:.2f}/{:.2f} Regen: {:.2f}",
												Scene::m_Player->GetMana(),
												Scene::m_Player->GetMaxMana(),
												Scene::m_Player->GetManaRegen());

		DrawTextEx(G_VARS.FONT, playerMana.c_str(), {CharacterPanelStatX, CharacterPanelStatY + 25 * G_VARS.HEIGHT_SCALE}, 18 * G_VARS.WIDTH_SCALE, 1.0f, WHITE);

		// Player Speed
		std::string playerSpeed = std::format("Speed: {:.2f}", 
												Scene::m_Player->GetSpeed());

		DrawTextEx(G_VARS.FONT, playerSpeed.c_str(), {CharacterPanelStatX, CharacterPanelStatY + 50 * G_VARS.HEIGHT_SCALE}, 18 * G_VARS.WIDTH_SCALE, 1.0f, WHITE);

		// Player Defense
		std::string playerDefense = std::format("Defense: {:.2f} : Dmg / {:.2f}", 
												Scene::m_Player->GetDefense(),
												Scene::m_Player->GetDefense() + 100 / 100);

		DrawTextEx(G_VARS.FONT, playerDefense.c_str(), {CharacterPanelStatX, CharacterPanelStatY + 75 * G_VARS.HEIGHT_SCALE}, 18 * G_VARS.WIDTH_SCALE, 1.0f, WHITE);
		
		// Player Damage
		std::string playerDamage = std::format("Damage: {:.2f}", 
												Scene::m_Player->GetDamage());

		DrawTextEx(G_VARS.FONT, playerDamage.c_str(), {CharacterPanelStatX, CharacterPanelStatY + 100 * G_VARS.HEIGHT_SCALE}, 18 * G_VARS.WIDTH_SCALE, 1.0f, WHITE);

		// Player Spell Damage
		std::string spellDamage = std::format("Spell Damage: {:.2f}",
												Scene::m_Player->GetSpellDamage());

		DrawTextEx(G_VARS.FONT, spellDamage.c_str(), {CharacterPanelStatX, CharacterPanelStatY + 125 * G_VARS.HEIGHT_SCALE}, 18 * G_VARS.WIDTH_SCALE, 1.0f, WHITE);
	}
}

void SceneManager::DrawDeadScreen(){
	DrawRectangle(0, 0, (float)G_VARS.WIDTH, (float)G_VARS.HEIGHT, { 0, 0, 0, 2 });
	std::string pausedText = "YOU DIED!";
	int textSize = MeasureText(pausedText.c_str(), 100);
	DrawText(pausedText.c_str(), (G_VARS.WIDTH / 2.0f) - (textSize / 2.0f), G_VARS.HEIGHT / 2.5f, 100, RED);

	if(IsKeyPressed(KEY_ENTER)){
		G_VARS.RUNNING = false;

	}
}

void SceneManager::DrawPauseScreen(){
	DrawRectangle(0, 0, (float)G_VARS.WIDTH, (float)G_VARS.HEIGHT, { 0, 0, 0, 2 });

	for( size_t i = 0; i < G_VARS.KEYBINDS.size(); ++i){
		DrawText(G_VARS.KEYBINDS[i].c_str(), 0, i * 15, 15, WHITE); 
	}

	if(IsKeyPressed(KEY_ENTER)){
		G_VARS.RUNNING = false;

	}
}

void SceneManager::DrawDialogue(){
	// Size Variables

	// TODO Make states so that all of this code only runs once
	// When progressing dialogue, call an update to draw new text once
	// Small optimizations like this across the program can be used honestly
	float npcHeadshotWidth = 120.0f * G_VARS.WIDTH_SCALE;
	float npcHeadshotHeight = 90.0f * G_VARS.WIDTH_SCALE;
	float dialogueWindowHeight = 150.0f * G_VARS.HEIGHT_SCALE;

	// Rectangles
	Rectangle dialogueWindow = {0, (float)G_VARS.HEIGHT - dialogueWindowHeight, (float)G_VARS.WIDTH, dialogueWindowHeight};
	// Dialogue Headshot
	Rectangle dest = {
		(float)G_VARS.WIDTH - npcHeadshotWidth,
		(float)G_VARS.HEIGHT - (npcHeadshotHeight + dialogueWindowHeight),
		npcHeadshotWidth,
		npcHeadshotHeight

	};

	// Draw Recs
	DrawTexturePro(
		m_DialogueTex, 
		{0, 0, (float)m_DialogueTex.width, (float)m_DialogueTex.height},
		dialogueWindow,
		{},
		0.0f,
		WHITE);

	// W.I.P Npc Headshot
	DrawTexturePro(
		Scene::npcInDialogue->GetDialogueTexture(), 
		{(float)Scene::npcInDialogue->GetDialogueTexture().width, 
			0.0f, 
			-1.0f * (float)Scene::npcInDialogue->GetDialogueTexture().width, 
			(float)Scene::npcInDialogue->GetDialogueTexture().height}, 
		dest, 
		{}, 0.0f, WHITE);

	// Temp Text Size
	Vector2 escapeSize = MeasureTextEx(G_VARS.FONT, "'ESCAPE' To Leave Dialogue", 15 * G_VARS.WIDTH_SCALE, 1.0f);
	Vector2 npcTextSize = MeasureTextEx(G_VARS.FONT, Scene::npcInDialogue->GetDialogue().c_str(), 32 * G_VARS.WIDTH_SCALE, G_VARS.FONT_SPACING);

	// Text
	DrawTextEx(G_VARS.FONT, "<< 'Z'", {45 * G_VARS.WIDTH_SCALE, dialogueWindow.y + 15 * G_VARS.HEIGHT_SCALE}, 15 * G_VARS.WIDTH_SCALE, 1.0f, WHITE);
	DrawTextEx(G_VARS.FONT, "'SPACE' >>", {125 * G_VARS.WIDTH_SCALE, dialogueWindow.y + 15 * G_VARS.HEIGHT_SCALE}, 15 * G_VARS.WIDTH_SCALE, 1.0f, WHITE);
	DrawTextEx(G_VARS.FONT, "'ESCAPE' To Leave Dialogue", {(float)G_VARS.WIDTH - escapeSize.x - 45 * G_VARS.WIDTH_SCALE, dialogueWindow.y + 15 * G_VARS.HEIGHT_SCALE}, 15 * G_VARS.WIDTH_SCALE, 1.0f, WHITE);
	DrawTextEx(G_VARS.FONT, Scene::npcInDialogue->GetDialogue().c_str(), {(float)G_VARS.WIDTH / 2.0f - npcTextSize.x / 2.0f, dialogueWindow.y + dialogueWindow.height / 2.5f}, 32 * G_VARS.WIDTH_SCALE, G_VARS.FONT_SPACING, WHITE);


	// Input Listening
	if(IsKeyPressed(KEY_SPACE)){
		// TODO Create a check so that if youre on the last line of dialogue
		// Exit out of dialogue
		// Else progress it
		Scene::npcInDialogue->ProgressDialogue();
	}

	if(IsKeyPressed(KEY_Z)){
		Scene::npcInDialogue->StepBackDialogue();
	}
}

// Inventory Screen
void SceneManager::DrawInventory(){
	Rectangle InventoryDest = {
		(float)G_VARS.WIDTH / 1.46f, 
		(float)G_VARS.HEIGHT - 300 * G_VARS.HEIGHT_SCALE, 
		200 * G_VARS.WIDTH_SCALE,
		300 * G_VARS.HEIGHT_SCALE
	};

	DrawTexturePro(
		m_InventoryTex, 
		{0.0f, 0.0f, (float)m_InventoryTex.width, (float)m_InventoryTex.height}, 
		InventoryDest, {}, 0.0f, WHITE);

	float itemWidth = 45 * G_VARS.WIDTH_SCALE;
	float itemHeight = 50 * G_VARS.HEIGHT_SCALE;
	int posX = InventoryDest.x + 20 * G_VARS.WIDTH_SCALE;
	int posY = InventoryDest.y + 25 * G_VARS.HEIGHT_SCALE;
	int offSetX = 55;
	int offSetY = 65;
	int rowCount = 1;
	//
	for( auto& [item, itemCount] : Scene::m_Player->GetInventory()){
		if(item != nullptr){
			item.get()->SetRect() = {
				(float)posX, 
				(float)posY,
				itemWidth,
				itemHeight
			};

			// I want to be able to mouse over items and draw the border around them...
			// Shouldn't be too hard tbh
			// Create a rectangle around the mouse in the 2d space
			// Check for collision in update probably when inventory is open
			// Set a state for drawing this item selected rectangle
			DrawRectangleRec(item.get()->GetRect(), GRAY);

			DrawTexturePro(
				item.get()->GetTexture(), 
				{0.0f, 0.0f, (float)item.get()->GetTexture().width, (float)item.get()->GetTexture().height}, 
				item.get()->GetRect(), {}, 0.0f, WHITE);

			// TODO Fix Text Count Positioning
			// Fixed for now, anything in double digits fucks it up again, either cap stack size or account for it
			DrawText(std::to_string(itemCount).c_str(), 
					item.get()->GetRect().x + item.get()->GetRect().width - 8 * G_VARS.WIDTH_SCALE, 
					item.get()->GetRect().y + item.get()->GetRect().height - 20 * G_VARS.HEIGHT_SCALE, 
					15 * G_VARS.WIDTH_SCALE, WHITE);

			if(rowCount >= 3){
				posX = InventoryDest.x + 20 * G_VARS.WIDTH_SCALE;
				posY += offSetY * G_VARS.HEIGHT_SCALE;
				rowCount = 1;
			}else{
				++rowCount;	
				posX += offSetX * G_VARS.WIDTH_SCALE;
			}
		}else{
			std::cout << "Something wrong with the item..." << std::endl;
		}
	}
	if(G_VARS.ITEM_SELECTED && Scene::m_SelectedItem != nullptr){
		DrawRectangleLines(Scene::m_SelectedItem->GetRect().x,
							Scene::m_SelectedItem->GetRect().y,
							Scene::m_SelectedItem->GetRect().width,
							Scene::m_SelectedItem->GetRect().height,
							YELLOW);
	}
}

void SceneManager::DrawSelectedItem(){
	// Item Card
	Rectangle ItemCardDest = {
		// G_VARS.WIDTH / 5.3f * G_VARS.WIDTH_SCALE,
		G_VARS.WIDTH / 2.0f - ((350 * G_VARS.WIDTH_SCALE) / 2.0f),
		G_VARS.HEIGHT / 1.8f,
		350 * G_VARS.WIDTH_SCALE,
		250 * G_VARS.HEIGHT_SCALE
	};

	DrawTexturePro(
		m_ItemCardTex, 
		{0.0f, 0.0f, (float)m_ItemCardTex.width, (float)m_ItemCardTex.height}, 
		ItemCardDest, 
		{}, 0.0f, WHITE);

	// Item Name
	Vector2 nameTextSize = MeasureTextEx(G_VARS.FONT, Scene::m_SelectedItem->GetItemName().c_str(), 16 * G_VARS.WIDTH_SCALE + G_VARS.HEIGHT_SCALE, 1.0f);
	float nameXPos = ItemCardDest.x + ItemCardDest.width / 2.0f - nameTextSize.x / 2.0f;

	DrawTextEx(G_VARS.FONT, Scene::m_SelectedItem->GetItemName().c_str(), {nameXPos, ItemCardDest.y + 10 * G_VARS.HEIGHT_SCALE}, 18 * G_VARS.WIDTH_SCALE + G_VARS.HEIGHT_SCALE, 1.0f, WHITE);

	// Item Texture
	float itemWidth = 100 * G_VARS.WIDTH_SCALE;
	float itemHeight = 100 * G_VARS.HEIGHT_SCALE;
	float posX = ItemCardDest.x + ItemCardDest.width / 2.0f - itemWidth / 2.0f;
	float posY = ItemCardDest.y + 35 * G_VARS.HEIGHT_SCALE;

	Rectangle dest = {
		posX,
		posY,
		itemWidth,
		itemHeight
	};

	DrawTexturePro(
		Scene::m_SelectedItem->GetTexture(), 
		{0.0f, 0.0f, (float)Scene::m_SelectedItem->GetTexture().width, (float)Scene::m_SelectedItem->GetTexture().height}, 
		dest, 
		{}, 1.0f, WHITE);

	// Item Description
	Vector2 descriptionTextSize = MeasureTextEx(G_VARS.FONT, Scene::m_SelectedItem->GetItemDescription().c_str(), 25 * G_VARS.WIDTH_SCALE + G_VARS.HEIGHT_SCALE, 1.0f);
	float descriptionXPos = ItemCardDest.x + ItemCardDest.width / 2.0f - descriptionTextSize.x / 2.0f;
	DrawTextEx(G_VARS.FONT, Scene::m_SelectedItem->GetItemDescription().c_str(), {descriptionXPos, dest.y + dest.height + 20}, 25 * G_VARS.WIDTH_SCALE + G_VARS.HEIGHT_SCALE, 1.0f, WHITE);
}
