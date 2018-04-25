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
CPlayer* player = new CPlayer(kStartingBalance);

const float footStepTimePeriod = 0.5f;
float footStepTimer = footStepTimePeriod;

bool moved = false;

//methods
void Developer(ICamera * myCamera, IFont * myFont, IModel * GunDummy, float wheelMovement);
void SetCameraFPS(ICamera * myCamera, IModel* fpsDummy);
void SetCameraTopDown(ICamera * myCamera);
bool Found(CBuilding* array[gMapWidth][gMapHeight], int x, int z);
bool SphereSphereCD(float sphere1X, float sphere1Z, float sphere1Radius, float sphere2X, float sphere2Z, float sphere2Radius);
float MAX(float a, float b);
void BorderCollision(IModel* fpsDummy, float oldX, float oldZ);
void CreateTower(CBuilding * BuildingArray[gMapWidth][gMapHeight], IMesh * DummyMesh, IMesh * AmmoMesh, int currentX, int currentZ, EBuildingType Type, IMesh * TowerMesh, int Cost);

sf::Vector3f listenerPos(0.0, 0.0, 0.0);
sf::Vector3f listenerForward(0.0, 0.0, -1.0);
sf::Vector3f listenerUp(0.0, 1.0, 0.0);



void main()
{
<<<<<<< HEAD
	const int kNumSounds = 7;
	Sound sounds[kNumSounds];

	sounds[0].soundName = "musicloop.wav";
	sounds[1].soundName = "shootgun.wav";
	sounds[2].soundName = "reload.wav";
	sounds[3].soundName = "upgrade.wav";
	sounds[4].soundName = "sell.wav";
	sounds[5].soundName = "footstep.wav";
	sounds[6].soundName = "enemymove.wav";

	for (int i = 0; i < kNumSounds; ++i)
	{
		if (!sounds[i].buffer.loadFromFile(sounds[i].soundName))
		{
			cout << "Error loading sound file" << endl;
			while (!_kbhit());
			return;
		}
		// Sources
		// Indicate that our sound source will use the buffer we just loaded
		sounds[i].sound.setBuffer(sounds[i].buffer);
		// Set the properties of the source. Details of all available properties are in the SFML documentation of the Sound class
		sounds[i].sound.setVolume(100.0f); // 0 to 100
		sounds[i].sound.setPitch(1.0f);
		sounds[i].sound.setLoop(false);
		sounds[i].sound.setPosition(sounds[i].soundPos);
	}
	sounds[0].sound.setLoop(true);
	sounds[0].sound.play();

	sounds[5].sound.setVolume(40.0f);

	//****************
	// Listener

	// Set the properties of the listener. These are all the available listener properties
	// Note how this is doen with static member functions - there is no listener variable
	sf::Listener::setGlobalVolume(100.0f); // 0 to 100
	sf::Listener::setPosition(listenerPos);
	sf::Listener::setDirection(listenerForward);
	sf::Listener::setUpVector(listenerUp);


	gameType mode = start;
	// Create a 3D engine (using TLX engine here) and open a window for it
	//myEngine->StartFullscreen();
	myEngine->StartWindowed(1600, 900);
	const int kMenuPosX = myEngine->GetWidth() - 1600;
=======
	////////////////
	// S E T U P //
	//////////////

	gameType Mode = StartGame;
#ifdef _DEBUG
	myEngine->StartWindowed();
#endif // DEBUG

#ifndef _DEBUG
	myEngine->StartFullscreen(1600, 900);
#endif // !DEBUG
	
	
	
	const int kMenuPosX = 0;
>>>>>>> master
	const int kMenuPosY = myEngine->GetHeight() - 120;

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media"); //default folder
	myEngine->AddMediaFolder("media\\x"); //models
	myEngine->AddMediaFolder("media\\p"); //images
	myEngine->AddMediaFolder("media2\\");	//extra
	IFont* myFont = myEngine->LoadFont("Arial", 30); //Font

	///////////////
	// S T A R T //
	///////////////
	ISprite * StartButton = myEngine->CreateSprite("start Button.png", 0.0f, 0.0f, 0.0f);
	StartButton->SetPosition(myEngine->GetWidth() - 125, myEngine->GetHeight() - 50);

	IMesh * MeshTest = myEngine->LoadMesh("Building07.x");
	IModel * StartModel = MeshTest->CreateModel(0.0f, -1000.0f, 100.0f);


	//Meshes
	IMesh* CubeMesh = myEngine->LoadMesh("cube.x");
	IMesh* SphereMesh = myEngine->LoadMesh("sphere.x");
	IMesh* DummyMesh = myEngine->LoadMesh("dummy.x");
	IMesh* SkyMesh = myEngine->LoadMesh("SkyboxTowerDefense.x");
	IModel* skyBox = SkyMesh->CreateModel(0.0f, -1000.0f, 0.0f);
	IMesh* GroundMesh = myEngine->LoadMesh("ground.x");
	IModel* ground = GroundMesh->CreateModel(0.0f, -0.1f, 0.0f);

	//Variables	
	float frameTime = myEngine->Timer(); // frame time
	bool built = false; //checks if a tower is fully built before swapping state

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

	shared_ptr<Node> Start(new Node); 
	shared_ptr<Node> Goal(new Node);
	IModel * ModelArray[gMapWidth][gMapHeight]; //Array of all the cubes

	//Get Map
	string UserInputMap = "m"; // Name of map
	GetMap(Pathfind.CurrentMap, UserInputMap, Start, Goal); 
	DisplayMap(Pathfind.CurrentMap);	//Display the loaded Map to console

	//Create Player
	IModel *PlayerDummy = DummyMesh->CreateModel(CubeSize * Start->x, 16.0f, CubeSize * Start->y);

	//Create Goal
	IModel *GoalModel = CubeMesh->CreateModel(CubeSize * scale * Goal->x, 10.0f, CubeSize * scale * Goal->y);
	GoalModel->Scale(scale);

	//Start and Goal Validation
	if (Pathfind.CurrentMap[Start->x][Start->y] == 0) cout << "the start goal is in a wall... Please adjust your Coords file " << endl;
	else if (Pathfind.CurrentMap[Goal->x][Goal->y] == 0)	cout << "the end goal is in a wall and cannot be reached... Please Adjust your coords file " << endl;
	Pathfind.CircleMesh = myEngine->LoadMesh("circle.x");

	Pathfind.AStar(Start, Goal);
	Pathfind.UpdateLine();


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

	////////////////////////////////
	// T O W E R  B U I L D I N G // Ben's Code
	////////////////////////////////

	IMesh* Tower1Mesh = myEngine->LoadMesh("Building03.x");
	IMesh* Tower2Mesh = myEngine->LoadMesh("Building07.x");
	IMesh* Wall1Mesh = myEngine->LoadMesh("LargeGenericCrate.x");
	IMesh* AmmoMesh = myEngine->LoadMesh("Projectile3.x");

	CBuilding* BuildingArray[gMapWidth][gMapHeight];

	// Used to check iff building exists already or not
	for (int x = 0; x < gMapWidth; ++x)
	{
		for (int z = 0; z < gMapHeight; ++z)
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
	CurrentTileModel->SetSkin("newblue.png");
	CurrentTileModel->Scale(scale);

	ISprite* SMenuSprite = nullptr;
	int menuSelection = 1;
	int oldMenuSelection = menuSelection;
	//CPlayer* player = new CPlayer(kStartingBalance);


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
	gunModel->MoveY(-kHideY);
	float rofTimer = 0.0f; //Rate Of Fire Timer
	float reloadTimer = 5.0f;

	bool canFire = true;

	//////////////////////
	// G A M E  L O O P //
	//////////////////////
	while (myEngine->IsRunning())
	{

		frameTime = myEngine->Timer(); // Time between frames

		///////////
		// F P S // jons code
		///////////

		if (Mode == Fps)
		{
			//old position
			float oldX = fpsDummy->GetX();
			float oldZ = fpsDummy->GetZ();

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
				moved = true;
			}

			if (myEngine->KeyHeld(Key_S))
			{
				fpsDummy->MoveLocalZ(-frameTime * gameSpeed);
				moved = true;
			}

			if (myEngine->KeyHeld(Key_A))
			{
				fpsDummy->MoveLocalX(-frameTime * gameSpeed);
				moved = true;
			}

			if (myEngine->KeyHeld(Key_D))
			{
				fpsDummy->MoveLocalX(frameTime * gameSpeed);
				moved = true;
			}

			if (moved)
			{
				footStepTimer += frameTime;
				if (footStepTimer >= footStepTimePeriod)
				{
					float high = 1.25f;
					float low = 0.75f;
					float randPitch = low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
					sounds[5].sound.setPitch(randPitch);
					sounds[5].sound.play();

					footStepTimer = 0.0f;
				}

				moved = false;
			}


			BorderCollision(fpsDummy, oldX, oldZ);


			if (myEngine->KeyHit(Key_R))
			{
				reloadTimer = 0.0f;
				sounds[2].sound.play();
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

					sounds[1].sound.play();
				}
			}

			//enemys
			for (int i = 0; i < enemyList.size(); i++)
			{
				if (!enemyList[i].EndReached)
				{
					enemyList[i].EndReached = Pathfind.Patrol(enemyList[i].model, enemyList[i].PatrolRoute, PathRadius);
					myFont->Draw(to_string(enemyList[i].PatrolRoute), 0, i * 25);
				}

				else
				{
					enemyList[i].model->SetY(-kHideY);
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

			for (int x = 0; x < gMapWidth; ++x)
			{
				for (int z = 0; z < gMapHeight; ++z)

				{
					for (int z = 0; z < kSizeZ; ++z)
					{
							BuildingArray[x][z]->Attack(enemyList, frameTime);
					}
				
				}
			}


		////////////////////
		// T O P  D O W N //
		////////////////////

		else if (Mode == TopDown)
		{

			////////////////////////////
			// P A T H  F I N D I N G // Dans Code
			////////////////////////////
			

			//////////////////////////////////
			// T O W E R  P L A C E M E N T // Bens Code
			//////////////////////////////////

			stringstream outText;
			outText << "Balance" << player->GetBalance();
			myFont->Draw(outText.str(), 20, 20);
			outText.str(""); // Clear myStream

			//Move the block that places the towers
			if (myEngine->KeyHit(kKeyMoveUp))
			{
				if (currentZ < pBoundaryZ)
				{
					CurrentTileModel->MoveZ(kCurrentMoveSpeed * scale);
					currentZ++;
				}
			}
			else if (myEngine->KeyHit(kKeyMoveDown))
			{
				if (currentZ > nBoundaryZ)
				{
					CurrentTileModel->MoveZ(-kCurrentMoveSpeed * scale);
					currentZ--;
				}
			}
			else if (myEngine->KeyHit(kKeyMoveLeft))
			{
				if (currentX > nBoundaryX)
				{
					CurrentTileModel->MoveX(-kCurrentMoveSpeed * scale);
					currentX--;
				}
			}
			else if (myEngine->KeyHit(kKeyMoveRight))
			{
				if (currentX < pBoundaryX)
				{
					CurrentTileModel->MoveX(kCurrentMoveSpeed * scale);
					currentX++;
				}
			}

			// Menu Key Control
			if (myEngine->KeyHit(kKeyTower))
			{
				menuSelection = 1;
			}
			else if (myEngine->KeyHit(kKeyWall))
			{
				menuSelection = 2;
			}
			else if (myEngine->KeyHit(kKeyUpgrade))
			{
				menuSelection = 3;
			}
			else if (myEngine->KeyHit(kKeySell))
			{
				menuSelection = 4;
			}
			else if (myEngine->KeyHit(kKeyContinue))
			{
				menuSelection = 5;
			}

			// Scroll wheel menu
			float wheelMovement = myEngine->GetMouseWheelMovement();
			if (wheelMovement > 0.0f)
			{
				if (menuSelection < 5)
				{
					++menuSelection;
				}
				else if(menuSelection == 5)
				{
					menuSelection = 1;
				}
			}
			else if (wheelMovement < 0.0f)
			{
				if (menuSelection > 1)
				{
					--menuSelection;
				}
				else if (menuSelection == 1)
				{
					menuSelection = 5;
				}
			}

			// Arrow keys
			if (myEngine->KeyHit(Key_Right))
			{
				if (menuSelection < 5)
				{
					++menuSelection;
				}
				else if (menuSelection == 5)
				{
					menuSelection = 1;
				}
			}
			else if (myEngine->KeyHit(Key_Left))
			{
				if (menuSelection > 1)
				{
					--menuSelection;
				}
				else if (menuSelection == 1)
				{
					menuSelection = 5;
				}
			}
			
			// Menu sprites
			if (SMenuSprite != NULL)
			{
				myEngine->RemoveSprite(SMenuSprite);
			}
			switch (menuSelection)
			{
			case kBuildTowerButton:
				SMenuSprite = myEngine->CreateSprite("Menu Sel1.png", kMenuPosX, kMenuPosY);
				break;
			case kBuildWallButton:
				SMenuSprite = myEngine->CreateSprite("Menu Sel2.png", kMenuPosX, kMenuPosY);
				break;
			case kUpgradeBuildingButton:
				SMenuSprite = myEngine->CreateSprite("Menu Sel3.png", kMenuPosX, kMenuPosY);
				break;
			case kSellBuildingButton:
				SMenuSprite = myEngine->CreateSprite("Menu Sel4.png", kMenuPosX, kMenuPosY);
				break;
			case kContinueButton:
				SMenuSprite = myEngine->CreateSprite("Menu Sel5.png", kMenuPosX, kMenuPosY);
				break;
			default:
				break;
			}

			//on selection
			if (myEngine->KeyHit(kKeySelect))
			{

				switch (menuSelection)
				{
				case kBuildTowerButton:
					CreateTower(BuildingArray, DummyMesh, AmmoMesh, currentX, currentZ, tower1, Tower1Mesh, kTower1Cost);
					break;
				case kBuildWallButton:
					CreateTower(BuildingArray, DummyMesh, AmmoMesh, currentX, currentZ, wall, Wall1Mesh, kWall1Cost);
					break;
				case kUpgradeBuildingButton:
				{
					// Upgrade tower
					if (player->GetBalance() + kTower2Cost >= 0)
					{
						if (BuildingArray[currentX][currentZ]->GetType() == tower1 && Found(BuildingArray, currentX, currentZ))
						{
							BuildingArray[currentX][currentZ]->UpgradeBuilding(Tower2Mesh, currentX * scale * CubeSize, currentZ  * scale * CubeSize);
							player->ChangeBalance(kTower2Cost);

							sounds[3].sound.play();
						}
					}
				}
					break;
				case kSellBuildingButton:
				{
					// Sell building
					int x = currentX;
					int z = currentZ;

					if (Found(BuildingArray, x, z))
					{
						EBuildingType type = BuildingArray[x][z]->GetType();

						switch (type)
						{
						case none:
							break;
						case tower1:
							player->ChangeBalance(kTower1Sale);
							break;
						case tower2:
							player->ChangeBalance(kTower2Sale);
							break;
						case wall:
							break;
						default:
							break;
						}

						BuildingArray[x][z]->SellBuilding(AmmoMesh);
						Pathfind.CurrentMap[x][z] = 1; //set it to a wall
<<<<<<< HEAD
						Pathfind.DeleteEverything(CircleMesh);
						Pathfind.kCurveCounter = 0;
						DisplayMap(Pathfind.CurrentMap);
						//Check pathfinding
						if (Pathfind.AStar(Start, Goal, Pathfind.CurrentMap, CircleMesh))
						{
							Pathfind.LineMaker(CircleMesh);
						}

						sounds[4].sound.play();
=======
						Pathfind.AStar(Start, Goal); //redo the path
>>>>>>> master
					}
					break;
				}
				case kContinueButton: 
				{
					if (Mode == TopDown)
					{
						Mode = Fps;
						SetCameraFPS(myCamera, fpsDummy);
						CurrentTileModel->MoveY(-kHideY);
						SMenuSprite->SetZ(-1);
						gunModel->MoveY(kHideY);
					}

					else if (Mode == Fps)
					{
						Mode = TopDown;
						SetCameraTopDown(myCamera);
						CurrentTileModel->MoveY(kHideY);
						for (int i = 0; i < kClipSize; ++i)
						{
							laser[i]->SetPosition(0.0f, -kHideY, 0.0f);
						}
						//reload
						laserCounter = 0;
						gunModel->MoveY(-kHideY);
					}

					else if (Mode == StartGame)
					{
						Mode = TopDown;
						SetCameraTopDown(myCamera);
					}
				}
				default:
					break;
				}
			}
		}

		// Game state: START
		else if (Mode == StartGame)
		{
			StartModel->RotateLocalY( gameSpeed * frameTime);
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
					delete(StartButton);
					StartModel->ResetOrientation();
					StartModel->SetPosition(-CubeSize * 2.0f, 0.0f, 0.0f);
					Mode = TopDown;
				}
			}

			else
			{
				StartButton->SetPosition(myEngine->GetWidth() - 125, myEngine->GetHeight() - 50);
			}
		}

		///////////////
		// ANIMATION // - Placed here to stop tower freezing in air when switching to FPS mode
		///////////////
		//built = false;
		for (int x = 0; x < gMapWidth; ++x)
		{
			for (int z = 0; z < gMapHeight; ++z)
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

		
		// Extra checks
		if (myEngine->KeyHit(Key_Escape)) myEngine->Stop();


		myEngine->DrawScene(); // Draw the scene
	}


	/////////////
	// Delete // When the game loop exits and you need to clean up the tiles
	////////////
	CubeMesh->RemoveModel(GoalModel);
	Pathfind.DeleteEverything(); //delete floor tiles and red squares
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
	std::exit(0);
}


//Method definitions (put in seperate files later?)
void Developer(ICamera * myCamera, IFont * myFont, IModel * GunDummy, float wheelMovement)
{
	//Camera
	myFont->Draw(to_string(myCamera->GetX()), 0, 0);
	myFont->Draw(to_string(myCamera->GetY()), 0, 20);
	myFont->Draw(to_string(myCamera->GetZ()), 0, 40);

	//GunDummy
	myFont->Draw(to_string(GunDummy->GetX()), 0, 60);
	myFont->Draw(to_string(GunDummy->GetY()), 0, 80);
	myFont->Draw(to_string(GunDummy->GetZ()), 0, 100);
	myFont->Draw(to_string(wheelMovement), 0, 120);
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


bool Found(CBuilding* array[gMapWidth][gMapHeight], int x, int z)
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

void BorderCollision(IModel* fpsDummy, float oldX, float oldZ)
{
	if (fpsDummy->GetX() > ((CubeSize * gMapWidth)-5) * scale)
	{
		fpsDummy->SetX(oldX);
	}
	else if (fpsDummy->GetX() < -5 *scale)
	{
		fpsDummy->SetX(oldX);
	}
	if (fpsDummy->GetZ() > ((CubeSize * gMapHeight) - 5) * scale)
	{
		fpsDummy->SetZ(oldZ);
	}
	else if (fpsDummy->GetZ() < -5 * scale)
	{
		fpsDummy->SetZ(oldZ);
	}
}

void ChangeState(gameType Type)
{
	switch (Type)
	{
	case StartGame:
		break;
	case TopDown:
		myEngine->StopMouseCapture();


		break;
	case Fps:
		myEngine->StartMouseCapture();


		break;
	case Paused:
		break;
	case End:
		break;
	default:
		break;
	}

	


}


//BuildingArray[currentX][currentZ]->CreateModel
//(currentX * scale * CubeSize, currentZ * scale * CubeSize, EBuildingType::tower1, Tower1Mesh, DummyMesh, AmmoMesh);
//
//player->ChangeBalance(kTower1Cost); // Deduct balance

void CreateTower(CBuilding * BuildingArray[gMapWidth][gMapHeight], IMesh * DummyMesh, IMesh * AmmoMesh, int currentX, int currentZ, EBuildingType Type, IMesh * TowerMesh, int Cost)
{
	// Create Tower if player has funds
	if (player->GetBalance() > -kTower1Cost)
	{
		//if tower doesn't already exist
		if (!Found(BuildingArray, currentX, currentZ))
		{

			if (Pathfind.BuildTower(currentX, currentZ))
			{
				BuildingArray[currentX][currentZ]->CreateModel
				(currentX * scale * CubeSize, currentZ * scale * CubeSize, Type, TowerMesh, DummyMesh, AmmoMesh);

				player->ChangeBalance(Cost); // Deduct balance
			}

		}
	}
}