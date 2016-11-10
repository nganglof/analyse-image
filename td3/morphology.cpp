#include <morphology.hpp>

using namespace cv;

void
mm(Mat se, Mat ims, Mat imd, void (*pf)(uchar, uchar*))
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

void
maximum(uchar val, uchar* max)
{
  	*max = std::max(val,*max);
}

void
minimum(uchar val, uchar* min)
{
  	*min = std::min(val,*min);
}

