#include "structure.h"

void Structure::Update(float& dT){
	if(m_Active){
		if(m_HasAnimation){
			m_RunningTime += dT;
			if(m_RunningTime > m_UpdateTime){
				m_RunningTime = 0;
				if(m_CurrentFrame > m_MaxFrames){
					m_CurrentFrame = 0;
				}else{
					m_CurrentFrame++;
				}
			}
		}

		if(m_UpdateCollider){
			// Seperate any collider updates by id (use enum for clarity)
			if(m_ID == GlobalVariables::StructureIDs::TREE_ID){
				m_Collider.max.y = m_Pos.y + m_Size.y / 4.0f;
			}
			m_UpdateCollider = false;
		}
	}
}

void Structure::Draw(Camera3D& camera){
	if(m_Active){
		Vector3 forward = Vector3Subtract(camera.target, camera.position);
		Vector3 up = { 0.0f, 1.0f, 0.0f };
		Vector3 right = Vector3CrossProduct(up, forward);
		up = Vector3CrossProduct(forward, right);
		up = Vector3Normalize(up);

		Rectangle source = {
			(float)m_CurrentFrame * m_Texture.width / m_MaxFrames, 
			0.0f, 
			m_Facing * m_Texture.width / m_MaxFrames, 
			(float)m_Texture.height};

		DrawBillboardPro(camera, m_Texture, source, m_Pos, up, m_Size, {}, 0.0f, m_Color);

		if(G_VARS.DEBUG_MODE && m_HasCollider){
			DrawBoundingBox( m_Collider, m_Color);
		}
	}
}

void Structure::TakeDamage(float damage, size_t& toolTier){
	if(toolTier >= m_Tier){
		std::cout << "Tool Tier High Enough" << std::endl;
		if(m_Health - damage <= 0){
			std::cout << "Resource Depleted" << std::endl;
			if(m_CurrentFrame != 1){
				m_CurrentFrame = 1;
				m_UpdateCollider = true;
			}
			std::cout << m_Health << std::endl;
		}
		std::cout << "Taken Damage" << std::endl;
		m_Health -= damage;
	}
}

BoundingBox& Structure::GetCollider(){
	return m_Collider;
}

float& Structure::GetHealth(){
	return m_Health;
}

bool& Structure::GetState(){
	return m_Active;
}

bool& Structure::HasCollider(){
	return m_HasCollider;
}

Vector3& Structure::GetPos(){
	return m_Pos;
}

GlobalVariables::StructureIDs& Structure::GetID(){
	return m_ID;
}

int& Structure::SetFrame(){
	return m_CurrentFrame;
}

