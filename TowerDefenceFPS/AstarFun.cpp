#include "AstarFun.h"

//The Bezier Equation
float ASTAR::Bezier(float P1, float P2, float P3, float P4, float t)
{
	return pow((1 - t), 3) * P1 + 3 * t * pow((1 - t), 2) * P2 + 3 * pow(t, 2) * (1 - t) * P3 + pow(t, 3) * P4;
}

//Check Close List
bool ASTAR::CheckCloseList(shared_ptr<Node> Current)
{
	for (int j = 0; j < OpenList.size(); j++)
	{
		if (Current->x == OpenList[j]->x && Current->y == OpenList[j]->y)
		{
			return true;
		}
	}

	for (int j = 0; j < CloseList.size(); j++)
	{
		if (Current->x == CloseList[j]->x && Current->y == CloseList[j]->y)
		{
			//If the Total score is bigger than The one on the close list ignore it
			if (Current->TotalScore >= CloseList[j]->TotalScore)
			{
				return true;
			}

			else //if not then get rid of the one on the close list and add this one to the open list again
			{
				//Keeps a record of what tiles have been created to be deleted later
				OpenList.push_back(Current);
				CloseList.erase(CloseList.begin() + j); //Get rid of from list

				return true;
			}
		}
	}
	return false;
}

void ASTAR::CreateNode(int x, int y, shared_ptr<Node> Goal, shared_ptr<Node> Previous, int Map[gMapWidth][gMapHeight], IMesh* CubeMesh)
{
	shared_ptr<Node> NewNode(new Node);
	NewNode->x = x;
	NewNode->y = y;
	NewNode->Weight = Map[x][y];
	NewNode->Parent = Previous;
	NewNode->ManDistance = abs(NewNode->x - Goal->x) + abs(NewNode->y - Goal->y);
	NewNode->TotalScore = Previous->TotalScore + NewNode->ManDistance + NewNode->Weight - Previous->ManDistance;

	if (NewNode->Weight == 0)
	{
		return; //Node is a wall
	}
	if (CheckCloseList(NewNode))
	{
		return; //Node already exists
	}

	OpenList.push_back(NewNode); //Add to close list

}

void ASTAR::LineMaker(IMesh * CubeMesh)
{
	//Smooth Curve 
	float P1x;
	float P2x;
	float X;
	float P1y;
	float P2y;
	float Y;
	float fCurve = CurveDensity;
	int NumLeft = FinalPath.size();

	for (int i = 0; i < FinalPath.size() - 1; )
	{
		P1x = (float)FinalPath[i]->x;
		P2x = (float)FinalPath[i + 1]->x;
		P1y = (float)FinalPath[i]->y;
		P2y = (float)FinalPath[i + 1]->y;

		for (int j = 0; j < (CurveDensity / 4); j++)
		{
			//Draw along the Y Axis
			if (P1y != P2y)
			{
				if (P1y < P2y)
				{
					X = (float)FinalPath[i]->x;
					Y = (float)FinalPath[i]->y + (j / (fCurve / 4.0f));
				}

				else
				{
					X = (float)FinalPath[i]->x;
					Y = (float)FinalPath[i]->y - (j / (fCurve / 4.0f));
				}
			}

			//Draw along X
			else
			{
				if (P1x < P2x)
				{
					X = (float)FinalPath[i]->x + (j / (fCurve / 4.0f));
					Y = (float)FinalPath[i]->y;
				}
				else
				{
					X = (float)FinalPath[i]->x - (j / (fCurve / 4.0f));
					Y = (float)FinalPath[i]->y;
				}
			}

			//Models
			Curve[kCurveCounter] = CubeMesh->CreateModel(CubeSize * X * scale, 1.0f, CubeSize * Y  * scale);
			Curve[kCurveCounter]->SetSkin("circle2.jpg");
			//Curve[kCurveCounter]->Scale(0.1f);
			kCurveCounter++;
		}
		i++;

		NumLeft = i;
	}
}

