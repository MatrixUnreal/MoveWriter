#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <functional>

#include <sys/types.h>
#include <unistd.h>
#include <memory>

#include "MyCamera.h"
//#include "FaceDetector.h"
#include "ThreadSafeDetector.h"


using namespace std;
using namespace cv;

int maxDuration=100;	

void runCam(Camera camera, std::shared_ptr<ThreadSafeDetector> detector)
{

	printf("PID of this process: %d\n", getpid());
	std::cout << "Thread No: " << pthread_self() << std::endl;
	std::cout<<camera.getPath()<<std::endl;

	bool isJustStartWriteVideo=false;

	while(true)
	{
		std::vector<bbox_t>boxes;
		std::vector<cv::Rect>v_rect;
		camera.getFrame();
		if(!camera.frame.empty())
		boxes = detector->detect(camera.frame);
		
		for(auto box:boxes)				
			v_rect.push_back(cv::Rect(box.x,box.y,box.w,box.h));

		camera.setDrawRectangles(v_rect);

		//if((faceDetector.getFaces(camera[numCamera].frame)).size()!=0)std::cout<<",";	
		if(camera.move_detect(70) )
		{
			if(!isJustStartWriteVideo)
			{
				camera.setTimer();
			}
			isJustStartWriteVideo=true;
			camera.startWrite();
			camera.setDuration(maxDuration);	
		}
		if(!camera.getDuration() || camera.isTimeUp())
		{			
			isJustStartWriteVideo=false;
			camera.stopWrite();
		}
		else camera.duration--;
	}
}


const char * const CAM_CONFIG_NAME = "config.cfg";
int main() {
	//FaceDetector faceDetector;
	//faceDetector.init_face_detect();
	
	const std::string DARKNET_CONFIG_PATH = "data/yolov3-tiny_obj.cfg";
	const std::string DARKNET_WEIGHTS_PATH = "data/yolov3-tiny_obj_19200.weights";
	std::shared_ptr<ThreadSafeDetector> detector =
		std::make_shared<ThreadSafeDetector>(DARKNET_CONFIG_PATH, DARKNET_WEIGHTS_PATH, 0);

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

	int numCamera=0;
	vector<String> urlOfCamera;
	Camera *camera= new Camera [camList.size()];


	for (auto camCfgName : camList)
	{
		Camera::Config cfg;
		cfg.parseParams(camCfgName);		
		urlOfCamera.push_back(cfg.url);

		camera[numCamera].setNameVideoCamName(cfg.camId);
		camera[numCamera].setPath(cfg.url);	
		camera[numCamera].setNameVideoCab(cfg.office);
		camera[numCamera].setConfig(cfg);
	    camera[numCamera].initCamera();	   

		numCamera++;
	}
	
	numCamera=0;

	std::vector<std::shared_ptr<std::thread>>thr;
	for (auto unit : camList)
	{
		thr.push_back(std::make_shared<std::thread>(runCam,std::ref(camera[numCamera]),detector));
		//thr[numCamera]->join();
		std::cout << "Run thread" << std::endl;
		numCamera++;
	}


	numCamera=0;
	
	std::cout << "Start program" << std::endl;

	printf("PID of main process: %d\n", getpid());
	std::cout << "Thread main No: " << pthread_self() << std::endl;

	while(1){};
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