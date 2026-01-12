#include "npc.h"
#include "classes/utilities/globalVariables.h"

// TODO how to fix this so that i can wrap certain dialogue while still being able to play it all when giving a quest???
// When its less than the size of the dialogue, it increases by one
// Maybe add an extra check so that when its == to the size
// Increase by one but after closing dialogue or we need a seperate tracker for it
// Then we have an else statement that handles resetting it back to zero
size_t Npc::ProgressDialogue(){
	if(m_DialogueCount < m_Dialogue.size() - 1){
		m_DialogueCount++;
		return m_DialogueCount;
	}else{
		G_VARS.IN_DIALOGUE = false;
		m_DialogueCount = 0;
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

size_t& Npc::ReturnDialogueCount(){
	return m_DialogueCount;
}

size_t Npc::ReturnDialogueSize(){
	return m_Dialogue.size();
}

bool& Npc::GetState(){
	return m_Alive;
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