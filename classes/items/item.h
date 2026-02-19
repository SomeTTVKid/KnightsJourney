#pragma once
#include "raylib.h"
#include "classes/utilities/globalVariables.h"
#include <iostream>

// TODO Think about how we want to structure this class
// Maybe some sort of bool to keep track if the item is in world space?
// What if we want to create an item in the world? it would need a position...
// We need to check for duplicates in the inventory
// If we have one, stack by increasing that items count
// If not insert new item
// If above item stack limit
// Think about how to do this

// IDEA
// Each of these subclasses have different data BUT we can probably reuse variables
// Consumable needs some sort of variable to replenish by
// Armor needs some sort of variable to reduce damage by
// Weapon needs some sort of variable to increase damage by

// THINKING
// We have an item on the ground, its a health potion
// We set its m_InWorldSpace to true;
// Its then assigned a position in the world to be rendered at 
// WITH its own BoundingBox member variable
// We need a function to call to add it to player inventory
// WHAT IF WE ADD A MEMBER VARIABLE TO CERTAIN ENTITIES FOR INVENTORY
// Default the arguement to false in entity constructor
// If its not we need to pass in a list of items for that entity's inventory
// Add a member variable std::map<Item* item, int itemCount> m_Inventory;
		// static void AddToInventory(Entity* entity, Item* item, int itemCount);
		// entity->GetInventory().insert({item, itemCount});

// ITEMS TO ADD
// SUBCLASS OF CONSUMABLE
	// 1.) Health Potion
	// 2.) Mana Potion
// SUBCLASS OF ARMOR
	// 3.) Helmet
	// 4.) Chest-Plate
	// 5.) Legs
// SUBCLASS OF WEAPON
	// 6.) Weapon

class Entity;

class Item{
	public:
		Item(bool inWorldSpace = false, Vector3 worldSpacePos = {}) 
			: m_IsInWorldSpace(inWorldSpace), m_WorldPos(worldSpacePos){
				if(m_IsInWorldSpace){
					// Set bounding box
					m_Active = true;
					// We will have to redo this halfsize variable later
					// TODO Think about this later...
					float Half_Size = G_VARS.POTION_SIZE.x / 2.0f;
					Vector3 colliderMin = { m_WorldPos.x - Half_Size, m_WorldPos.y, m_WorldPos.z - Half_Size };
					Vector3 colliderMax = { colliderMin.x + G_VARS.POTION_SIZE.x + G_VARS.POTION_SIZE.x, colliderMin.y + G_VARS.POTION_SIZE.y, colliderMin.z + G_VARS.POTION_SIZE.x };
					m_Collider = { colliderMin, colliderMax };
				}
			}
		virtual ~Item(){
			std::cout << "Item deleted from memory" << std::endl;
		}

		enum class ItemTag{
			CONSUMABLE,
			EQUIPMENT
		};

		enum class ItemID{
			HEALTH_POTION,
			MANA_POTION,
			HELMET,
			CHEST,
			LEGS,
			WEAPON
		};

		virtual void Draw(Camera3D& camera);
		//
		virtual bool& GetState();
		virtual Texture2D& GetTexture();
		virtual Vector3& GetPos();
		virtual BoundingBox& GetCollider();
		virtual const Rectangle& GetRect() const;
		virtual Rectangle& SetRect();
		virtual float& GetRestoreAmount();
		virtual float& GetDefenseAmount();
		virtual float& GetDamageAmount();
		virtual const ItemID& GetID() const;
		virtual const ItemTag& GetItemTag() const;
		virtual std::string& GetItemName();
		virtual std::string& GetItemDescription();

		bool& IsStackable();

	protected:
		bool m_IsInWorldSpace;
		bool m_Active = false;
		bool m_IsStackable = false;
		Vector3 m_WorldPos;
		Texture2D m_Texture;
		BoundingBox m_Collider;
		Rectangle m_Rect{};
		float m_RestorationAmount{};
		float m_DefenseAmount{};
		float m_Damage{};
		ItemID m_ID;
		ItemTag m_TAG;
		std::string m_ItemName;
		std::string m_ItemDescription;

};

class Weapon : public Item{
	public:
		Weapon(Vector3& pos, bool animated = false, int maxFrames = 1)
			: Item(true, pos){}
		virtual ~Weapon(){
			std::cout << "Weapon Destroyed!" << std::endl;
		}


};

class WoodenSword : public Weapon{
	public:
		WoodenSword(Vector3& pos) : Weapon(pos){
			m_Texture = m_WoodenSword;
			m_Damage = 1.0f;
			m_ID = ItemID::WEAPON;
			m_TAG = ItemTag::EQUIPMENT;
			m_ItemName = "Wooden Sword";
			m_ItemDescription = "Deals 1 point of damage.";
		};
		~WoodenSword(){
			std::cout << "Wooden Sword Destroyed!" << std::endl;
		};

		static inline Texture2D m_WoodenSword;

};