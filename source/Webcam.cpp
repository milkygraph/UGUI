#include "Webcam.hpp"
#include "Utils.hpp"

void Webcam::StartCapture() {
	m_Capture.open(0);

	// Set the webcam resolution to 640x480 and 30 FPS
	m_Capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	m_Capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
	m_Capture.set(cv::CAP_PROP_FPS, 30);

	m_Capturing = true;
	m_CaptureThread = std::thread(&Webcam::CaptureThread, this);
}

void Webcam::StopCapture() {
	m_Capturing = false;
	if (m_CaptureThread.joinable()) {
		m_CaptureThread.join();
	}
	m_Capture.release();
}

Texture2D Webcam::GetTexture() {
	if (m_NewFrameAvailable) {
		cv::flip(m_Frame, m_Frame, 1);
		Image image = MatToRaylibImage(m_Frame);
		if (m_FrameTexture.id == 0)
			m_FrameTexture = LoadTextureFromImage(image);
		else
			UpdateTexture(m_FrameTexture, image.data);
		UnloadImage(image);
		m_NewFrameAvailable = false;
	}
	return m_FrameTexture;
}

void Webcam::CaptureThread() {
	while (m_Capturing) {
		cv::Mat frame;
		m_Capture >> frame;
		if (!frame.empty()) {
			std::lock_guard<std::mutex> lock(m_FrameMutex);
			m_Frame = frame;
			m_NewFrameAvailable = true;
		}
	}
}