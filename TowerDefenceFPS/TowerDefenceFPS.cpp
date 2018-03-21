// Tower Defence Game, A program using the TL-Engine

#include "TL-Engine.h"
#include "_GameControls.h" //Controls games
#include "AstarFun.h" //Astar functionality (Dan)
#include "FPS.h" // Controls FPS aspect of game (Jon)
#include "Top Down Controls.h" // Controls Tower building (Ben)

using namespace tle;
I3DEngine* myEngine = New3DEngine(kTLX);

//Globals
const float gameSpeed = 20;
ASTAR Pathfind;
bool DoOnce = true;

//methods
void Developer(ICamera * myCamera, IFont * myFont, IModel * GunDummy);
void SetCameraFPS(ICamera * myCamera, IModel* fpsDummy);
void SetCameraTopDown(ICamera * myCamera);
bool Found(CBuilding* array[kSizeX][kSizeZ], int x, int z);
bool SphereSphereCD(float sphere1X, float sphere1Z, float sphere1Radius, float sphere2X, float sphere2Z, float sphere2Radius);
float MAX(float a, float b);


void main()
{
	gameType mode = start;
	// Create a 3D engine (using TLX engine here) and open a window for it
	//myEngine->StartFullscreen();
	myEngine->StartWindowed(1920, 1080);
	const int kMenuPosY = myEngine->GetHeight() - 120;

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	myEngine->AddMediaFolder("media\\x");
	myEngine->AddMediaFolder("media\\p");
	myEngine->AddMediaFolder("media2\\");

	///////////////
	// S T A R T //
	///////////////
	ISprite * StartButton = myEngine->CreateSprite("start Button.png", 0.0f, 0.0f, 0.0f);
	StartButton->SetPosition(myEngine->GetWidth() - 125, myEngine->GetHeight() - 50);

	IMesh * MeshTest = myEngine->LoadMesh("Building07.x");
	IModel * ModelTest = MeshTest->CreateModel(0.0f, -1000.0f, 100.0f);

	//Font
	IFont* myFont = myEngine->LoadFont("Arial", 30);

	//Meshes
	IMesh* CubeMesh = myEngine->LoadMesh("cube.x");
	IMesh* SphereMesh = myEngine->LoadMesh("sphere.x");
	IMesh* DummyMesh = myEngine->LoadMesh("dummy.x");
	IMesh* SkyMesh = myEngine->LoadMesh("Skybox 07.x");
	IModel* skyBox = SkyMesh->CreateModel(0.0f, -1000.0f, 0.0f);

	//Variables	
	float frameTime = myEngine->Timer();

	//////////
	//Camera//
	//////////
	ICamera* myCamera = myEngine->CreateCamera(kManual);
	myCamera->SetPosition(0.0f, -1000.0f, 0.0f);
	float cameraYRotation = 0.0f;
	float cameraXRotation = 0.0f;


	///////////////////////
	// M A P & A S T A R // Dans Code
	///////////////////////
	//int CurrentMap[gMapWidth][gMapHeight]; //Array of all the Map Numbers
	shared_ptr<Node> Start(new Node);
	shared_ptr<Node> Goal(new Node);
	IModel * ModelArray[gMapWidth][gMapHeight];
	string UserInputMap = "m"; // Name of map
	GetMap(Pathfind.CurrentMap, UserInputMap, Start, Goal); //Validation Loop
	DisplayMap(Pathfind.CurrentMap);	//Display the loaded Map to console

										//Create Player (start)
	IModel *PlayerDummy = DummyMesh->CreateModel(CubeSize * Start->x, 16.0f, CubeSize * Start->y);

	//Create Goal
	IModel *GoalModel = CubeMesh->CreateModel(CubeSize * scale * Goal->x, 10.0f, CubeSize * scale * Goal->y);
	GoalModel->Scale(scale);

	//Start and Goal Validation
	if (Pathfind.CurrentMap[Start->x][Start->y] == 0) cout << "the start goal is in a wall... Please adjust your Coords file " << endl;
	else if (Pathfind.CurrentMap[Goal->x][Goal->y] == 0)	cout << "the end goal is in a wall and cannot be reached... Please Adjust your coords file " << endl;

	//Create grid of floor tiles
	for (int i = 0; i < gMapWidth; i++)
	{
		for (int j = 0; j < gMapHeight; j++)
		{
			ModelArray[i][j] = CubeMesh->CreateModel(scale * CubeSize * i, -scale * 5, scale * CubeSize * j);
			ModelArray[i][j]->Scale(scale);
			switch (Pathfind.CurrentMap[i][j])
			{
			case Wall: ModelArray[i][j]->SetSkin("brick1.jpg");
				break;
			case Clear: ModelArray[i][j]->SetSkin("metalFloor.png");
				break;
			case Wood: ModelArray[i][j]->SetSkin("Grass1.jpg");
				break;
			case Water: ModelArray[i][j]->SetSkin("CueTip.jpg");
				break;
			}
		}
	}

	//bool Checker = true;


	////////////////////////////////
	// T O W E R  B U I L D I N G // Ben's Code
	////////////////////////////////

	IMesh* Tower1Mesh = myEngine->LoadMesh("Building03.x");
	IMesh* Tower2Mesh = myEngine->LoadMesh("Building07.x");
	IMesh* Wall1Mesh = myEngine->LoadMesh("LargeGenericCrate.x");
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


	int currentX = 0;
	int currentZ = 0;
	IModel* CurrentTileModel;
	IMesh* TileMesh = myEngine->LoadMesh("Square.x");
	CurrentTileModel = TileMesh->CreateModel(currentX, (-scale * 5), currentZ);
	CurrentTileModel->RotateLocalX(90.0f);
	CurrentTileModel->SetSkin("blue.png");
	CurrentTileModel->Scale(scale);

	ISprite* SMenuSprite = nullptr;
	EMouseCapture mouseCap = on;
	ESelectedMenu menuSelection = buildTower;
	ESelectedMenu oldMenuSelection = menuSelection;
	CPlayer* player = new CPlayer(kStartingBalance);


	///////////
	// F P S // Jons Code
	///////////

	IMesh* gunMesh = myEngine->LoadMesh("autofrag.x");

	IModel* fpsDummy = DummyMesh->CreateModel(kFpsStartCoords[0], kFpsStartCoords[1], kFpsStartCoords[2]);
	IModel* gunModel = gunMesh->CreateModel(0.3f, -0.2f, 0.5f);
	IModel* gunDummy = DummyMesh->CreateModel(kFpsStartCoords[0], kFpsStartCoords[1], kFpsStartCoords[2]);

	IMesh* laserMesh = myEngine->LoadMesh("Projectile3.x");
	IModel* laser[kClipSize];
	for (int i = 0; i < kClipSize; i++)
	{
		laser[i] = laserMesh->CreateModel(0, -100, 0);
	}
	int laserCounter = 0;

	IMesh* CircleMesh = myEngine->LoadMesh("circle.x");


	///////////////////
	// E N E M I E S //
	///////////////////

	IMesh* ballMesh = myEngine->LoadMesh("cube.x");
	vector <enemy> enemyList;

	enemyList.push_back({ ballMesh->CreateModel(0, 0, 0),10.0f,  10, 0, 0.0f });
	enemyList.push_back({ ballMesh->CreateModel(-100, 0, 0),10.0f,  10, 0, 0.0f });
	enemyList.push_back({ ballMesh->CreateModel(-200, 0, 0),10.0f,  10, 0, 0.0f });
	enemyList.push_back({ ballMesh->CreateModel(-300, 0, 0),10.0f,  10, 0, 0.0f });
	enemyList.push_back({ ballMesh->CreateModel(-400, 0, 0),10.0f,  10, 0, 0.0f });

	gunDummy->SetPosition(0, 0, 1.0f);
	gunDummy->AttachToParent(myCamera);
	gunModel->AttachToParent(myCamera);
	float rofTimer = 0.0f; //Rate Of Fire Timer
	float reloadTimer = 5.0f;

	bool canFire = true;

	//////////////////////
	// G A M E  L O O P //
	//////////////////////
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		frameTime = myEngine->Timer(); // Time between frames
		//Developer(myCamera, myFont, gunDummy);
		//Developer(myCamera, myFont);


		///////////
		// F P S // jons code
		///////////

		if (mode == fps)
		{

			cameraXRotation = (myEngine->GetMouseMovementY());

			if (cameraXRotation > kCameraSensitivity)
			{
				cameraXRotation = kCameraSensitivity;
			}
			else if (cameraXRotation < -kCameraSensitivity)
			{
				cameraXRotation = -kCameraSensitivity;
			}

			cameraYRotation = (myEngine->GetMouseMovementX());

			fpsDummy->RotateLocalY(cameraYRotation);
			myCamera->RotateLocalX(cameraXRotation);


			//controls
			if (myEngine->KeyHeld(Key_W))
			{
				fpsDummy->MoveLocalZ(frameTime * gameSpeed);
			}

			if (myEngine->KeyHeld(Key_S))
			{
				fpsDummy->MoveLocalZ(-frameTime * gameSpeed);
			}

			if (myEngine->KeyHeld(Key_A))
			{
				fpsDummy->MoveLocalX(-frameTime * gameSpeed);
			}

			if (myEngine->KeyHeld(Key_D))
			{
				fpsDummy->MoveLocalX(frameTime * gameSpeed);
			}

			if (myEngine->KeyHit(Key_R))
			{
				reloadTimer = 0.0f;
			}

			//reload
			if (reloadTimer < 2.0f)
			{
				reloadTimer += frameTime;
				if (reloadTimer > 2.0f)
				{
					laserCounter = 0;
				}
			}

			rofTimer += frameTime;
			if (rofTimer > kRateOfFire)
			{
				rofTimer = 0.0f;
				canFire = true;
			}

			//fire
			if (myEngine->KeyHeld(Mouse_LButton) && canFire)
			{
				if (laserCounter < kClipSize)
				{
					laser[laserCounter]->SetPosition(gunDummy->GetX(), gunDummy->GetY(), gunDummy->GetZ());
					laser[laserCounter]->LookAt(myCamera);
					laser[laserCounter]->RotateLocalY(180);
					laser[laserCounter]->MoveLocalY(-5);
					laser[laserCounter]->MoveLocalX(9);

					canFire = false;
					laserCounter++;

				}


			}

			//enemys
			for (int i = 0; i < enemyList.size(); i++)
			{
				if (!enemyList[i].EndReached)
				{
					enemyList[i].EndReached = Pathfind.Patrol(enemyList[i].model, enemyList[i].PatrolRoute, PathRadius);
				}
			}

			myFont->Draw("Ammo: " + to_string(kClipSize - laserCounter), myEngine->GetWidth() - 150, myEngine->GetHeight() - 50);

			//checks clip size
			for (int i = 0; i < kClipSize; i++)
			{

				if (laser[i]->GetY() > -20.0f)
				{

					laser[i]->MoveLocalZ(kProjectileSpeed * gameSpeed * frameTime);

					float distance = sqrt(
						((laser[i]->GetX() - myCamera->GetX()) * (laser[i]->GetX() - myCamera->GetX())) +
						((laser[i]->GetZ() - myCamera->GetZ()) * (laser[i]->GetZ() - myCamera->GetZ()))
					);

					if (distance > 500)
					{
						laser[i]->SetY(-100);
					}


					//for (int j = 0; j < enemyList.size(); j++)	//run once for each enemy
					//{

					//	if (pointToSphereCollision(laser[i], enemyList[j].model, 10))
					//{
					//		enemyList[j].health -= kLaserDamage;
					//		if (enemyList[j].health <= 0)
					//		{
					//			enemyList[j].model->SetY(-100);
					//		}

					//		laser[i]->SetY(-100);
					//	}
					//}

				}


			}


			///////////////////
			// TOWER ATTACKS //
			///////////////////
			bool shoot = false;
			for (int x = 0; x < kSizeX; ++x)
			{
				for (int z = 0; z < kSizeZ; ++z)
				{
					for (auto& elt : enemyList)
					{
						shoot = BuildingArray[x][z]->EnemyInRange(elt.model->GetX(), elt.model->GetY());
						if (shoot)
						{
							BuildingArray[x][z]->Attack(elt.model, frameTime);
						}
					}
				}
			}
		}


		////////////////////
		// T O P  D O W N //
		////////////////////

		else if (mode == topDown)
		{

			////////////////////////////
			// P A T H  F I N D I N G // Dans Code
			////////////////////////////

			//Creating inital line
			if (DoOnce)
			{
				if (Pathfind.AStar(Start, Goal, Pathfind.CurrentMap, CircleMesh))
				{
					Pathfind.LineMaker(CircleMesh);
				}
				DoOnce = !DoOnce;
			}

			//////////////////////////////////
			// T O W E R  P L A C E M E N T // Bens Code
			//////////////////////////////////

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

			//Cursor Movement
			if (myEngine->KeyHit(kKeyMoveUp))
			{
				int newZ = currentZ;
				if (newZ < pBoundaryZ)
				{
					CurrentTileModel->MoveZ(kCurrentMoveSpeed * scale);
					currentZ++;
				}
			}
			else if (myEngine->KeyHit(kKeyMoveDown))
			{
				int newZ = currentZ;
				if (newZ > nBoundaryZ)
				{
					CurrentTileModel->MoveZ(-kCurrentMoveSpeed * scale);
					currentZ--;
				}
			}
			else if (myEngine->KeyHit(kKeyMoveLeft))
			{
				int newX = currentX;
				if (newX > nBoundaryX)
				{
					CurrentTileModel->MoveX(-kCurrentMoveSpeed * scale);
					currentX--;
				}
			}
			else if (myEngine->KeyHit(kKeyMoveRight))
			{
				int newX = currentX;
				if (newX < pBoundaryX)
				{
					CurrentTileModel->MoveX(kCurrentMoveSpeed * scale);
					currentX++;
				}
			}

			if (myEngine->KeyHit(kKeyTower))
			{
				menuSelection = buildTower;
			}
			else if (myEngine->KeyHit(kKeyWall))
			{
				menuSelection = buildWall;
			}
			else if (myEngine->KeyHit(kKeyUpgrade))
			{
				menuSelection = upgrade;
			}
			else if (myEngine->KeyHit(kKeySell))
			{
				menuSelection = sell;
			}

			if (myEngine->KeyHit(kKeySelect))
			{

				switch (menuSelection)
				{
				case upgrade:
				{
					// Upgrade tower
					if (player->GetBalance() + kTower2Cost >= 0)
					{
						int x = currentX;
						int z = currentZ;

						if (BuildingArray[x][z]->GetType() == tower1 && Found(BuildingArray, x, z))
						{
							BuildingArray[x][z]->UpgradeBuilding(Tower2Mesh, currentX * scale * CubeSize, currentZ  * scale * CubeSize);

							player->ChangeBalance(kTower2Cost);
						}
					}
				}
				break;
				case sell:
				{
					// Sell building
					int x = currentX;
					int z = currentZ;

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

						BuildingArray[x][z]->SellBuilding(AmmoMesh);
						Pathfind.CurrentMap[x][z] = 1; //set it to a wall
						Pathfind.DeleteEverything(CircleMesh);
						Pathfind.kCurveCounter = 0;
						DisplayMap(Pathfind.CurrentMap);
						//Check pathfinding
						if (Pathfind.AStar(Start, Goal, Pathfind.CurrentMap, CircleMesh))
						{
							Pathfind.LineMaker(CircleMesh);
						}
					}
					break;
				}
				case buildTower:
					// Create Tower

					//if player has funds
					if (player->GetBalance() > 0)
					{
						int x = currentX;
						int z = currentZ;
						bool CanCreate = true;

						//if tower is placed on Astar Line
						for (int i = 0; i < Pathfind.FinalPath.size(); i++)
						{
							//find the tile that it was placed on
							if (x == Pathfind.FinalPath[i]->x && z == Pathfind.FinalPath[i]->y)
							{
								Pathfind.CurrentMap[x][z] = 0; //set it to a wall
								Pathfind.DeleteEverything(CircleMesh);
								Pathfind.kCurveCounter = 0;
								DisplayMap(Pathfind.CurrentMap);
								//Check pathfinding
								if (Pathfind.AStar(Start, Goal, Pathfind.CurrentMap, CircleMesh))
								{
									Pathfind.LineMaker(CircleMesh);
								}

								//if pathfinder didn't find its end
								else
								{
									CanCreate = false;
									Pathfind.CurrentMap[x][z] = 1;
									Pathfind.DeleteEverything(CircleMesh);
									Pathfind.kCurveCounter = 0;
									DisplayMap(Pathfind.CurrentMap);
									if (Pathfind.AStar(Start, Goal, Pathfind.CurrentMap, CircleMesh))
									{
										Pathfind.LineMaker(CircleMesh);
									}
								}

							}
						}
						//if tower doesn't already exist
						if (!Found(BuildingArray, x, z) && CanCreate)
						{
							//Set Map to 0
							Pathfind.CurrentMap[x][z] = 0;

							//Create the damn thing
							BuildingArray[x][z]->CreateModel(currentX * scale * CubeSize, currentZ * scale * CubeSize, EBuildingType::tower1, Tower1Mesh, DummyMesh, AmmoMesh);

						
							BuildingArray[x][z]->AnimBuy(frameTime);
							
							

							player->ChangeBalance(kTower1Cost);
						}
					}

					break;
				case buildWall:
					// Create Wall
					if (player->GetBalance() > 0)
					{
						int x = currentX;
						int z = currentZ;
						bool CanCreate = true;

						//if tower is placed on Astar Line
						for (int i = 0; i < Pathfind.FinalPath.size(); i++)
						{
							//find the tile that it was placed on
							if (x == Pathfind.FinalPath[i]->x && z == Pathfind.FinalPath[i]->y)
							{
								Pathfind.CurrentMap[x][z] = 0; //set it to a wall
								Pathfind.DeleteEverything(CircleMesh);
								Pathfind.kCurveCounter = 0;
								DisplayMap(Pathfind.CurrentMap);
								//Check pathfinding
								if (Pathfind.AStar(Start, Goal, Pathfind.CurrentMap, CircleMesh))
								{
									Pathfind.LineMaker(CircleMesh);
								}

								//if pathfinder didn't find its end
								else
								{
									CanCreate = false;
									Pathfind.CurrentMap[x][z] = 1;
									Pathfind.DeleteEverything(CircleMesh);
									Pathfind.kCurveCounter = 0;
									DisplayMap(Pathfind.CurrentMap);
									if (Pathfind.AStar(Start, Goal, Pathfind.CurrentMap, CircleMesh))
									{
										Pathfind.LineMaker(CircleMesh);
									}
								}

							}
						}

						if (!Found(BuildingArray, x, z) && CanCreate)
						{
							BuildingArray[x][z]->CreateModel(currentX * scale * CubeSize, currentZ * scale * CubeSize, EBuildingType::wall, Wall1Mesh, DummyMesh, AmmoMesh);

							player->ChangeBalance(kWall1Cost);
						}
					}
					break;
				default:
					break;
				}
			}

			// Menu
			if (SMenuSprite != NULL)
			{
				myEngine->RemoveSprite(SMenuSprite);
			}
				switch (menuSelection)
				{
				case buildTower:
					SMenuSprite = myEngine->CreateSprite("Menu Sel1.png", kMenuPosX, kMenuPosY);
					break;
					case buildWall:
						SMenuSprite = myEngine->CreateSprite("Menu Sel2.png", kMenuPosX, kMenuPosY);
						break;
					case upgrade:
						SMenuSprite = myEngine->CreateSprite("Menu Sel3.png", kMenuPosX, kMenuPosY);
						break;
					case sell:
						SMenuSprite = myEngine->CreateSprite("Menu Sel4.png", kMenuPosX, kMenuPosY);
						break;
				default:
					break;
				}


			///////////////
			// ANIMATION //
			///////////////
			bool built = false;

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
					}
				}
			}
		}


		// Game state: START
		else if (mode == start)
		{
			ModelTest->RotateLocalY(gameSpeed * frameTime * 0.01f);
			myFont->Draw("Defender: A Tower Defence Game ", myEngine->GetWidth() / 2, 0, kBlack, kCentre);

			//Collision detection for start button
			if (myEngine->GetMouseX() < myEngine->GetWidth() && myEngine->GetMouseX() > (myEngine->GetWidth() * 0.75) &&
				myEngine->GetMouseY() < myEngine->GetHeight() && myEngine->GetMouseY() > (myEngine->GetHeight() * 0.9)
				)
			{
				if (myEngine->KeyHeld(Mouse_LButton))
				{
					SetCameraTopDown(myCamera);
					StartButton->SetPosition(-1000, -1000);
					ModelTest->ResetOrientation();
					ModelTest->SetPosition(-CubeSize * 2.0f, 0.0f, 0.0f);
					mode = topDown;
				}
			}

			else
			{
				StartButton->SetPosition(myEngine->GetWidth() - 125, myEngine->GetHeight() - 50);
			}
		}

		//exit
		if (myEngine->KeyHit(Key_Escape)) myEngine->Stop();

		//swap modes
		if (myEngine->KeyHit(Key_5))
		{
			if (mode == topDown)
			{
				mode = fps;
				SetCameraFPS(myCamera, fpsDummy);
				CurrentTileModel->MoveY(-kHideY);
				SMenuSprite->SetZ(-1);
			}

			else if (mode == fps)
			{
				mode = topDown;
				SetCameraTopDown(myCamera);
				CurrentTileModel->MoveY(kHideY);
				for (int i = 0; i < kClipSize; ++i)
				{
					laser[i]->SetPosition(0.0f, -kHideY, 0.0f);
				}
				//reload
				laserCounter = 0;
			}

			else if (mode == start)
			{
				mode = topDown;
				SetCameraTopDown(myCamera);
			}
		}
	}

	/////////////
	// Delete // When the game loop exits and you need to clean up the tiles
	////////////
	CubeMesh->RemoveModel(GoalModel);
	Pathfind.DeleteEverything(CircleMesh); //delete floor tiles and red squares
	Pathfind.kCurveCounter = 0; // Reset

								//Remove Floor Tiles
	for (int i = 0; i < gMapWidth; i++)
	{
		for (int j = 0; j < gMapHeight; j++)
		{
			CubeMesh->RemoveModel(ModelArray[i][j]);
		}
	}
	myEngine->Delete(); 	// Delete the 3D engine now we are finished with it
	exit(0);
}


