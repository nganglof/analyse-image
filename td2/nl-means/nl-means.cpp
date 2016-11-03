#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

bool
isCorrect(int i, int j, Mat mat){
	return (i >= 0 && j >= 0 && i < mat.size().height && j < mat.size().width);
}

float
patchesDistance(Mat mat, int p[], int q[], int n){
	float S = 0;
	int pxi,pxj,qxi,qxj;
	for(int i = -n; i < n; ++i)
	{
		for(int j = -n; j < n; ++j)
		{
			pxi = p[0] + i;
			pxj = p[1] + j;
			qxi = q[0] + i;
			qxj = q[1] + j;
			if(isCorrect(pxi, pxj, mat) && isCorrect(qxi, qxj, mat))
			{
				S += pow(mat.at<uchar>(pxi, pxj) - mat.at<uchar>(qxi, qxj), 2); 
			}
			
		}
	}
	return S / pow(2*n+1,2);
}

void
process(float sigma, const char* imsname, const char* imdname)
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


	for(unsigned int i = 0 ; i < height ; ++i)
	{
		for(unsigned int j = 0 ; j < width ; ++j)
		{
			unsigned int iMin = std::max(0, (int) i - radius);
			unsigned int iMax = std::min((int) height-1, (int) i + radius);
			unsigned int jMin = std::max(0, (int) j - radius);
			unsigned int jMax = std::min((int) width-1, (int) j + radius);
					
			float w;

			for(unsigned int x = jMin; x <= jMax; ++x){
				for(unsigned int y = iMin; y <= iMax; ++y){
					
				}
			}



		}
	}


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
