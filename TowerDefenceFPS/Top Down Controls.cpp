#include "Top Down Controls.h"

// A "buffer" holds sound data, but the creation of a buffer on its own doesn't play a sound. It is the
// equivalent of a mesh in the TL-Engine
sf::SoundBuffer towerBuffer;


// A "sound" is an actual sound in the world. A sound must be associated with a buffer to indicate
// which sound data to play. Sources are equivalent to models in the TL-Engine
sf::Sound towerSound;

// Each sound source has several properties, see the code for examples. Here we store position and 
// velocity of the sound source above, using a SFML class Vector3f to hold the xyz values
// The velocity is to move the sound around and also for the Doppler effect. In fact SFML does not
// support Doppler, so the velocity is just for our own purposes. Look at the "OpenAL Soft" library
// if you want to do Doppler in your own projects
sf::Vector3f towerSoundPos(0.0, 0.0, 0.0);
//sf::Vector3f towerSoundVelocity(0.0, 0.0, 0.0);


// There is always assumed to be a listener in the wold although we don't need a specific listener variable.
// Listeners also have properties (examples in code). Here we store the listener's position
sf::Vector3f towerListenerPos(0.0, 0.0, 0.0);

// The listener may be at an angle (which may affect the perception of sound). Here we store the 
// orientation of the listener by specifying their facing direction and upward direction.
// These xyz vectors can be extracted from a world or view matrix
// NOTE: SFML uses a right-handed system for 3D coordinates. To convert from the left-handed system
// we normally use, we must negate all Z values (facing direction has -ve Z below). Without this
// change the sounds from the left and right speakers will be the wrong way round
sf::Vector3f towerListenerForward(0.0, 0.0, -1.0);
sf::Vector3f towerListenerUp(0.0, 1.0, 0.0);

///////////////////
//// AUDIO CLASS //
///////////////////


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

///////////
// AUDIO //
///////////
void CBuilding::PlaySound()
{
	if (!towerBuffer.loadFromFile("drop.wav"))
	{
		cout << "Error loading sound file" << endl;
		while (!_kbhit());
		return;
	}


	//****************
	// Sources

	// Indicate that our sound source will use the buffer we just loaded
	towerSound.setBuffer(towerBuffer);

	// Set the properties of the source. Details of all available properties are in the SFML documentation of the Sound class
	towerSound.setVolume(100.0f); // 0 to 100
	towerSound.setPitch(1.0f);
	towerSound.setLoop(false);
	towerSound.setPosition(towerSoundPos);


	//****************
	// Listener

	// Set the properties of the listener. These are all the available listener properties
	// Note how this is doen with static member functions - there is no listener variable
	sf::Listener::setGlobalVolume(100.0f); // 0 to 100
	sf::Listener::setPosition(towerListenerPos);
	sf::Listener::setDirection(towerListenerForward);
	sf::Listener::setUpVector(towerListenerUp);

	towerSound.play();
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
	else if (towerHeight <= kFloorHeight && mState == building)
	{
		mModel->SetY(kFloorHeight);
		if (mType == tower1)
		{
			mAmmo->SetY(kFloorHeight + 5.0f);
		}
		mState = built;

		PlaySound();
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

void CBuilding::Attack(vector <enemy> enemyList, float frameTime)
{
	for (auto& elt : enemyList)
	{
		float enemyX = (elt).model->GetX();
		float enemyZ = (elt).model->GetZ();
		bool shoot = this->EnemyInRange(enemyX, enemyZ);
		if (shoot)
		{
			mAmmo->LookAt((elt).model);
			mAmmo->MoveLocalZ(100.0f * frameTime);

			bool hit = SphereSphereCD(mAmmo->GetX(), mAmmo->GetZ(), 1.0f,
				enemyX, enemyZ, (elt).diameter);

			if (hit || mAmmoTimer > 3.0f)
			{
				mAmmo->SetPosition(mX, kFloorHeight + 5.0f, mZ);
				if (mAmmoTimer > 3.0f)
				{
					mAmmoTimer = 0.0f;
				}
			}
			break;
		}
	}

	mAmmoTimer += frameTime;
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
