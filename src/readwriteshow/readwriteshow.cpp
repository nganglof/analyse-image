#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* imsname, const char* imdname)
{
	Mat imsimg = imread(imsname,1);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}

	Mat imdimg(100,200,CV_8UC3, Scalar(255,0,255));

	namedWindow("Display window imd",WINDOW_AUTOSIZE);
	imshow("Display window imd",imdimg);

	namedWindow("Display window ims",WINDOW_AUTOSIZE);
	imshow("Display window ims",imsimg);

	Size imssize = imsimg.size();
	int width = imssize.width;
	int height = imssize.height;

	std::cout << "Dimension " << imsname << " : " << width << "x" << height << endl;
	
	try
	{
		imwrite(imdname,imdimg);
	}
	catch(cv::Exception& e)
	{
		cerr << "Error writing file \"" << imdname << "\". \nReason: " << e.msg << endl;
		exit(EXIT_FAILURE);
	}

	waitKey(0);
}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<"imsname imdname"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 2
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(argv[1], argv[2]);
	return EXIT_SUCCESS;
}
