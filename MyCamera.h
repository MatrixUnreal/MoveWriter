#pragma once
#include "opencv2/opencv.hpp"

#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <stdio.h>

#include "Video.h"
#include "DBAccessor.hpp"


class Camera
{
public:
	Camera(){
		Video::createDateVideoFolder();
	};
	void initCamera();
	Camera(cv::String path);
	~Camera();

	bool getFrame();
	bool move_detect(int sensitivity);
	const cv::String currentDateTime();
	int getDuration();
	int setDuration(int input);
	void startWrite();
	void stopWrite();
	void setPath(cv::String input);
	void setNameVideoCamName(cv::String input);
	void setNameVideoCab(cv::String input);
	cv::String getNameVideoCamName();
	cv::String getNameVideoCab();
	int duration=0;	

private:
	cv::Mat frame,lastframe;
	bool canWrite = false;
	cv::String path;
	cv::String nameVideoFolder="Video/";
	cv::String nameVideoCab="";
	cv::String nameVideoCamName="";
	cv::String nameVideoFirstPart="";
	cv::String nameVideoExtention=".avi";
	cv::VideoCapture vcap;
	int frame_width, frame_height,frame_rate;
	cv::VideoWriter video;	
	cv::String pathWriter;
};


