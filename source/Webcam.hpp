#pragma once

#include <atomic>
#include <thread>
#include <opencv2/opencv.hpp>
#include "VideoSource.hpp"

class Webcam : public VideoSource {
public:
	Webcam() = default;
	~Webcam() = default;
	void StartCapture() override;
	void StopCapture() override;
	Texture2D GetTexture() override;
	void CaptureThread();

private:
	cv::VideoCapture m_Capture;

	std::atomic<bool> m_Capturing = false;
	std::atomic<bool> m_NewFrameAvailable = false;
	std::mutex m_FrameMutex;
	std::thread m_CaptureThread;
};