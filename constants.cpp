
#include <list>
#include "constants.h"

bool pixelEqual(const RGB& lhs, const RGB& rhs)
{
	if (lhs.r == rhs.r && lhs.g == rhs.g&&lhs.b == rhs.b)
	{
		return true;
	}
	return false;
}
bool coordEqual(Coord coord1, Coord coord2)
{
	if (coord1.x == coord2.x && coord1.y == coord2.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool withinCertainRange(const Coord coord1, const Coord coord2,int range)
{
	if ((coord1.x == coord2.x + range || coord1.x == coord2.x-range || coord1.x == coord2.x) && (coord1.y == coord2.y + range || coord1.y == coord2.y-range || coord1.y == coord2.y))
	{
		return true;
	}
	return false;
}

int HashTable::hashFunction(Coord myCoord)
{
	return ((myCoord.x * 37 - 13) + (myCoord.y * 39)) % 1000;
}

void HashTable::insertIntoHash(Coord myCoord)
{
	int key = hashFunction(myCoord);
	if(buckets[key] == nullptr)
	{
		buckets[key] = new std::list<Coord>;
		buckets[key]->push_back(myCoord);

	}
	else
	{
		buckets[key]->push_back(myCoord);
	}
}

bool withinRadius(Coord middle, double radius, int i, int j)
{
	int x = i - middle.x;
	int y = j - middle.y;
	if (radius*radius >= x*x + y*y)
	{
		return true;
	}
	return false;
}