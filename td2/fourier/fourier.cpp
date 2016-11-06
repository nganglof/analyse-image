#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void
process(const char* imsname, int freq)
{
	Mat imsimg = imread(imsname,CV_LOAD_IMAGE_GRAYSCALE);

	if(!imsimg.data){
		std::cerr << "No image data" << endl;
		exit(EXIT_FAILURE);
	}

	Size imssize = imsimg.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;

	Mat imgMat = Mat_<float>(imsimg);
	Mat fourierMat = Mat_<std::complex<float> >(height, width);
	Mat ampMat(height, width, CV_8UC1);
	Mat freqMat(height, width, CV_8UC1);
	
	dft(imgMat, fourierMat, DFT_COMPLEX_OUTPUT);	
	
	/* Max amplitude */
	float max = 0;
	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			std::complex<float> c = fourierMat.at<std::complex<float> >(i,j);
			max = (log(abs(c)) > max) ? log(abs(c)) : max;
		}
	}
	
	/* Création des matrices d'amplitude et de phase (normalisées + centrées) */
	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			std::complex<float> c = fourierMat.at<std::complex<float> >(i,j);
			freqMat.at<uchar>((i + height / 2) % height, (j + width / 2) % width) = (arg(c) + M_PI) * 255 / (2 * M_PI);
			ampMat.at<uchar>((i + height / 2) % height, (j + width / 2) % width) = (1 + log(abs(c))) * 255 / (1 + max);
		}
	}
	
	Mat ampMatModif = ampMat.clone();
	ampMatModif.at<uchar>((freq + height / 2) % height, width / 2) = 0;
	ampMatModif.at<uchar>((-freq + height / 2) % height, width / 2) = 0;
	
	
	imwrite("phase.png",freqMat);
	imwrite("magnitude.png",ampMat);
	imwrite("magnitude-modify.png",ampMatModif);

	/* Bon jusqu'ici ça marche bien,  le reste c'est à dire la transformée inverse ben c'est pas aussi cool :s */
	
	/* Début des emmerdes */

	/* Modification de la matrice fourier pour transformée inverse */
	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			float amp = exp(ampMatModif.at<uchar>((i - height / 2) % height, (j - width / 2) % width) * (1 + max) / 255 - 1);
			float arg = freqMat.at<uchar>((i - height / 2) % height, (j - width / 2) % width) * (2 * M_PI) / 255 - M_PI;
			std::complex<float> c(amp * cos(arg), amp * sin(arg));
			//fourierMat.at<std::complex<float> >(i,j) = c;
		}
	}
	
	dft(fourierMat, imgMat, DFT_INVERSE | DFT_REAL_OUTPUT);
	
	Mat inverse(height, width, CV_8UC1);
	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			inverse.at<uchar>(i,j) = (char)imgMat.at<float>(i,j);
		}
	}
	imwrite("inverse.png",inverse);
}

void
usage(const char* s)
{
	std::cerr<<"Usage: "<<s<<" <ims> <freq>"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 2
int
main(int argc, char* argv[])
{
	if(argc != (param+1))
		usage(argv[0]);
	process(argv[1],atoi(argv[2]));
	return EXIT_SUCCESS;
}
