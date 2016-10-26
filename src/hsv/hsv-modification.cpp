#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const float h, const float s, const float v, const char* imsname, const char* imdname)
{
	Mat imsimg = imread(imsname,1);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	
	Mat hsvimg;
	cvtColor(imsimg, hsvimg, COLOR_BGR2HSV);
	vector<Mat> hsvimgchannels;
	split(hsvimg, hsvimgchannels);
	imshow("h channel", hsvimgchannels[0]);
	imshow("s channel", hsvimgchannels[1]);
	imshow("v channel", hsvimgchannels[2]);
	waitKey(0);
	
	Size imssize = imsimg.size();
	int width = imssize.width;
	int height = imssize.height;
	
	Mat H(height,width, CV_8U, h);
	Mat S(height,width, CV_8U, s);
	Mat V(height,width, CV_8U, v);
	
	hsvimgchannels[0] = hsvimgchannels[0] + H;
	hsvimgchannels[1] = hsvimgchannels[1] + S;
	hsvimgchannels[2] = hsvimgchannels[2] + V; 
	
	Mat hsvresult;
	Mat result;
	merge(hsvimgchannels, hsvresult);
	cvtColor(hsvresult, result, COLOR_HSV2BGR);
	
	imshow("New image", result);
	waitKey(0);
	
	
	try
	{
		imwrite(imdname,result);
	}
	catch(cv::Exception& e)
	{
		cerr << "Error writing file \"" << imdname << "\". \nReason: " << e.msg << endl;
		exit(EXIT_FAILURE);
	}
	
}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<"h s v imsname imdname"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 5
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(atof(argv[1]), atof(argv[2]), atof(argv[3]), argv[4], argv[5]);
	return EXIT_SUCCESS;
}
