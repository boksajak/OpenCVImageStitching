#include "common.h"
#include "ImageStitching.h"

// OpenCV includes
#include <opencv2/opencv.hpp>
#include "opencv2/stitching/stitcher.hpp"

namespace ImageStitching {

	// Returns stitched image for cylindrical wrapping (inputImages should be sorted in 
	Image StitchImages(std::vector<std::string> &inputImages) {
		Image result;

		std::vector<cv::Mat> loadedImages;

		for (size_t i = 0; i < inputImages.size(); i++) {
			cv::Mat img = cv::imread(inputImages[i], 1);

			// downsample to 1/4
			//resize(img,img,cv::Size(img.size().width * 0.25f, img.size().height * 0.25f));//resize image

			loadedImages.push_back(img);
		}

		cv::Mat resultImg;
		cv::Stitcher stitcher = cv::Stitcher::createDefault();
		cv::Stitcher::Status status = stitcher.stitch(loadedImages, resultImg);

		if (status != cv::Stitcher::OK)
		{
			TRACE_ERROR("Can't stitch images, error code = " << status << std::endl);
		}

		std::vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);

		imwrite("result.png", resultImg, compression_params);

		resize(resultImg,resultImg,cv::Size(resultImg.size().width * 0.1f, resultImg.size().height * 0.1f));//resize image
		cv::imshow( "Result", resultImg );

		return result;

	}
}