#pragma once

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <deque>
#include <sstream>
#include "_GameControls.h"
#include "AstarFun.h"

using namespace tle;

const float kFpsStartCoords[3] = { 10.0f * scale * 0.0f, 12.0f, 10.0f * scale * 20.0f };
const float kCameraSensitivity = 2.0f;

const float kProjectileSpeed = 10.0f;
const float kRateOfFire = 0.15f;
const int kClipSize = 20;

class enemy
{
private:

	IModel* mModel;
	float mDiameter;
	int mHealth;
	int mDamage;
	float mSpeed;
	bool mEndReached = false;
	int mPatrolRoute = 0;

public:

	//Getters 
	virtual IModel* GetModel() { return mModel; };
	virtual float GetDiameter() { return mDiameter; };
	virtual int GetHealth() { return mHealth;  };
	virtual int GetDamage() { return mDamage; };
	virtual float GetSpeed() { return mSpeed;  };
	virtual bool GetEndReached() { return mEndReached;  };
	virtual int &GetPatrolRoute() { return mPatrolRoute; };

	//Setters
	virtual void SetModel(IModel* Model) { mModel = Model; };
	virtual void SetDiameter(float Diameter) { mDiameter = Diameter;};
	virtual void SetHealth(int Health) { mHealth = Health;};
	virtual void SetDamage(int Damage) { mDamage = Damage;};
	virtual void SetSpeed(float Speed) { mSpeed = Speed;};
	virtual void SetEndReached(bool EndReached) { mEndReached = EndReached;};
	virtual void SetPatrolRoute(int PatrolRoute) { mPatrolRoute = PatrolRoute;};
};


class Tank : public enemy
{
public:
	Tank::Tank()
	{
		SetDiameter(20); 
		SetHealth(20); 
		SetDamage(2); 
		SetSpeed(20);  
	};
	
};

bool pointToSphereCollision(IModel* object1, IModel* object2, int radius);
