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
	
	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;

	Mat seimg = imread(sename,0);
	if(!seimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}

	Mat imdimg(height, width, CV_8UC1);

 	void (*pfmin)(uchar, uchar*);
 	pfmin = &minimum;
 	void (*pfmax)(uchar, uchar*);
 	pfmax = &maximum;
	mm(seimg,imsimg,imdimg,pfmax);
	imsimg = imdimg.clone();
	mm(seimg,imsimg,imdimg,pfmin);

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

