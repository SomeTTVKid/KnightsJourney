#include "structure.h"
#include "classes/utilities/globalVariables.h"

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