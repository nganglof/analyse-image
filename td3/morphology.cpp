#include <morphology.hpp>

using namespace cv;

void mm(Mat se, Mat ims, Mat imd, void (*pf)(uchar, uchar*))
{
	Size imssize = ims.size();
	int width = imssize.width;
	int height = imssize.height;

  	Size sesize = se.size();
	int widthSE = sesize.width;
	int heightSE = sesize.height;

  	for(int i = 0; i < height; ++i)
  	{
  		for(int j = 0; j < width; ++j)
  		{
  			imd.at<uchar>(i,j) = ims.at<uchar>(i,j);

  			for(int u = 0; u < heightSE; ++u)
		  	{
		  		for(int v = 0; v < widthSE; ++v)
		  		{
		  			if(se.at<uchar>(u, v) 
		  				&& (i-heightSE/2+u) >= 0
		  				&& (j-widthSE/2+v) >= 0
		  				&& (i-heightSE/2+u) < height
		  				&& (j-widthSE/2+v) < width)
	  				{
	  					pf(ims.at<uchar>(i-heightSE/2+u, j-widthSE/2+v), &imd.at<uchar>(i,j));
					}
		  		}
		  	}
  		}
  	}

}

void maximum(uchar val, uchar* max)
{
  	*max = std::max(val,*max);
}

void minimum(uchar val, uchar* min)
{
  	*min = std::min(val,*min);
}

void rangeChange(uchar min, uchar max, Mat ims, Mat imd)
{
	Size imssize = ims.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;
	uchar minMat = 255;
	uchar maxMat = 0;
	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			uchar val = ims.at<uchar>(i,j);
			minMat = (minMat > val) ? val : minMat;
			maxMat = (maxMat < val) ? val : maxMat;
		}
	}
	
	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			uchar val = ims.at<uchar>(i,j);
			float t = ((float)val - (float)minMat) / ((float)maxMat - (float)minMat);
			float newVal = t * ((float)max - (float)min) + (float)min;
			imd.at<uchar>(i,j) = newVal;
		}
	}
}


void matDiff(Mat ims, Mat imd, Mat res)
{
	Size imssize = ims.size();
	unsigned int width = imssize.width;
	unsigned int height = imssize.height;
	for(unsigned int i = 0; i < height; ++i)
	{
		for(unsigned int j = 0; j < width; ++j)
		{
			uchar val1 = ims.at<uchar>(i,j);
			uchar val2 = imd.at<uchar>(i,j);
			res.at<uchar>(i,j) = ((val1 != 0) && (val2 == 0)) * 255;
		}
	}
}

