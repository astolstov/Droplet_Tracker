#pragma once
#ifndef FILESEARCH_H
#define FILESEARCH_H

#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2\opencv.hpp>
#include <sstream>
using namespace cv;
using namespace std;

namespace fs = std::experimental::filesystem;

void findTheFileNames(std::vector<std::string>* myFileNames)
{
	std::cout << "Input Path To Folder: ";
	std::string myFilePath;
	std::cin >> myFilePath;


	std::string path = myFilePath;
	for (auto & entry : fs::directory_iterator(path))
	{
		std::string myFileName = (entry.path()).string();
		std::cout << myFileName << std::endl;
		myFileNames->push_back(myFileName);
	}
}

int findTheVideoName()
{
	std::cout << "Would you like to break apart a video? (Y/N)";
	std::string answer;
	std::cin >> answer;
	if (answer == "Y")
	{
		std::cout << "Input Path To Video: ";
		std::string myFilePath;
		std::cin >> myFilePath;

		std::cout << "How Long is the Video (seconds only):";
		std::string myVideoLengthString;
		std::cin >> myVideoLengthString;

		//std::stringstream val(myVideoLengthString);
		//int videoLength = 0;
		//val >> videoLength;
		int videoLength=myVideoLengthString.length()+3;
		VideoCapture cap(myFilePath);
		int frameRate = cap.get(CV_CAP_PROP_FPS);
		if (!cap.isOpened()) {
			cout << "Error opening video stream or file" << endl;
			return -1;
		}
		int counter = -1;
		for (;;) { //forever
			Mat frame;
			cap >> frame;



			if (frame.empty()) break; // end of video stream
			imshow("Frame", frame);
			std::string fileName = "C:/videoOfFrames/img_";
			//String name = format("img_%04d.jpg", counter++); // NEW !
			//name = fileName.append(name);
			counter++;
			int frameTimeStamp = 1000 * counter / frameRate;
			std::string timeStamp = std::to_string(frameTimeStamp);
			while (timeStamp.length() < videoLength)
			{
				timeStamp.insert(0, "0");
			}
			fileName.append(timeStamp);
			fileName.append(".jpg");
			cout << fileName << endl;
			imwrite(fileName, frame);

			if (waitKey(1) == 27) break; // stop capturing by pressing ESC 
		}
	}

	// When everything done, release the video capture object
	//cap.release();

	// Closes all the frames
	//destroyAllWindows();

	return 0;


}

#endif