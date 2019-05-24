
#include <string>
#include "imageCompare.h"
#include "ImageData.h"
#include <vector>
#include <opencv2\opencv.hpp>
#include "constants.h"
//////
#include <cmath>

using namespace std;
using namespace cv;


ImageCompare::~ImageCompare()
{
	delete myImageToCompare;
}

void ImageCompare::setBackgroundImage(const std::string& filename)
{
	myBackgroundImage = new ImageData(filename);
}

void ImageCompare::setCompareImage(const std::string& filename)
{
	myImageToCompare= new ImageData(filename);
}

bool ImageCompare::backgroundSameSizeAsMainImage()
{
	int heightMain = myImageToCompare->returnMainImagePointer()->size().height;
	int widthMain = myImageToCompare->returnMainImagePointer()->size().width;
	int backgroundHeight= myBackgroundImage->returnMainImagePointer()->size().height;
	int backgroundWidth= myBackgroundImage->returnMainImagePointer()->size().width;
	if (heightMain == backgroundHeight&&widthMain == backgroundWidth)
	{
		return true;
	}
	else
	{
 		resize(*(myImageToCompare->returnMainImagePointer()), *(myImageToCompare->returnMainImagePointer()), Size(backgroundWidth,backgroundHeight));//May have to  flip height and width here
		resize(*(myImageToCompare->returnSmallerImagePointer()), *(myImageToCompare->returnSmallerImagePointer()), Size(backgroundWidth*SCALEFACTOR, backgroundHeight*SCALEFACTOR));
	}
	return false;
}

bool ImageCompare::comparePixelAt(int x, int y)
{

	RGB compare;
	RGB background;

	if (x >= myImageToCompare->getSmallImageHeight()||y>=myImageToCompare->getSmallImageWidth()||x<0||y<0)
	{
		return false;
	}
	int width  = myImageToCompare->getSmallImageWidth();
	int height = myImageToCompare->getSmallImageHeight();

	Vec3b intensity1 = myImageToCompare->returnSmallerImagePointer()->at<Vec3b>(x, y);
	compare.b = intensity1.val[0];//had them multiply by 255 before but it seems that my imshow would not work if i had done that no idea why
	compare.g = intensity1.val[1];
	compare.r = intensity1.val[2];

	Vec3b intensity2 = myBackgroundImage->returnSmallerImagePointer()->at<Vec3b>(x, y);
	background.b = intensity2.val[0];
	background.g = intensity2.val[1];
	background.r = intensity2.val[2];

	if (bigDifferenceInColor(compare, background))
	{
		RGB rgb;
		rgb.b = 0;
		rgb.g = 255;
		rgb.r = 0;
		myImageToCompare->setPixel(x, y, rgb);
		return true;
	}
	return false;
}

void ImageCompare::compareToBackground(Coord middle, double radius)
{
	if (!backgroundSameSizeAsMainImage())
	{
		//exit(1);
	}

	for (int i = 0; i < myBackgroundImage->getSmallImageHeight();i++)
	{
		for (int j = 0;j < myBackgroundImage->getSmallImageWidth(); j++)
		{
			if(withinRadius(middle, radius, i,j))
				if (comparePixelAt(i, j))
				{
					insertIntoList(i, j);
				}
		}
	}
} 

void ImageCompare::clearVector()
{
	listOfPositions.clear();
	averagePosition.clear();
}

bool ImageCompare::bigDifferenceInColor(RGB compare, RGB background)
{
	
	if (compare.r > (background.r+BIGDIFFERENCE)|| compare.r < (background.r - BIGDIFFERENCE))
		return true;
	if (compare.g >(background.g + BIGDIFFERENCE) || compare.g < (background.g - BIGDIFFERENCE))
		return true;
	if (compare.b >(background.b + BIGDIFFERENCE) || compare.b < (background.b - BIGDIFFERENCE))
		return true;
	return false;
}

std::vector<int> ImageCompare::getXAveragePositions() const
{
	std::vector<int> xPositions;
	for (int i = 0; i < averagePosition.size(); i += 2)
	{
		xPositions.push_back(averagePosition[i]);
	}
	return xPositions;
}

std::vector<int> ImageCompare::getYAveragePositions() const
{
	std::vector<int> yPositions;
	for (int i = 1; i < averagePosition.size(); i += 2)
	{
		yPositions.push_back(averagePosition[i]);
	}
	return yPositions;
}

