#pragma once
#include "classes/entities/entity.h"
#include <string>

// TODO plan out the dialogue and story LMAO

class Npc : public Entity{
	public:
		Npc(const Texture2D& texture, const Texture2D& headshot, Vector2& size, Vector3& position, float facing, bool hasCollider, int& id, std::vector<int>& dialogueIndexs) : 
			Entity(texture, size, position, facing, hasCollider, id), m_DialogueTexture(headshot){
				for( int index : dialogueIndexs){
					m_Dialogue.push_back(dialogue[index]);
				}
			}
		~Npc(){
			std::cout << "Npc Deleted" << std::endl;
		}
		void Draw(Camera3D& camera) override;
		Texture2D& GetDialogueTexture();
		BoundingBox& GetInteractCollider();
		std::string& GetDialogue();
		size_t ProgressDialogue();
		size_t StepBackDialogue();
	private:
		Texture2D m_DialogueTexture;
		Vector3 minRange = { m_Pos.x - m_Size.x / 2.0f , m_Pos.y, m_Pos.z - m_Size.x / 2.0f};
		Vector3 maxRange = { m_Pos.x + m_Size.x + m_Size.x / 2.0f, m_Pos.y + m_Size.y, m_Pos.z + m_Size.x / 2.0f};
		BoundingBox m_InteractCollider = {minRange, maxRange};
		std::vector<std::string> m_Dialogue{};
		size_t m_DialogueCount{};

		// Fix last line of dialogue
		static inline std::string dialogue[]{
			"Thank you for saving me Traveler!",
			"Those goblins kidnapped me from my house!",
			"My name is 'John', I'm the local blacksmith at the town West of here.",
			"Please talk to me at my forge in town, I have something to ask of you.",
			"Thank you so much Traveler for freeing me from those goblins!",
			"TODO ADD FIRST QUEST HERE",
			"I'll pray you make it safely."
		};

};
