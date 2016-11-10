#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

enum {SQUARE, DIAMOND, DISK, LINE_V, DIAG_R, LINE_H, DIAG_L, CROSS, PLUS};

#define MAXVALUE 255

void 
process(const int shape, const int halfsize, const char* imdname)
{
  (void) shape;
  (void) halfsize;
  (void) imdname;  
}

void 
usage (const char *s)
{
  std::cerr<<"Usage: "<<s<<" shape halfsize se-name\n"<<std::endl;
  exit(EXIT_FAILURE);
}

#define param 3
int 
main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(atoi(argv[1]), atoi(argv[2]), argv[3]);
  return EXIT_SUCCESS;  
}

