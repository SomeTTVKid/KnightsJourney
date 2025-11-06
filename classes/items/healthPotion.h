#pragma once
#include "item.h"

class HealthPotion : public Item{
	public:
		HealthPotion(bool isInWorld = false, Vector3 worldPos = {})	
			: Item(isInWorld, worldPos){
				m_Texture = m_HealthPotionTex;
				m_RestorationAmount = 2.5f;
				m_WorldPos = worldPos;
				m_ID = ItemID::HEALTH_POTION;
				m_ItemName = "Low-Quality Health Potion";
				m_ItemDescription = "Restores 2.5 Health Points.";
			}
		~HealthPotion(){}

		static inline Texture2D m_HealthPotionTex;
		void Draw(Camera3D& camera) override;
		
};