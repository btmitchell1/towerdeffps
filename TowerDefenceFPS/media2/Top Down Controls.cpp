// Top Down Controls.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <deque>
#include <memory>
#include <iostream>
#include <sstream>
#include <stdlib.h> // General console window includes
#include <conio.h>
#include <ctype.h>
#include <time.h>

using namespace tle;

//#include <Audio.hpp>

const enum EMouseCapture { on, off };
const enum EMousePosition { uninit, mUp, mDown, mLeft, mRight };
const enum EBuildingType { none, tower1, tower2, wall };
const enum ETowerState { null, building, built };

// Controls
const EKeyCode kKeyMoveUp = Key_W;
const EKeyCode kKeyMoveDown = Key_S;
const EKeyCode kKeyMoveLeft = Key_A;
const EKeyCode kKeyMoveRight = Key_D;
const EKeyCode kKeyCreate = Mouse_LButton;
const EKeyCode kKeyUpgrade = Mouse_RButton;
const EKeyCode kKeyQuit = Key_Escape;

// Camera Constants
const int kSizeX = 10;
const int kSizeZ = 10;
const ECameraType kCameraType = kManual;
const float kCameraX = 40.0f;
const float kCameraY = 400.0f;
const float kCameraZ = 12.0f;
const float kCameraTilt = 75.0f;

// World
const float kFloorHeight = 24.5f;
const float scale = 50.0f;
const float nBoundaryX = 0.0f;
const float nBoundaryZ = 0.0f;
const float pBoundaryX = kSizeX * scale;
const float pBoundaryZ = kSizeZ * scale;

// Tower //
// Animation
const float kTowerDropHeight = 300.0f;
const float kTowerDropSpeed = 400.0f;
// Attack
const int kTowerDamage = 10;
const float kTowerAttackSpeed = 1.0f;
const float kTowerAttackRadius = 200.0f;

// Enemy //
const float kEnemyRadius = 1.0f;

// Tile
const float kCurrentMoveSpeed = 50.0f;

// UI
const int kMouseBoundaryX = 640;
const int kMouseBoundaryY = 360;
const float kMenuPosX = 0.0f;
const float kMenuPosY = -200.0f;

///////////////////
//// AUDIO CLASS //
///////////////////
//sf::SoundBuffer towerBuffer;
//sf::Sound towerSound;
//sf::Vector3f towerSoundPos(0.0, 0.0, 0.0);
//sf::Vector3f towerVelocity(0.0, 0.0, 0.0);
//sf::Vector3f listenerPos(0.0, 0.0, 0.0);
//sf::Vector3f listenerForward(0.0, 0.0, -1.0);
//sf::Vector3f listenerUp(0.0, 1.0, 0.0);
/////////////////////////////////////////

////////////////////
// BUILDING CLASS //
////////////////////
class CBuilding {
private:
	IModel* mModel;
	IMesh* mMesh;
	IModel* mDummy;
	IModel* mAmmo;
	EBuildingType mType;
	ETowerState mState;
	float mX;
	float mZ;
public:
	CBuilding();

	void CreateModel(float, float, EBuildingType, IMesh*, IMesh*, IMesh*);
	void UpgradeBuilding(IMesh*, float, float);
	void SellBuilding();

	IModel* GetModel();
	IModel* GetDummy();
	EBuildingType GetType();
	float GetX();
	float GetZ();
	ETowerState GetState();

	void ChangeState(ETowerState);

	void AnimBuy(float);

	bool EnemyInRange(float, float);

	void Attack(IModel*, float);
};

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
	mAmmo = ammoMesh->CreateModel(x, kFloorHeight, z);

	mType = type;

	mState = building;
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

void CBuilding::SellBuilding()
{
	mMesh->RemoveModel(mModel);
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

	if (towerHeight >= kFloorHeight)
	{
		mModel->MoveY(-kTowerDropSpeed * frameTime);
	}
	else
	{
		mState = built;
		//towerSound.play();
	}
}

