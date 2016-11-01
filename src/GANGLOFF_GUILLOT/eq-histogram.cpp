#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* imsname)
{
	Mat imsimg = imread(imsname,0);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	
	float histogram[256] = {0.};
	float histogramCumNorm[256] = {0.};
	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;
	unsigned int intmax = 0;

	Mat imdimg(height, width, CV_8UC1);
	Mat imdimgcv(height, width, CV_8UC1);
	equalizeHist(imsimg,imdimgcv);
	
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
	for(unsigned int i = 0; i < 255; ++i)
	{
		if((histogram[i]!= 0) && (i>intmax))
			intmax = i;
	} 
	
	//histogramme cummulé normalisé
	float NM = (float)height * (float)width;
	histogramCumNorm[0] = histogram[0]/NM;
	for(unsigned int i = 1; i < 255; ++i)
		histogramCumNorm[i] = histogramCumNorm[i-1] + histogram[i]/NM;

	for(unsigned int y = 0 ; y < (height) ; y++)
		for(unsigned int x = 0 ; x < (width) ; x++)
			imdimg.at<uchar>(y,x)= 255 * histogramCumNorm[imsimg.at<uchar>(y,x)] ;

	Mat imdimgdiff = imdimg - imdimgcv;

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
