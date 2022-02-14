#pragma once
#include "WebRTCInitializer.h"

class WEBRTCPLUGIN_API CodeStoryVideoStreamReceiver : public rtc::VideoSinkInterface<webrtc::VideoFrame>
{
public:
	virtual void OnFrame(const webrtc::VideoFrame& frame) override;
	
};
