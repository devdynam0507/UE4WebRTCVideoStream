#pragma once
#include "WebRTCInitializer.h"

class WEBRTCPLUGIN_API CodeStoryVideoStreamReceiver : public rtc::VideoSinkInterface<webrtc::VideoFrame>, webrtc::ObserverInterface
{
public:
	virtual void OnFrame(const webrtc::VideoFrame& frame) override;
	virtual void OnDiscardedFrame() override;

	virtual void OnChanged() override;
	
};

class WEBRTCPLUGIN_API CodeStoryRtcReceiverObserver : public webrtc::RtpReceiverObserverInterface
{
public:
	virtual void OnFirstPacketReceived(cricket::MediaType media_type) override;
};
