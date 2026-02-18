#pragma once
#include "classes/entities/entity.h"
#include "classes/items/item.h"
#include <string>
#include <vector>
#include <memory>

class Player : public Entity{
	public:
		Player(const Texture2D& texture, Vector2& size, Vector3& position, float facing, bool hasCollider, int& id) 
		: Entity(texture, size, position, facing, hasCollider, id), m_LastPos(position), m_SpawnPoint(position){
			m_Idle = m_Texture;
			m_Health = 10.0f;
		} 
		~Player(){}
		void Draw(Camera3D& camera) override;
		void checkWorldBounds();
		void Update(float& dT) override;
		void TakeDamage(float damage);
		void AddToInventory(std::shared_ptr<Item>& item);
		void RemoveFromInventory(std::shared_ptr<Item>& item);
		void UseItem(std::shared_ptr<Item>& item);
		void UseConsumable(std::shared_ptr<Item>& item);
		void EquipEquipment(std::shared_ptr<Item>& item);
		const std::map<std::shared_ptr<Item>, int>& GetInventory() const;
		// Getters for equipment
		Item* GetMeleeWeapon() const;
		Item* GetWandWeapon() const;
		Item* GetNecklaceSlot() const;
		Item* GetRing01Slot() const;
		Item* GetRing02Slot() const;
		Vector2& GetWorldSize();
		Vector3& GetLastPos();
		Vector3& GetSpawnPoint();
		float& GetHealth() override;
		float& GetMana();
		float& GetMaxHealth();
		float& GetMaxMana();
		float& GetHealthRegen();
		float& GetManaRegen();
		float& GetDamage();
		float& GetFacing();
		float& GetDefense();
		float& GetSpeed();
		float& GetSpellDamage();
		size_t& GetInventoryMaxSize();
		//
		Texture2D m_SpellTexture = LoadTexture("classes/projectiles/artwork/fireball_image.png");
		//
		const float m_HealthPerHeart = 1.0f;
		const float m_ManaPerStar = 1.0f;
		//
		static inline bool m_OnHalfHeart = false;
		static inline bool m_OnHalfStar = false;
		//
		static inline bool m_AxeEquipped = false;
		static inline int m_AxeTier = 1;
		//
		static inline bool m_ToolSwung = false;
		// 
		static inline Sound m_InventoryOpen;
		static inline Sound m_InventoryClose;
		static inline Sound m_WalkingSound;
		static inline Sound m_RunningSound;
	private:	
		Vector2 worldSize;
		Vector3 m_LastPos;
		Vector3 m_SpawnPoint;
		Texture2D m_Idle;
		Texture2D m_Running = LoadTexture("classes/player/artwork/player_run.png");
		// Player Stats
		int m_BaseSpeed {1};
		float m_MaxHealth = 10.0f;
		float m_MaxMana = 5.0f;
		float m_Mana = m_MaxMana;
		float m_HealthRegen = 0.05f;
		float m_ManaRegen = 0.02f;
		float m_CurrentSpeed{};
		float m_SprintSpeed {1.5f};
		float m_Damage {1.0f};
		float m_SpellDamage {2.0f};
		float m_SpellCost{};
		float m_Defense{};
		size_t m_MaxInventorySize = 12;
		// Spell settings
		int m_SelectedSpell = 0;
		bool m_Casted = false;
		bool m_TookDamage = false;
		bool m_Sprinting = false;
		bool m_Walking = false;
		float m_SpellCooldown{};
		float m_CastTimer{};
		// Tool Cooldown
		float m_SwingCooldown{};
		float m_MaxSwingCooldown { 1.0f };
		// Taking damage
		float m_FlashTimer{};
		float m_FlashMaxTime {0.4f};
		// Walk audio
		float m_WalkSoundTimer{};
		float m_WalkSoundMax {2.5f};
		//
		std::map<std::shared_ptr<Item>, int> m_Inventory{};
		std::shared_ptr<Item> m_MeleeSlot = nullptr;
		std::shared_ptr<Item> m_WandSlot = nullptr;
		std::shared_ptr<Item> m_NecklaceSlot = nullptr;
		std::shared_ptr<Item> m_Ring01Slot = nullptr;
		std::shared_ptr<Item> m_Ring02Slot = nullptr;

};
