#pragma once

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <deque>
#include "_GameControls.h"

using namespace tle;

//const float PlayerSpeed = 100.0f;

//Nodes
const float NodeHeight = 10.0f;
const float CubeSize = 10.0f;

//Array of Spline Cubes
const int CurveDensity = 20;
const float PathRadius = 0.1f;

const int SIZE = 10;

struct SModel
{
	IMesh* mesh;
	IModel* model;
	int x;
	int z;
	int level;
};


struct Coords
{
	int x;
	int y;
};

class ASTAR {
public:

	int kCurveCounter = 0;
	const float kEnemySpeed = 0.5f;
	int CurrentMap[gMapWidth][gMapHeight];

	IModel* Curve[1000];
	vector <shared_ptr<Node> > OpenList;
	vector <shared_ptr<Node> > CloseList;
	vector <shared_ptr<Node> >  FinalPath;

	IMesh * CircleMesh;

	float Bezier(float P1, float P2, float P3, float P4, float t);
	bool CheckCloseList(shared_ptr<Node> Current);
	void CreateNode(int x, int y, shared_ptr<Node> Goal, shared_ptr<Node> Previous);
	void UpdateLine();
	void reconstructpath(shared_ptr<Node> current, shared_ptr<Node> Start);
	bool AStar(shared_ptr<Node> Start, shared_ptr<Node> Goal);
	bool BuildTower(int currentX, int currentZ);
	bool CheckPointReached(IModel* Player, IModel* CheckPoint, float Radius);
	bool Patrol(IModel * Player, int & PatrolNum, float Timer);
	void DeletePath();
	void DeleteList();
	void DeleteEverything();

	//bool AStar(shared_ptr<Node> Start, shared_ptr<Node> Goal, IMesh * CircleMesh);

	//bool LocateNewPath(shared_ptr<Node> Start, shared_ptr<Node> Goal, int CurrentMap[gMapWidth][gMapHeight], IMesh * CircleMesh);
	//bool BuildTower(shared_ptr<Node> Start, shared_ptr<Node> Goal, IMesh * CircleMesh, int currentX, int currentZ);
	//void CheckRoute();
};
