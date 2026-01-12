#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <algorithm>

struct GlobalVariables{
	enum SCENEIDS{
		MAIN_MENU,
		LEVEL_01,
		LEVEL_02,
		LEVEL_03,
		BSINTERIOR
	};

	enum class StructureIDs{
		TREE_ID,
		STONE_ID,
		GRASS_ID,
		BUILDING_ID,
		ORE_ID
	};

	std::vector<std::string> KEYBINDS{
		{"'E' To Interact"},
		{"'TAB' To Open Inventory"},
		{"'LMB or G' To Attack"},
		{"'RMB' To Cast Spell"},
		{"'1' To Switch Spells"},
		{"'F' To Toggle Fullscreen"},
		{"'~' To Toggle Debug-Mode"},
		{"'Esc' To Pause Game AND Leave Dialogue"},
		{"'M' To Print Player Coordinates"},
		{"'2' To Equip Axe"},
		{"'3' To Increase Axe Tier"}
	};

	// States
	bool FULLSCREEN = false;
	bool FIRST_PERSON = false;
	bool DEBUG_MODE = false;
	bool IS_PAUSED = false;
	bool RUNNING = true;
	bool DISPLAY_TEXT = false;
	bool IN_DIALOGUE = false;
	bool INVENTORY_OPEN = false;
	bool ITEM_SELECTED = false;
	bool CHARACTER_PANEL = false;
	bool DYSLEXIC = false;
	bool POPUP_TEXT = false;

	// Skin States
	bool UPDATE_SKIN = false;
	bool IS_KNIGHT = true;
	bool IS_CHICKEN = false;

	// Screen
	int MONITOR;
	int BASE_WIDTH = 800;
	int BASE_HEIGHT = 600;
	int WIDTH = BASE_WIDTH;
	int HEIGHT = BASE_HEIGHT;
	int FULLSCREEN_WIDTH;
	int FULLSCREEN_HEIGHT;
	float WIDTH_SCALE = WIDTH / 1000.0f;
	float HEIGHT_SCALE = HEIGHT / 1000.0f;

	// Id
	int PLAYER_ID = 0;
	int ENEMY_ID = 1;
	int NPC_ID = 2;
	int STRUCTURE_ID = 3;
	int PROJECTILE_ID = 4;

	// Texture facing direction
	float RIGHT = 1.0f;
	float LEFT = -1.0f;

	// Audio
	float VOLUME = 0.1f;

	// Game Speed
	float TICK_SPEED = 1.0f;

	// Font
	static inline Font FONT{};
	float DEFAULT_FONT_SPACING = 1.0f;
	float FONT_SPACING = DEFAULT_FONT_SPACING;
	float FONT_DYSLEXIC_SPACING = 2.5f;

	// Text
	std::string INTERACT_TEXT = "Press 'E' To Interact";

	// Entity Sizes
	// TODO At some point remove these variables and only use prefabs
	// Ez, pass size in param by default instead of by reference?
	// Then we could always pass whatever we want whenever
	Vector2 PLAYER_SIZE = { 1.0f, 1.0f};
	Vector2 KNIGHT_SIZE = { 0.95f, 0.95f };
	Vector2 GOBLIN_SIZE = { 0.8f, 0.8f };
	Vector2 CHICKEN_SIZE = { 0.5f, 0.5f };
	Vector2 NPC_SIZE = { 0.95f, 0.95f };
	Vector2 TREE_SIZE = { 2.5f, 3.5f };
	Vector2 GRASS01_SIZE = { 0.5f, 0.5f };
	Vector2 HOUSE_SIZE = { 3.5f, 2.3f };
	Vector2 TALL_BUILDING_SIZE = { 3.5f, 2.9f };
	Vector2 THIN_BUILDING_SIZE = { 2.5f, 2.3f };
	Vector2 CAMPFIRE_SIZE = { 1.0f, 1.0f };
	Vector2 SIGN_SIZE = { 1.0f, 1.0f };
	Vector2 LOADER_SIZE = { 1.0f, 0.85f };
	Vector2 ROCK_SIZE = { 2.0f, 1.8f };
	Vector2 SMALL_ROCK_SIZE = { 0.65f, 0.65f };
	Vector2 POTION_SIZE = { 0.5f, 0.5f };

};

extern GlobalVariables G_VARS;