void ASTAR::reconstructpath(shared_ptr<Node> current, shared_ptr<Node> Start, IMesh * CubeMesh)
{

	while (current->x != Start->x || current->y != Start->y)
	{
		FinalPath.insert(FinalPath.begin(), current);
		for (int i = 0; i < CloseList.size(); i++)
		{
			if (current->Parent->x == CloseList[i]->x &&  current->Parent->y == CloseList[i]->y)
			{
				current = CloseList[i];
				i = CloseList.size() - 1;
			}
		}
	}

	FinalPath.insert(FinalPath.begin(), Start);

	//make line
	LineMaker(CubeMesh);


}

//F(n) = G(n) + H(n)
bool ASTAR::AStar(shared_ptr<Node> Start, shared_ptr<Node> Goal, int CurrentMap[gMapWidth][gMapHeight], IMesh * CircleMesh)
{
	int SortCount = 0;
	OpenList = { Start };
	shared_ptr<Node> Current(new Node); //Smart Pointer

	while (!OpenList.empty())
	{
		//Set Current
		int SmallNum = 1000000;
		int Counter = 0;
		for (int i = 0; i < OpenList.size(); i++)
		{
			if (OpenList[i]->TotalScore < SmallNum)
			{
				SmallNum = OpenList[i]->TotalScore;
				Current = OpenList[i];
				Counter = i;
			}
		}

		OpenList.erase(OpenList.begin() + Counter); //Get rid of current from open list
		CloseList.push_back(Current); //Add to close list

									  //Check if we've reached the Goal
		if (Current->x == Goal->x && Current->y == Goal->y)
		{
			reconstructpath(Current, Start, CircleMesh);
			return true; //Successsss
		}

		//Checks North, East, South, West for new nodes
		if (Current->y < (gMapHeight - 1)) CreateNode(Current->x, Current->y + 1, Goal, Current, CurrentMap, CircleMesh);	 //(North) node, 
		if (Current->x < (gMapWidth - 1)) CreateNode(Current->x + 1, Current->y, Goal, Current, CurrentMap, CircleMesh);	 //(East) node, 
		if (Current->y > 0) CreateNode(Current->x, Current->y - 1, Goal, Current, CurrentMap, CircleMesh);	 //(South) node, 
		if (Current->x > 0) CreateNode(Current->x - 1, Current->y, Goal, Current, CurrentMap, CircleMesh);	 //(West) node, 

	}

	cout << "No Path Found :(" << endl;
	return false; // if no path found
}

bool ASTAR::CheckPointReached(IModel* Player, IModel* CheckPoint, float Radius)
{
	float x = (CheckPoint->GetX() - Player->GetX()) * (CheckPoint->GetX() - Player->GetX());
	float z = (CheckPoint->GetZ() - Player->GetZ()) * (CheckPoint->GetZ() - Player->GetZ());
	float Distance = sqrt(x + z);

	if (Distance - Radius < 0) return true;
	else return false;
}

bool ASTAR::Patrol(IModel* Player, int &PatrolNum, float Timer)
{
	Player->LookAt(Curve[PatrolNum]);
	Player->MoveLocalZ(kEnemySpeed * Timer);

	if (PatrolNum > (kCurveCounter - ( kCurveCounter / 9)))
	{
		return true;
	}

	//If Reached look at next checkpoint
	if (CheckPointReached(Player, Curve[PatrolNum], PathRadius))
	{
		PatrolNum = PatrolNum + 1;
	}

	//when end reached


	return false;
}

/////////////////
// D E L E T E //
/////////////////

void ASTAR::DeletePath(IMesh * SphereMesh)
{
	//Remove Tiles
	int i = 0;
	while (i < kCurveCounter)
	{
		SphereMesh->RemoveModel(Curve[i]);
		i++;
	}
}

void ASTAR::DeleteList()
{
	//Remove items from the Lists
	while (!OpenList.empty())
	{
		OpenList.pop_back();
	}
	while (!CloseList.empty())
	{
		CloseList.pop_back();
	}
	while (!FinalPath.empty())
	{
		FinalPath.pop_back();
	}
}

void ASTAR::DeleteEverything(IMesh * SphereMesh)
{
	DeletePath(SphereMesh);
	DeleteList();
}