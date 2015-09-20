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

#include <sstream>
#include <string>
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
const int CURSER_PADDING_HORZ = 300;
const int CURSER_PADDING_VERT = 150;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = CAM_WIDTH*CAM_HEIGHT / 1.5;

const int MAX_NUM_OBJECTS = 50;

int currentlyHitting = 0;

int countPurp = 0;
int countRed = 0;


vector<int> curXs;
vector<int> curYs;



void DoubleClick(int x, int y)
{
	const double XSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
	const double YSCALEFACTOR = 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	double cx = cursorPos.x * XSCALEFACTOR;
	double cy = cursorPos.y * YSCALEFACTOR;

	double nx = x * XSCALEFACTOR;
	double ny = y * YSCALEFACTOR;

	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;

	Input.mi.dx = (LONG)nx;
	Input.mi.dy = (LONG)ny;

	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &Input, sizeof(INPUT));
	SendInput(1, &Input, sizeof(INPUT));

	Input.mi.dx = (LONG)cx;
	Input.mi.dy = (LONG)cy;

	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

	SendInput(1, &Input, sizeof(INPUT));
}




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

string intToString(int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void drawObject(int x, int y, Mat &frame) {

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!

	//UPDATE:JUNE 18TH, 2013
	//added 'if' and 'else' statements to prevent
	//memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

	circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);
	if (y - 25>0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
	if (y + 25<CAM_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, CAM_HEIGHT), Scalar(0, 255, 0), 2);
	if (x - 25>0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
	if (x + 25<CAM_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(CAM_WIDTH, y), Scalar(0, 255, 0), 2);

	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);

}

/*void find_moments(Mat gray)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(gray, canny_output, 50, 150, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Get the moments
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	///  Get the mass centers:
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
	}

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	printf("\t Info: Area and Contour Length \n");
	for (int i = 0; i< contours.size(); i++)
		printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));

}*/


