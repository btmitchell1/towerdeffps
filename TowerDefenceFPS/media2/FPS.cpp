#include "FPS.h"

bool pointToSphereCollision(IModel* object1, IModel* object2, int radius)
{

	float distance = cbrt(
		((object1->GetX() - object2->GetX()) * (object1->GetX() - object2->GetX())) +
		((object1->GetY() - object2->GetY()) * (object1->GetY() - object2->GetY())) +
		((object1->GetZ() - object2->GetZ()) * (object1->GetZ() - object2->GetZ()))
	);

	if (distance < radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}
