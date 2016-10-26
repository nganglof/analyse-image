#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

float
leveledValue(float value,float min,float max){
	float ret = value;
	if(value <= min)
		ret = min;
	else if (value>=max)
		ret =max;
	return ret;
}

void
process(const char* imsname, int radius, int constante)
{
	Mat imsimg = imread(imsname,0);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	

	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;

	//Mat thimg(height, width, CV_8UC1);
	Mat thcvmeanimg(height, width, CV_8UC1);
	Mat thcvgaussimg(height, width, CV_8UC1);
	//Mat diffimg(height, width, CV_8UC1);
	
	adaptiveThreshold(imsimg,thcvmeanimg,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,radius,constante);
	adaptiveThreshold(imsimg,thcvgaussimg,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,radius,constante);

	//imwrite("th.png",thimg);
	imwrite("th-cv-mean.png",thcvmeanimg);
	imwrite("th-cv-gauss.png",thcvgaussimg);
	//imwrite("diff.png",diffimg);
}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<"<ims> <radius> <const>"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 3
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(argv[1],atoi(argv[2]),atoi(argv[3]));
	return EXIT_SUCCESS;
}
