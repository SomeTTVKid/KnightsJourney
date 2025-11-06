#pragma once
#include "sceneManager.h"
#include "raylib.h"
#include "classes/scenes/scene.h"
#include "classes/entities/entity.h"
#include "classes/utilities/globalVariables.h"

// What if we passed a sound to each level loader...
// I want to be able to play different sounds for different loaders
// Rather have a set sound for certain levels maybe...

class LevelLoader : public Entity{
	public:
		LevelLoader(Texture2D& texture, int newScene, Vector3 position, Vector3 outPosition, Vector2& size) : 
			Entity(texture, size, position, 1.0f, false, G_VARS.STRUCTURE_ID, 1), m_SceneID(newScene), m_OutPosition(outPosition){
				m_Min = { m_Pos.x, m_Pos.y, m_Pos.z};
				m_Max = { m_Pos.x + m_Size.x, m_Pos.y + 0.2f, m_Pos.z + m_Size.x};
				m_InteractCollider = { m_Min, m_Max };
			}
		~LevelLoader(){
			std::cout << "Level Loader Removed" << std::endl;
		}
		void MoveScene();
		void Draw(Camera3D& camera) override;
		BoundingBox& GetInteractCollider();
		Vector3& GetOutPos();

		static inline Sound m_TransitionSound;
	private:
		int m_SceneID;
		Vector3 m_OutPosition;
		Vector3 m_Min;
		Vector3 m_Max;
		BoundingBox m_InteractCollider;

	public:
		Color m_Color = YELLOW;
};
