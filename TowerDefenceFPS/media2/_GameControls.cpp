#include "_GameControls.h"

bool GetMap(int MapArray[gMapWidth][gMapHeight], string MapFile, shared_ptr<Node> Start, shared_ptr<Node> Goal)
{
	//Read in Map file Given its prefix
	string CoordFile;
	CoordFile = MapFile + "Coords.txt";
	MapFile = MapFile + "map.txt";
	ifstream MapFileIn(MapFile);

	// Error catcher for missing map
	if (!MapFileIn)
	{
		cout << "ERROR: ";
		cout << "Can't open file " + MapFile + ", File May be missing... \n";
		for (int i = 0; i < gMapWidth; i++)
		{
			for (int j = 0; j < gMapHeight; j++)
			{
				MapArray[i][j] = 1;
			}
		}
	}

	// If file was read succesfully, write the numbers to an array
	else {

		// Read map variables
		MapFileIn >> noskipws;
		int CounterX = 0;					// Counting along X axis
		int CounterY = (gMapHeight - 1);		// Offset so that (x,y) co-ordinates match Array[X][Y] 

		while (!MapFileIn.eof())
		{
			// Reads each character of the file
			char ch;
			MapFileIn >> ch;

			//Adding Numbers to array
			if (ch != 10) {
				int MapInt = (int)ch - '0';		// Convert ch to int to be added to the array
				MapArray[CounterX][CounterY] = MapInt;  // Add the int to the Array

														//When The X counter reaches the right hand side, reset to the left hand side and starts the next line
				CounterX++;
				if (CounterX > (gMapWidth - 1)) {
					CounterX = 0;
					CounterY--;
				}
			}
		}

		// Confirm map has loaded
		cout << "[INFO] " << MapFile << " Successfully Loaded." << endl;
	}

	// Read In given map
	ifstream CoordFileIn(CoordFile);

	// Error catcher for missing map
	if (!CoordFileIn)
	{
		cout << "ERROR: ";
		cout << "Can't open file " + CoordFile + ", File May be missing... \n";
		Start->x = 0;
		Start->y = 0;
		Goal->x = gMapWidth - 1;
		Goal->y = gMapHeight - 1;
		return false;
	}

	else {

		// Read map variables
		CoordFileIn >> noskipws;
		int CoordArray[5];
		int i = 0;
		string holder = "";
		int MapInt = 0;
		while (!CoordFileIn.eof())
		{
			// Reads each character of the file
			char ch;
			CoordFileIn >> ch;

			//Adding Numbers to array
			if (CoordFileIn.eof())
			{

			}

			else if (ch == 10 || ch == ' ') //skip
			{
				CoordArray[i] = stoi(holder);
				holder = "";
				i++;
			}

			else
			{
				MapInt = (int)ch - '0';		// Convert ch to int to be added to the array
				holder = holder + to_string(MapInt);

			}
		}

		//Setup Start and End Nodes
		Start->x = CoordArray[0];
		Start->y = CoordArray[1];
		Goal->x = CoordArray[2];
		Goal->y = CoordArray[3];

		cout << "[INFO] " << CoordFile << " Successfully Loaded." << endl; // Confirm map has loaded
		return true;
	}
}

// Prints the current map: (MapArray)
void DisplayMap(int a[gMapWidth][gMapHeight])
{
	system("cls");
	for (int i = 0; i < gMapHeight; i++) //loops though the collums
	{
		for (int j = 0; j < gMapWidth; j++) //loops though the rows
		{
			cout << a[j][(gMapWidth - 1) - i] << " "; //prints The correct way up
		}
		cout << endl;
	}
}
