#include "common.h"
#include "ImageStitching.h"

// DevIL Library Includes
//#include "il.h"
//#include "ilu.h"
//#include "ilut.h"

// OpenCV
#include <opencv2/opencv.hpp>

namespace ImageStitching {

	// Returns stitched image for cylindrical wrapping (inputImages should be sorted in 
	Image StitchImages(std::vector<std::string> &inputImages) {
		Image result;

		for (size_t i = 0; i < inputImages.size(); i++) {
			cv::Mat image;
			image = cv::imread( inputImages[i], 1 );


			//ILuint ImageName;
			//ilGenImages(1, &ImageName);
			//ilBindImage(ImageName);
			//ilLoadImage(inputImages[i].c_str()); 
		}

		return result;
	}
}