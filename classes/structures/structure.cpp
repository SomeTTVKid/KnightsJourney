#include "structure.h"
#include "classes/scenes/scene.h"
#include <format>

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

		// Update Depleted and Replenished Resource
		if(m_UpdateCollider){
			// Seperate any collider updates by id 
			// So we can change textures or colliders for each resource
			if(m_ID == GlobalVariables::StructureIDs::TREE_ID){
				if(m_Depleted){
					m_Collider.max.y = m_ColliderMin.y + m_Size.y / 4.0f;
					m_CurrentLeavesCollider = m_EmptyLeavesCollider;
					m_CurrentFrame = 1;
				}else{
					m_Collider.max.y = m_ColliderMin.y + m_Size.y;
					m_CurrentLeavesCollider = m_LeavesCollider;
					m_CurrentFrame = 0;
				}

			}
			m_UpdateCollider = false;
		}

		// Replenish Resource
		// TODO Maybe make a timer that if the resource isnt attacked again after so long, itll start replenishing
		// Can just be a member variable of structure base class 
		if(m_Health < m_MaxHealth && m_Depleted){
			m_Health += 1.0f * dT * G_VARS.TICK_SPEED; 
			m_Health = std::clamp(m_Health, 0.0f, m_MaxHealth);
		}else{
			m_Depleted = false;
			m_UpdateCollider = true;
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
			DrawBoundingBox( m_CurrentLeavesCollider, RED);
			DrawBoundingBox( m_InteractCollider, GREEN);
		}

		// The issue is actually we need to call this in the 2d world space..
		// Think about adding a second camera thats 2d
		// Look into the workings of the Camera2D
		// Upon further inspection I do infact need a camera2D for this...
		// Could also hook up all the ui draw calls inside that camera
		// if(m_InteractedWith){

		// 	// Display text
		// 	// Update opacity
		// 	// Once this all runs
		// 	// Set state to false
		// 	DrawTextEx(G_VARS.FONT, m_Text.c_str(), m_TextPosition, 38 * G_VARS.WIDTH_SCALE, G_VARS.FONT_SPACING, {255, 255, 255, m_TextOpacity});
		// }
	}
}

void Structure::TakeDamage(float damage){
	if(m_Health - damage <= 0 && m_Depleted != true){
		m_Depleted = true;
		m_UpdateCollider = true;
	}
	m_Health -= damage;
	m_Health = std::clamp(m_Health, 0.0f, m_MaxHealth);
	m_Text = std::format("{:.2f}/{:.2f}", m_Health, m_MaxHealth);
	m_InteractedWith = true;
	m_TextPosition = GetWorldToScreen(m_Pos, Scene::m_Camera);
}

BoundingBox& Structure::GetCollider(){
	return m_Collider;
}

BoundingBox& Structure::GetCurrentLeavesCollider(){
	return m_CurrentLeavesCollider;
}

BoundingBox& Structure::GetInteractCollider(){
	return m_InteractCollider;
}

float& Structure::GetHealth(){
	return m_Health;
}

int& Structure::GetTier(){
	return m_Tier;
}

float& Structure::GetMaxHealth(){
	return m_MaxHealth;
}

bool& Structure::GetState(){
	return m_Active;
}

bool& Structure::HasCollider(){
	return m_HasCollider;
}

bool& Structure::IsDepleted(){
	return m_Depleted;
}

Vector3& Structure::GetPos(){
	return m_Pos;
}

GlobalVariables::StructureIDs& Structure::GetID(){
	return m_ID;
}
