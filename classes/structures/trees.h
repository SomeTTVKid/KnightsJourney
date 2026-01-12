#include "structure.h"
#include "classes/utilities/globalVariables.h"

// TODO
// DONE // Oak			common, found in many biomes (Forests, Plains, Swamps, etc.). Can drop apples when leaves decay.	Standard Brown
// DONE // Birch		Has white bark and pale wood.	Pale/Light Brown
// DONE // Spruce		colder biomes (Taiga, Snowy Plains, Grove). Taller, denser, and darker leaves.	Dark Brown

// Jungle		Jungle biomes. Ranges from small bushes to very tall trees; can have cocoa pods growing on them.	Light Brownish/Tan
// Acacia		Savanna biomes. Features unique diagonal trunks.	Orange/Reddish
// Dark Oak		Dark Forest biomes. Has a thick 2x2 trunk and dense canopy.	Dark Brown
// Mangrove		Swamp biomes, often growing in water. Known for aerial roots.	Deep Red-Orange
// Cherry		Cherry Grove biomes. Distinctive pink leaves with falling petal particles.	Pinkish/Light
// Azalea   	The "trunk" is a single oak log, with unique azalea leaves.	N/A (uses Oak wood)
// Pale Oak		Pale Garden biome. Has thick 2x2 trunks, grey leaves, and pale hanging moss.	Pale Gray

class OakTree : public Structure{
	public:
		OakTree(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::TREE_ID)	
			: Structure(pos, size, id){
				m_Texture = m_OakTreeTex;
				m_Tier = 1;
				m_Health = m_BaseTreeHealth + (m_Tier * 10.0f);
				m_MaxHealth = m_Health;
				// Collision
				m_ColliderMin = { pos.x + m_Size.x / 3.0f, pos.y, pos.z - m_Size.x / 8.0f };
				m_ColliderMax = { m_ColliderMin.x + m_Size.x / 4.0f, m_ColliderMin.y + m_Size.y, m_ColliderMin.z + m_Size.x / 6.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
				// Leaves
				m_LeavesColliderMin = { pos.x, pos.y + m_Size.y / 2.5f, pos.z };
				m_LeavesColliderMax = { m_LeavesColliderMin.x + m_Size.x, m_LeavesColliderMin.y + m_Size.y / 1.25f, m_LeavesColliderMin.z + m_Size.x / 20.0f };
				m_LeavesCollider = { m_LeavesColliderMin, m_LeavesColliderMax };
				// Interact
				m_InteractColliderMin = { pos.x + 0.5f, pos.y, pos.z - 0.3f };
				m_InteractColliderMax = { m_InteractColliderMin.x + 1.5f, m_InteractColliderMin.y + 0.3f, m_InteractColliderMin.z + 0.5f };
				m_InteractCollider = { m_InteractColliderMin, m_InteractColliderMax };
			}
		~OakTree(){
			std::cout << "Oak Tree Removed" << std::endl;
		}

		static inline Texture2D m_OakTreeTex;
};

class BirchTree : public Structure{
	public:
		BirchTree(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::TREE_ID)
			: Structure(pos, size, id){
				m_Texture = m_BirchTreeTex;
				m_Tier = 2;
				m_Health = m_BaseTreeHealth + (m_Tier * 10.0f);
				m_MaxHealth = m_Health;
				// Collision
				m_ColliderMin = { pos.x + m_Size.x / 3.0f, pos.y, pos.z - m_Size.x / 8.0f };
				m_ColliderMax = { m_ColliderMin.x + m_Size.x / 3.8f, m_ColliderMin.y + m_Size.y, m_ColliderMin.z + m_Size.x / 6.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
				// Leaves
				m_LeavesColliderMin = { pos.x, pos.y + m_Size.y / 2.5f, pos.z };
				m_LeavesColliderMax = { m_LeavesColliderMin.x + m_Size.x, m_LeavesColliderMin.y + m_Size.y / 1.25f, m_LeavesColliderMin.z + m_Size.x / 20.0f };
				m_LeavesCollider = { m_LeavesColliderMin, m_LeavesColliderMax };
				//
				m_CurrentLeavesCollider = m_LeavesCollider;
				// Interact
				m_InteractColliderMin = { pos.x + 0.5f, pos.y, pos.z - 0.3f };
				m_InteractColliderMax = { m_InteractColliderMin.x + 1.5f, m_InteractColliderMin.y + 0.3f, m_InteractColliderMin.z + 0.5f };
				m_InteractCollider = { m_InteractColliderMin, m_InteractColliderMax };
			}
		~BirchTree(){
			std::cout << "Birch Tree Removed" << std::endl;
		}

		static inline Texture2D m_BirchTreeTex;
};

class SpruceTree : public Structure{
	public:
		SpruceTree(Vector3 pos, Vector2& size, GlobalVariables::StructureIDs id = GlobalVariables::StructureIDs::TREE_ID)
			: Structure(pos, size, id){
				m_Texture = m_SpruceTreeTex;
				m_Tier = 3;
				m_Health = m_BaseTreeHealth + (m_Tier * 10.0f);
				m_MaxHealth = m_Health;
				// Collision
				m_ColliderMin = { pos.x + m_Size.x / 3.0f, pos.y, pos.z - m_Size.x / 8.0f };
				m_ColliderMax = { m_ColliderMin.x + m_Size.x / 2.8f, m_ColliderMin.y + m_Size.y, m_ColliderMin.z + m_Size.x / 6.0f };
				m_Collider = { m_ColliderMin, m_ColliderMax };
				// Leaves
				m_LeavesColliderMin = { pos.x, pos.y + m_Size.y / 3.8f, pos.z };
				m_LeavesColliderMax = { m_LeavesColliderMin.x + m_Size.x, m_LeavesColliderMin.y + m_Size.y, m_LeavesColliderMin.z + m_Size.x / 20.0f };
				m_LeavesCollider = { m_LeavesColliderMin, m_LeavesColliderMax };
				// Interact
				m_InteractColliderMin = { pos.x + 0.5f, pos.y, pos.z - 0.3f };
				m_InteractColliderMax = { m_InteractColliderMin.x + 1.5f, m_InteractColliderMin.y + 0.3f, m_InteractColliderMin.z + 0.5f };
				m_InteractCollider = { m_InteractColliderMin, m_InteractColliderMax };
			}

		~SpruceTree(){
			std::cout << "Spruce Tree Removed" << std::endl;
		}

		static inline Texture2D m_SpruceTreeTex;
};