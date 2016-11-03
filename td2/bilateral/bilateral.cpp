#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(float sigma_s, float sigma_g, const char* imsname, const char* imdname)
{
	Mat imsimg = imread(imsname,0);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}
	
	
	/* pre-process */
	unsigned int k = 0;
	while(exp(- (float)k*k / (2 * sigma_s*sigma_s)) > 0.1)
	{
		++k;
	}
	unsigned int radius = k+1;
	float tabS[k+1];
	float tabG[256];
	
	for(unsigned int j = 0; j < 256; ++j)
	{
		tabG[j] = exp(- (float)j*j / (2 * sigma_g*sigma_g));
	}
	for(unsigned int j = 0; j < radius; ++j)
	{
		tabS[j] = exp(- (float)j*j / (2 * sigma_s*sigma_s));
		
	}
	
	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;

	Mat imdimg(height,width,CV_8UC1, Scalar(0,0,0));
	Mat cvimg(height,width,CV_8UC1, Scalar(0,0,0));

	for(unsigned int i = 0 ; i < height ; ++i)
	{
		for(unsigned int j = 0 ; j < width ; ++j)
		{
			
			unsigned int iMin = std::max(0, (int)i - (int)radius);
			unsigned int iMax = std::min((int)height-1, (int) i + (int)radius);
			unsigned int jMin = std::max(0, (int)j - (int)radius);
			unsigned int jMax = std::min((int)width-1, (int)j + (int)radius);
			
			
						
			float N = 0.0;
			float D = 0.0;
			for(unsigned int x = jMin; x <= jMax; ++x)
			{
				for(unsigned int y = iMin; y <= iMax; ++y)
				{
					int Ip = imsimg.at<uchar>(i,j);
					int Iq = imsimg.at<uchar>(y,x);
					int NX = x - j;
					NX = abs(NX);
					int NY = y - i;
					NY = abs(NY);
					
					int norm = NX + NY;
					
					if(norm <= (int)radius)
					{
						int Inorm = abs(Ip - Iq);
						N += tabS[norm] * tabG[Inorm] * Iq;
						D += tabS[norm] * tabG[Inorm];
					}
				}
			}
			
			float I = N / D;
			imdimg.at<uchar>(i,j) = I;
		}
	}
	
	bilateralFilter(imdimg,cvimg,2 * radius + 1 ,sigma_g,sigma_s);
	imwrite("cv.png",cvimg);
	
	Mat diff = abs(imdimg-cvimg);
	imwrite("diff.png",diff);

	try
	{
		imwrite(imdname,imdimg);
	}
	catch(cv::Exception& e)
	{
		cerr << "Error writing file \"" << imdname << "\". \nReason: " << e.msg << endl;
		exit(EXIT_FAILURE);
	}

}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<"<sigma_s> <sigma_g> <ims> <imd>"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 4
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(atof(argv[1]), atof(argv[2]), argv[3], argv[4]);
	return EXIT_SUCCESS;
}
