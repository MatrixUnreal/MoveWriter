#include <opencv2/opencv.hpp>
class FaceDetector
{
public:
	FaceDetector(){};
	~FaceDetector(){};
	void init_face_detect();
	std::vector<cv::Rect> getFaces(const cv::Mat& image);	
};