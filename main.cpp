#include "raylib.h"
#include "classes/player/player.h"
#include "classes/scenes/scene.h"
#include "classes/utilities/sceneManager.h"
#include "classes/utilities/globalVariables.h"
#include "classes/utilities/levelLoader.h"
#include "classes/projectiles/projectile.h"
#include "classes/items/healthPotion.h"
#include "classes/items/manaPotion.h"

// valgrind --tool=callgrind ./KnightsJourney
// Potential TODO
// Could also look into where exactly our raycast is postitioned in regards to the player size, y might be off

// WHEN ADDING A NEW SCENE
// Create new scene header and cpp file
// Add new scene in enum class inside of global variables
// Add new switch case statement inside of scenemanager.cpp, Line 56. ChangeScene()
// Fill out new scene with functions

// TODO NEXT

// TODO Make size a default parameter inside of structure params, make it {1.0f, 1.0f};
// Let the class take in a parameter but default it to the base size of items/structures

// TODO Redo our shared pointers BACK into unique pointers since we do NOT need no shared pointers

// TODO Remove public member variables from player class and add in getters/setters

// TODO Add in the ability to unequip items/weapons

// TODO Add in the ability to equip equipment and auto un-equip currently equipped items
// Add checks for clicking on an item when that slot is already taken up
// Prompt to equip new item
// If so, move the currently equipped item back to inventory and move the selected item to the slot

// TODO Make a stat for spell cooldown? for jewelry pieces to effect
// Think more about how we want stats to be effected by what slots?

// TODO Think about making some sort of day and night cycle...
// Early ideas are putting a transparent rectangle ontop of the screen that slowly changes color and transparency??
// Def would need to think about shadows maybe?
// Maybe adding in 'Light sources' would be cool

// TODO Create two functions for the rendering of melee weapons and wands
// Make sure to include them when calling swing/cast 
// Each one will need a timer for rendering the image of the weapon
// Melee weapon will need a swing while its being rendered
// Make sure to use the vector from player->cursor when rendering to have it look like its 'rotated' properly

// TODO Make weapon class for real ;-;
// Weapon should rotate around player fixed distance
// Weapon should have a facing direction based off of mouse on right or left
// Weapon should swing
// On swing we should create the hitbox where the weapon is...BE GENEROUS

// TODO Break up entity class so enemys can be prefabs and same with npcs

// TODO Add npc interaction into each scene individually so we can force certain interactions
// Maybe add in an extra button press to accept a 'quest' so that you cant spam through and miss instructions
// Dunno if we really need this tbh, with ActiveQuest in now

// TODO Figure out how to make a local state for popup text function so that we can have multiple of them on the screen at once
// The real issue here is this needs to be called within an area that we are already calling Mode3D
// We also need to make sure that the text doesnt get updated with say another ones data I.E
// If we attack one tree and its health it 20/40 and then quickly attack another and its health is 10/40
// We need to properly display both health values in the text without the second one updating the first

// TODO Fix our structure ids for trees and mine-able stone at least
// Do we need another enum class for resource vs non-resource?
// should be using that id later to give resources

// TODO Fix all collider creations to use colliderMin inside of colliderMax, refactor how we calculate our sizes (HOUSE CLASS NEEDS THIS CURRENTLY)
// Probably need to fix this with entity class once we refactor it whenever that will be

// TODO When doing quest system, create a pointer to the active quest to display it in sceneManager
// Might not do this, could have something more simple instead...idk

// TODO Minimize the amount of times we draw static things
// Wait, since the screen is cleared, can we even do this?

// TODO Create player textures
// Resize player && || Trees for better sizes

// TODO New main menu artwork :D add in new trees and hopefully new enemy models

// TODO Think about how to add enemy sounds,
// How would we have multiple enemy audios in a scene with the easy method though?
// Option 1.) EASY - Add a sound parameter to enemy class for audio
// 		  1a.) Create static sounds in scene loading and pass by reference
// Option 2.) HARD - Break-up entity class like items and projectiles
//		  2a.) Create enemy prefabs that come with their own sounds and stats

// TODO Think about our wall situation
// Make Bottom wall half height roughly, little bit taller than map offset
// Needs to be billboard/entity, could make a new class for it
// Needs a textures that will stretch nicely
// Needs to look good from ORTHOGRPAHIC and PERSPECTIVE

void LoadGlobalTexturesAndAudio();

