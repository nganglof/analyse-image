#include <iostream>
#include <cstdlib>

#include <morphology.hpp>

using namespace cv;
using namespace std;

void process(const char* imboundname, const char* imsname, const char* imdname)
{

	Mat imsimg = imread(imsname,0);
	Mat imboundimg = imread(imboundname,0);

	if((!imsimg.data) || (!imboundimg.data)){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	
	Size imssize = imsimg.size();
	int width = imssize.width;
	int height = imssize.height;

	Mat imdimg(height, width, CV_8UC3);

  	for(int i = 0; i < height; ++i)
  	{
  		for(int j = 0; j < width; ++j)
  		{
  			if(imboundimg.at<uchar>(i,j))
			{
				imdimg.at<Vec3b>(i,j)[0]=0;
				imdimg.at<Vec3b>(i,j)[1]=0;
				imdimg.at<Vec3b>(i,j)[2]=255;
			}
			else
			{
				for(int c = 0 ; c < 3 ; c++){
					imdimg.at<Vec3b>(i,j)[c]=imsimg.at<uchar>(i,j);
	  			}
			}
  		}
  	}

	imwrite(imdname,imdimg);

}

void usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" im-boundary ims imd\n"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 3
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1],argv[2],argv[3]);
  return EXIT_SUCCESS;  
}

