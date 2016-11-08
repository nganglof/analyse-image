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
	
	Mat padded;
    int m = getOptimalDFTSize( imsimg.rows );
    int n = getOptimalDFTSize( imsimg.cols );
    copyMakeBorder(imsimg, padded, 0, m - imsimg.rows, 0, n - imsimg.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);

    dft(complexI, complexI);

    split(complexI, planes);                  
    Mat mag = planes[0].clone();
    Mat arg = planes[1].clone();
    cartToPolar(planes[0], planes[1], mag, arg);

    mag += Scalar::all(1);
    log(mag, mag);

    // crop the spectrum, if it has an odd number of rows or columns
    mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int ci = mag.rows;
    int cj = mag.cols;	
	Mat magC = mag.clone();
    Mat argC = arg.clone();
	for(int i = 0; i < ci; ++i)
	{
		for(int j = 0; j < cj; ++j)
		{
			magC.at<float>(i,j) = mag.at<float>((i + ci/2)%ci, (j + cj/2)%cj);
			argC.at<float>(i,j) = arg.at<float>((i + ci/2)%ci, (j + cj/2)%cj);
		}
	}

	Mat magCM = magC.clone();
	magCM.at<float>((freq + ci/2)%ci, ci/2) = 0;
	magCM.at<float>((-freq + ci/2)%ci, ci/2) = 0;
	
	for(int i = 0; i < ci; ++i)
	{
		for(int j = 0; j < cj; ++j)
		{
			mag.at<float>(i,j) = magCM.at<float>((i + ci/2)%ci, (j + cj/2)%cj);
		}
	}
	
	exp(mag, mag);
	mag -= Scalar::all(1);
    
	polarToCart(mag, arg, planes[0], planes[1]);
	merge(planes, 2, complexI);
	Mat inverseTransform;
    dft(complexI, inverseTransform, DFT_INVERSE|DFT_REAL_OUTPUT);
	
    normalize(magC, magC, 0, 255, CV_MINMAX); 
    normalize(magCM, magCM, 0, 255, CV_MINMAX); 
    normalize(argC, argC, 0, 255, CV_MINMAX); 
    normalize(planes[0], planes[0], 0, 255, CV_MINMAX); 
    normalize(inverseTransform, inverseTransform, 0, 255, CV_MINMAX);
	
    imwrite("magnitude.png", magC);
    imwrite("magnitude-modify.png", magCM);
    imwrite("phase.png", argC);
    imwrite("inverse.png", inverseTransform);
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
