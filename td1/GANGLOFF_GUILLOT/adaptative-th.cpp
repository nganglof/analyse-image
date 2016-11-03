#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

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

	Mat thimg(height, width, CV_8UC1);
	Mat thcvmeanimg(height, width, CV_8UC1);
	Mat thcvgaussimg(height, width, CV_8UC1);
	Mat diffimg(height, width, CV_8UC1);
	
	adaptiveThreshold(imsimg,thcvmeanimg,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,2*radius+1,constante);
	adaptiveThreshold(imsimg,thcvgaussimg,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,2*radius+1,constante);
	
	for(unsigned int i = 0 ; i < height ; ++i)
	{
		for(unsigned int j = 0 ; j < width ; ++j)
		{
			unsigned int iMin = std::max(0, (int) i - radius);
			unsigned int iMax = std::min((int) height-1, (int) i + radius);
			unsigned int jMin = std::max(0, (int) j - radius);
			unsigned int jMax = std::min((int) width-1, (int) j + radius);
			
						
			int mean = 0;
			int cmp = 0;
			for(unsigned int x = jMin; x <= jMax; ++x)
			{
				for(unsigned int y = iMin; y <= iMax; ++y)
				{
					mean += imsimg.at<uchar>(y,x);
					cmp++;
				}
			}
			mean = mean / cmp;
			thimg.at<uchar>(i,j) = (imsimg.at<uchar>(i,j) > (mean - constante)) * 255;
			diffimg.at<uchar>(i,j) = abs(thimg.at<uchar>(i,j) - thcvmeanimg.at<uchar>(i,j));			
		}
	}

	imwrite("th.png",thimg);
	imwrite("th-cv-mean.png",thcvmeanimg);
	imwrite("th-cv-gauss.png",thcvgaussimg);
	imwrite("diff.png",diffimg);
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
