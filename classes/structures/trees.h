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
				m_Health = m_BaseTreeHealth + m_Tier * 10.0f;
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
				m_Health = m_BaseTreeHealth + m_Tier * 10.0f;
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
				m_Health = m_BaseTreeHealth + m_Tier * 10.0f;
			}

		~SpruceTree(){
			std::cout << "Spruce Tree Removed" << std::endl;
		}

		static inline Texture2D m_SpruceTreeTex;
};