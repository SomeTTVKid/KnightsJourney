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
		//
		static inline Texture2D m_ItemCardTex; 
		static inline Texture2D m_InventoryTex;
		static inline Texture2D m_DialogueTex;
		static inline Texture2D m_ActionBarTex;
		
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
		bool m_Transitioning = false; 
		bool m_PendingPause = false;
		bool m_UpdateUI = false;
		// 0 = fade-out
		// 1 = fade-in
		int m_TransitionPhase = 0; 
		float m_TransitionTimer = 0.0f;
		float m_TransitionMaxTime = 0.6f; 
		float m_Opacity = 0.0f; 
		float m_PauseTimer = 0.0f;
		float m_PauseTimerMax = 0.1f;
		std::string frametime;
		//
		// Might change this later down the line, no clue if performance loss from doing things this way
		float m_CharacterPanelX = G_VARS.WIDTH / 7.0f * G_VARS.WIDTH_SCALE;
		//
		Texture2D m_Heart = LoadTexture("classes/utilities/artwork/singleHeart.png");
		Texture2D m_HalfHeart = LoadTexture("classes/utilities/artwork/halfHeart.png");
		Texture2D m_EmptyHeart = LoadTexture("classes/utilities/artwork/emptyHeart.png");
		Texture2D m_ManaStar = LoadTexture("classes/utilities/artwork/manaStar.png");
		Texture2D m_HalfManaStar = LoadTexture("classes/utilities/artwork/halfManaStar.png");
		Texture2D m_EmptyManaStar = LoadTexture("classes/utilities/artwork/emptyManaStar.png");
		//
};
