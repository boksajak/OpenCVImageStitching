#include "common.h"
#include "ImageStitching.h"

// OpenCV
#include <opencv2/opencv.hpp>

namespace ImageStitching {

	// Returns stitched image for cylindrical wrapping (inputImages should be sorted in 
	Image StitchImages(std::vector<std::string> &inputImages) {
		Image result;

		std::vector<cv::Mat> loadedImages;

		for (size_t i = 0; i < inputImages.size(); i++) {
			loadedImages.push_back(cv::imread(inputImages[i], 1));
		}

		cv::imshow("first image",loadedImages[0]);

		return result;
	}
}