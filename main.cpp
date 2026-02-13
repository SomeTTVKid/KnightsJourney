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

// TODO NEXT

// TODO Move our signs into the structure class

// TODO Add npc interaction into each scene individually so we can force certain interactions
// Maybe add in an extra button press to accept a 'quest' so that you cant spam through and miss instructions
// Dunno if we really need this tbh, with ActiveQuest in now

// TODO Figure out how to make a local state for popup text function so that we can have multiple of them on the screen at once
// The real issue here is this needs to be called within an area that we are already calling Mode3D
// We also need to make sure that the text doesnt get updated with say another ones data I.E
// If we attack one tree and its health it 20/40 and then quickly attack another and its health is 10/40
// We need to properly display both health values in the text without the second one updating the first

// TODO Remove public member variables from player class and add in getters/setters

// TODO Fix our structure ids for trees and mine-able stone at least
// should be using that id later to give resources

// TODO Make size a default parameter inside of structure params, make it {1.0f, 1.0f};

// TODO Fix all collider creations to use colliderMin inside of colliderMax, refactor how we calculate our sizes
// Probably need to fix this with entity class once we refactor it whenever that will be

// TODO Create campfire structure class, move all artwork out of main artwork file

// TODO Cleanup artwork folder
// Add artwork folders to the classes

// TODO When doing quest system, create a pointer to the active quest to display it in sceneManager

// TODO Minimize the amount of times we draw static things
// Wait, since the screen is cleared, can we even do this?

// TODO Create player textures
// Resize player && || Trees for better sizes

// TODO New main menu artwork :D add in new trees and hopefully new enemy models

// TODO Make weapon class for real ;-;
// Weapon should rotate around player fixed distance
// Weapon should have a facing direction based off of mouse on right or left
// Weapon should swing
// On swing we should create the hitbox where the weapon is...BE GENEROUS

// TODO Break up entity class so enemys can be prefabs and same with npcs

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

// WHEN ADDING A NEW SCENE
// Create new scene header and cpp file
// Add new scene in enum class inside of global variables
// Add new switch case statement inside of scenemanager.cpp, Line 56. ChangeScene()
// Fill out new scene with functions

int main(){
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(G_VARS.WIDTH, G_VARS.HEIGHT, "Knight's Journey!");
	InitAudioDevice();
	SetTargetFPS(60);
	SetExitKey(KEY_NULL);

	G_VARS.MONITOR = GetCurrentMonitor();
	G_VARS.FULLSCREEN_WIDTH = GetMonitorWidth(G_VARS.MONITOR);
	G_VARS.FULLSCREEN_HEIGHT = GetMonitorHeight(G_VARS.MONITOR);

// These are 'Permanent' member variables that are used through the game lifetime
// Create a function/manager class to load in all of these textures{
// Weapons
	WoodenSword::m_WoodenSword = LoadTexture("classes/weapons/artwork/woodSword.png");

// Spells
	// Fireball
	Fireball::m_Fireball = LoadTexture("classes/projectiles/artwork/fireball_animated.png");
	Fireball::m_FireballSound = LoadSound("audio/fireball.wav");
	Fireball::m_FireballExplosionSound = LoadSound("audio/fireball_explosion.wav");

	// ForstOrb
	FrostOrb::m_FrostOrb = LoadTexture("classes/projectiles/artwork/frostorb_animated.png");
	FrostOrb::m_FrostOrbSound = LoadSound("audio/frostorb.wav");
	FrostOrb::m_FrostOrbHitSound = LoadSound("audio/frostorb_hit.wav");

	// LightningBolt
	LightningBolt::m_LightningBolt = LoadTexture("classes/projectiles/artwork/lightningbolt_animated.png");
	LightningBolt::m_LightningBoltSound = LoadSound("audio/lightningorb.wav");
	LightningBolt::m_LightningBoltZapSound = LoadSound("audio/lightningbolt_zap.wav");

	// Items
	HealthPotion::m_HealthPotionTex = LoadTexture("classes/items/artwork/healthPotion.png");
	ManaPotion::m_ManaPotionTex = LoadTexture("classes/items/artwork/manaPotion.png");

	// SceneManager
	SceneManager::m_ItemCardTex = LoadTexture("classes/utilities/artwork/ItemCardBackground.png");
	SceneManager::m_InventoryTex = LoadTexture("classes/utilities/artwork/InventoryBackground.png");
	SceneManager::m_DialogueTex = LoadTexture("classes/utilities/artwork/DialogueBackground.png");
	SceneManager::m_ActionBarTex = LoadTexture("classes/utilities/artwork/ActionBarBackground.png");

	// Scene
	Scene::m_PickUpAudio = LoadSound("audio/item_pickup.wav");

	// Level Loader
	LevelLoader::m_TransitionSound = LoadSound("audio/levelExit.wav");

	// Player 
	// TODO add new sounds for using/equipping items
	Player::m_InventoryOpen = LoadSound("audio/inventory_open.wav");
	Player::m_InventoryClose = LoadSound("audio/inventory_close.wav");
	Player::m_WalkingSound = LoadSound("audio/walking.wav");
	Player::m_RunningSound = LoadSound("audio/running.wav");

	// GLOBAL STATIC VARIABLE
	G_VARS.FONT = LoadFont("font/ladyRadical.ttf");

//}
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