//Method definitions (put in seperate files later?)
void Developer(ICamera * myCamera, IFont * myFont, IModel * GunDummy)
{
	//Camera
	myFont->Draw(to_string(myCamera->GetX()), 0, 0);
	myFont->Draw(to_string(myCamera->GetY()), 0, 20);
	myFont->Draw(to_string(myCamera->GetZ()), 0, 40);

	//GunDummy
	myFont->Draw(to_string(GunDummy->GetX()), 0, 60);
	myFont->Draw(to_string(GunDummy->GetY()), 0, 80);
	myFont->Draw(to_string(GunDummy->GetZ()), 0, 100);
}

void SetCameraFPS(ICamera * myCamera, IModel* fpsDummy)
{
	myCamera->ResetOrientation();
	myCamera->SetPosition(0, -5, 0);
	fpsDummy->SetPosition(Pathfind.FinalPath[Pathfind.FinalPath.size() - 2]->x * scale * CubeSize, scale * 5.0f, Pathfind.FinalPath[Pathfind.FinalPath.size() - 2]->y * scale * CubeSize);

	myCamera->AttachToParent(fpsDummy);
	//myCamera->MoveLocalZ(30);
}

void SetCameraTopDown(ICamera * myCamera)
{
	//Setup Camera
	myCamera->DetachFromParent();
	myCamera->SetPosition((CubeSize / 2 * scale * gMapWidth) - (CubeSize * 2), scale * 12.0f * MAX(gMapHeight, gMapWidth), (CubeSize / 2 * scale * gMapHeight - (CubeSize * 6)));
	myCamera->ResetOrientation();
	myCamera->RotateX(90.0f);
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

float MAX(float a, float b)
{
	a < b ? a = b : a;

	return a;
}