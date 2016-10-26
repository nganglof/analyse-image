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
process(const char* imsname)
{
	Mat imsimg = imread(imsname,0);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	
	float histogram[256];
	float histogramCumNorm[256];
	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;
	unsigned int intmax = 0;

	Mat imdimg(height, width, CV_8UC1);
	Mat imdimgcv(height, width, CV_8UC1);
	Mat imdimgdiff(height, width, CV_8UC1);
	equalizeHist(imsimg,imdimgcv);
	
	for(unsigned int i = 0; i < 256; ++i)
	{
		histogram[i] = 0;
		histogramCumNorm[i] = 0;
	} 
	
	//creation histogramme
	for(unsigned int y = 0 ; y < (height) ; y++)
	{
		for(unsigned int x = 0 ; x < (width) ; x++)
		{
			int value = imsimg.at<uchar>(y,x);
			histogram[value]++;
		}
	}
	
	//intmax
	for(unsigned int i = 0; i < 256; ++i)
	{
		if((histogram[i]!= 0) && (i>intmax))
			intmax = i;
	} 
	
	//histogramme normalisé
	float NM = (float)height * (float)width;
	for(unsigned int i = 0; i < 256; ++i)
	{
		histogram[i] = leveledValue(((float)histogram[i])/NM,0,1);
	} 

	//histogramme cummulé normalisé
	for(unsigned int i = 0; i < 256; ++i)
	{
		for(unsigned int j = 0; j <= i; ++j)
		{
			histogramCumNorm[i] += histogram[j];
		}
		histogramCumNorm[i] = leveledValue(histogramCumNorm[i],0,1);
	}

	
	for(unsigned int y = 0 ; y < (height) ; y++)
	{
		for(unsigned int x = 0 ; x < (width) ; x++)
		{
			int value = imsimg.at<uchar>(y,x);
			imdimg.at<uchar>(y,x)= (int)leveledValue((((float)intmax) * histogramCumNorm[value]),0.,255.) ;
			imdimgdiff.at<uchar>(y,x) = imdimgcv.at<uchar>(y,x)- imdimg.at<uchar>(y,x);
		}
	}
	
	imwrite("eq.png",imdimg);
	imwrite("eq-cv.png",imdimgcv);
	imwrite("diff.png",imdimgdiff);
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
