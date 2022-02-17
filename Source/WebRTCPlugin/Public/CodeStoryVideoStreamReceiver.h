#pragma once
#include "WebRTCInitializer.h"

class WEBRTCPLUGIN_API FCodeStoryVideoStreamReceiverInterface
{
public:
	virtual void OnFrame(rtc::scoped_refptr<webrtc::I420BufferInterface> FrameBuffer, int VideoWidth, int VideoHeight) = 0;
};

class WEBRTCPLUGIN_API CodeStoryVideoStreamReceiver : public rtc::VideoSinkInterface<webrtc::VideoFrame>, webrtc::ObserverInterface
{
public:
	CodeStoryVideoStreamReceiver(TSharedPtr<FCodeStoryVideoStreamReceiverInterface> CodeStoryReceiver)
		: CodeStoryReceiver(CodeStoryReceiver)
	{}
	
	virtual void OnFrame(const webrtc::VideoFrame& frame) override;
	virtual void OnDiscardedFrame() override;

	virtual void OnChanged() override;

private:
	TSharedPtr<FCodeStoryVideoStreamReceiverInterface> CodeStoryReceiver;
};

class WEBRTCPLUGIN_API CodeStoryRtcReceiverObserver : public webrtc::RtpReceiverObserverInterface
{
public:
	virtual void OnFirstPacketReceived(cricket::MediaType media_type) override;
};
