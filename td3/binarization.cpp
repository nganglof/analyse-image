#include <iostream>
#include <cstdlib>

#include <morphology.hpp>

using namespace cv;
using namespace std;

void process(int thmin, int thmax, const char* imsname, const char* imdname)
{

	Mat imsimg = imread(imsname,0);
	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	
	Size imssize = imsimg.size();
	int width = imssize.width;
	int height = imssize.height;

	Mat imdimg(height, width, CV_8UC1);

  	for(int i = 0; i < height; ++i)
  	{
  		for(int j = 0; j < width; ++j)
  		{
  			if((imsimg.at<uchar>(i,j) >= thmin) &&  (imsimg.at<uchar>(i,j) <= thmax))
			{
				imdimg.at<uchar>(i,j) = 255;
			}
			else
			{
				imdimg.at<uchar>(i,j) = 0;
			}
  		}
  	}

	imwrite(imdname,imdimg);

}

void usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" thmin thmax ims imd\n"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 4
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(atoi(argv[1]),atoi(argv[2]),argv[3],argv[4]);
  return EXIT_SUCCESS;  
}

