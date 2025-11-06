#pragma once
#include "item.h"

class ManaPotion : public Item{
	public:
		ManaPotion(bool isInWorld = false, Vector3 worldPos = {})	
			: Item(isInWorld, worldPos){
				m_Texture = m_ManaPotionTex;
				m_RestorationAmount = 0.5f;
				m_WorldPos = worldPos;
				m_ID = ItemID::MANA_POTION;
				m_ItemName = "Low-Quality Mana Potion";
				m_ItemDescription = "Restores 0.5 Units of Mana.";
			}
		~ManaPotion(){}

		static inline Texture2D m_ManaPotionTex;
		void Draw(Camera3D& camera) override;
};