#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* imsname, const char* imdname)
{

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
