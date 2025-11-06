#include "enemy.h"
#include "classes/utilities/globalVariables.h"

Vector3& Enemy::GetLastPos(){
	return m_LastPos;
}

void Enemy::ReturnToSpawn(float& dT){
	m_Direction = Vector2Subtract({m_SpawnPos.x, m_SpawnPos.z}, {m_Pos.x, m_Pos.z});
	m_Direction = Vector2Normalize(m_Direction);
	m_Direction.x < 0.0f ? m_Facing = -1.0f : m_Facing = 1.0;
	m_Pos.x += m_Direction.x * m_Speed * dT;
	m_Pos.z += m_Direction.y * m_Speed * dT;

}

void Enemy::TakeDamage(float damage){
	if(m_Health - damage <= 0.0f){
		m_Alive = false;
	}else{
		m_Health -= damage;
		m_TookDamage = true;
		m_InCombat = true;
		m_CombatTimer = 0.0f;
		m_CombatMaxTime = 4.5f;
	}
}

void Enemy::Attack(){
	float DamageAfterDefense = m_Damage / (m_Player->GetDefense() + 100 / 100);
	std::cout << DamageAfterDefense << std::endl;
	m_Player->TakeDamage(DamageAfterDefense);
	m_AttackTimer = 0.0f;

}

void Enemy::SetTarget(Player* player){
	m_Player = player;
}

void Enemy::PathFind(float& dT){
	m_Direction = Vector2Subtract({m_Player->GetPos().x, m_Player->GetPos().z}, {m_Pos.x, m_Pos.z});
	m_Direction = Vector2Normalize(m_Direction);
	m_Direction.x < 0.0f ? m_Facing = -1.0f : m_Facing = 1.0;
	m_AttackTimer += dT;
	if(m_AttackTimer >= m_AttackCooldown){
		if(CheckCollisionBoxes(this->GetCollider(), m_Player->GetCollider())){
			m_Pos = m_LastPos;
			Attack();
			m_CombatTimer = 0.0f;
			// Knockback
			m_Player->GetPos().x += m_Direction.x * 10.0f * dT;
			m_Player->GetPos().z += m_Direction.y * 10.0f * dT;
		}
		m_Pos.x += m_Direction.x * m_Speed * dT;
		m_Pos.z += m_Direction.y * m_Speed * dT;
	}
	m_CombatTimer += dT;
	if(m_CombatTimer >= m_CombatMaxTime){
		m_InCombat = false;
	}
}

void Enemy::Update(float& dT){
	if(m_Alive){
		Entity::Update(dT);

		// Hit Feedback
		if(m_TookDamage){
			m_FlashTimer += dT;
			if(m_FlashTimer >= m_FlashMaxTime){
				m_Color = WHITE;
				m_TookDamage = false;
				m_FlashTimer = 0.0f;

			}else{
				m_Color = RED;
			}
		}

		m_LastPos = m_Pos;

		// Pathfinding
		if(G_VARS.DEBUG_MODE != true){
			if(m_InCombat){
				PathFind(dT);
			}else if(Vector2Length(Vector2Subtract({m_Player->GetPos().x, m_Player->GetPos().z}, {m_Pos.x, m_Pos.z})) <= m_AggroRange + m_Size.x){
				m_InCombat = true;
				m_CombatTimer = 0.0f;
			}else{
				auto distanceFromSpawn = Vector2Subtract({m_SpawnPos.x, m_SpawnPos.z}, {m_Pos.x, m_Pos.z});
				if(Vector2Length(distanceFromSpawn) < 0.1f){
					return;
				}
				ReturnToSpawn(dT);
			}
		}
	}
}

void Enemy::Draw(Camera3D& camera){
	if(m_Alive){
		Entity::Draw(camera);
		if(G_VARS.DEBUG_MODE){
			DrawCircle3D(GetPos(), m_AggroRange, { 180.0f, 0.0f, 0.0f }, 90.0f, YELLOW);
		}
	}
}