int main(){
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(G_VARS.WIDTH, G_VARS.HEIGHT, "Knight's Journey!");
	InitAudioDevice();
	SetTargetFPS(60);
	SetExitKey(KEY_NULL);

	G_VARS.MONITOR = GetCurrentMonitor();
	G_VARS.FULLSCREEN_WIDTH = GetMonitorWidth(G_VARS.MONITOR);
	G_VARS.FULLSCREEN_HEIGHT = GetMonitorHeight(G_VARS.MONITOR);

	LoadGlobalTexturesAndAudio();

	// Player Variables
	Texture2D playerTex = LoadTexture("classes/player/artwork/player_idle.png");
	// Texture2D playerTex = LoadTexture("classes/player/artwork/Phin_Player_W.png");
	Vector3 playerPos = { 0.0f, 1.0f, 4.9f };

	// Player Instantiation
	Player* player = new Player(playerTex, G_VARS.PLAYER_SIZE, playerPos, G_VARS.RIGHT, true, G_VARS.PLAYER_ID);

	//-------------------------------------SCENE SETUP---------------------------------
	Scene::SetPlayer(player);
	Projectile::SetPlayer(player);

	SceneManager::GetInstance().ChangeScene(G_VARS.MAIN_MENU);

//-------------------------------------MAIN LOOP-----------------------------------
	while(G_VARS.RUNNING){
		float dT = GetFrameTime();

		if(WindowShouldClose()){
			G_VARS.RUNNING = false;

		}

		// Update
		SceneManager::GetInstance().Update(dT);

		// Draw
		SceneManager::GetInstance().Draw();

	}

	CloseAudioDevice();

	CloseWindow();

}

void LoadGlobalTexturesAndAudio(){
	// These are 'Permanent' member variables that are used through the game lifetime
// Create a function/manager class to load in all of these textures{
// Weapons
	WoodenSword::m_WoodenSword = LoadTexture("classes/weapons/artwork/woodSword.png");

// Spells
	// Fireball
	Fireball::m_Fireball = LoadTexture("classes/projectiles/artwork/fireball_animated.png");
	Fireball::m_FireballSound = LoadSound("audio/spells/fireball.wav");
	Fireball::m_FireballExplosionSound = LoadSound("audio/spells/fireball_explosion.wav");

	// ForstOrb
	FrostOrb::m_FrostOrb = LoadTexture("classes/projectiles/artwork/frostorb_animated.png");
	FrostOrb::m_FrostOrbSound = LoadSound("audio/spells/frostorb.wav");
	FrostOrb::m_FrostOrbHitSound = LoadSound("audio/spells/frostorb_hit.wav");

	// LightningBolt
	LightningBolt::m_LightningBolt = LoadTexture("classes/projectiles/artwork/lightningbolt_animated.png");
	LightningBolt::m_LightningBoltSound = LoadSound("audio/spells/lightningorb.wav");
	LightningBolt::m_LightningBoltZapSound = LoadSound("audio/spells/lightningbolt_zap.wav");

	// Items
	HealthPotion::m_HealthPotionTex = LoadTexture("classes/items/artwork/healthPotion.png");
	ManaPotion::m_ManaPotionTex = LoadTexture("classes/items/artwork/manaPotion.png");

	// SceneManager
	SceneManager::m_ItemCardTex = LoadTexture("classes/utilities/artwork/ui/ItemCardBackground.png");
	SceneManager::m_InventoryTex = LoadTexture("classes/utilities/artwork/ui/InventoryBackground.png");
	SceneManager::m_DialogueTex = LoadTexture("classes/utilities/artwork/ui/DialogueBackground.png");
	SceneManager::m_ActionBarTex = LoadTexture("classes/utilities/artwork/ui/ActionBarBackground.png");

	// Scene
	Scene::m_PickUpAudio = LoadSound("audio/interaction/item_pickup.wav");

	// Level Loader
	LevelLoader::m_TransitionSound = LoadSound("audio/levelChange/levelExit.wav");

	// Player 
	// TODO add new sounds for using/equipping items
	Player::m_InventoryOpen = LoadSound("audio/ui/inventory_open.wav");
	Player::m_InventoryClose = LoadSound("audio/ui/inventory_close.wav");
	Player::m_WalkingSound = LoadSound("audio/player/walking.wav");
	Player::m_RunningSound = LoadSound("audio/player/running.wav");

	// GLOBAL STATIC VARIABLE
	G_VARS.FONT = LoadFont("font/ladyRadical.ttf");

//}
}