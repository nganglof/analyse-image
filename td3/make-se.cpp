#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

enum {SQUARE, DIAMOND, DISK, LINE_V, DIAG_R, LINE_H, DIAG_L, CROSS, PLUS};

#define MAXVALUE 255

void process(const int shape, const int halfsize, const char* imdname)
{

	unsigned int sidesize = halfsize*2+1;
	Mat imdimg(sidesize, sidesize, CV_8UC1, Scalar(0));

	switch(shape){

		case SQUARE:
		case DIAMOND:
		case DISK:
			for(int i = -halfsize ; i <= halfsize ; ++i)
				for(int j = -halfsize ; j <= halfsize ; ++j)
					imdimg.at<uchar>(halfsize + i, halfsize + j) = 
						(shape==DIAMOND)*((abs(i) + abs(j)) <= halfsize) * 255
						+  (shape==SQUARE)*255
						+  (shape==DISK)*(sqrt(i*i +j*j) <= halfsize) * 255;
		break;

		case LINE_V:
			for(unsigned int i = 0 ; i < sidesize ; ++i)
				imdimg.at<uchar>(i,halfsize) = 255;
		break;

		case DIAG_R:
			for(unsigned int j = 0 ; j < sidesize ; ++j)
				imdimg.at<uchar>(sidesize-j-1,j) = 255;
		break;

		case LINE_H:
			for(unsigned int j = 0 ; j < sidesize ; ++j)
				imdimg.at<uchar>(halfsize,j) = 255;
		break;

		case DIAG_L:
			for(unsigned int i = 0 ; i < sidesize ; ++i)
				imdimg.at<uchar>(i,i) = 255;
		break;
		
		case CROSS:
			for(unsigned int i = 0 ; i < sidesize ; ++i){
				imdimg.at<uchar>(sidesize-i-1,i) = 255;
				imdimg.at<uchar>(i,i) = 255;
			}
		break;

		case PLUS:
			for(unsigned int j = 0 ; j < sidesize ; ++j){
				imdimg.at<uchar>(halfsize,j) = 255;
				imdimg.at<uchar>(j,halfsize) = 255;
			}
		break;

		default:
			std::cerr<<"Unknown shape\n"<<std::endl;
			break;
	}

	imwrite(imdname,imdimg);

}

void usage (const char *s)
{
	std::cerr<<"Usage: "<<s<<" shape halfsize se-name\n"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 3
int main( int argc, char* argv[] )
{
	if(argc != (param+1))
		usage(argv[0]);
	process(atoi(argv[1]), atoi(argv[2]), argv[3]);
	return EXIT_SUCCESS;  
}

