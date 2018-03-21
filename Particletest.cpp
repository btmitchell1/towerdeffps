// ParticleSystem.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

/* FUNCTIONS */
float random(int, int);

/* CONSTANTS */
const float kXVelocityMax = 75.0f;
const float kYVelocityMax = 500.0f;
const float kZVelocityMax = 50.0f;
const float kMaxLifetime = 1.0f;
const float kGravity = 100.0f;
const int kNumParticles = 1000;
const float kFloor = 0.0f;

struct particle
{
	IModel *model;
	float moveVector[3];
	float lifetime;
};

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );
	myEngine->AddMediaFolder("./media");

	/**** Set up your scene here ****/
	/* SKYBOX */
	IMesh* skyboxMesh = myEngine->LoadMesh("stars.x");
	IModel* skybox = skyboxMesh->CreateModel();

	/* FLOOR */
	IMesh* floorMesh = myEngine->LoadMesh("floor.x");
	IModel* floor = floorMesh->CreateModel();

	/* PARTICLES */
	IMesh* quadMesh = myEngine->LoadMesh("sphere.x");
	particle quad[kNumParticles];

	for (int i = 0; i < kNumParticles; i++)
	{
		quad[i].model = quadMesh->CreateModel();

		float xVel = random(-kXVelocityMax, kXVelocityMax);
		float yVel = random(-kYVelocityMax, kYVelocityMax);
		float zVel = random(-kZVelocityMax, kZVelocityMax);

		quad[i].moveVector[0] = xVel;
		quad[i].moveVector[1] = yVel;
		quad[i].moveVector[2] = zVel;

		quad[i].lifetime = random(0.0f, kMaxLifetime);

		quad[i].model->SetSkin("RedBall.jpg");

		quad[i].model->Scale(0.1f);
		quad[i].model->ScaleZ(0.0000001f);
	}

	/* CAMERA */
	ICamera* myCamera = myEngine->CreateCamera(kFPS, 0.0f, 60.0f, -180.0f);

	float timeBase = myEngine->Timer();
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		float frameTime = myEngine->Timer();

		/**** Update your scene each frame here ****/
		bool particleEmitted = false;

		for (int i = 0; i < kNumParticles; i++)
		{
			quad[i].model->LookAt(myCamera);

			quad[i].moveVector[1] = quad[i].moveVector[1] + -kGravity * frameTime;

			quad[i].model->MoveX(quad[i].moveVector[0] * frameTime);
			quad[i].model->MoveY(quad[i].moveVector[1] * frameTime);
			quad[i].model->MoveZ(quad[i].moveVector[2] * frameTime);

			// Decrease lifetime
			quad[i].lifetime -= frameTime;

			if (quad[i].lifetime < 0 || quad[i].model->GetY() < kFloor || particleEmitted == false)
			{
				quad[i].model->SetPosition(0.0f, 0.0f, 0.0f);

				particleEmitted = true;

				float xVel = random(-kXVelocityMax, kXVelocityMax);
				float yVel = random(-kYVelocityMax, kYVelocityMax);
				float zVel = random(-kZVelocityMax, kZVelocityMax);
				quad[i].moveVector[0] = xVel;
				quad[i].moveVector[1] = yVel;
				quad[i].moveVector[2] = zVel;

				// Reset lifetime to random
				quad[i].lifetime = random(0.0f, kMaxLifetime);
			}
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

float random(int rangeMin, int rangeMax)
{
	float result = (float)rand() / (float)(RAND_MAX + 1);
	result *= (float)(rangeMax - rangeMin);
	result += rangeMin;

	return result;
}

