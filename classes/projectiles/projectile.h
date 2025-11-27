#pragma once
#include "raylib.h"
#include "raymath.h"
#include "classes/player/player.h"
#include "classes/utilities/globalVariables.h"

// KEEP IN MIND: Future spells must have static member variable to access mana cost...sorry future me :P

class Projectile{
	public:
		Projectile(Vector3 pos, Vector2 mousePos) 
			: m_Position(pos), m_MousePos(mousePos){
				if(mousePos.y > G_VARS.HEIGHT / 2.0f){
					m_Position.z += 0.1f;
				}else{
					m_Position.z -= 0.1f;
				}
				m_Position.x += 0.25f;

		}
		virtual ~Projectile(){
			std::cout << "Projectile Deleted!" << std::endl;
			PlaySound(m_HitSound);
		};
		void Update(float& dT);
		void Draw(Camera3D& camera);
		Vector2& GetVector();
		Vector3& GetPos();
		bool& GetState();
		BoundingBox& GetCollider();
		Texture2D& GetTexture();
		const float& ManaCost() const;
		const float& GetDamage() const;
		const float& SpellCooldown() const;
		//
		static inline Player* m_Player = nullptr;
		static void SetPlayer(Player* Player);
		//
		Sound m_CastingSound{};
		Sound m_HitSound{};
	protected:
		Vector3 m_Position;
		Vector2 m_MousePos;
		Vector2 m_Direction;
		Vector2 m_Size{};
		BoundingBox m_Collider;
		Texture2D m_Texture;
		Color m_Color = WHITE;
		bool m_Alive = true;
		int m_Frame{};
		int m_MaxFrames { 4 };
		float m_LifeTime{};
		float m_CoolDown{};
		float m_Timer { 0.0f };
		float m_Speed { 1.0f };
		float m_Damage { 1.0f };
		float m_UpdateTime = 1.0 / 4.0;
		float m_RunningTime{};
		float m_ManaCost{};
		
};

class Fireball : public Projectile{
	public:
		Fireball(Vector3 pos, Vector2 mousePos) 
			: Projectile(pos, mousePos){
				m_Size = m_FireballSize;
				m_Texture = m_Fireball;
				m_LifeTime = 2.0f; 
				m_Speed = 2.5f; 
				m_Damage = 2.0f;
				m_Color = WHITE;
				m_ManaCost = m_SpellCost;
				m_CoolDown = 1.0f;
				m_CastingSound = m_FireballSound;
				m_HitSound = m_FireballExplosionSound;
		}
		~Fireball(){}
		
		static inline Sound m_FireballSound;
		static inline Sound m_FireballExplosionSound;
		static inline Texture2D m_Fireball;

		static inline Vector2 m_FireballSize = { 0.65f, 0.65f };
		static inline float m_SpellCost = 1.0f;

};

class FrostOrb : public Projectile{
	public:
		FrostOrb(Vector3 pos, Vector2 mousePos) 
			: Projectile(pos, mousePos){
				m_Size = m_FrostOrbSize;
				m_Texture = m_FrostOrb;
				m_LifeTime = 3.5f; 
				m_Speed = 1.2f; 
				m_Damage = 2.5f;
				m_Color = WHITE;
				m_ManaCost = m_SpellCost;
				m_CoolDown = 2.5f;
				m_CastingSound = m_FrostOrbSound;
				m_HitSound = m_FrostOrbHitSound;
		}
		~FrostOrb(){}
		
		static inline Sound m_FrostOrbSound;
		static inline Sound m_FrostOrbHitSound;
		static inline Texture2D m_FrostOrb;

		static inline Vector2 m_FrostOrbSize = { 0.75f, 0.75f };
		static inline float m_SpellCost = 1.5f;

};

class LightningBolt : public Projectile{
	public:
		LightningBolt(Vector3 pos, Vector2 mousePos) 
			: Projectile(pos, mousePos){
				m_Size = m_LightningBoltSize;
				m_Texture = m_LightningBolt;
				m_LifeTime = 1.0f;
				m_Speed = 3.5f;
				m_Damage = 1.5f;
				m_Color = YELLOW;
				m_ManaCost = m_SpellCost;
				m_CoolDown = 0.5f;
				m_CastingSound = m_LightningBoltSound;
				m_HitSound = m_LightningBoltZapSound;
			}
			~LightningBolt(){}

			static inline Sound m_LightningBoltSound;
			static inline Sound m_LightningBoltZapSound;
			static inline Texture2D m_LightningBolt;

			static inline Vector2 m_LightningBoltSize = { 0.55f, 0.55f };
			static inline float m_SpellCost = 0.5f;

};
