#include "levelLoader.h"

void LevelLoader::MoveScene(){
	PlaySound(m_TransitionSound);
	SceneManager::GetInstance().RequestSceneChange(m_SceneID);
}

void LevelLoader::Draw(Camera3D& camera){
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

	Vector3 Pos = m_Pos;
	Pos.y = 0.0f;
	Pos.z += 0.3f;

	DrawBillboardPro(camera, m_Texture, source, Pos, up, m_Size, {}, 0.0f, WHITE);

	if(G_VARS.DEBUG_MODE){
		DrawBoundingBox(m_InteractCollider, m_Color);
	}
}

BoundingBox& LevelLoader::GetInteractCollider(){
	return m_InteractCollider;
}

Vector3& LevelLoader::GetOutPos(){
	return m_OutPosition;
}