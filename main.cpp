#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <functional>

#include <sys/types.h>
#include <unistd.h>


#include "MyCamera.h"

using namespace std;
using namespace cv;

//bool breakTask=false;

//Camera camera("rtsp://admin:startup123@192.168.1.222:554/Streaming/Channels/102/");
//Camera camera("rtsp://admin:admin@192.168.90.168:554/MPEG-4/ch12/main/av_stream");
Camera camera1("rtsp://admin:startup123@192.168.1.220:554/Streaming/Channels/102/");
Camera camera2("rtsp://admin:startup123@192.168.1.222:554/Streaming/Channels/102/");
//Camera camera1("rtsp://admin:admin@192.168.90.168");
//Camera camera2("rtsp://admin:admin@192.168.90.169");

void createAlphaMat(Mat &mat)
{
    CV_Assert(mat.channels() == 4);
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            Vec4b& bgra = mat.at<Vec4b>(i, j);
            bgra[0] = UCHAR_MAX; // Blue
            bgra[1] = saturate_cast<uchar>((float (mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX); // Green
            bgra[2] = saturate_cast<uchar>((float (mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX); // Red
            bgra[3] = saturate_cast<uchar>(0.5 * (bgra[1] + bgra[2])); // Alpha
        }
    }
}


int main() {
	
	int maxDuration=90;	
	camera1.nameVideoCamName="cam1";
	camera2.nameVideoCamName="cam2";

	/*Mat image=imread("1.jpg",1);
	vector<uchar> buf;

	cout<<image.size()<<"\n";
	 vector<int> param;
      vector<uchar> buff;
      param.push_back(CV_IMWRITE_JPEG_QUALITY); 
      param.push_back(1); // jpeg quality 
      cv::imencode(".jpg", image, buff, param);

     cout<<"Size using jpg compression "<< (int) buff.size()<<"\n";
*/
	std::cout << "Start program" << std::endl;

	printf("PID of this process: %d\n", getpid());
	cv::Mat frame;
	camera2.getFrame();	

	
	while (true) {

		camera1.getFrame();		
		if(camera1.move_detect(70) )
			{
				camera1.canWrite=true;
				camera1.duration=maxDuration;				
			}
		if(!camera1.duration)
		{			
			camera1.canWrite=false;
		}
		else camera1.duration--;

		camera2.getFrame();		
		if(camera2.move_detect(80) )
			{
				camera2.canWrite=true;
				camera2.duration=maxDuration;				
			}
		if(!camera2.duration)
		{			
			camera2.canWrite=false;
		}
		else camera2.duration--;

	}
	return 0;
}
