//============================================================================
// Name        : HelloCV.cpp
// Author      : David Gros and Marianne Olsen
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <windows.h>
#include "wtypes.h"
using namespace cv;
using namespace std;

string vidName = "MyVideo";
string modName = "Mod";
string modNameTwo = "Modtwo";
string contrlName = "c";
int iLastX = -1;
int iLastY = -1;
const int CAM_WIDTH = 1280;
const int CAM_HEIGHT = 720;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
const int CURSER_PADDING_HORZ = 100;
const int CURSER_PADDING_VERT = 50;

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


int main(int argc, char** argv)
{
	//GetDesktopResolution(SCREEN_HEIGHT, SCREEN_WIDTH);
	SCREEN_HEIGHT = 1080;
	SCREEN_WIDTH = 1920;
	int c;
	
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened()) {
		cout << "aw no camera";
		// check if we succeeded
		return -1;
	}
	
	cap.set(CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
	
	namedWindow(vidName, CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	namedWindow(modName, CV_WINDOW_AUTOSIZE);
	namedWindow(modNameTwo, CV_WINDOW_AUTOSIZE);
	
	//Set up control window
	namedWindow(contrlName, CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 28;
	int iHighS = 255;

	int iLowV = 253;
	int iHighV = 255;
	//Create trackbars in "Control" window
	createTrackbar("LowH", contrlName, &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", contrlName, &iHighH, 179);

	createTrackbar("LowS", contrlName, &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", contrlName, &iHighS, 255);

	createTrackbar("LowV", contrlName, &iLowV, 255); //Value (0 - 255)
	createTrackbar("HighV", contrlName, &iHighV, 255);
	
	//Capture a temporary image from the camera
	Mat imgTmp;
	
	cap.read(imgTmp);

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);
	

	for (;;) {
		Mat frame;
		
		bool bSuccess = cap.read(frame); // read a new frame from video

		
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		flip(frame, frame, 1);
		// Manipulate
		Mat imgHSV;
		cvtColor(frame, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
		
		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		Mat imgMorph;
		//morphological opening (removes small objects from the foreground)
		erode(imgThresholded, imgMorph, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgMorph, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(imgThresholded, imgMorph, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgMorph, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		

		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgMorph);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		// if the area <= 10000
		if (dArea > 10000)
		{
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			{
				//Draw a red line from the previous point to the current point
				line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(255,0,0), 2);
				
			}

			iLastX = posX;
			iLastY = posY;

			int cursX = (int)((double)(posX - CURSER_PADDING_HORZ) / ((double)CAM_WIDTH - CURSER_PADDING_HORZ * 2) * (double)SCREEN_WIDTH);
			if (cursX < 0)
				cursX = 0;
			int cursY = (int)((double)(posY - CURSER_PADDING_VERT) / (double)(CAM_HEIGHT - CURSER_PADDING_VERT* 2) *  (double)SCREEN_HEIGHT);
			if (cursY < 0)
				cursY = 0;
			//cout << "x: " << SCREEN_WIDTH << " y: " << SCREEN_HEIGHT;
			SetCursorPos(cursX, cursY);


		}
		frame = frame + imgLines;
		//imgMorph = imgMorph + imgLines;
		//Display
		imshow(vidName, frame);
		imshow(modName, imgThresholded);
		imshow(modNameTwo, imgMorph);

		//Exit if needed
		c = cvWaitKey(10); // wait 10 ms or for key stroke
		if (c == 27)
			break; // if ESC, break and quit
	}
	/* clean up */
	cap.release();
	/*cvReleaseCapture(&cv_cap);*/
	cvDestroyWindow("MyVideo");
	//cvDestroyWindow("Mod");

}
