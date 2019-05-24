#pragma once
#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <opencv2\opencv.hpp>
#include "constants.h"
#include <string>
#include <fstream>
#include <string>
#include <vector>

class ImageData
{
public:
	ImageData(const std::string& filename);//insert the name of the file name
	ImageData(const std::string& filename, int width, int height);
	//~ImageData();
	int getImageWidth() const;											//gets the image width
	int getImageHeight() const;											//gets the image height
	int getSmallImageWidth() const;										//gets the image width
	int getSmallImageHeight() const;									//gets the image height
	RGB getPixel(int row, int col, cv::Mat* image) const;				//gets the pixel at the row and column
	void setPixel(int row, int col, RGB rgb);							//changes the pixel at that row and column to the specified RGB color
	cv::Mat* returnMainImagePointer() { return &mainImage; }			//returns pointer to the main(original) image you are using
	cv::Mat* returnSmallerImagePointer() { return &smallerMainImage; }	//returns pointer to the smaller image you are using
	void saveImage(const std::string& flename);							//saves image to the disk
	
	void recursiveFunction(HashTable* myPixelPositions, std::vector<Coord>* point1, Coord currentPoint); 
	void recursiveFunctionHelper(HashTable* myPixelPositions, std::vector<Coord>* point1, Coord currentPoint, int xDir, int yDir);
	void breakApartTheHash(HashTable* myPixelPositions, std::vector<Coord>* point1);
	std::vector<Coord>* findStationaryPoints(RGB rgb);					//finds the stationary points in the images (9/23/2018: since this is for the lab, I will make it look for a square and then reduce the image to be that square currently not working at all)
	void findAllStationaryPointPixels(RGB rgb, Coord& returnCoord, double& radius);					//finds the pixels that correspond to the stationary points in the image
	std::vector<Coord>* averagePositionOfStationaryPoints(RGB rgb);		//finds the average positions of the middle of the stationary points
	Coord findIntersectionPoint(std::vector<Coord>* myPositions);		//returns the coordinate of the middle 
	double findRadius(std::vector<Coord>* myPositions);					//returns the diameter
private:
	cv::Mat mainImage;													//the image fully sized main image we are using
	cv::Mat smallerMainImage;											//main image that we have descaled by SCALEFACTOR (found in constants.h)
	RGB* myPixels;														//myPixel array
	
};



#endif //IMAGEDATA_H

