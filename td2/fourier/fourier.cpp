#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* imsname, int freq)
{
	Mat imsimg = imread(imsname,CV_LOAD_IMAGE_GRAYSCALE);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}

	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;

	Mat imgMat = Mat_<float>(imsimg);
	Mat fourierMat = Mat_<std::complex<double> >(height, width);
	Mat ampMat(height, width, CV_8UC1);
	Mat freqMat(height, width, CV_8UC1);
	
	dft(imgMat, fourierMat, DFT_COMPLEX_OUTPUT);
	
	float max = 0;
	
	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			std::complex<float> c = fourierMat.at<std::complex<float> >(i,j);
			max = (log(abs(c)) > max) ? log(abs(c)) : max;
		}
	}
	
	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			std::complex<float> c = fourierMat.at<std::complex<float> >(i,j);
			freqMat.at<uchar>((i + height / 2) % height,(j + width / 2) % width) = (arg(c) + M_PI) * 255 / (2 * M_PI);
			ampMat.at<uchar>((i + height / 2) % height,(j + width / 2) % width) = (1 + log(abs(c))) * 255 / (1 + max);
		}
	}
	
	Mat ampMatModif(ampMat);
	
	
	imwrite("phase.png",freqMat);
	imwrite("magnitude.png",ampMat);

}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<" <ims> <freq>"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 2
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(argv[1],atoi(argv[2]));
	return EXIT_SUCCESS;
}
