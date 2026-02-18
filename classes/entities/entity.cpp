#include "classes/entities/entity.h"
#include "classes/enemies/enemy.h"

Texture2D& Entity::GetTexture(){
	return m_Texture;
}

bool& Entity::GetState(){
	return m_Alive;
}

float& Entity::GetHealth(){
	return m_Health;
}

int& Entity::GetID(){
	return m_ID;
}

bool& Entity::HasCollider(){
	return m_HasCollider;
}

Vector3& Entity::GetPos(){
	return m_Pos;
}

BoundingBox Entity::GetCollider(){
	return m_Collider;
}

void Entity::Draw(Camera3D& camera){
	if(m_Alive){
		Vector3 forward = Vector3Subtract(camera.target, camera.position);
		Vector3 up = { 0.0f, 1.0f, 0.0f };
		Vector3 right = Vector3CrossProduct(up, forward);
		up = Vector3CrossProduct(forward, right);
		up = Vector3Normalize(up);

		Rectangle source = {
			(float)m_Frame * m_Texture.width / m_MaxFrames, 
			0.0f, 
			m_Facing * m_Texture.width / m_MaxFrames, 
			(float)m_Texture.height};

		DrawBillboardPro(camera, m_Texture, source, m_Pos, up, m_Size, {}, 0.0f, m_Color);

		if(this->m_HasCollider){
			Vector3 colliderMin = { m_Pos.x + m_Size.x / 4.0f, m_Pos.y, m_Pos.z - m_Size.x / 6.0f };
			Vector3 colliderMax = { m_Pos.x + m_Size.x - m_Size.x / 4.0f, m_Pos.y + m_Size.y, m_Pos.z + m_Size.x / 20.0f };
			m_Collider = { colliderMin, colliderMax };
			if(G_VARS.DEBUG_MODE){
				DrawBoundingBox( m_Collider, m_Color);
			}
		}
	}
}

void Entity::Update(float& dT){
	if(m_Alive){
		runningTime += dT;
		if(runningTime > updateTime){
			runningTime = 0;
			if(m_Frame > m_MaxFrames){
				m_Frame = 0;
			}else{
				m_Frame++;
			}

		}
	}

}