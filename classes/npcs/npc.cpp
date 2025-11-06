#include "npc.h"
#include "classes/utilities/globalVariables.h"

size_t Npc::ProgressDialogue(){
	if(m_DialogueCount < m_Dialogue.size() - 1){
		m_DialogueCount++;
		return m_DialogueCount;
	}else{
		return m_DialogueCount;
	}
}

size_t Npc::StepBackDialogue(){
	if(m_DialogueCount > 0){
		m_DialogueCount--;
		return m_DialogueCount;
	}else{
		return m_DialogueCount;
	}
}

Texture2D& Npc::GetDialogueTexture(){
	return m_DialogueTexture;
}

std::string& Npc::GetDialogue(){
	return m_Dialogue[m_DialogueCount];
}

BoundingBox& Npc::GetInteractCollider(){
	return m_InteractCollider;
}

void Npc::Draw(Camera3D& camera){
	Entity::Draw(camera);

	if(G_VARS.DEBUG_MODE){
		DrawBoundingBox(m_InteractCollider, YELLOW);
	}
}