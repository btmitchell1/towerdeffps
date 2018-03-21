#pragma once

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <deque>
#include "_GameControls.h"
#include "AstarFun.h"
using namespace tle;

const float kFpsStartCoords[3] = { 10.0f * scale * 9.0f, 10.0f, 10.0f * scale * 9.0f };
const float kCameraSensitivity = 2.0f;

const float kProjectileSpeed = 10.0f;
const float kRateOfFire = 0.15f;
const int kClipSize = 20;

struct enemy
{
	IModel* model;
	float diameter;
	int health;
	int damage;
	float speed;
	bool EndReached = false;
	int PatrolRoute = 0;
};

bool pointToSphereCollision(IModel* object1, IModel* object2, int radius);