// Methods
bool Found(CBuilding* array[kSizeX][kSizeZ], int x, int z);

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
	mAmmo->MoveLocalZ(70.0f * frameTime);

	bool hit = SphereSphereCD(mAmmo->GetX(), mAmmo->GetZ(), 1.0f,
		enemy->GetX(), enemy->GetZ(), 5.0f);

	if (hit)
	{
		mAmmo->SetPosition(mX, kFloorHeight, mZ);
	}
}

///////////////////////////////////////

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

/////////////////
// ENEMY CLASS //
/////////////////
class CEnemy {
private:
	IModel * mModel;
	IMesh* mMesh;
public:
	void CreateModel(IMesh*, float, float);
};

void CEnemy::CreateModel(IMesh* mesh, float x, float z)
{
	mMesh = mesh;

	mModel = mesh->CreateModel(x, kTowerDropHeight, z);
}
/////////////////////////////////////////

///////////
// AUDIO //
///////////
//sf::SoundBuffer buffer;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	myEngine->AddMediaFolder("./media2");
	myEngine->AddMediaFolder("./media/x");
	myEngine->AddMediaFolder("./media/p");
	myEngine->AddMediaFolder("./media");

	/**** Set up your scene here ****/

	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);
	////////////
	// Camera //
	////////////
	ICamera* camera = myEngine->CreateCamera(kManual, kCameraX, kCameraY, kCameraZ);
	camera->RotateLocalX(kCameraTilt);

	////////////
	// Models //
	////////////
	IModel* FloorModel;
	IMesh* FloorMesh = myEngine->LoadMesh("Floor.x");
	FloorModel = FloorMesh->CreateModel(0.0f, kFloorHeight, 0.0f);

	IMesh* Tower1Mesh = myEngine->LoadMesh("turret.x");

	IMesh* Tower2Mesh = myEngine->LoadMesh("sphere.x");

	IMesh* Wall1Mesh = myEngine->LoadMesh("cube.x");

	IMesh* DummyMesh = myEngine->LoadMesh("dummy.x");

	IMesh* AmmoMesh = myEngine->LoadMesh("bullet.x");

	CBuilding* BuildingArray[kSizeX][kSizeZ];

	// Used to check iff building exists already or not
	for (int x = 0; x < kSizeX; ++x)
	{
		for (int z = 0; z < kSizeZ; ++z)
		{
			BuildingArray[x][z] = new CBuilding;
		}
	}

	float currentX = 0.0f;
	float currentZ = 0.0f;
	IModel* CurrentTileModel;
	IMesh* TileMesh = myEngine->LoadMesh("Square.x");
	CurrentTileModel = TileMesh->CreateModel(currentX, 0.0f, currentZ);
	CurrentTileModel->RotateLocalX(90.0f);
	CurrentTileModel->SetSkin("blue.png");
	CurrentTileModel->Scale(scale / 10.0f);

	// ENEMY 
	IMesh* EMesh = myEngine->LoadMesh("sphere.x");
	IModel* EModel = EMesh->CreateModel(50.0f, kFloorHeight, 50.0f);

	ISprite* SMenuSprite = nullptr;
	EMouseCapture mouseCap = on;
	EMousePosition mousePos = uninit;
	EMousePosition oldMousePos = uninit;

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

	CPlayer* player = new CPlayer(kStartingBalance);

	//// Audio
	//if (!buffer.loadFromFile("tada.wav"))
	//{
	//	cout << "Error loading sound file" << endl;
	//	while (!_kbhit());
	//	return;
	//}

	//towerSound.setBuffer(buffer);

	//towerSound.setVolume(100.0f); // 0 to 100
	//towerSound.setPitch(1.0f);
	//towerSound.setLoop(false);
	//towerSound.setPosition(towerSoundPos);

	//sf::Listener::setGlobalVolume(100.0f); // 0 to 100
	//sf::Listener::setPosition(listenerPos);
	//sf::Listener::setDirection(listenerForward);
	//sf::Listener::setUpVector(listenerUp);

	// TIME BASE
	myEngine->Timer();

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		if (myEngine->KeyHeld(kKeyQuit))
		{
			myEngine->Stop();
		}

		float frameTime = myEngine->Timer(); // Time between frames

		int mouseX = myEngine->GetMouseX();
		int mouseY = myEngine->GetMouseY();
		stringstream outText;
		outText << "Balance" << player->GetBalance();
		myFont->Draw(outText.str(), 20, 20);
		outText.str(""); // Clear myStream


		if (mouseCap == on)
		{
			myEngine->StartMouseCapture();
		}
		else
		{
			myEngine->StopMouseCapture();
		}

		/**** Update your scene each frame here ****/
		if (myEngine->KeyHit(kKeyMoveUp))
		{
			float newZ = currentZ + kCurrentMoveSpeed;
			if (newZ <= pBoundaryZ)
			{
				CurrentTileModel->MoveZ(kCurrentMoveSpeed);
				currentZ += kCurrentMoveSpeed;

			}
		}
		else if (myEngine->KeyHit(kKeyMoveDown))
		{
			float newZ = currentZ - kCurrentMoveSpeed;
			if (newZ >= nBoundaryZ)
			{
				CurrentTileModel->MoveZ(-kCurrentMoveSpeed);
				currentZ -= kCurrentMoveSpeed;
			}
		}
		else if (myEngine->KeyHit(kKeyMoveLeft))
		{
			float newX = currentX - kCurrentMoveSpeed;
			if (newX >= nBoundaryX)
			{
				CurrentTileModel->MoveX(-kCurrentMoveSpeed);
				currentX -= kCurrentMoveSpeed;
			}
		}
		else if (myEngine->KeyHit(kKeyMoveRight))
		{
			float newX = currentX + kCurrentMoveSpeed;
			if (newX <= pBoundaryX)
			{
				CurrentTileModel->MoveX(kCurrentMoveSpeed);
				currentX += kCurrentMoveSpeed;
			}
		}

		if (myEngine->KeyHeld(kKeyUpgrade))
		{
			if (SMenuSprite == nullptr)
			{
				int x, y;
				x = myEngine->GetMouseX();
				y = myEngine->GetMouseY();
				SMenuSprite = myEngine->CreateSprite("menu copy.png", kMenuPosX, kMenuPosY);
				mouseCap = off;
			}
			else
			{
				int x, y;
				x = mouseX - kMouseBoundaryX;
				y = mouseY - kMouseBoundaryY;
				/////////////////////////////////////
				if (x > 0 && y > 0)
				{
					if (x > y)
					{
						mousePos = mRight;
					}
					else
					{
						mousePos = mDown;
					}
				}
				else if (x > 0 && y < 0)
				{
					if (x > abs(y))
					{
						mousePos = mRight;
					}
					else
					{
						mousePos = mUp;
					}
				}
				else if (x < 0 && y < 0)
				{
					if (x < y)
					{
						mousePos = mLeft;
					}
					else
					{
						mousePos = mUp;
					}
				}
				else if (x < 0 && y > 0)
				{
					if (abs(x) > y)
					{
						mousePos = mLeft;
					}
					else
					{
						mousePos = mDown;
					}
				}
				/////////////////////////////////
			}

			if (mousePos != oldMousePos)
			{
				myEngine->RemoveSprite(SMenuSprite);
				switch (mousePos)
				{
				case uninit:
					break;
				case mUp:
					SMenuSprite = myEngine->CreateSprite("menu up.png", kMenuPosX, kMenuPosY);
					break;
				case mDown:
					SMenuSprite = myEngine->CreateSprite("menu down.png", kMenuPosX, kMenuPosY);
					break;
				case mLeft:
					SMenuSprite = myEngine->CreateSprite("menu left.png", kMenuPosX, kMenuPosY);
					break;
				case mRight:
					SMenuSprite = myEngine->CreateSprite("menu right.png", kMenuPosX, kMenuPosY);
					break;
				default:
					break;
				}
			}
			else
			{
				oldMousePos = mousePos;
			}
		}
		else
		{
			if (SMenuSprite != nullptr)
			{
				myEngine->RemoveSprite(SMenuSprite);
				SMenuSprite = nullptr;
				mouseCap = on;
			}

			switch (mousePos)
			{
			case uninit:
				break;
			case mUp:
			{
				// Upgrade tower
				if (player->GetBalance() + kTower2Cost >= 0)
				{
					int x = (int)currentX / 50;
					int z = (int)currentZ / 50;

					if (BuildingArray[x][z]->GetType() == tower1 && Found(BuildingArray, x, z))
					{
						BuildingArray[x][z]->UpgradeBuilding(Tower2Mesh, currentX, currentZ);

						player->ChangeBalance(kTower2Cost);
					}
				}
			}
			break;
			case mDown:
			{
				int x = (int)currentX / 50;
				int z = (int)currentZ / 50;

				if (Found(BuildingArray, x, z))
				{
					EBuildingType type = BuildingArray[x][z]->GetType();

					switch (type)
					{
					case 1:
					{
						player->ChangeBalance(kTower1Sale);
						break;
					}
					case 2:
					{
						player->ChangeBalance(kTower2Sale);
						break;
					}
					}

					BuildingArray[x][z]->SellBuilding();
				}
				break;
			}
			case mLeft:
				// Create Tower
				if (player->GetBalance() + kTower1Cost >= 0)
				{
					int x = (int)currentX / 50;
					int z = (int)currentZ / 50;

					if (!Found(BuildingArray, x, z))
					{
						BuildingArray[x][z]->CreateModel(currentX, currentZ, EBuildingType::tower1, Tower1Mesh, DummyMesh, AmmoMesh);

						BuildingArray[x][z]->AnimBuy(frameTime);

						player->ChangeBalance(kTower1Cost);
					}
				}
				break;
			case mRight:
				// Create Wall
				if (player->GetBalance() + kWall1Cost >= 0)
				{
					int x = (int)currentX / 50;
					int z = (int)currentZ / 50;

					if (!Found(BuildingArray, x, z))
					{
						BuildingArray[x][z]->CreateModel(currentX, currentZ, EBuildingType::wall, Wall1Mesh, DummyMesh, AmmoMesh);

						player->ChangeBalance(kWall1Cost);
					}
				}
				break;
			default:
				break;
			}

			mousePos = uninit;
		}

		///////////////
		// ANIMATION //
		///////////////
		bool built = false;
		bool shoot = false;
		float eX = EModel->GetX();
		float eZ = EModel->GetZ();
		IModel* tmp;
		for (int x = 0; x < kSizeX; ++x)
		{
			for (int z = 0; z < kSizeZ; ++z)
			{
				if (Found(BuildingArray, x, z))
				{
					built = BuildingArray[x][z]->GetState();
					if (built)
					{
						BuildingArray[x][z]->AnimBuy(frameTime);
					}

					shoot = BuildingArray[x][z]->EnemyInRange(eX, eZ);
					if (shoot)
					{
						tmp = BuildingArray[x][z]->GetDummy();
						tmp->LookAt(EModel);
						BuildingArray[x][z]->Attack(EModel, frameTime);
					}
				}
			}
		}

		///////////////////
		// TOWER ATTACKS //
		///////////////////



	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

bool Found(CBuilding* array[kSizeX][kSizeZ], int x, int z)
{
	EBuildingType type = array[x][z]->GetType();

	if (type > none)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool SphereSphereCD(float sphere1X, float sphere1Z, float sphere1Radius,
	float sphere2X, float sphere2Z, float sphere2Radius)
{
	float vectorX = sphere1X - sphere2X;
	float vectorZ = sphere1Z - sphere2Z;

	float collisionDistance = sqrt(vectorX * vectorX +
		vectorZ * vectorZ);

	if (collisionDistance < sphere1Radius + sphere2Radius)
	{
		return true;
	}

	return false;
}