#pragma once
#include "classes/entities/entity.h"
#include "classes/player/player.h"

class Enemy : public Entity{
	public:
		Enemy(const Texture2D& texture, Vector2& size, Vector3& position, float facing, bool hasCollider, int& id, float hp, float speed, float aggroRange, float damage) 
		: Entity(texture, size, position, facing, hasCollider, id, 4, hp), m_Speed(speed), m_AggroRange(aggroRange), m_LastPos(position), m_SpawnPos(position), m_Damage(damage){
			
		} 
		~Enemy(){
			std::cout << "Enemy Deleted!" << std::endl;
		}
		void Draw(Camera3D& camera) override;
		void Update(float& dT) override;
		static void SetTarget(Player* player);
		void Attack();
		void TakeDamage(float damage);
		void PathFind(float& dT);
		void ReturnToSpawn(float& dT);
		Vector3& GetLastPos();
	private:
		float m_Speed;
		float m_AggroRange;
		Vector3 m_LastPos;
		Vector3 m_SpawnPos;
		Vector2 m_Direction;
		float m_Damage {1.0f};
		float m_AttackCooldown {1.0f};
		float m_AttackTimer{};
		float m_FlashTimer{};
		float m_FlashMaxTime {0.4f};
		float m_CombatTimer{};
		float m_CombatMaxTime {2.5f};
		bool m_TookDamage = false;
		bool m_InCombat = false;
		bool m_AtSpawn = false;
		static inline Player* m_Player = nullptr;
		
};

