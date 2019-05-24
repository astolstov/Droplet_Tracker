#pragma once
#ifndef COMPAREIMAGES_INCLUDED
#define COMPAREIMAGES_INCLUDED

#include "constants.h"
#include <string>
#include <vector>
#include <stack>

class ImageData;
class ImageCompare
{
public:
	~ImageCompare();
	void setBackgroundImage(const std::string& filename);									//sets the background image we are using
	void setCompareImage(const std::string& filename);										//Sets the compare image we are using
	void insertIntoList(int x, int y);														//inserts the position x,y into the listOfPositions
	void compareToBackground(Coord middle, double radius);									//finds the pixel locations where there exists a difference and inserts the positions into the listOfPositions
	void givePostion();																		//inserts the average into the averagePosition vector
	std::vector<int> getXAveragePositions() const;											//returns a vector with all the average positions of the differences
	std::vector<int> getYAveragePositions() const;											//returns a vector with all the average positions of the differences
	void drawSquareAroundPosition(int x, int y);											//draws a square around the position x,y
	void drawSide(int x, int y, int side, int size);										//draws a side of the object with size specified  (x and y made into floats to allow division by integer to not truncate a result)
	void saveImage(std::string location);													//saves image to file location
	void clearVector();																		//clears the compareTo vector
	bool bigDifferenceInColor(RGB compare, RGB background);									//given two rgb values, if they have a large difference in color, will yield true if difference exists false otherwise
	bool backgroundSameSizeAsMainImage();													//Checks to see if the size of the background image and our image is the same
	bool comparePixelAt(int x, int y);														//return true if there is a significant difference at that pixel position
	ImageData* returnCompareImagePointer() const { return myImageToCompare; }				//returns the image pointer to the image we are comparing to the background
	ImageData* returnBackgroundImagePointer() const { return myBackgroundImage; }			//returns the background image pointer
private:
	struct Node
	{
		Node(int x, int y) : coordx(x), coordy(y){}
		int coordx;
		int coordy;
	};

	ImageData* myImageToCompare;															//pointer to the image we are comparing to the background
	ImageData* myBackgroundImage;															//pointer to the background image
	std::vector<int> averagePosition;														//x,y positions of the average of the differences
	int m_height;																			//height of image
	int m_width;																			//width of image
	std::vector<std::vector<Node*>> listOfPositions;										//x,y positions of the differences
};


#endif //COMPAREIMAGES_INCLUDED