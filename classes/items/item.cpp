#include "item.h"
#include "raymath.h"

void Item::Draw(Camera3D& camera){
	if(m_IsInWorldSpace){
		Vector3 forward = Vector3Subtract(camera.target, camera.position);
		Vector3 up = { 0.0f, 1.0f, 0.0f };
		Vector3 right = Vector3CrossProduct(up, forward);
		up = Vector3CrossProduct(forward, right);
		up = Vector3Normalize(up);

		Rectangle source = {
			0.0f, 
			0.0f, 
			(float)m_Texture.width, 
			(float)m_Texture.height};

		DrawBillboardPro(camera, m_Texture, source, m_WorldPos, up, G_VARS.POTION_SIZE, {}, 0.0f, WHITE);

		if(G_VARS.DEBUG_MODE){
			DrawBoundingBox( m_Collider, WHITE);
		}

	}
}

bool& Item::IsStackable(){
	return m_IsStackable;
}

// Gettings/Setters
bool& Item::GetState(){
	return m_Active;
}

std::string& Item::GetItemName(){
	return m_ItemName;
}

std::string& Item::GetItemDescription(){
	return m_ItemDescription;
}

Texture2D& Item::GetTexture(){
	return m_Texture;
}

Vector3& Item::GetPos(){
	return m_WorldPos;
}

BoundingBox& Item::GetCollider(){
	return m_Collider;
}

const Rectangle& Item::GetRect() const{
	return m_Rect;
}

Rectangle& Item::SetRect(){
	return m_Rect;
}

float& Item::GetRestoreAmount(){
	return m_RestorationAmount;
}

float& Item::GetDefenseAmount(){
	return m_DefenseAmount;
}

float& Item::GetDamageAmount(){
	return m_Damage;
}

const Item::ItemID& Item::GetID() const{
	return m_ID;
}

const Item::ItemTag& Item::GetItemTag() const{
	return m_TAG;
}