void ImageCompare::givePostion()//This function takes the average position in pixels of the image and inserts it into a 2 element array
{
	float xtotal = 0;
	float ytotal = 0;

	for (int i = 0; i < listOfPositions.size(); i++)
	{
		for (int j = 0; j < listOfPositions[i].size(); j++)
		{
			xtotal += (listOfPositions[i])[j]->coordx;
			ytotal += (listOfPositions[i])[j]->coordy;
		}
//Convert the things to be the x,y positions in the larger main image
		xtotal /= listOfPositions[i].size();
		ytotal /= listOfPositions[i].size();
		int height=myImageToCompare->getSmallImageHeight();
		float percentageX = xtotal / myImageToCompare->getSmallImageHeight();
		float percentageY = ytotal / myImageToCompare->getSmallImageWidth();
		xtotal = myImageToCompare->getImageHeight()*percentageX;//The new larger x
		ytotal = myImageToCompare->getImageWidth()*percentageY;
		int x = round(xtotal);
		int y = round(ytotal);
		averagePosition.push_back(x);
		averagePosition.push_back(y);
		xtotal = 0;
		ytotal = 0;
	}
	if (listOfPositions.size() == 0)
	{
		return;
	}
}

void ImageCompare::drawSquareAroundPosition(int x, int y)
{
	int j = 0;
	for (int i = 0; i < 4; i++)
	{
		drawSide(x, y, i, SIZE);
	}
}

void ImageCompare::saveImage(std::string fileLocation)
{
	namedWindow("image", WINDOW_NORMAL);
	cv::resizeWindow("image", 1280, 1980);
	//imshow("image", *(myImageToCompare->returnMainImagePointer()));
	//waitKey(0);
	myImageToCompare->saveImage(fileLocation);
}

void ImageCompare::insertIntoList(int x, int y)
{
	Node* newNode = new Node(x, y);
	int j = 0;
	bool inputedANumber = false;
	if (listOfPositions.size() == 0)
	{
		//if the list is empty we can insert a new vector
		std::vector<Node*> myNewVec;
		myNewVec.push_back(newNode);
		listOfPositions.push_back(myNewVec);
		return;
	}
	else
	{
		for (int i = 0; i < listOfPositions.size();)
		{
			if (newNode->coordx >(listOfPositions[i][j]->coordx + 20) || newNode->coordx < (listOfPositions[i][j]->coordx - 20) || newNode->coordy >(listOfPositions[i][j]->coordy + 20) || newNode->coordy < (listOfPositions[i][j]->coordy - 20))
			{
				i++;
			}
			else
			{
				inputedANumber = true;
				listOfPositions[i].push_back(newNode);
				return;
			}
		}
	}
	if (inputedANumber == false)
	{
		std::vector<Node*> myNewVec;
		myNewVec.push_back(newNode);
		listOfPositions.push_back(myNewVec);
		return;
	}
}

void ImageCompare::drawSide(int x, int y, int side, int size)
{
	RGB rgb;
	rgb.r = 0;
	rgb.g = 255;
	rgb.b = 0;


	///////////////////////

	///////////////////////
	int xDifference = size / 2;
	int yDifference = size / 2;
	//What we did here is find the percentage of the original image that the point is located, and then moved the same percentage for the main Image in order to save it
	//draw around the center of the difference

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			myImageToCompare->setPixel(x - 1 + i, y - 1 + j,rgb);
		}
	}
	switch (side)
	{
	case 0:
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				myImageToCompare->setPixel(x - xDifference + i, y - yDifference + j,rgb);
			}
		}
		namedWindow("image", WINDOW_NORMAL);
		resizeWindow("image", 300, 500);
		imshow("image", *(myImageToCompare->returnMainImagePointer()));
		break;
	case 1:
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				myImageToCompare->setPixel(x - xDifference + i, y + yDifference + j,rgb);
			}
		}
		namedWindow("image", WINDOW_NORMAL);
		resizeWindow("image", 300, 500);
		imshow("image", *(myImageToCompare->returnMainImagePointer()));
		break;
	case 2:
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				myImageToCompare->setPixel(x - xDifference + j, y - yDifference + i,rgb);
			}
		}
		namedWindow("image", WINDOW_NORMAL);
		resizeWindow("image", 300, 500);
		//imshow("image", *(myImageToCompare->returnMainImagePointer()));
		break;
	case 3:
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				myImageToCompare->setPixel(x + xDifference + j, y - yDifference + i,rgb);
			}
		}
		namedWindow("image", WINDOW_NORMAL);
		resizeWindow("image", 300, 500);
		imshow("image", *(myImageToCompare->returnMainImagePointer()));
		waitKey(1);
		break;
	default:
		exit(1);
	}
}
