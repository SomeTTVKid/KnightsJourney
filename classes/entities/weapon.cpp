#include "weapon.h"

void Weapon::Draw(Camera3D& camera){
	m_Facing = m_Player->GetFacing();
	m_Pos.x = m_Player->GetPos().x + (m_WeaponOffset * m_Facing);
	m_Pos.z = m_Player->GetPos().z;

	Entity::Draw(camera);
}

void Weapon::SetPlayer(Player* player){
	m_Player = player;
}