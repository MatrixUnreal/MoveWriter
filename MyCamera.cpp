#include "MyCamera.h"

using namespace std;
using namespace cv;

bool Camera::move_detect(int sensitivity)
{
	if(this->frame.empty() ||this->lastframe.empty())return false;

	cv::Mat imgDifference, gray;
	absdiff(this->frame, this->lastframe, imgDifference);
	IplImage* _image = new IplImage(imgDifference);

	IplImage* bin;
	cv::waitKey(1);
	assert(_image != 0);
	bin = cvCreateImage(cvGetSize(_image), IPL_DEPTH_8U, 1);
	cvConvertImage(_image, bin, CV_BGR2GRAY);
	cvCanny(bin, bin, 50, 200);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;
	int contours_count = cvFindContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	cvtColor(this->frame, gray, cv::COLOR_BGR2GRAY);

	for (CvSeq* current = contours; current != NULL; current = current->h_next)
	{
		CvPoint2D32f center;
		float radius = 0;

		cvMinEnclosingCircle(current, &center, &radius);
		if (radius > sensitivity)
		{
			std::cout << "_" << std::endl;
			cvReleaseMemStorage(&storage);
			cvReleaseImage(&bin);
			delete(_image);
			return true;
		}
	}

	cvReleaseMemStorage(&storage);
	cvReleaseImage(&bin);
	delete(_image);
	return false;
}

Camera::Camera(String path)
{
	this->path=path;
	this->vcap.open(path);
	if (!this->vcap.isOpened()) {
	cout << "Error opening video stream or file" << endl;
	}
	frame_width = this->vcap.get(CV_CAP_PROP_FRAME_WIDTH);
    frame_height = this->vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
	frame_rate = this->vcap.get(CAP_PROP_FPS);
	nameVideoFirstPart=currentDateTime();	
}

void Camera::initCamera()
{
	this->vcap.open(path);
	if (!this->vcap.isOpened()) {
	cout << "Error opening video stream or file" << endl;
	cout << "path= "<<path<<endl;
	}
	frame_width = this->vcap.get(CV_CAP_PROP_FRAME_WIDTH);
    frame_height = this->vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
	frame_rate = this->vcap.get(CAP_PROP_FPS);
	nameVideoFirstPart=currentDateTime();
}

bool Camera::getFrame()
{
	//if(this->frame.cols!=frame_width || this->frame.rows!=frame_height||this->frame.empty())
	this->lastframe = this->frame.clone();
	this->vcap >> this->frame;	

	time_t     now = time(0);
    struct tm  tstruct;
    char       month[20];
    char       day[10];
    tstruct = *localtime(&now);
    strftime(month, sizeof(month), "%Y-%m", &tstruct);
    strftime(day, sizeof(day), "%d", &tstruct);

	if(canWrite) 
		{	
			if(!this->video.isOpened())
			{
				Video::createDateVideoFolder();
				this->pathWriter=this->nameVideoFolder+month+"/"+day+"/"+this->nameVideoFirstPart+"="+this->nameVideoCab+"="+this->nameVideoCamName+this->nameVideoExtention;
				std::cout << "Start record" << std::endl;
				this->video.open(pathWriter, CV_FOURCC('M', 'P', '4', 'V'), frame_rate? frame_rate:15, Size(frame_width, frame_height), true);
				//this->video.open(pathWriter, CV_FOURCC('H', '2', '6', '4'), frame_rate? frame_rate:15, Size(frame_width, frame_height), false);
				//this->video.open(pathWriter, CV_FOURCC('M','J','P','G'), frame_rate? frame_rate:15, Size(frame_width, frame_height), true);
			}
			if(this->frame.cols!=frame_width || this->frame.rows!=frame_height ||this->frame.empty())
			{
				std::cout << "frame is wrong" << std::endl;
			}
			else
			{
				this->video.write(this->frame);
			}
		}
	else 
		{
			if(this->video.isOpened()){
				std::cout << "Stop record" << std::endl;
				this->video.release();
				std::cout << currentDateTime() << std::endl;
				cv::String newName=this->nameVideoFolder+month+"/"+day+"/"+this->nameVideoFirstPart+"="+currentDateTime()+"="+this->nameVideoCab+"="+this->nameVideoCamName+this->nameVideoExtention;
				
				if ( rename( this->pathWriter.c_str(), newName.c_str() ) != 0 ) 
				{
        			std::cout << "Ошибка переименования файла\n";
        			return false;
        		}

			}

			this->nameVideoFirstPart=currentDateTime();
		}
	
	/*char c = (char)waitKey(33);
	if (c == 27)
	{
		video.release();
		return true;
	}*/
	return true;
}

Camera::~Camera()
{
	video.release();
	vcap.release();
}

// Get current date/time, format is YYYY-MM-DD|HH:mm:ss
const String Camera::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);
    return buf;
}

int Camera::getDuration()
{
	return duration;
}

int Camera::setDuration(int input)
{
	this->duration=input;
}

void Camera::startWrite()
{
	this->canWrite=true;
}

void Camera::stopWrite()
{
	this->canWrite=false;
}

void Camera::setPath(String input)
{
	this->path=input;
}

void Camera::setNameVideoCamName(cv::String input)
{
	this->nameVideoCamName=input;
}

cv::String Camera::getNameVideoCamName()
{
	return this->nameVideoCamName;
}

void Camera::setNameVideoCab(cv::String input)
{
	this->nameVideoCab=input;
}

cv::String Camera::getNameVideoCab()
{
	return this->nameVideoCab;
}