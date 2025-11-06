#pragma once
#include "raylib.h"
#include "classes/projectiles/projectile.h"
#include "classes/enemies/enemy.h"
#include "classes/items/item.h"
#include "classes/npcs/npc.h"
#include "classes/structures/structure.h"
#include <vector>
#include <variant>
#include <memory>

class Player;
class Entity;
class LevelLoader;

using DrawableVariant = std::variant<Entity*, Projectile*, Npc*, LevelLoader*, Enemy*, Item*, Structure*>;

class Scene{
	public:
		virtual void Load();
		virtual void Update(float& dT);
		virtual void Draw();
		virtual void Unload();
		virtual ~Scene() = default;

		void BuildFrame();
		void SetInteractText();
		void DisplayInteractText();
		//
		static inline Camera3D m_Camera = {
			{ 0.0f, 1.0f, 10.0f }, 	// Position
			{ 0.0f, 0.0f, 0.0f },	// Target
			{ 0.0f, 1.0f, 0.0f },	// Up
			35.0f,					// Fovy
			CAMERA_PERSPECTIVE		// Projection
		};
		//
		static inline Player* m_Player = nullptr;
		static void SetPlayer(Player* Player);
		//
		static inline Npc* npcInDialogue = nullptr;
		static inline Item* m_SelectedItem = nullptr;
		//
		static inline std::vector<DrawableVariant> m_DrawList;
		static inline std::vector<Entity*> m_Entities;
		static inline std::vector<LevelLoader*> m_LevelLoaders;
		static inline std::vector<std::unique_ptr<Npc>> m_Npcs;
		static inline std::vector<std::unique_ptr<Projectile>> m_Projectiles;
		static inline std::vector<std::unique_ptr<Enemy>> m_Enemies;
		static inline std::vector<std::unique_ptr<Item>> m_Items;
		static inline std::vector<std::unique_ptr<Structure>> m_Structures;
		//
		static inline Sound m_PickUpAudio;
		static inline Sound m_LevelEnterAudio;
		static inline Sound m_LevelExitAudio;
		//
		static inline Music m_LevelMusic;
		//

		Ray m_OpacityRay = { 0 };
		RayCollision m_RayCollision = { 0 };
	protected:
		Vector2 worldSpace;
		float DepthTimer{};

};
