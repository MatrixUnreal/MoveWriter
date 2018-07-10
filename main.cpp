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

using namespace std;
using namespace cv;

vector<String> urlOfCamera{ "rtsp://admin:startup123@192.168.1.220:554/Streaming/Channels/102/",
							"rtsp://admin:startup123@192.168.1.221:554/Streaming/Channels/102/",
							"rtsp://admin:startup123@192.168.1.222:554/Streaming/Channels/102/",
							"rtsp://admin:startup123@172.20.0.2:554/Streaming/Channels/102/",
							"rtsp://admin:startup123@172.20.0.4:554/Streaming/Channels/102/",
							"rtsp://admin:startup123@172.20.0.5:554/Streaming/Channels/101/",
							"rtsp://admin:startup123@172.20.0.6:554/Streaming/Channels/102/"};

Camera *camera= new Camera [urlOfCamera.size()];

int main() {

	int maxDuration=90;	
	int numCamera=0;
	for(auto url:urlOfCamera)
	{
		camera[numCamera].setNameVideoCamName("cam"+to_string(numCamera));
		camera[numCamera].setPath(url);	
		camera[numCamera].setNameVideoCab(to_string(numCamera))	;
	    camera[numCamera].initCamera();	    	
		numCamera++;
	}
	numCamera=0;

	std::cout << "Start program" << std::endl;

	printf("PID of this process: %d\n", getpid());

	//std::cin.get();

	while (true) {

		camera[numCamera].getFrame();		
		if(camera[numCamera].move_detect(100) )
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
