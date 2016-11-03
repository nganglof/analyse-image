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
	Mat cvimg(height, width, CV_8UC1);

	medianBlur(imsimg,cvimg,2*radius+1);

	for(unsigned int i = 0 ; i < height ; ++i)
	{
		for(unsigned int j = 0 ; j < width ; ++j)
		{
			unsigned int iMin = std::max(0, (int) i - radius);
			unsigned int iMax = std::min((int) height-1, (int) i + radius);
			unsigned int jMin = std::max(0, (int) j - radius);
			unsigned int jMax = std::min((int) width-1, (int) j + radius);
						
			vector<int> values;
			for(unsigned int x = jMin; x <= jMax; ++x)
				for(unsigned int y = iMin; y <= iMax; ++y)
					values.push_back(imsimg.at<uchar>(y,x));
	
			std::sort(values.begin(),values.end());

			int median;

			if((values.size()%2)==0){
				//si nombre pair
				int med1 = values[values.size()/2];
				int med2 = values[(values.size()/2)-1];
				median = (med1+med2)/2;
			}
			else{
				//si nombre impair
				median = values[(values.size())/2.];
			}
			imdimg.at<uchar>(i,j) = median;
		}
	}

	Mat diffimg = abs(imdimg - cvimg);

	imwrite(imdname,imdimg);
	imwrite("cv.png",cvimg);
	imwrite("diff.png",diffimg);
}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<"<r> <ims> <imd>"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 3
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(atoi(argv[1]),argv[2],argv[3]);
	return EXIT_SUCCESS;
}
