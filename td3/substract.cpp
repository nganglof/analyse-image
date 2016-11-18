#include <iostream>
#include <cstdlib>

#include <morphology.hpp>

using namespace cv;
using namespace std;

void process(const char* imsname1, const char* imsname2, const char* imdname)
{

	Mat imsimg1 = imread(imsname1,0);
	Mat imsimg2 = imread(imsname2,0);
	if((!imsimg1.data) || (!imsimg2.data)){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	
	Mat diff = abs(imsimg1-imsimg2);

	imwrite(imdname,diff);
}

void usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" ims1 ims2 imd\n"<<std::endl;
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

