#pragma once
#include "WebRTCInitializer.h"

class CodeStoryVideoStreamReceiver : public rtc::VideoSinkInterface<webrtc::VideoFrame>
{
public:
	void OnFrame(const webrtc::VideoFrame& frame) override;
};
