#include "projectile.h"
#include "raymath.h"

void Projectile::SetPlayer(Player* Player){
	m_Player = Player;
}

void Projectile::Draw(Camera3D& camera){
	if(m_Alive){
		Vector3 forward = Vector3Subtract(camera.target, camera.position);
		Vector3 up = { 0.0f, 1.0f, 0.0f };
		Vector3 right = Vector3CrossProduct(up, forward);
		up = Vector3CrossProduct(forward, right);
		up = Vector3Normalize(up);

		Rectangle source = {
			(float)m_Frame * m_Texture.width / m_MaxFrames, 
			0.0f, 
			(float)m_Texture.width / m_MaxFrames, 
			(float)m_Texture.height};

		DrawBillboardPro(camera, m_Texture, source, m_Position, up, m_Size, {}, 0.0f, WHITE);

		Vector3 colliderMin = { m_Position.x + m_Size.x / 6.5f, m_Position.y, m_Position.z - m_Size.x / 2.5f};
		Vector3 colliderMax = { m_Position.x + m_Size.x - m_Size.x / 6.5f, m_Position.y + m_Size.y, m_Position.z + m_Size.x / 8.5f};
		m_Collider = { colliderMin, colliderMax };
		if(G_VARS.DEBUG_MODE){
			DrawBoundingBox( m_Collider, m_Color);
		}
	}
}

void Projectile::Update(float& dT){
	if(m_Alive){
		m_Timer += dT;
		if(m_Timer >= m_LifeTime){
			m_Alive = false;
		}else{
			Vector2 startPos = {G_VARS.WIDTH / 2.0f, G_VARS.HEIGHT / 2.0f};
			m_Direction = Vector2Subtract(m_MousePos, startPos);
			m_Direction = Vector2Normalize(m_Direction);

			m_Position.x += m_Direction.x * m_Speed * dT;
			m_Position.z += m_Direction.y * m_Speed * dT;

			m_RunningTime += dT;
			if(m_RunningTime > m_UpdateTime){
				m_RunningTime = 0;
				if(m_Frame > m_MaxFrames){
					m_Frame = 0;
				}else{
					m_Frame++;
				}

			}
		}
	}

}

Vector2& Projectile::GetVector(){
	return m_Direction;
}

Vector3& Projectile::GetPos(){
	return m_Position;
}

BoundingBox& Projectile::GetCollider(){
	return m_Collider;
}

bool& Projectile::GetState(){
	return m_Alive;
}

const float& Projectile::ManaCost() const{
	return m_ManaCost;
}

const float& Projectile::GetDamage() const{
	return m_Damage;
}

const float& Projectile::SpellCooldown() const{
	return m_CoolDown;
}

Texture2D& Projectile::GetTexture(){
	return m_Texture;
}
