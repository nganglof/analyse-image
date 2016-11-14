#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* imsname)
{
	int radius = 2;
	Mat G = (Mat_<uchar>(2*radius+1,2*radius+1) << 	1,4,6,4,1,
													4,16,24,16,4,
													6,24,36,24,6,
													4,16,24,16,4,
													1,4,6,4,1);

	Mat imsimg = imread(imsname,0);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	
	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;

	Mat reduce(height/2,width/2,CV_8UC1, Scalar(0,0,0));
	Mat expan(height,width,CV_8UC1, Scalar(0,0,0));
	Mat diff(height, width, CV_8UC1, Scalar(0,0,0));
	Mat reduceCV(height/2,width/2,CV_8UC1, Scalar(0,0,0));
	Mat expanCV(height,width,CV_8UC1, Scalar(0,0,0));
	Mat diffCV(height, width, CV_8UC1, Scalar(0,0,0));
	
	/* Reduce */
	for(unsigned int i = 0 ; i < height ; i += 2)
	{
		for(unsigned int j = 0 ; j < width ; j += 2)
		{
			int iMin = std::max(0, (int)i - (int)radius) - (int)i;
			int iMax = std::min((int)height-1, (int) i + (int)radius) - (int)i;
			int jMin = std::max(0, (int)j - (int)radius) - (int)j;
			int jMax = std::min((int)width-1, (int)j + (int)radius) - (int)j;
			
			double S = 0;
			for(int x = iMin; x <= iMax; ++x)
			{
				for(int y = jMin; y <= jMax; ++y)
				{
					S += (double)imsimg.at<uchar>(i+x,j+y) * (double)G.at<uchar>(x + radius, y + radius);
				}
			}
			S = S / (double)256;
			reduce.at<uchar>(i/2,j/2) = (uchar)S;
			expan.at<uchar>(i,j) = (uchar)S;
		}
	}
	
	Mat tmp = expan.clone();
	
	/* Expand */
	for(unsigned int i = 0 ; i < height ; ++i)
	{
		for(unsigned int j = 0 ; j < width ; ++j)
		{
			int iMin = std::max(0, (int)i - (int)radius) - (int)i;
			int iMax = std::min((int)height-1, (int) i + (int)radius) - (int)i;
			int jMin = std::max(0, (int)j - (int)radius) - (int)j;
			int jMax = std::min((int)width-1, (int)j + (int)radius) - (int)j;
			
			double S = 0;
			for(int x = iMin; x <= iMax; ++x)
			{
				for(int y = jMin; y <= jMax; ++y)
				{
					S += (double)tmp.at<uchar>(i+x,j+y) * (double)G.at<uchar>(x + radius, y + radius);
				}
			}
			S = S / (double)64;
			expan.at<uchar>(i,j) = (uchar)S;
		}
	}
	absdiff(imsimg, expan, diff);
	
	imshow("CoinReduce", reduce);
	waitKey(0);
	imshow("CoinExpand", expan);
	waitKey(0);
	imshow("CoinDiff", diff);
	waitKey(0);
	
	pyrDown(imsimg, reduceCV, Size(width/2, height/2));
	pyrUp(reduceCV, expanCV, Size(width, height));
	absdiff(imsimg, expanCV, diffCV);
	
	imshow("CoinReduce", reduceCV);
	waitKey(0);
	imshow("CoinExpand", expanCV);
	waitKey(0);
	imshow("CoinDiff", diffCV);
	waitKey(0);
	
}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<" <ims>"<<std::endl;
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
