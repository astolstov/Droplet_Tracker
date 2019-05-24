#include "gatherData.h"
#include "imageCompare.h"
#include "imageData.h"
#include <string>
#include <iostream>
#include <fstream>
#include <string>
//DECLARE THE ARRAY HERE
using namespace cv;
GatherData::GatherData(int lengthOfArray, std::vector<std::string> arrayOfFileNames, std::string MyDataText)
	:m_lengthOfArray(lengthOfArray), dataFile(MyDataText)
{
	m_arrayOfFileNames.resize(lengthOfArray);
	for (int i = 0; i < m_lengthOfArray; i++)
	{
		m_arrayOfFileNames[i] = arrayOfFileNames[i];
	}
}

void GatherData::createBlankImage(int width, int height)
{

	blankImage = new ImageData("myBlankTemplate.jpg",width,height);//cant create so for now ill just read from a certain blank image and resize
}

void GatherData::compareAll(RGB rgb)
{
	ImageCompare comp;
	comp.setBackgroundImage(m_arrayOfFileNames[0]);
	createBlankImage(comp.returnBackgroundImagePointer()->getImageWidth(), comp.returnBackgroundImagePointer()->getImageHeight());

	ImageData* background = comp.returnBackgroundImagePointer();
	Coord middlePoint;
	double radius; 
	background->findAllStationaryPointPixels(rgb, middlePoint, radius);

	for (int i = 1; i < m_lengthOfArray; i++)
	{
		comp.clearVector();
		comp.setCompareImage(m_arrayOfFileNames[i]);
		comp.compareToBackground(middlePoint, radius);
		comp.givePostion();
		findPosition(comp);
		drawTheSquares(comp, i);
		drawPointsOnBlankImage(i);
		comp.saveImage("C:/Users/Alex/Pictures/Image1.jpg");	
	}
	saveBlankImage();
  	outputData(middlePoint,radius); //For now keep this hidden
}

void GatherData::saveBlankImage()
{
	blankImage->saveImage("C:/Users/Alex/Pictures/Image_Blank1.jpg");
}

void GatherData::findPosition(ImageCompare& comp)
{
	std::vector<Coord> newPosition;
	std::vector<int> xCoords = (comp.getXAveragePositions());
	std::vector<int> yCoords = (comp.getYAveragePositions());
	for (int i = 0; i < xCoords.size(); i++)
	{
		Coord newCoord;
		newCoord.x = xCoords[i];
		newCoord.y = yCoords[i];
		if (newCoord.x == -1 || newCoord.y == -1)
		{
			//outputData(center);
			return;
		}
		newPosition.push_back(newCoord);
	}
	myDropletPosition.push_back(newPosition);
}

void GatherData::drawPointsOnBlankImage(int i)
{
	RGB rgb;
	rgb.r = 255;
	rgb.g = 100;
	rgb.b = 50;
	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", *(blankImage->returnMainImagePointer()));
	waitKey(1);
	for (int j = 0; j < (myDropletPosition[i - 1]).size(); j++)
	{
		for (int n = 0; n < 3; n++)
		{
			for (int k = 0; k < 3; k++)
			{
				blankImage->setPixel(myDropletPosition[i - 1][j].x + n - 1, myDropletPosition[i - 1][j].y + k - 1, rgb);
			}
		}
	}
	//cv::line(drawing, x, y, x, y, color);
	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", *(blankImage->returnMainImagePointer()));
	waitKey(1);
}

void GatherData::drawTheSquares(ImageCompare& comp, int i)
{
	for (int j = 0; j < (myDropletPosition[i - 1]).size(); j++)
	{
		comp.drawSquareAroundPosition(myDropletPosition[i - 1][j].x, myDropletPosition[i - 1][j].y);
	}
}

void GatherData::outputData(Coord center,double radius) const
{
	std::ofstream myFile;
	myFile.open(dataFile);
	if (myDropletPosition.size() == 0)
	{
		std::cout << "There were no differences found.";
		return;
	}
	for (int i = 0; i < myDropletPosition.size(); i++)
	{
		int time = getTheTimeFromFilename(i,center);
		for (int j = 0; j < myDropletPosition[i].size(); j++)
		{

			myFile << "Droplet #" << j + 1 << " :=: " << "   Y:   " << ((myDropletPosition[i][j].x-center.x)/radius)*WELLSIZERADIUS<<" mm" << "   X:   " << ((myDropletPosition[i][j].y-center.y)/radius)*WELLSIZERADIUS<<" mm"<<"      T:"<<time<< std::endl;
		}
	}
	myFile.close();
}//OUTPUTS ALL THE DATA WE HAVE INSIDE OF OUR DATA ARRAY

int GatherData::getTheTimeFromFilename(int i, Coord center) const
{
	//ImageName_123
	std::string myPictureName = m_arrayOfFileNames[i];
	std::string myTime;
	for (int j = 0; j < myPictureName.length(); j++)
	{
		if (myPictureName[j] == '_')
		{
			myTime = myPictureName.substr(j+1);
			return std::stoi(myTime.c_str());
		}
	}
}