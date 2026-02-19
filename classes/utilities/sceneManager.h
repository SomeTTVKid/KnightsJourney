#pragma once
#include "classes/scenes/scene.h"
#include "classes/entities/entity.h"
#include <algorithm>
#include <vector>

class Enemy;
class Npc;

class SceneManager{
	public:
		// Delete copy & move assignment and constructors
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

		static SceneManager& GetInstance();
		static int& GetSceneID();

		static void DepthBuffer(std::vector<DrawableVariant>& drawList);

		void RequestSceneChange(int sceneID);
		void ChangeScene(int sceneID);
		void Update(float& dT);
		void DrawPauseScreen();
		void DrawUI();
		void DrawDeadScreen();
		void Draw();
		void DrawDialogue();
		void DrawSelectedItem();
		void DrawInventory();
		void DrawHeartsAndMana();
		void PopupText(float timeToDisplay, float& dT, std::string text, Vector3& position);
		void ScaleUI();
		void DrawCharacterPanelRectangles();
		std::string& SetActiveQuest();
		Scene* GetCurrentScene();
		// Equipment slots
		Rectangle& GetMeleeSlotRec();
		Rectangle& GetWandSlotRec();
		Rectangle& GetNecklaceSlotRec();
		Rectangle& GetRing01SlotRec();
		Rectangle& GetRing02SlotRec();
		//
		static inline Texture2D m_ItemCardTex; 
		static inline Texture2D m_InventoryTex;
		static inline Texture2D m_DialogueTex;
		static inline Texture2D m_ActionBarTex;
		//
		bool m_Transitioning = false; 
		// 0 = fade-out
		// 1 = fade-in
		int m_TransitionPhase = 0; 
		
	private:
		SceneManager() = default;
		static SceneManager m_Instance;
		static inline Scene* m_CurrentScene = nullptr;
		int m_PendingScene = -1;
		int fullHearts{};
		int maxHearts{};
		int fullStars{};
		int maxStars{};
		static inline int m_CurrentSceneID = -1;
		float m_TransitionTimer = 0.0f;
		float m_TransitionMaxTime = 0.6f; 
		float m_Opacity = 0.0f; 
		//
		bool m_PendingPause = false;
		float m_PauseTimer = 0.0f;
		float m_PauseTimerMax = 0.1f;
		bool m_UpdateUI = false;
		std::string frametime;
		std::string m_ActiveQuest {"Save the BlackSmith from the Goblins!"};
		//
	//

	// Character Panel Rectangles
		// Character Panel
		Rectangle m_CharacterPanel{
			G_VARS.WIDTH / 7.0f * G_VARS.WIDTH_SCALE,
			G_VARS.HEIGHT - 300 * G_VARS.HEIGHT_SCALE, 
			200 * G_VARS.WIDTH_SCALE, 
			300 * G_VARS.HEIGHT_SCALE
		};

		// Melee Weapon Slot
		Rectangle m_MeleeSlot{
			m_CharacterPanel.x + 11 * G_VARS.WIDTH_SCALE,
			m_CharacterPanel.y + 5,
			m_CharacterPanel.width / 5,
			135 * G_VARS.HEIGHT_SCALE
		};

		// Wand Slot
		Rectangle m_WandSlot{
			m_MeleeSlot.x + m_MeleeSlot.width + 2,
			m_CharacterPanel.y + 5,
			m_CharacterPanel.width / 5,
			135 * G_VARS.HEIGHT_SCALE
		};

		// Necklace Slot
		Rectangle m_NecklaceSlot{
			m_WandSlot.x + m_WandSlot.width + 2,
			m_CharacterPanel.y + 5,
			m_CharacterPanel.width / 5,
			135 * G_VARS.HEIGHT_SCALE
		};

		// Ring01 Slot
		Rectangle m_Ring01Slot{
			m_NecklaceSlot.x + m_NecklaceSlot.width + 2,
			m_CharacterPanel.y + 5,
			m_CharacterPanel.width / 4,
			66 * G_VARS.HEIGHT_SCALE
		};

		// Ring02 Slot
		Rectangle m_Ring02Slot{
			m_NecklaceSlot.x + m_NecklaceSlot.width + 2,
			m_Ring01Slot.y + m_Ring01Slot.height + 2,
			m_CharacterPanel.width / 4,
			66 * G_VARS.HEIGHT_SCALE
		};

		// Action Bar
		Rectangle m_ActionBar = {
			G_VARS.WIDTH / 2.0f - 80 * G_VARS.WIDTH_SCALE / 2.0f, 
			G_VARS.HEIGHT - 150 * G_VARS.HEIGHT_SCALE, 
			80 * G_VARS.WIDTH_SCALE, 
			95 * G_VARS.HEIGHT_SCALE
		};

		Vector2 m_SpellTexDimension = {50 * G_VARS.WIDTH_SCALE, 75 * G_VARS.HEIGHT_SCALE};

		// Active Quest
		Rectangle m_ActiveQuestRec = {
		0.0f,
		175 * G_VARS.HEIGHT_SCALE,
		200 * G_VARS.WIDTH_SCALE,
		85 * G_VARS.HEIGHT_SCALE
		};

		// Inventory
		Rectangle m_InventoryDest = {
			(float)G_VARS.WIDTH / 1.46f, 
			(float)G_VARS.HEIGHT - 300 * G_VARS.HEIGHT_SCALE, 
			200 * G_VARS.WIDTH_SCALE,
			300 * G_VARS.HEIGHT_SCALE
		};

		// Item Card
		Rectangle m_ItemCardDest = {
			G_VARS.WIDTH / 2.0f - ((350 * G_VARS.WIDTH_SCALE) / 2.0f),
			G_VARS.HEIGHT / 1.8f,
			350 * G_VARS.WIDTH_SCALE,
			250 * G_VARS.HEIGHT_SCALE
		};

		//
		Texture2D m_Heart = LoadTexture("classes/utilities/artwork/ui/singleHeart.png");
		Texture2D m_HalfHeart = LoadTexture("classes/utilities/artwork/ui/halfHeart.png");
		Texture2D m_EmptyHeart = LoadTexture("classes/utilities/artwork/ui/emptyHeart.png");
		Texture2D m_ManaStar = LoadTexture("classes/utilities/artwork/ui/manaStar.png");
		Texture2D m_HalfManaStar = LoadTexture("classes/utilities/artwork/ui/halfManaStar.png");
		Texture2D m_EmptyManaStar = LoadTexture("classes/utilities/artwork/ui/emptyManaStar.png");
		//
};
