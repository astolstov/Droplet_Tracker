#include <stdio.h>
#include <opencv2\opencv.hpp>
#include "constants.h"
#include "ImageData.h"
#include <vector>
#include <string>
#include <list>
#include <cmath>
using namespace cv;

ImageData::ImageData(const std::string& filename)
{
	mainImage = imread(filename);
	//mainImage.convertTo(mainImage, CV_32FC3, 1.f/255,0);//Changed from CV_32FC3 to CV_32FC1,,, , 1.f / 255.0
	smallerMainImage = mainImage;
	//smallerMainImage.convertTo(smallerMainImage, CV_32FC3, 1.f/255,0);
	//resize(smallerMainImage,smallerMainImage, Size(mainImage.cols*SCALEFACTOR,mainImage.rows*SCALEFACTOR));
}

ImageData::ImageData(const std::string& filename, int height, int width)
{
	mainImage = imread(filename);
	resize(mainImage, mainImage, Size(height, width));
	smallerMainImage = mainImage;
	resize(smallerMainImage, smallerMainImage, Size(mainImage.cols*SCALEFACTOR, mainImage.rows*SCALEFACTOR));
}

int ImageData::getImageWidth() const
{
	return mainImage.size().width;
}////get width

int ImageData::getImageHeight() const
{
	return mainImage.size().height;
}//get height

int ImageData::getSmallImageWidth() const
{
	return smallerMainImage.size().width;
}////get width

int ImageData::getSmallImageHeight() const
{
	return smallerMainImage.size().height;
}

RGB ImageData::getPixel(int row, int col,Mat* image) const
{
	RGB rgb;
	Vec3b intensity = image->at<Vec3b>(row, col);
	float blue = intensity.val[0];
	float green = intensity.val[1];
	float red = intensity.val[2];
	rgb.r = red;
	rgb.b = blue;
	rgb.g = green;
	return rgb;
}//Gets the certain pixel

void ImageData::setPixel(int row, int col,RGB rgb)
{	
	if (row > getImageHeight()-1 || row < 0)
	{
		return;
	}
	if (col > getImageWidth()-1  || col < 0)
	{
		return;
	}
	Vec3b intensity = mainImage.at<Vec3b>(row, col);
	intensity[0] = rgb.b;
	intensity[1] = rgb.g;
	intensity[2] = rgb.r;
	int height = mainImage.size().height;
	int width = mainImage.size().width;
	mainImage.at<Vec3b>(row, col) = intensity;
}//sets the pixel in the array 

void ImageData::saveImage(const std::string& filename)
{
	imwrite(filename, mainImage);
}

