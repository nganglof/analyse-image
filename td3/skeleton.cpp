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
	void (*pfmin)(uchar, uchar*);
 	pfmin = &minimum;
 	void (*pfmax)(uchar, uchar*);
 	pfmax = &maximum;
		
	Mat tmp(height, width, CV_8UC1);
	Mat tmp2(height, width, CV_8UC1);
	Mat tmp3(height, width, CV_8UC1);
	while(countNonZero(imsimg) != 0)
	{
		mm(seimg,imsimg,tmp,pfmin);
		mm(seimg,tmp,tmp2,pfmax);
		matDiff(tmp2,tmp,tmp3);	
		if(countNonZero(tmp3) == 0)
		{
			break;
		}
		imsimg = imsimg - tmp3;	
	}
	imwrite(imdname,imsimg);

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

