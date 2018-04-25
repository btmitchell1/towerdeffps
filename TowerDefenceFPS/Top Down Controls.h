#pragma once

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <deque>
#include <memory>
#include <iostream>
#include <sstream>
#include <stdlib.h> // General console window includes
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include "_GameControls.h"
#include "FPS.h"

using namespace tle;

#include <Audio.hpp>



// Tower //

// Tile
const float kCurrentMoveSpeed = 10.0f;

// UI
const int kMouseBoundaryX = 640;
const int kMouseBoundaryY = 360;


// BALANCE CONSTANTS //
const int kStartingBalance = 500;

// Towers
// Level 1
const int kTower1Cost = -100;
const int kTower1Sale = 80;
// Level2
const int kTower2Cost = -125;
const int kTower2Sale = 100;

// Walls
// Level 1
const int kWall1Cost = -75;
const int kWall1Sale = 50;



///////////////////
//// AUDIO CLASS //
///////////////////

/////////////////////////////////////////

////////////////////
// BUILDING CLASS //
////////////////////
class CBuilding {
private:
	IModel * mModel;
	IMesh* mMesh;
	IModel* mDummy;
	IModel* mAmmo;
	EBuildingType mType;
	ETowerState mState;
	float mX;
	float mZ;

	float mDropMutliplier;

	float mAmmoTimer;
public:
	CBuilding();

	void CreateModel(float, float, EBuildingType, IMesh*, IMesh*, IMesh*);
	void UpgradeBuilding(IMesh*, float, float);
	void SellBuilding(IMesh*);

	//getters
	IModel* GetModel();
	IModel* GetDummy();
	EBuildingType GetType();
	float GetX();
	float GetZ();
	ETowerState GetState();


	//setters
	void SetType(EBuildingType Type) { mType = Type; };
	void SetModel(IModel* Model) { mModel = Model; };

	void PlaySound();

	void ChangeState(ETowerState);

	void AnimBuy(float);

	bool EnemyInRange(float, float);

	void Attack(vector <enemy>, float);
};



//////////////////
// PLAYER CLASS //
//////////////////
class CPlayer {
private:
	int mBalance;
public:
	CPlayer(int);
	void ChangeBalance(int);
	int GetBalance();
};