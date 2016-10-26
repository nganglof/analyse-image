#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* imsname)
{
	Mat imsimg = imread(imsname,1);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	
	unsigned int histogram[256];
	float histogramNorm[256];
	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;
	
	Mat imdimg(height, width, CV_8UC3);
	
	for(unsigned int i = 0; i < 256; ++i)
	{
		histogram[i] = 0;
	} 
	
	
	/* creation histogramme */
	for(unsigned int y = 0 ; y < (height) ; y++)
	{
		for(unsigned int x = 0 ; x < (width) ; x++)
		{
			std::cout << imsimg.at<Vec3b>(y,x)[0] << endl;
			histogram[imsimg.at<Vec3b>(y,x)[0]]++;
		}
	}
	
	unsigned int intmax = 0;
	/* intmax */
	for(unsigned int i = 0; i < 256; ++i)
	{
		if(intmax < histogram[i])
		{
			intmax = histogram[i];
		}
	} 
	
	for(unsigned int i = 0; i < 256; ++i)
	{
		for(unsigned int j = 0; j <= i; ++j)
		{
			histogramNorm[i] += histogram[j];
		}
	}
	
	for(unsigned int i = 0; i < 256; ++i)
	{
		histogramNorm[i] = (float)histogramNorm[i] / ((float)height * (float)width);
	} 
	
	for(unsigned int y = 0 ; y < (height) ; y++)
	{
		for(unsigned int x = 0 ; x < (width) ; x++)
		{
			imdimg.at<Vec3b>(y,x)[0]= (int)(intmax * histogramNorm[imsimg.at<Vec3b>(y,x)[0]]) ;
			imdimg.at<Vec3b>(y,x)[1]= (int)(intmax * histogramNorm[imsimg.at<Vec3b>(y,x)[0]]) ;
			imdimg.at<Vec3b>(y,x)[2]= (int)(intmax * histogramNorm[imsimg.at<Vec3b>(y,x)[0]]) ;
		}
	}
	
	imwrite("eq.png",imdimg);
}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<"<ims>"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 1
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(argv[1]);
	return EXIT_SUCCESS;
}
