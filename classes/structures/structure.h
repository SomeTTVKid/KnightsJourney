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

class Structure{
	public:
		Structure(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs& id) 	
			: m_Pos(pos), m_Size(size), m_ID(id){
					if(m_HasCollider){
						switch(m_ID){
							case GlobalVariables::StructureIDs::TREE_ID:
								// Length, Height, Width
								m_ColliderMin = { m_Pos.x + m_Size.x / 3.0f, m_Pos.y, m_Pos.z - m_Size.x / 8.0f};
								m_ColliderMax = { m_Pos.x + m_Size.x / 1.7f, m_Pos.y + m_Size.y, m_Pos.z + m_Size.x / 20.0f};
								m_Collider = { m_ColliderMin, m_ColliderMax };
								break;
							
							case GlobalVariables::StructureIDs::BUILDING_ID: 
								std::cout << "Building Collider Built" << std::endl;
								m_ColliderMin = { m_Pos.x + m_Size.x / 4.5f, m_Pos.y, m_Pos.z - m_Size.x / 6.0f};
								m_ColliderMax = { m_Pos.x + m_Size.x - m_Size.x / 4.0f, m_Pos.y + m_Size.y, m_Pos.z + m_Size.x / 20.0f};
								m_Collider = { m_ColliderMin, m_ColliderMax };
								break;

							case GlobalVariables::StructureIDs::GRASS_ID: break;

							case GlobalVariables::StructureIDs::ORE_ID: break;

							default: std::cout << "Error in Structure Constructor" << std::endl; break;
						}
						// Vector3 colliderMin = { m_Pos.x + m_Size.x / 4.0f, m_Pos.y, m_Pos.z - m_Size.x / 6.0f};
						// Vector3 colliderMax = { m_Pos.x + m_Size.x - m_Size.x / 4.0f, m_Pos.y + m_Size.y, m_Pos.z + m_Size.x / 20.0f};
						// m_Collider = { colliderMin, colliderMax };
					}
				}
		virtual ~Structure(){}
		virtual void Update(float& dT);
		virtual void Draw(Camera3D& camera);
		BoundingBox& GetCollider();
		float& GetHealth();
		bool& GetState();
		bool& HasCollider();
		Vector3& GetPos();
		GlobalVariables::StructureIDs& GetID();
		int& SetFrame();
		void TakeDamage(float damage, size_t& toolTier);

	protected:
		Vector3 m_Pos;
		Vector2& m_Size;
		Texture2D m_Texture;
		Vector3 m_ColliderMin;
		Vector3 m_ColliderMax;
		BoundingBox m_Collider{};
		GlobalVariables::StructureIDs m_ID;
		size_t m_Tier;
		float m_Health;
		bool m_HasCollider = true;
		bool m_Active = true;
		bool m_HasAnimation = false;
		bool m_UpdateCollider = false;
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