#include "structure.h"
#include "classes/utilities/globalVariables.h"

class House01 : public Structure{
	public:
		House01(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::BUILDING_ID)
			: Structure(pos, size, id){
				m_Texture = m_House01Tex;
				m_MaxFrames = 1;
				// Collider
				m_ColliderMin = { m_Pos.x + m_Size.x / 4.5f, m_Pos.y, m_Pos.z - m_Size.x / 6.0f };
				m_ColliderMax = { m_Pos.x + m_Size.x - m_Size.x / 4.0f, m_Pos.y + m_Size.y, m_Pos.z + m_Size.x / 20.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
			}
		~House01(){
			std::cout << "House01 Removed" << std::endl;
		}

		static inline Texture2D m_House01Tex;

};

class House02 : public Structure{
	public:
		House02(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::BUILDING_ID)
			: Structure(pos, size, id){
				m_Texture = m_House02Tex;
				m_MaxFrames = 1;
				// Collider
				m_ColliderMin = { m_Pos.x + m_Size.x / 4.5f, m_Pos.y, m_Pos.z - m_Size.x / 6.0f };
				m_ColliderMax = { m_Pos.x + m_Size.x - m_Size.x / 4.0f, m_Pos.y + m_Size.y, m_Pos.z + m_Size.x / 20.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
			}
		~House02(){
			std::cout << "House02 Removed" << std::endl;
		}

		static inline Texture2D m_House02Tex;

};

class Shed : public Structure{
	public:
		Shed(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::BUILDING_ID)
			: Structure(pos, size, id){
				m_Texture = m_ShedTex;
				m_MaxFrames = 1;
				// Collider
				m_ColliderMin = { m_Pos.x + m_Size.x / 4.5f, m_Pos.y, m_Pos.z - m_Size.x / 6.0f };
				m_ColliderMax = { m_Pos.x + m_Size.x - m_Size.x / 4.0f, m_Pos.y + m_Size.y, m_Pos.z + m_Size.x / 20.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
			}
		~Shed(){
			std::cout << "Shed Removed" << std::endl;
		}

		static inline Texture2D m_ShedTex;

};

class Inn : public Structure{
	public:
		Inn(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::BUILDING_ID)
			: Structure(pos, size, id){
				m_Texture = m_InnTex;
				m_MaxFrames = 1;
				// Collider
				m_ColliderMin = { m_Pos.x + m_Size.x / 4.5f, m_Pos.y, m_Pos.z - m_Size.x / 6.0f };
				m_ColliderMax = { m_Pos.x + m_Size.x - m_Size.x / 4.0f, m_Pos.y + m_Size.y, m_Pos.z + m_Size.x / 20.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
			}
		~Inn(){
			std::cout << "Inn Removed" << std::endl;
		}

		static inline Texture2D m_InnTex;

};