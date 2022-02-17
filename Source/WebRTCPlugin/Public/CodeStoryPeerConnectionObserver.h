#pragma once
#include "CodeStoryVideoStreamReceiver.h"

class WEBRTCPLUGIN_API CodeStoryPeerSetSessionDescriptionObserver : public webrtc::SetSessionDescriptionObserver
{
public:
	static CodeStoryPeerSetSessionDescriptionObserver* CreateObserver();

	virtual void OnSuccess() override;
	virtual void OnFailure(webrtc::RTCError error) override;
};
