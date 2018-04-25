#include "Top Down Controls.h"

CBuilding::CBuilding()
{
	mType = none;
	mState = null;
}

void CBuilding::CreateModel(float x, float z, EBuildingType type, IMesh* mesh, IMesh* dummyMesh, IMesh* ammoMesh)
{
	mX = x;
	mZ = z;

	mMesh = mesh;
	mModel = mesh->CreateModel(x, kTowerDropHeight, z);
	mDummy = dummyMesh->CreateModel(x, kFloorHeight, z);
	mAmmo = ammoMesh->CreateModel(x, -kHideY, z);

	mType = type;

	mState = building;

	mDropMutliplier = 1.0f;

	mAmmoTimer = 0.0f;
}

void CBuilding::UpgradeBuilding(IMesh* newMesh, float x, float z)
{
	// Balance check and Found occur outside class
	if (mType == tower1)
	{
		mMesh->RemoveModel(mModel);
		mType = tower2;
		mModel = newMesh->CreateModel(x, kFloorHeight, z);

		mMesh = newMesh;
	}
}

void CBuilding::SellBuilding(IMesh* ammoMesh)
{
	mMesh->RemoveModel(mModel);
	ammoMesh->RemoveModel(mAmmo);
	
	mType = none;

	mState = null;

	mType = EBuildingType::none;
}

/////////////
// GETTERS //
/////////////

IModel* CBuilding::GetModel()
{
	return mModel;
}

IModel* CBuilding::GetDummy()
{
	return mDummy;
}

EBuildingType CBuilding::GetType()
{
	return mType;
}

float CBuilding::GetX()
{
	return mX;
}

float CBuilding::GetZ()
{
	return mZ;
}

ETowerState CBuilding::GetState()
{
	return mState;
}

/////////////
// SETTERS //
/////////////

void CBuilding::ChangeState(ETowerState state)
{
	mState = state;
}

///////////////
// ANIMATION //
///////////////

void CBuilding::AnimBuy(float frameTime)
{
	float towerHeight = mModel->GetY();
	float distance = towerHeight - kFloorHeight;

	if (towerHeight >= kFloorHeight && mState == building)
	{
		mModel->MoveY(-kTowerDropSpeed * frameTime * mDropMutliplier);
		if (mModel->GetY() < 200.0f)
		{
			if (mDropMutliplier > 0.1f)
			mDropMutliplier -= 0.004f;
		}
	}
	else
	{
		mModel->SetY(kFloorHeight);
		if (mType == tower1)
		{
			mAmmo->SetY(kFloorHeight + 5.0f);
		}
		mState = built;
		//towerSound.play();
	}
}

// Methods
bool Found(CBuilding* array[gMapWidth][gMapHeight], int x, int z);

bool SphereSphereCD(float sphere1X, float sphere1Z, float sphere1Radius,
	float sphere2X, float sphere2Z, float sphere2Radius);
//////////////////
// TOWER ATTACK //
//////////////////

bool CBuilding::EnemyInRange(float eX, float eZ)
{
	if (mState == built && (mType == tower1 || mType == tower2))
	{
		return SphereSphereCD(mX, mZ, kTowerAttackRadius,
			eX, eZ, kEnemyRadius);
	}

	return false;
}

void CBuilding::Attack(IModel* enemy, float frameTime)
{
	mAmmo->LookAt(enemy);
	mAmmo->MoveLocalZ(100.0f * frameTime);

	bool hit = SphereSphereCD(mAmmo->GetX(), mAmmo->GetZ(), 1.0f,
		enemy->GetX(), enemy->GetZ(), 2.0f);

	if (mAmmoTimer > 3.0f || hit)
	{
		mAmmo->SetPosition(mX, kFloorHeight + 5.0f, mZ);
		mAmmoTimer = 0.0f;
	}
	else
	{
		mAmmoTimer += frameTime;
	}
}

///////////////////////////////////////


CPlayer::CPlayer(int balance)
{
	mBalance = balance;
}

void CPlayer::ChangeBalance(int balanceChange)
{
	mBalance += balanceChange;
}

int CPlayer::GetBalance()
{
	return mBalance;
}
///////////////////////////////////////



///////////
// AUDIO //
///////////
//sf::SoundBuffer buffer;
