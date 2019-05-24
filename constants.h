#pragma once
#ifndef CONSTANTS_INCLUDED


#define CONSTANTS_INCLUDED

#include <list>
#include <vector>

struct RGB
{
	float r;
	float g;
	float b;
	
};

struct Coord
{
	int x;
	int y;
};

class HashTable
{
public:
	HashTable() { buckets.resize(1000); }
	void insertIntoHash(Coord x);
	int hashFunction(Coord x);
	std::list<Coord>* at(int i) { return buckets.at(i); }
	int size() { return buckets.size(); }
private:
	std::vector<std::list<Coord>*> buckets;
};

bool coordEqual(Coord coord1, Coord coord2);
bool pixelEqual(const RGB& lhs, const RGB& rhs);			//given two RGB pixels, checks to see if they are the same (true if they are, false if they aren't)
bool withinCertainRange(const Coord coord1, const Coord coord2, int range);
bool withinRadius(Coord middle, double radius, int i, int j);
const double SCALEFACTOR = 1;								//amount we descale our main image by
const int SIZE = 50;										//Size of the square we draw around out difference
const int BIGDIFFERENCE = 40;								//Difference in RGB value from the background for us to consider it a viable difference
const int WELLSIZERADIUS = 85.65;






#endif CONSTANTS_INCLUDED
