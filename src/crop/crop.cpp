#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* imsname, int i0, int j0, int h, int w)
{
	Mat imsimg = imread(imsname,1);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}

	//crop pixel par pixel
	Mat imdimgcrop(h,w,CV_8UC3);
	for(int y = i0 ; y < (i0+h) ; y++)
		for(int x = j0 ; x < (j0+w) ; x++)
			for(int c = 0 ; c < 3 ; c++)
				imdimgcrop.at<Vec3b>(y-i0,x-j0)[c]=imsimg.at<Vec3b>(y,x)[c];
	imwrite("crop.png",imdimgcrop);

	//crop avec Rect
	Rect rectcrop(j0, i0, w, h);
	Mat imdimgcropcv = imsimg(rectcrop);
	imwrite("crop-cv.png",imdimgcropcv);
}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<"<ims> <i0> <j0> <h> <w>"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 5
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]),atoi(argv[5]));
	return EXIT_SUCCESS;
}
