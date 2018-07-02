#pragma once
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
class Camera
{
public:
	Mat frame,lastframe;
	bool canWrite = false;
	String path;
	String nameVideoFolder="Video/";
	String nameVideoCamName="";
	String nameVideoFirstPart="";
	String nameVideo=".avi";
	VideoCapture vcap;
	int frame_width, frame_height,frame_rate;
	VideoWriter video;
	int duration=0;

	Camera(String path);
	~Camera();
	bool getFrame();	
	bool move_detect(int sensitivity);
	const String currentDateTime();
	
};