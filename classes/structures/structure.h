#pragma once
#include "raylib.h"
#include "raymath.h"
#include "classes/utilities/globalVariables.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

// Maybe pass size by default in derived classes? 
// Will be able to create variance in sizes as well as have default ones for different types
// Spruce could be bigger and could remove sizes from g_vars

// TODO Maybe fine tune building colliders OR pass in size inside of derived class like trees
// Try passing in colliderMin and colliderMax in constructor and assigning it by default?

class Structure{
	public:
		Structure(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs& id) 	
			: m_Pos(pos), m_Size(size), m_ID(id){

				}
		virtual ~Structure(){}
		virtual void Update(float& dT);
		virtual void Draw(Camera3D& camera);
		BoundingBox& GetCollider();
		BoundingBox& GetCurrentLeavesCollider();
		BoundingBox& GetInteractCollider();
		float& GetHealth();
		float& GetMaxHealth();
		bool& GetState();
		bool& HasCollider();
		bool& IsDepleted();
		Vector3& GetPos();
		GlobalVariables::StructureIDs& GetID();
		void TakeDamage(float damage);
		int& GetTier();

	protected:
		Vector3 m_Pos;
		Vector2& m_Size;
		Texture2D m_Texture;
		// Collision
		Vector3 m_ColliderMin;
		Vector3 m_ColliderMax;
		BoundingBox m_Collider{};
		// Leaves
		Vector3 m_LeavesColliderMin;
		Vector3 m_LeavesColliderMax;
		BoundingBox m_LeavesCollider{};
		// Current Leaves Collider
		BoundingBox m_CurrentLeavesCollider{};
		// Empty Leaves Collider
		BoundingBox m_EmptyLeavesCollider{};
		// Interact
		Vector3 m_InteractColliderMin;
		Vector3 m_InteractColliderMax;
		BoundingBox m_InteractCollider{};
		GlobalVariables::StructureIDs m_ID;
		int m_Tier;
		float m_Health;
		float m_MaxHealth;
		bool m_HasCollider = true;
		bool m_Active = true;
		bool m_Depleted = false;
		bool m_HasAnimation = false;
		bool m_UpdateCollider = false;
		// Temp Text Display
		bool m_InteractedWith = false;
		std::string m_Text{};
		float m_CurrentTime{};
		float m_PopupTime = 1.5f;
		unsigned char m_TextOpacity {255};
		Vector2 m_TextPosition;
		//
		int m_CurrentFrame{};
		int m_MaxFrames = 2;
		float m_UpdateTime = 1.0 / 4.0;
		float m_RunningTime{};
		float m_Facing = 1.0f;
		//
		static inline float m_BaseTreeHealth = 30.0f;

	public:
		Color m_Color = WHITE;

};