void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed) {
	cout << "0a";
	Mat temp;
	threshold.copyTo(temp);
	cout << "00a";
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	cout << "000a";
	vector<Vec4i> hierarchy;
	cout << "1a";
	Mat img_canny;
	cout << "1.2a";
	Canny(temp, img_canny, 80, 120);
	cout << "1.3a";
	//find contours of filtered image using openCV findContours function
	findContours(img_canny, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	cout << "1.5a";
	double refArea = 0;
	bool objectFound = false;
	cout << "2a";
	cout << " h size = " << hierarchy.size();
	if (hierarchy.size() > 0) {
		cout << "3a";
		cout << "h size = " << hierarchy.size();
		int numObjects = hierarchy.size();
		cout << "3.5a";
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects<MAX_NUM_OBJECTS) {
			cout << "4a";
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {
				cout << "5a";
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea) {
					cout << "6a";
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;


			}
			//let user know you found an object
			if (objectFound == true) {
				cout << "7a";
				putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
				//draw object location on screen
				drawObject(x, y, cameraFeed);
			}
			cout << "8a";
		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
	cout << "9a";
}

void morph(Mat& imgThresholded, Mat& imgMorph){
	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgMorph, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholded, imgMorph, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (removes small holes from the foreground)
	dilate(imgThresholded, imgMorph, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholded, imgMorph, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

}

void MouseSetup(INPUT *buffer)
{
	buffer->type = INPUT_MOUSE;
	buffer->mi.dx = (0 * (0xFFFF / SCREEN_WIDTH));
	buffer->mi.dy = (0 * (0xFFFF / SCREEN_HEIGHT));
	buffer->mi.mouseData = 0;
	buffer->mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
	buffer->mi.time = 0;
	buffer->mi.dwExtraInfo = 0;
}

void MouseClick(INPUT *buffer)
{
	buffer->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
	SendInput(1, buffer, sizeof(INPUT));

	Sleep(10);

	buffer->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
	SendInput(1, buffer, sizeof(INPUT));
}
void MouseMoveAbsolute(INPUT *buffer, int x, int y)
{
	buffer->mi.dx = (x * (0xFFFF / SCREEN_WIDTH));
	buffer->mi.dy = (y * (0xFFFF / SCREEN_HEIGHT));
	buffer->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);

	SendInput(1, buffer, sizeof(INPUT));
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
	int iHighH = 110;

	int iLowS = 28;
	int iHighS = 255;

	int iLowV = 253;
	int iHighV = 255;
	//Create trackbars in "Control" window
	createTrackbar("LowH", contrlName, &iLowH, 0); //Hue (0 - 179)
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
		morph(imgThresholded, imgMorph);

		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgMorph);
		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		//Construct other
		Mat imgThresholdedOther;
		inRange(imgHSV, Scalar(110, iLowS, iLowV), Scalar(179, iHighS, iHighV), imgThresholdedOther); //Threshold the image
		Mat imgMorphOther;
		morph(imgThresholdedOther, imgMorphOther);

		Moments oMomentsOther = moments(imgMorphOther);
		double dM01Other = oMomentsOther.m01;
		double dM10Other = oMomentsOther.m10;
		double dAreaOther = oMomentsOther.m00;
		//cout << "Red: " + to_string(dArea) + " Purp " + to_string(dAreaOther);
		bool other = false;
		if (dAreaOther > dArea) {
			other = true;
			dArea = dAreaOther;
			dM01 = dM01Other;
			dM10 = dM01Other;
			cout << "other";
		}
		
		

		
		// if the area <= 10000
		if (dArea > 30000)
		{
			if (!other) {
				countRed++;
				countPurp--;
				if (countPurp < 0)
					countPurp = 0;
			}
			else {
				countPurp++;
				countRed--;
				if (countRed < 0)
					countRed = 0;
			}
			if (countPurp > 5) {
				currentlyHitting = 2;
			}
			else if (countRed > 10) {
				currentlyHitting = 1;
			}

			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			{
				line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(255, 0, 0), 2);
			}
			
			iLastX = posX;
			iLastY = posY;

			int cursX = (int)((double)(posX - CURSER_PADDING_HORZ) / ((double)CAM_WIDTH - CURSER_PADDING_HORZ * 2) * (double)SCREEN_WIDTH);
			if (cursX < 0)
				cursX = 0;
			int cursY = (int)((double)(posY - CURSER_PADDING_VERT) / (double)(CAM_HEIGHT - CURSER_PADDING_VERT* 2) *  (double)SCREEN_HEIGHT);
			if (cursY < 0)
				cursY = 0;
			
			
			//Draw a red line from the previous point to the current point
			INPUT    Input = { 0 };


			// left up

			if (currentlyHitting == 1) {
				curXs.push_back(cursX);
				curYs.push_back(cursY);
				/*::ZeroMemory(&Input, sizeof(INPUT));
				Input.type = INPUT_MOUSE;
				Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				::SendInput(1, &Input, sizeof(INPUT));
				//cout << "x: " << SCREEN_WIDTH << " y: " << SCREEN_HEIGHT;*/
			
				//	SetCursorPos(cursX, cursY);

				INPUT buffer = { 0 };

				MouseSetup(&buffer);

				MouseMoveAbsolute(&buffer, cursX, cursY);
				

			}
			else if (currentlyHitting == 2) {
				SetCursorPos(cursX, cursY);
				// left down 
				Input.type = INPUT_MOUSE;
				Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				::SendInput(1, &Input, sizeof(INPUT));

				Sleep(10);

				::ZeroMemory(&Input, sizeof(INPUT));
				Input.type = INPUT_MOUSE;
				Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				::SendInput(1, &Input, sizeof(INPUT));
				/*if (curXs.size() > 35) {
					INPUT buffer = { 0 };

					MouseSetup(&buffer);
					int sumX = 0;
					int sumY = 0;
					for (int i = 1; i <= 30; i++) {
						sumX += curXs[curXs.size() - i];
						sumY += curYs[curYs.size() - i];
					}
					MouseMoveAbsolute(&buffer, sumX / 29, sumY / 29);
					MouseClick(&buffer);

					Sleep(10);

					MouseClick(&buffer);
				}*/
				countPurp = 0;
			}

		}
		else {
		
			countPurp--;
			if (countPurp < 0)
				countPurp = 0;
			countRed--;
			if (countRed < 0)
				countRed = 0;
			INPUT    Input = { 0 };


			// left up

	
			/*	::ZeroMemory(&Input, sizeof(INPUT));
				Input.type = INPUT_MOUSE;
				Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				::SendInput(1, &Input, sizeof(INPUT)); */
			
		}
		//frame = frame + imgLines;
		Mat gray;
		cvtColor(frame, gray, CV_BGR2GRAY);

		int fx = 0;
		int fy = 0;
		//cout << "going into the filter";
		//trackFilteredObject(fx, fy, gray, frame);
		//cout << "and im out!";
		

		//imgMorph = imgMorph + imgLines;
		//Display
		imshow(vidName, frame);
		imshow(modName, imgMorphOther);
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
