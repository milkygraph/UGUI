#include <opencv2/opencv.hpp>
#include "raylib.h"

Image MatToRaylibImage(const cv::Mat& mat) {
	int width = mat.cols;
	int height = mat.rows;
	int channels = mat.channels();

	// Convert OpenCV BGR format to raylib's RGB format
	cv::Mat mat_rgb;
	cv::cvtColor(mat, mat_rgb, cv::COLOR_BGR2RGB);

	// Allocate the raylib Image
	Image image = {
			.data = (void*)RL_MALLOC(width * height * channels),
			.width = width,
			.height = height,
			.mipmaps = 1,
			.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8
	};

	// Copy data from OpenCV Mat to raylib Image
	memcpy(image.data, mat_rgb.data, width * height * channels);

	return image;
}