void ImageData::findAllStationaryPointPixels(RGB rgb,Coord& returnCoord, double& radius)
{

	float totalX=0;
	float totalY=0;
	int amountToDivideBy=0;
	Coord randomPointForRadius;
	for (int i = 0; i < getSmallImageHeight()-1; i++)
	{
		for (int j = 0; j < getSmallImageWidth()-1; j++)
		{
			RGB imgRGB = getPixel(i, j,&smallerMainImage);
			if (imgRGB.r <= rgb.r+10 && imgRGB.r >= rgb.r - 10 && imgRGB.g >= rgb.g - 10 && imgRGB.g <= rgb.g+10 && imgRGB.b <= rgb.b+10 && imgRGB.b >= rgb.b-10)
			{
				randomPointForRadius.x = i;
				randomPointForRadius.y = j;
				totalX += i;
				totalY += j;
				amountToDivideBy++;
			}
		}
	}
	if (amountToDivideBy != 0)
	{
		returnCoord.x = round(totalX / amountToDivideBy);
		returnCoord.y = round(totalY / amountToDivideBy);
	}
	double radiusX = returnCoord.x - randomPointForRadius.x+0.0;
	double radiusY = returnCoord.y - randomPointForRadius.y+0.0;
	radius = sqrt(radiusX*radiusX + radiusY*radiusY)-10;
}
/*
void ImageData::recursiveFunction(HashTable* myPixelPositions, std::vector<Coord>* point1, Coord currentPoint)
{
	for(int xDir=-1;xDir<2;xDir++)
		for(int yDir=-1;yDir<2;yDir++)
			if (xDir != 0 && yDir != 0)
			{
				recursiveFunctionHelper(myPixelPositions, point1,currentPoint, xDir, yDir);
			}
}
*/
/*
void ImageData::recursiveFunctionHelper(HashTable* myPixelPositions, std::vector<Coord>* point1,Coord currentPoint,int xDir, int yDir)
{
	//four different things
	Coord myCoord;
	myCoord.x = currentPoint.x + xDir;
	myCoord.y = currentPoint.y + yDir;
	int myBucketLocation = myPixelPositions->hashFunction(myCoord);
	std::list<Coord>* myList = myPixelPositions->at(myBucketLocation);
	if (!myList->empty())
	{
		for (std::list<Coord>::iterator i = myList->begin();i!=myList->end() ; ++i)
		{
			if (coordEqual(*i, myCoord))
			{
				point1->push_back(myCoord);
				myList->erase(i);
			}
		}
	}
	else
		return;
}
*/
/*
void ImageData::breakApartTheHash(HashTable* myPixelPositions, std::vector<Coord>* point1)
{
	for (int i = 0; i < myPixelPositions->size(); i++)
	{
		std::list<Coord>* myList = myPixelPositions->at(i);
		if (!myList->empty())
		{
			recursiveFunction(myPixelPositions, point1, myList->front());
			myList->erase(myList->begin());
			break;
		}
	}
}
*/
/*
std::vector<Coord>* ImageData::averagePositionOfStationaryPoints(RGB rgb)
{
	HashTable* myPixelPositions = findAllStationaryPointPixels(rgb);
	std::vector<Coord> allPositions;
	std::vector<Coord> vectorOfPoints;
	
	Coord myCoord;
	int x = 0;
	int y = 0;
	int j = 0;
	for (int i = 0; i < vectorOfPoints.size(); i++)
	{
		breakApartTheHash(myPixelPositions, &allPositions);
		for (; j < allPositions.size(); j++)
		{
			x += allPositions.at(j).x;
			y += allPositions.at(j).y;
		}
		if (j != 0)
		{
			myCoord.x = x / j;
			myCoord.y = y / j;
		}
		j = 0;
		vectorOfPoints.push_back(myCoord);
	}
	return &vectorOfPoints;
}
*/
/*
std::vector<Coord>* ImageData::findStationaryPoints(RGB rgb)
{
	std::vector<Coord> orderedPoints;
	//Attempt to group the stationary points so we can average them later
	std::vector<Coord>* pixelLocations = averagePositionOfStationaryPoints(rgb);
	Coord firstPoint = pixelLocations->at(0);
	int distance = 0;
	int point;
	for (int i = 1; i < pixelLocations->size(); i++)
	{
		int distanceCompare;
		distanceCompare = (firstPoint.x - pixelLocations->at(i).x)*(firstPoint.x - pixelLocations->at(i).x) + (firstPoint.y - pixelLocations->at(i).y)*(firstPoint.y - pixelLocations->at(i).y);
		if (distanceCompare > distance)
		{
			point = i;
		}
	}
	for (int i = 1; i < pixelLocations->size(); i++)
	{
		if (i != point)
		{
			orderedPoints.push_back(pixelLocations->at(i));
		}
	}
	orderedPoints.push_back(pixelLocations->at(point));
	orderedPoints.push_back(firstPoint);
	return &orderedPoints;
}

Coord ImageData::findIntersectionPoint(std::vector<Coord>* myCoord)
{
	
	int x[4];
	
	for (int i = 0; i < 4; i++)
	{
		x[i] = myCoord->at(i).x;
	}
	
	int y[4];
	
	for (int i = 0; i < 4; i++)
	{
		y[i] = myCoord->at(i).y;
	}
	
	double Xz;
	double Yz;
	double m1;
	double m2;
	
	m1 = (y[0] - y[1]) / (x[0] - x[1]);
	m2 = (y[2] - y[3]) / (x[2] - x[3]);
	
	Xz = ((m1*x[0] - y[0]) - (m2*x[2] - y[2])) / (m1 - m2);
	Yz = m1*(Xz - x[0]) + y[0];
	
	Coord coord;
	int X = round(Xz);
	int Y = round(Yz);
	coord.x = round(Xz);
	coord.y = round(Yz);
	
	return coord;
}

double ImageData::findRadius(std::vector<Coord>* myPositions)
{
	double diameter[2];
	int j = 0;
	for (int i = 1; i < 4; i+2)
	{

		int y = (myPositions->at(i - 1).y - myPositions->at(i).y)*(myPositions->at(i - 1).y - myPositions->at(i).y);
		int x = (myPositions->at(i - 1).x - myPositions->at(i).x)*(myPositions->at(i - 1).x - myPositions->at(i).x);
		diameter[j] = sqrt(y + x);
		j++;
	}
	return (diameter[1] + diameter[0]) / 4;
}
*/



