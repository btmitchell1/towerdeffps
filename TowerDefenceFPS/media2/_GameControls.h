#pragma once


#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

using namespace std;
using namespace tle;

const enum gameType { start, topDown, fps, paused, end };
const enum Types { Wall, Clear, Wood, Water };
const enum EBuildingType { none, tower1, tower2, wall };
const enum ETowerState { null, building, built };

struct Node
{
	int x, y, ManDistance = 0, Weight = 0, TotalScore = 0;
	shared_ptr<Node> Parent;
};

//wasd
//Player Controls
const EKeyCode kKeyMoveUp = Key_W;
const EKeyCode kKeyMoveDown = Key_S;
const EKeyCode kKeyMoveLeft = Key_A;
const EKeyCode kKeyMoveRight = Key_D;
const EKeyCode kKeyQuit = Key_Escape;

const EKeyCode kKeyTower = Key_1;
const EKeyCode kKeyWall = Key_2;
const EKeyCode kKeyUpgrade = Key_3;
const EKeyCode kKeySell = Key_4;
const EKeyCode kKeyContinue = Key_5;
const EKeyCode kKeySelect = Mouse_LButton;
const int kBuildTowerButton = 1;
const int kBuildWallButton = 2;
const int kUpgradeBuildingButton = 3;
const int kSellBuildingButton = 4;
const int kContinueButton = 5;

// Camera Constants
const int kSizeX = 10;
const int kSizeZ = 10;
const ECameraType kCameraType = kManual;
const float kCameraX = 40.0f;
const float kCameraY = 400.0f;
const float kCameraZ = 12.0f;
const float kCameraTilt = 75.0f;

// World
const float kFloorHeight = 0.0f;
const float scale = 5.0f;
const int nBoundaryX = 0;
const int nBoundaryZ = 0;
const int pBoundaryX = 9;
const int pBoundaryZ = 9;

// Tower Select Tile
const float kHideY = 1000.0f; // Length in Y that the tile moves when switching modes

// Tower //
// Animation
const float kTowerDropHeight = 500.0f;
const float kTowerDropSpeed = 800.0f;
// Attack
const int kTowerDamage = 10;
const float kTowerAttackSpeed = 1.0f;
const float kTowerAttackRadius = 200.0f;

// Enemy //
const float kEnemyRadius = 1.0f;


//Map
const int gMapWidth = 10;
const int gMapHeight = 10;

bool GetMap(int MapArray[gMapWidth][gMapHeight], string MapFile, shared_ptr<Node> Start, shared_ptr<Node> Goal);
void DisplayMap(int a[gMapWidth][gMapHeight]);
