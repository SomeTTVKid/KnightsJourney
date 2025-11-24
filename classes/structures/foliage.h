#include "structure.h"
#include "classes/utilities/globalVariables.h"

// Put Stones inside of here

// Grass
class Grass01 : public Structure{
	public:
		Grass01(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::GRASS_ID)
			: Structure(pos, size, id){
				m_Texture = m_Grass01Tex;
				m_HasCollider = false;
				m_HasAnimation = true;
				m_MaxFrames = 4;
			}
		~Grass01(){
			std::cout << "Grass01 Removed" << std::endl;
		}

		static inline Texture2D m_Grass01Tex;
};


// Stones
class Stone01 : public Structure{
	public:
		Stone01(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::STONE_ID)
			: Structure(pos, size, id){
				m_Texture = m_Stone01Tex;
				m_HasCollider = true;
				m_HasAnimation = false;
				m_MaxFrames = 1;
				// Collider
				// Needs to have less height
				m_ColliderMin = { m_Pos.x + m_Size.x / 7.3f, m_Pos.y, m_Pos.z - m_Size.x / 2.0f };
				m_ColliderMax = { m_Pos.x + m_Size.x - m_Size.x / 12.5f, m_Pos.y + m_Size.y / 2.0f, m_Pos.z + m_Size.x / 20.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };

			}
		~Stone01(){
			std::cout << "Stone01 Removed" << std::endl;
		}

		static inline Texture2D m_Stone01Tex;
};