#include <iostream>
#include <cstdlib>

#include <morphology.hpp>

using namespace cv;
using namespace std;

void process(const char* sename, const char* imsname, const char* imdname)
{

	Mat imsimg = imread(imsname,0);
	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	Mat seimg = imread(sename,0);
	if(!seimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;
	void (*pf)(uchar, uchar*);
 	pf = &minimum;
	
	
	Mat sum(height, width, CV_8UC1);	
	Mat tmp(height, width, CV_8UC1);
	rangeChange(0,1,imsimg,sum);
	while(countNonZero(imsimg) != 0)
	{
		mm(seimg,imsimg,tmp,pf);
		imsimg = tmp.clone();
		rangeChange(0,1,imsimg,tmp);
		sum = sum + tmp;

	}
	
	Mat imdimg(height, width, CV_8UC1);
	rangeChange(0,255,sum,imdimg);
	imwrite(imdname,imdimg);

}

void usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" im-se ims imd\n"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 3
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1], argv[2], argv[3]);
  return EXIT_SUCCESS;  
}

