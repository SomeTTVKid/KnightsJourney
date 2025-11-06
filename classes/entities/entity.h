#pragma once
#include "raylib.h"
#include "raymath.h"
#include "classes/utilities/globalVariables.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

class Enemy;

class Entity{
	public:
		Entity(const Texture2D& texture, Vector2& size, Vector3 position, float facing, bool hasCollider,  int& entityID, 
			int maxFrames = 4, float health = 1, bool alive = true) 
		: m_Texture(texture), m_Size(size), m_Pos(position), m_Facing(facing), m_HasCollider(hasCollider), m_ID(entityID), 
			m_MaxFrames(maxFrames), m_Health(health), m_Alive(alive){

    	}	
		virtual ~Entity(){}
		virtual void Update(float& dT);
		virtual void Draw(Camera3D& camera);
		virtual BoundingBox GetCollider();
		virtual float& GetHealth();
		virtual bool& GetState();
		bool& HasCollider();
		Vector3& GetPos();
		int& GetID();
		Texture2D& GetTexture();
	protected:
		Texture2D m_Texture;
		Vector2& m_Size;
		Vector3 m_Pos;
		float m_Facing {1.0f};
		bool m_HasCollider;
		int& m_ID;
		int m_MaxFrames{};
		float m_Health{};
		bool m_Alive;
		int m_Frame{};
		float updateTime = 1.0 / 4.0;
		float runningTime{};
		BoundingBox m_Collider{};
		unsigned char m_Opacity {255};
		
	public:
		Color m_Color = {255, 255, 255, m_Opacity};
};
