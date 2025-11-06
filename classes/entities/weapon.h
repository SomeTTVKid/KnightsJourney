#pragma once
#include "raylib.h"
#include "raymath.h"
#include "entity.h"
#include "classes/player/player.h"

// TODO add in a weapon class

class Weapon : public Entity{
	public:
		static inline bool m_DrawColliders = false;
		float m_Damage;
	public:
		Weapon(const Texture2D& weaponTex, Vector2& weaponSize, Vector3& weaponPos, float facing, bool hasCollider, int& id, float weaponDamage) :
			Entity(weaponTex, weaponSize, weaponPos, facing, hasCollider, id), m_Damage(weaponDamage){}
		static void SetPlayer(Player* player);
		void Draw(Camera3D& camera) override;
	private:
		float m_WeaponOffset {0.5f};
		static inline Player* m_Player = nullptr;

};
