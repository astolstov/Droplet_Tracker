
/*
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;
int main()
{
	Mat img = imread("lena.jpg");
	namedWindow("image", WINDOW_NORMAL);
	imshow("image", img);
	img.convertTo(img, CV_32FC3);
	Vec3f intensity = img.at<Vec3f>(0, 0);
	float blue = intensity.val[0];
	float green = intensity.val[1];
	float red = intensity.val[2];
	cout << blue << endl;
	cout << green << endl;
	cout << red << endl;
	waitKey();
}
Mat grHistogram(260, 301, CV_8UC3, Scalar(0, 0, 0));
The 8U means the 8-bit Usigned integer, C3 means 3 Channels for RGB color, and Scalar(0, 0, 0) is the initial value for each pixel. Similarly,
*/


//First I want to be able to access a picture
//Analyze picture for a background
//Or given a background image i find the new difference from it
//Find the difference and new position
//Take the data from the position
//and go from there

#include <stdio.h>
#include <setjmp.h>
#include <iostream>
#include <string>
#include "fileSearch.h"
#include "gatherData.h"
#include "constants.h"
int main()
{

	std::vector<std::string> FileNames;
	findTheVideoName();
	findTheFileNames(&FileNames);
	/*
	//Lets assume that image name is passed in the form name_time, name_frequency
	std::string baseName = "VID_20190301_141711680 ";
	int lengthOfArray = 23;
	std::string arrayOfFileNames[23] = {"imageframe_0.jpg","imageframe_2598.jpg","imageframe_2695.jpg","imageframe_2702.jpg","imageframe_2721.jpg","imageframe_2801.jpg","imageframe_2830.jpg","imageframe_2904.jpg","imageframe_3004.jpg","imageframe_3043.jpg","imageframe_3127.jpg","imageframe_3228.jpg","imageframe_3289.jpg","imageframe_3407.jpg","imageframe_3496.jpg","imageframe_3653.jpg","imageframe_3664.jpg","imageframe_3989.jpg","imageframe_4045.jpg","imageframe_4147.jpg","imageframe_4330.jpg","imageframe_4446.jpg","imageframe_4699.jpg"};
	for (int i = 0; i < lengthOfArray; i++)
		{
			arrayOfFileNames[i] = baseName +std::to_string(i)+".jpg";
		}
	*/
	
	std::string fileToWriteTo = "C:/dropletData/data.txt";
	int size = FileNames.size();
	GatherData dropletData(size, FileNames,fileToWriteTo);
	RGB rgb;
	rgb.r = 181;
	rgb.g = 230;
	rgb.b = 29;

	dropletData.compareAll(rgb);
  	return 0;
	
}


