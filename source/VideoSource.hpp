#pragma once
#include <raylib.h>

class VideoSource {
public:
	VideoSource() = default;
	~VideoSource() = default;
	virtual void StartCapture() = 0;
	virtual void StopCapture() = 0;
	virtual Texture2D GetTexture() = 0;

	cv::Mat GetFrame() const { return m_Frame; }

	bool IsCapturing() const { return m_Capturing; }
protected:
	cv::Mat m_Frame;
	Texture2D m_FrameTexture;
	bool m_Capturing = false;
};