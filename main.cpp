#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <functional>

#include <sys/types.h>
#include <unistd.h>

#include "MyCamera.h"
#include "FaceDetector.h"

using namespace std;
using namespace cv;

const char * const CAM_CONFIG_NAME = "config.cfg";
int main() {
	FaceDetector faceDetector;
	faceDetector.init_face_detect();
	auto camListCfg = getParams(CAM_CONFIG_NAME, "CamList", { "cams" });
	std::vector<std::string> camList;
	try
	{
		camList = split(camListCfg.at("cams"), ',',true);
	}
	catch (const std::out_of_range& ex)
	{
		std::cout << "Cannot find CamList in config file: " << ex.what() << std::endl;
	}

	int maxDuration=100;	
	int numCamera=0;
	vector<String> urlOfCamera;
	Camera *camera= new Camera [camList.size()];


	for (auto camCfgName : camList)
	{
		Camera::Config cfg;
		cfg.parseParams(camCfgName);
		std::cout<<cfg.camId<<"="<<cfg.url<<std::endl;
		urlOfCamera.push_back(cfg.url);

		camera[numCamera].setNameVideoCamName(cfg.camId);
		camera[numCamera].setPath(cfg.url);	
		camera[numCamera].setNameVideoCab(cfg.office);
	    camera[numCamera].initCamera();	   
	    sleep(1);
		numCamera++;
	}
	numCamera=0;
	
	std::cout << "Start program" << std::endl;

	printf("PID of this process: %d\n", getpid());

	while (true) {

		camera[numCamera].getFrame();	
		if((faceDetector.getFaces(camera[numCamera].frame)).size()!=0)std::cout<<",";	
		if(camera[numCamera].move_detect(70) )
			{
				camera[numCamera].startWrite();
				camera[numCamera].setDuration(maxDuration);	
			}
		if(!camera[numCamera].getDuration())
		{			
			camera[numCamera].stopWrite();
		}
		else camera[numCamera].duration--;

		++numCamera<urlOfCamera.size()?:numCamera=0;
	}
	delete [] camera;
	return 0;
}

//	std::cin.get();

//vector<String> urlOfCamera{ "rtsp://admin:startup123@192.168.1.220:554/Streaming/Channels/102/",
	//						"rtsp://admin:startup123@192.168.1.221:554/Streaming/Channels/102/",
	//						"rtsp://admin:startup123@192.168.1.222:554/Streaming/Channels/102/",
	//						"rtsp://admin:startup123@172.20.0.2:554/Streaming/Channels/102/",
	//						"rtsp://admin:startup123@172.20.0.4:554/Streaming/Channels/102/",
	//						"rtsp://admin:startup123@172.20.0.5:554/Streaming/Channels/101/",
	//						"rtsp://admin:startup123@172.20.0.6:554/Streaming/Channels/102/"};