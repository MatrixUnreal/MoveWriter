#include "FaceDetector.h"

cv::CascadeClassifier face_detector;

void FaceDetector::init_face_detect()
{
	//std::string face_cascade_name = "C:\\Rustam\\Library\\OpenCV32\\opencv\\sources\\data\\haarcascades\\haarcascade_upperbody.xml";
	//std::string face_cascade_name = "C:\\Rustam\\Library\\OpenCV32\\opencv\\sources\\data\\haarcascades\\haarcascade_profileface.xml";
	//std::string face_cascade_name = "C:\\Rustam\\Library\\OpenCV32\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt_tree.xml";
	//std::string face_cascade_name = "C:\\Rustam\\Library\\OpenCV32\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
	std::string face_cascade_name = "data/xml/haarcascade_frontalface_alt2.xml";
	//std::string face_cascade_name = "C:\\Rustam\\Library\\OpenCV32\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalcatface_extended.xml";
	//std::string face_cascade_name = "C:\\Rustam\\Library\\OpenCV32\\opencv\\sources\\data\\lbpcascades\\lbpcascade_frontalface.xml";
	face_detector.load(face_cascade_name);
	if (!face_detector.load(face_cascade_name))
	{
		std::cout << "error loading cascade" << std::endl;
	}
}

std::vector<cv::Rect> FaceDetector::getFaces(const cv::Mat& image)
{
	std::vector<cv::Rect>faces;
	cv::Mat gray;
	cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	//gray=gray(Rect(0,0, gray.rows/2,gray.cols/2));
		//copygray

	face_detector.detectMultiScale(gray, faces, 1.2, 8, 0 | 
		//CV_HAAR_DO_CANNY_PRUNING,
		//CV_HAAR_DO_ROUGH_SEARCH,
		//CV_HAAR_FEATURE_MAX,
		CV_HAAR_FIND_BIGGEST_OBJECT,
		//CV_HAAR_SCALE_IMAGE,
		cv::Size(0,0));

	return faces;
}