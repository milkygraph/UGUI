#pragma once
#include <raylib.h>

class VideoSource {
public:
	VideoSource() = default;
	~VideoSource() = default;
	virtual void StartCapture() = 0;
	virtual void StopCapture() = 0;
	virtual Texture2D GetTexture() = 0;

	bool IsCapturing() const { return m_Capturing; }
protected:
	Texture2D m_FrameTexture;
	bool m_Capturing = false;
};