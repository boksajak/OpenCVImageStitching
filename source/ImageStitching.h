#pragma once

namespace ImageStitching {

	struct Image {

		Image() : bytes(0), height(0), width(0) {}

		char* bytes;
		unsigned int height;
		unsigned int width;
	};

	// Returns stitched image for cylindrical wrapping (inputImages should be sorted in 
	Image StitchImages(std::vector<std::string> &inputImages);
}