#pragma once
#ifndef GATHERDATA_INCLUDED
#define GATHERDATA_INCLUDED
#include <string>
#include <vector>
#include "constants.h"
;
class ImageCompare;
class ImageData;
class GatherData
{
public:
	GatherData(int lengthOfArray, std::vector<std::string> FileNames,std::string dataFile);//Used to insert file names of images etc, to later go through and
																																														 //collect data
	~GatherData() {}
	void compareAll(RGB rgb);									//Compares each image to the background image
	void outputData(Coord center,double radius) const;			//outputs all the data
	void findPosition(ImageCompare& comp);						//moves all the difference positions into a queue
	void drawTheSquares(ImageCompare& comp,int i);				//draws the squares around the differences in the images
	void createBlankImage(int width, int height);				//create a blank image (currently takes a blank image I already have and resizes it)
	void drawPointsOnBlankImage(int i);							//draws the position of the droplet on the blank image
	void saveBlankImage();										//saves the blank image to the disk
	int getTheTimeFromFilename(int i, Coord center) const;					//gets the time value from the image name (Should be in the format name_timestamp)
private:
	ImageData* blankImage;										//The blank image
	std::string dataFile;										//string containing the notepad we are using to input data
	int m_lengthOfArray;										//amount of file names you have given
	std::vector<std::string> m_arrayOfFileNames;				//points to the first item of the filenames
	std::vector<std::vector<Coord>> myDropletPosition;			//coordinates of the droplets at various points
};



#endif //GATHERDATA_INCLUDED
