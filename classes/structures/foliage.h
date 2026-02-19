#include "structure.h"
#include "classes/utilities/globalVariables.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GRASS PREFABS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Grass01 : public Structure{
	public:
		Grass01(Vector3 pos, Vector2 size = { 0.5f, 0.5f }, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::GRASS_ID)
			: Structure(pos, id){
				m_Texture = m_Grass01Tex;
				m_HasCollider = false;
				m_HasAnimation = true;
				m_MaxFrames = 4;
				m_Size = size;
			}
		~Grass01(){
			std::cout << "Grass01 Removed" << std::endl;
		}

		static inline Texture2D m_Grass01Tex;
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~STONE PREFABS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Stone01 : public Structure{
	public:
		Stone01(Vector3 pos, Vector2 size = { 2.0f, 1.8f }, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::STONE_ID)
			: Structure(pos, id){
				m_Texture = m_Stone01Tex;
				m_HasCollider = true;
				m_HasAnimation = false;
				m_MaxFrames = 1;
				m_Size = size;
				// Collider
				m_ColliderMin = { m_Pos.x + m_Size.x / 7.3f, m_Pos.y, m_Pos.z - m_Size.x / 2.0f };
				m_ColliderMax = { m_ColliderMin.x + m_Size.x / 1.25f, m_ColliderMin.y + m_Size.y / 2.0f, m_ColliderMin.z + m_Size.x / 2.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };

			}
		~Stone01(){
			std::cout << "Stone01 Removed" << std::endl;
		}

		static inline Texture2D m_Stone01Tex;
};

class Stone02 : public Structure{
	public:
		Stone02(Vector3 pos, Vector2 size = { 0.65f, 0.65f }, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::STONE_ID)
			: Structure(pos, id){
				m_Texture = m_Stone02Tex;
				m_HasCollider = true;
				m_HasAnimation = false;
				m_MaxFrames = 1;
				m_Size = size;
				// Collider
				m_ColliderMin = { m_Pos.x - m_Size.x / 3.0f, m_Pos.y, m_Pos.z - m_Size.x / 2.0f };
				m_ColliderMax = { m_ColliderMin.x + m_Size.x + m_Size.x / 1.6f, m_ColliderMin.y + m_Size.y, m_ColliderMin.z + m_Size.x / 2.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
			}

		~Stone02(){
			std::cout << "Stone02 Removed" << std::endl;
		}

		static inline Texture2D m_Stone02Tex;
};


// Signs
class Sign01 : public Structure{
	public:
		Sign01(Vector3 pos, float facing = 1.0f, Vector2 size = { 1.0f, 1.0f }, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::SIGN_ID)
			// TODO Add in param for facing direction in constuctor!!!!
			: Structure(pos, id){
				m_Texture = m_Sign01Tex;
				m_HasCollider = true;
				m_HasAnimation = false;
				m_MaxFrames = 1;
				m_Facing = facing;
				m_Size = size;
				// Collider
				m_ColliderMin = { m_Pos.x + m_Size.x / 3.0f, m_Pos.y, m_Pos.z - m_Size.x / 2.5f };
				m_ColliderMax = { m_ColliderMin.x + m_Size.x / 3.0f, m_ColliderMin.y + m_Size.y / 2.0f, m_ColliderMin.z + m_Size.x / 2.5f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
		}

		~Sign01(){
			std::cout << "Sign01 Removed" << std::endl;
		}

		static inline Texture2D m_Sign01Tex;
};

// TODO Add a collider to this
// Campfires
class Campfire01 : public Structure{
	public:
		Campfire01(Vector3 pos, Vector2 size = { 1.0f, 1.0f }, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::CAMPFIRE_ID)
			: Structure(pos, id){
				m_Texture = m_Campfire01Tex;
				m_HasCollider = true;
				m_HasAnimation = true;
				m_MaxFrames = 4;
				m_Size = size;
				// Collider
				m_ColliderMin = { m_Pos.x + m_Size.x / 3.5f, m_Pos.y, m_Pos.z - m_Size.x / 3.0f };
				m_ColliderMax = { m_ColliderMin.x + m_Size.x / 2.0f, m_ColliderMin.y + m_Size.y, m_ColliderMin.z + m_Size.x / 4.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
		}

		~Campfire01(){
			std::cout << "Campfire01 Removed" << std::endl;
		}

		static inline Texture2D m_Campfire01Tex;
};