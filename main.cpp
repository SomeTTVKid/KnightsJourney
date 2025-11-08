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

// TODO Redo tree colliders to have two colliders
// One collision collider and one interact collider
// Use interact collider for raycasting to turn transparent && Woodcutting
// Give different trees different sized colliders, Spruce needs to be bigger so far

// TODO Clamp UI to a max size when scaling it, maybe just clamp WIDTH/HEIGHT_SCALE?

// TODO Scale interact text and fix y position when scaling it

// TODO Fix rock bounding box, make it bigger in general

// TODO Move mapoffset into base scene class

// TODO Rename loader position and loader out position to be less vague

// TODO Minimize the amount of times we draw static things
// Wait, since the screen is cleared, can we even do this?

// TODO In update replenish resource health slowly
// When a resource health is below or equal to 0
// Set a state in structure called m_Replenishing
// Check when attacking if the structures state is true or false
// Only allow attacking if false
// In update check if state is true
// If so add back health * dT and clamp at maxHealthValue
// Reset texture

// TODO Create player textures
// Resize player && || Trees for better sizes

// TODO In structures add switch statement to constructor
// Check for tree id 
// Set smaller colliders 

// TODO Also in structures switch for rocks
// Future proof for different rocks...
// Make z depth for collider bigger, for percieved rock size

// TODO New main menu artwork :D add in new trees and hopefully new enemy models

// TODO Make weapon class for real ;-;
// Weapon should rotate around player fixed distance
// Weapon should have a facing direction based off of mouse on right or left
// Weapon should swing
// On swing we should create the hitbox where the weapon is...BE GENEROUS

// TODO Cleanup artwork folder
// Add artwork folders to the classes

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
	SceneManager::m_ItemCardTex = LoadTexture("artwork/ItemCardBackground.png");
	SceneManager::m_InventoryTex = LoadTexture("artwork/InventoryBackground.png");
	SceneManager::m_DialogueTex = LoadTexture("artwork/DialogueBackground.png");
	SceneManager::m_ActionBarTex = LoadTexture("artwork/ActionBarBackground.png");

	// Scene
	Scene::m_PickUpAudio = LoadSound("audio/item_pickup.wav");

	// Level Loader
	LevelLoader::m_TransitionSound = LoadSound("audio/levelExit.wav");

	// Player 
	Player::m_InventoryOpen = LoadSound("audio/inventory_open.wav");
	Player::m_InventoryClose = LoadSound("audio/inventory_close.wav");
	Player::m_WalkingSound = LoadSound("audio/walking.wav");
	Player::m_RunningSound = LoadSound("audio/running.wav");

	// GLOBAL STATIC VARIABLE
	G_VARS.FONT = LoadFont("font/ladyRadical.ttf");

//}
	// Player Variables
	Texture2D playerTex = LoadTexture("classes/player/artwork/player_idle.png");
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