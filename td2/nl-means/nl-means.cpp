#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(int radius, const char* imsname, const char* imdname)
{
	Mat imsimg = imread(imsname,0);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}

	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;

	Mat imdimg(height, width, CV_8UC1);



	imwrite(imdname,imdimg);

}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<"<sigma> <ims> <imd>"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 3
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(atof(argv[1]),argv[2],argv[3]);
	return EXIT_SUCCESS;
}
